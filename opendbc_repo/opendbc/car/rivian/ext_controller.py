import math
from collections import deque
import numpy as np

from opendbc.car.lateral import (
  apply_driver_steer_torque_limits, common_fault_avoidance,
  apply_steer_angle_limits_vm, get_max_angle_delta_vm,
)
from opendbc.car.rivian.values import CarControllerParams as CCP, RivianFlags
from opendbc.car.vehicle_model import VehicleModel

# EPAS angle envelope (EPAS_High_Angle_Cmd_Err)
EPAS_FW_MAX_ANGLE_BP = [0.0, 2.78, 5.56, 8.33, 12.50, 16.67, 22.22, 27.78]  # m/s
EPAS_FW_MAX_ANGLE_V  = [500, 500,  250,  150,  85,    56,    40,    25   ]  # deg

# EPAS windowed rate limit (EPAS_High_Actual_Angle_Rate_Err)
EPAS_FW_RATE_BP = [5.56, 8.33, 12.50, 16.67]  # m/s
EPAS_FW_RATE_V  = [4.50, 1.50, 0.60,  0.18 ]  # deg/frame

EPAS_FW_ANGLE_MARGIN = 0.98
EPAS_FW_RATE_MARGIN  = 0.94

# final per-frame cap inside panda's jerk limit
PANDA_STEP_MARGIN = 0.9

MIN_TORQUE_FRAMES = 50
HANDOFF_EXIT_DEG = 15.0      # hand back to angle when the wheel is within this of the commanded angle
UNWIND_HANDOFF_RATE = 40.0  # max wheel speed in deg/s to hand back to angle
EAC_RECOVER_FRAMES = 15     # angle frames with the EPAS EAC not active before falling back to torque (~0.15s, normal activation is under 0.05s)

# blip the TOI request bit at high angle so the EPAS does not latch ToiFlt
TOI_MAX_ANGLE_DEG = 90
TOI_MAX_ANGLE_FRAMES = 89        # frames held high before a blip (~0.9s)
TOI_BLIP_FRAMES = 2              # frames to release ACM_lkaActToi

# Above this wheel angle the rack is saturated >75% of the time (route data); cap output so the
# controller can recover from saturation faster when geometry eases
HIGH_ANGLE_THRESHOLD_DEG = 90
HIGH_ANGLE_CAP_FRAC = 0.95

# make-before-break for a *predictable* forced angle->torque transition (low-speed threshold or the
# driver's force-torque toggle). The torque channel is rate-limited up from 0 at STEER_DELTA_UP and
# takes ~0.5s to reach the torque that holds the current curve; panda enforces the same limit, so we
# cannot warm-start it. Instead keep the EPAS angle servo holding the wheel (angle stays active) while
# torque ramps underneath it, then release angle once torque can carry the load. Without this the wheel
# is unsupported for the whole ramp and unwinds, then the ramping torque overshoots (the lateral jerk).
#
# Exit is by TORQUE ACHIEVED, not wall-clock: release angle once |apply_torque| reaches
# TORQUE_PREARM_EXIT_FRAC of the torque that holds the current curve. That fraction is relative, so it
# scales with curve sharpness and speed automatically - a sharp low-speed bend needs more torque and is
# allowed to ramp longer, a shallow one exits sooner. The frame cap is ONLY an absolute safety backstop
# so a stalled ramp (driver countering, TOI angle-limit blip) cannot pin the angle servo forever; it is
# sized to exceed the physical ramp time of the sharpest possible low-speed curve
# (0.85 * max STEER_MAX(385) / STEER_DELTA_UP(3) ~= 109 frames) with margin, so under a healthy ramp the
# fractional exit always fires first, on every curve. Do not tune the cap down to a curve-specific value
# - that reintroduces the exact speed/curve dependence this design removes.
TORQUE_PREARM_EXIT_FRAC = 0.85    # release angle once |apply_torque| >= this * |feedforward hold torque|
TORQUE_PREARM_MIN_HOLD = 20       # below this |hold torque| there is nothing to hold; switch immediately
TORQUE_PREARM_MAX_FRAMES = 150    # ~1.5s absolute backstop only; healthy ramps exit on EXIT_FRAC first
TORQUE_PREARM_STALL_FRAMES = 12   # consecutive no-new-peak frames that mark the torque ramp as stalled
TORQUE_PREARM_ABORT_LOCKOUT = 50  # frames to stay on angle after a stalled handoff before re-attempting


class _RateBudget:
  # sliding-window budget for the EPAS rate limit; history is CAN-quantized to 0.1 deg
  WINDOW_USER_FRAMES = 16
  WINDOW_TIME_S = 0.16

  def __init__(self):
    self.history = deque([0.0] * self.WINDOW_USER_FRAMES, maxlen=self.WINDOW_USER_FRAMES)

  def push(self, sent_angle: float) -> None:
    self.history.append(round(sent_angle * 10) / 10)

  def bounds(self, threshold_dps: float, margin: float):
    cmd_oldest = self.history[0]
    budget = threshold_dps * self.WINDOW_TIME_S * margin
    return cmd_oldest - budget, cmd_oldest + budget


def get_safety_CP():
  from opendbc.car.rivian.interface import CarInterface
  return CarInterface.get_non_essential_params("RIVIAN_R1")


class ExternalController:
  def __init__(self, CP):
    self.CP = CP
    self.steer_ratio = CP.steerRatio
    self.wheelbase = CP.wheelbase
    self.VM = VehicleModel(get_safety_CP())
    # without angle hardware this collapses to a plain torque controller: torque_active
    # is pinned while lateral is active and the angle channel never engages
    self.angle_supported = bool(CP.flags & RivianFlags.ANGLE_HARNESS)
    self.gen2 = bool(CP.flags & RivianFlags.GEN2)

    # hands-on
    self.wheel_touch_cnt = 0
    self.torsion_cnt = 0
    self.torsion_sign = 0
    self.hands_on = False

    # cooperative torque mode
    self.torque_active = False
    self.torque_active_frames = 0
    self.lat_active_last = False
    self.eac_dead_frames = 0
    # make-before-break: torque ramps up under a still-active angle command on a forced transition
    self.torque_prearm = False
    self.prearm_frames = 0
    self.prearm_torque_peak = 0     # highest |apply_torque| reached this handoff; stall = no new peak
    self.prearm_stall_frames = 0
    self.prearm_abort_lockout = 0
    # last-handoff telemetry for road-test triage (surface via logs / watch the rlog overlap)
    self.prearm_last_outcome = ""   # "reached" | "backstop" | "abort"
    self.prearm_last_hold = 0
    self.prearm_last_frames = 0
    self.prearm_last_peak = 0
    # driver-forced full-time torque: pin torque-only and never hand off to angle (set from a param
    # via CarController, resets each drive). Reuses the torque-only-hardware path, no new safety surface.
    self.force_torque = False
    # below the configured min angle speed: pin torque-only this frame (set from a param via
    # CarController). Same torque-only path as force_torque, no new safety surface.
    self.low_speed_force = False

    # angle command
    self.apply_angle_last = 0.0
    self.angle_active = False
    self.rate_budget = _RateBudget()
    # liveParameters, pushed in from card each frame
    self.roll = 0.0
    self.angle_offset_deg = 0.0

    # cooperative torque
    self.apply_torque_last = 0   # rate-limiter memory; frozen through a blip
    self.torque_cmd = 0          # what actually goes on the wire (0 during a blip)
    # decoupled from torque_active so a blip does not flip angle or feature mode
    self.toi_angle_limit_counter = 0
    self.toi_act_cmd = False     # sent into ACM_lkaActToi, low for 2 frames during a blip

  def update(self, CS, lat_active: bool, actuators):
    self._update_hands_on(CS)
    desired_angle = math.degrees(self.VM.get_steer_from_curvature(-float(actuators.curvature), CS.out.vEgo, self.roll)) + self.angle_offset_deg
    self._update_torque_active(CS, lat_active, desired_angle, actuators)
    self._update_angle(CS, lat_active, desired_angle)
    self._update_torque(CS, actuators)

  def _update_wheel_touched(self, wheel_touched, wheel_touched_min_count):
    self.wheel_touch_cnt += 1 if wheel_touched else -1
    self.wheel_touch_cnt = int(np.clip(self.wheel_touch_cnt, 0, wheel_touched_min_count * 2 + 1))
    return self.wheel_touch_cnt > wheel_touched_min_count

  def _update_torsion(self, torque, torque_threshold, torsion_min_count):
    abs_torque = abs(torque)
    pressed = abs_torque > torque_threshold
    sign = int(np.sign(torque))
    # reset on sign flip, opposing torque applications shouldn't accumulate
    if pressed and self.torsion_sign and sign != self.torsion_sign:
      self.torsion_cnt = 0
    else:
      self.torsion_cnt += max(1, math.ceil(abs_torque / torque_threshold)) if pressed else -1
      self.torsion_cnt = int(np.clip(self.torsion_cnt, 0, torsion_min_count * 2 + 1))
    if pressed:
      self.torsion_sign = sign
    return self.torsion_cnt > torsion_min_count

  def _update_hands_on(self, CS):
    # hands-on if any of: capacitive sensor, EPAS-side level, or torsion bar
    # GEN2 (2025+) has no SCCM_WheelTouch on the bus (carstate leaves it None)
    if not self.gen2 and CS.sccm_wheel_touch is not None:
      calibration = CS.sccm_wheel_touch["SETME_X52"]
      wheel_touch = self._update_wheel_touched(CS.sccm_wheel_touch["SCCM_WheelTouch_CapacitiveValue"] > calibration * 0.9, 25)
    else:
      wheel_touch = False
    torsion = self._update_torsion(CS.out.steeringTorque, 4.0, 9)
    self.hands_on = wheel_touch or torsion or CS.hands_on_level > 1

  def _reset_prearm(self):
    self.torque_prearm = False
    self.prearm_frames = 0
    self.prearm_torque_peak = 0
    self.prearm_stall_frames = 0

  def _end_prearm(self, outcome: str, hold_target: int):
    # record why the overlap ended so a road test can be triaged from the log
    self.prearm_last_outcome = outcome
    self.prearm_last_hold = hold_target
    self.prearm_last_frames = self.prearm_frames
    self.prearm_last_peak = self.prearm_torque_peak
    self._reset_prearm()

  def _update_torque_active(self, CS, lat_active: bool, desired_angle: float, actuators):
    self.torque_active_frames = self.torque_active_frames + 1 if self.torque_active else 0

    # torque-only hardware: no angle channel to fall back on, torque is the only lateral channel
    if not self.angle_supported:
      self.torque_active = lat_active
      self._reset_prearm()
      self.prearm_abort_lockout = 0
      self.eac_dead_frames = 0
      self.lat_active_last = lat_active
      return

    # driver forced full-time torque, or below the configured min angle speed: pin torque, but make
    # this a *make-before-break* handoff when the EPAS is actively holding an angle. Ramp torque up
    # under the still-active angle command (see _update_torque / _update_angle) and only release angle
    # once torque can carry the load, so the wheel is never left unsupported during the STEER_DELTA_UP
    # ramp. eac_dead_frames reset so a stale count doesn't bite when toggling back mid-drive.
    if self.force_torque or self.low_speed_force:
      self.eac_dead_frames = 0
      self.lat_active_last = lat_active
      if not lat_active:
        self.torque_active = False
        self._reset_prearm()
        self.prearm_abort_lockout = 0
        return
      if self.torque_active:
        self._reset_prearm()
        return
      # currently on angle. after a stalled handoff, stay on angle for the lockout before re-attempting
      if self.prearm_abort_lockout > 0:
        self.prearm_abort_lockout -= 1
        self._reset_prearm()
        return
      # decide whether to overlap (prearm) or switch immediately
      epas_holding = CS.eac_status == 2
      driver_took_over = self.hands_on and CS.out.steeringPressed
      steer_max = round(float(np.interp(CS.out.vEgoRaw, CCP.STEER_MAX_LOOKUP[0], CCP.STEER_MAX_LOOKUP[1])))
      hold_target = abs(int(round(float(actuators.torque) * steer_max)))
      if not epas_holding or driver_took_over or hold_target < TORQUE_PREARM_MIN_HOLD:
        # EPAS not holding (nothing to lose), driver already steering, or a near-straight wheel: switch now
        self.torque_active = True
        self._reset_prearm()
        return
      # overlap: torque ramps underneath the held angle
      self.torque_prearm = True
      self.prearm_frames += 1
      # track ramp progress: a new torque peak clears the stall count; no new peak = the ramp is stuck
      if abs(self.apply_torque_last) > self.prearm_torque_peak:
        self.prearm_torque_peak = abs(self.apply_torque_last)
        self.prearm_stall_frames = 0
      else:
        self.prearm_stall_frames += 1
      reached = abs(self.apply_torque_last) >= TORQUE_PREARM_EXIT_FRAC * hold_target
      stalled = self.prearm_stall_frames >= TORQUE_PREARM_STALL_FRAMES
      if reached:
        # bumpless: torque now carries the curve, release the angle servo. This is the exit on every
        # healthy ramp regardless of curve sharpness (relative threshold), not the frame cap.
        self.torque_active = True
        self._end_prearm("reached", hold_target)
      elif self.prearm_frames >= TORQUE_PREARM_MAX_FRAMES:
        # absolute backstop (~1.5s): cannot hold both channels forever. If the ramp stalled below the
        # hold torque, releasing angle now would reproduce the jerk, so ABORT back to angle (the servo
        # keeps holding the wheel) and lock out re-entry briefly; otherwise complete the switch. A stall
        # on a real curve should be rare (driver override is handled above).
        if stalled:
          self._end_prearm("abort", hold_target)
          self.prearm_abort_lockout = TORQUE_PREARM_ABORT_LOCKOUT
        else:
          self.torque_active = True
          self._end_prearm("backstop", hold_target)
      return

    # EPAS available and no published EacErrorCode
    epas_ready = CS.eac_status == 1 and CS.eac_error_code == 0
    # is the EPAS actually steering on angle
    eac_active = CS.eac_status == 2
    # how far the wheel is from the angle openpilot wants
    gap = abs(desired_angle - CS.out.steeringAngleDeg)

    if not lat_active:
      self.torque_active = False
    # enter torque the moment the driver touches the wheel, which is when the EPAS drops angle control
    elif self.hands_on and CS.out.steeringPressed:
      self.torque_active = True
    # EPAS lost angle and won't recover, torque re-arms it
    elif self.eac_dead_frames >= EAC_RECOVER_FRAMES:
      self.torque_active = True
    # fresh engage while EPAS is not ready yet
    elif not self.lat_active_last and not epas_ready:
      self.torque_active = True
    # hand back to angle once hands off and the wheel is settled near the commanded angle
    elif self.torque_active and self.torque_active_frames >= MIN_TORQUE_FRAMES and not self.hands_on and epas_ready:
      fw_max = float(np.interp(CS.out.vEgoRaw, EPAS_FW_MAX_ANGLE_BP, EPAS_FW_MAX_ANGLE_V)) * EPAS_FW_ANGLE_MARGIN
      in_envelope = abs(CS.out.steeringAngleDeg) < fw_max
      # only once the wheel motion fits the EPAS rate budget
      thr_dps = float(np.interp(CS.out.vEgoRaw, EPAS_FW_RATE_BP, EPAS_FW_RATE_V)) * 100.0
      lo, hi = self.rate_budget.bounds(thr_dps, EPAS_FW_RATE_MARGIN)
      rate_settled = lo <= CS.out.steeringAngleDeg <= hi and abs(CS.out.steeringRateDeg) < UNWIND_HANDOFF_RATE
      if in_envelope and rate_settled and gap < HANDOFF_EXIT_DEG:
        self.torque_active = False

    # count consecutive frames we are trying to steer on angle but the EPAS EAC is not active
    if lat_active and not self.torque_active and not eac_active:
      self.eac_dead_frames += 1
    else:
      self.eac_dead_frames = 0

    self.lat_active_last = lat_active

  def _update_angle(self, CS, lat_active: bool, desired_angle: float):
    self.angle_active = lat_active and not self.torque_active

    apply_angle = desired_angle

    # use future v_ego so the jerk limit ramps the angle down before the lat-accel envelope shrinks
    v_lookahead = max(CS.out.vEgoRaw + max(CS.out.aEgo, 0.0), 1.0)
    apply_angle = apply_steer_angle_limits_vm(apply_angle, self.apply_angle_last, v_lookahead,
                                              CS.out.steeringAngleDeg, self.angle_active, CCP, self.VM)

    if self.angle_active:
      # EPAS absolute envelope
      fw_max = float(np.interp(CS.out.vEgoRaw, EPAS_FW_MAX_ANGLE_BP, EPAS_FW_MAX_ANGLE_V)) * EPAS_FW_ANGLE_MARGIN
      apply_angle = float(np.clip(apply_angle, -fw_max, fw_max))

      # EPAS windowed rate budget
      thr_dps = float(np.interp(CS.out.vEgoRaw, EPAS_FW_RATE_BP, EPAS_FW_RATE_V)) * 100.0
      lo, hi = self.rate_budget.bounds(thr_dps, EPAS_FW_RATE_MARGIN)
      apply_angle = float(np.clip(apply_angle, lo, hi))

      # panda's per-frame jerk limit
      step = get_max_angle_delta_vm(max(CS.out.vEgoRaw, 1.0), self.VM, CCP) * PANDA_STEP_MARGIN
      apply_angle = float(np.clip(apply_angle, self.apply_angle_last - step, self.apply_angle_last + step))

    self.apply_angle_last = apply_angle
    self.rate_budget.push(apply_angle)

  def _update_torque(self, CS, actuators):
    if not (self.torque_active or self.torque_prearm):
      self.apply_torque_last = 0
      self.torque_cmd = 0
      self.toi_act_cmd = False
      self.toi_angle_limit_counter = 0
      return

    v_ego = CS.out.vEgoRaw
    steer_max = round(float(np.interp(v_ego, CCP.STEER_MAX_LOOKUP[0], CCP.STEER_MAX_LOOKUP[1])))
    new_torque = int(round(float(actuators.torque) * steer_max))
    apply_torque = apply_driver_steer_torque_limits(new_torque, self.apply_torque_last,
                                                    CS.out.steeringTorque, CCP, steer_max)

    if abs(CS.out.steeringAngleDeg) > HIGH_ANGLE_THRESHOLD_DEG:
      cap = int(round(steer_max * HIGH_ANGLE_CAP_FRAC))
      apply_torque = max(-cap, min(cap, apply_torque))

    # blip the TOI request when held at high angle so the EPAS does not latch ToiFlt.
    # apply_torque_last is FROZEN through the blip so torque resumes at the pre-blip value
    # (no assist sawtooth); the panda holds last torque for its rate limit through a
    # tolerated steer_req cut, so the resume passes safety (dev-shipped behavior).
    self.toi_angle_limit_counter, toi_act = common_fault_avoidance(
      abs(CS.out.steeringAngleDeg) >= TOI_MAX_ANGLE_DEG, self.torque_active or self.torque_prearm,
      self.toi_angle_limit_counter, TOI_MAX_ANGLE_FRAMES, TOI_BLIP_FRAMES)
    self.toi_act_cmd = toi_act
    if toi_act:
      self.apply_torque_last = apply_torque
      self.torque_cmd = apply_torque
    else:
      self.torque_cmd = 0
