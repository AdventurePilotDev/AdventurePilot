import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
from opendbc.car.lateral import apply_driver_steer_torque_limits, common_fault_avoidance
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.rivian.riviancan import create_lka_steering, create_longitudinal, create_wheel_touch, create_adas_status
from opendbc.car.rivian.torque_rt import TorqueRtLimiter
from opendbc.car.rivian.values import CarControllerParams, RivianFlags

from opendbc.sunnypilot.car.rivian.mads import MadsCarController

MAX_ANGLE_DEG = 90
MAX_ANGLE_FRAMES = 89
BLIP_FRAMES = 2
# Right turns require more torque to achieve equivalent lateral acceleration (measured asymmetry on R1T/R1S 2023)
# Above this wheel angle the rack is saturated >75% of the time (route data); cap output so the
# controller can recover from saturation faster when geometry eases
HIGH_ANGLE_THRESHOLD_DEG = 90
HIGH_ANGLE_CAP_FRAC = 0.95

# A latched EPAS ToiFlt does not time out: the EPAS ignored torque for 59 s and 75 s on route
# 2bba20cd6136cc27/0000007a--9d80d483b8 while openpilot showed engaged. It clears ~20 ms after any frame with the
# TOI request low (5 of 5 times on that route), so release the request for a blip once it has latched. Not while the
# panda is still refusing torque frames: the EPAS loses 0x120 again and the fault comes straight back.
TOI_CLEAR_LATCH_FRAMES = 3       # ToiFlt seen this many frames in a row before releasing
TOI_CLEAR_QUIET_FRAMES = 5       # and no panda refusal for this many frames
TOI_CLEAR_RETRY_FRAMES = 50      # at most one release every 0.5 s while it stays latched
# A release is itself a blip. Two blips inside one 250 ms panda interval leave its real-time reference older than
# TorqueRtLimiter allows for, and the resume is refused, so keep every release at least this far from any other blip.
TOI_CLEAR_BLIP_GAP_FRAMES = 30


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.apply_torque_last = 0
    self.packer = CANPacker(dbc_names[Bus.pt])
    # set by card: False while selfdrived has a NO_ENTRY event (openpilot would refuse to engage)
    self.openpilot_engageable = True
    # set here when a driver ACC engage request was hidden from the ACM; card reads and clears it
    self.engage_request_blocked = False
    self.engage_request_prev = False
    self.angle_limit_counter = 0
    self.cancel_frames = 0
    self.rt_limiter = TorqueRtLimiter()  # keeps torque requests inside the panda's real-time check
    # releasing the TOI request to clear a latched EPAS ToiFlt
    self.toi_fault_frames = 0
    self.frames_since_refusal = TOI_CLEAR_QUIET_FRAMES
    self.toi_clear_frames = 0    # release frames still to send
    self.toi_clear_cooldown = 0

  def _toi_clear_release(self, CS, lka_act_toi: bool) -> bool:
    """True on the frames where the TOI request must be released to clear a latched EPAS ToiFlt. The release goes out
    as a blip (torque 0, request low), which the panda accepts at any time: it is not a steer_req mismatch."""
    if not self.mads.lat_active:
      self.toi_fault_frames = 0
      self.toi_clear_frames = 0
      self.toi_clear_cooldown = 0
      return False
    self.toi_fault_frames = self.toi_fault_frames + 1 if getattr(CS, "toi_fault", False) else 0
    self.toi_clear_cooldown = max(self.toi_clear_cooldown - 1, 0)
    if (self.toi_clear_frames == 0 and self.toi_clear_cooldown == 0 and self.toi_fault_frames >= TOI_CLEAR_LATCH_FRAMES and
        self.frames_since_refusal >= TOI_CLEAR_QUIET_FRAMES and lka_act_toi and
        self.rt_limiter.frames_since_blip >= TOI_CLEAR_BLIP_GAP_FRAMES):
      self.toi_clear_frames = BLIP_FRAMES
      self.toi_clear_cooldown = TOI_CLEAR_RETRY_FRAMES
      # the release also serves the high-angle blip, so restart that count: the next one is a full interval away
      self.angle_limit_counter = 0
    if self.toi_clear_frames > 0:
      self.toi_clear_frames -= 1
      return True
    return False

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    if getattr(CS, "torque_tx_refused", False):
      # the panda refused a torque frame: it has zeroed its torque memory, so restart from zero
      self.apply_torque_last = 0
      self.rt_limiter.refused()
      self.frames_since_refusal = 0
    else:
      self.frames_since_refusal = min(self.frames_since_refusal + 1, TOI_CLEAR_QUIET_FRAMES)

    apply_torque = 0
    steer_max = round(float(np.interp(CS.out.vEgoRaw, CarControllerParams.STEER_MAX_LOOKUP[0],
                                      CarControllerParams.STEER_MAX_LOOKUP[1])))
    if self.mads.lat_active:
      new_torque = int(round(CC.actuators.torque * steer_max))
      apply_torque = apply_driver_steer_torque_limits(new_torque, self.apply_torque_last,
                                                      CS.out.steeringTorque, CarControllerParams, steer_max)
      if abs(CS.out.steeringAngleDeg) > HIGH_ANGLE_THRESHOLD_DEG:
        cap = int(round(steer_max * HIGH_ANGLE_CAP_FRAC))
        apply_torque = max(-cap, min(cap, apply_torque))
      apply_torque = self.rt_limiter.limit(apply_torque)
    else:
      self.rt_limiter.reset()

    self.angle_limit_counter, lka_act_toi = common_fault_avoidance(
      abs(CS.out.steeringAngleDeg) >= MAX_ANGLE_DEG,
      self.mads.lat_active,
      self.angle_limit_counter,
      MAX_ANGLE_FRAMES,
      BLIP_FRAMES,
    )
    if self._toi_clear_release(CS, lka_act_toi):
      lka_act_toi = False

    blip = self.mads.lat_active and not lka_act_toi
    send_torque = 0 if blip else apply_torque
    if not blip:
      self.apply_torque_last = apply_torque
      if self.mads.lat_active:
        self.rt_limiter.sent(apply_torque)
    else:
      self.rt_limiter.blip()

    can_sends.append(create_lka_steering(self.packer, self.frame, CS.acm_lka_hba_cmd, send_torque, CC.enabled, CC.latActive, self.mads, lka_act_toi))

    if self.frame % 5 == 0 and not (self.CP.flags & RivianFlags.GEN2):
      can_sends.append(create_wheel_touch(self.packer, CS.sccm_wheel_touch, self.mads.lat_active))

    # Stock ACC cancel: openpilot declined (noEntry) or dropped (soft/immediate disable) an engagement while the ACM is
    # in ACC. Tell the ACM the driver cancelled so it exits ACC cleanly; under openpilot long it would otherwise sit in
    # ACC with nothing accepting its long request and latch an ACC fault. The ACM needs to see "available" before it
    # will accept "unavailable"; send "available" right away as the VDM itself takes a few frames to acknowledge.
    interface_status = None
    if CC.cruiseControl.cancel:
      interface_status = 1 if self.cancel_frames < 5 else 0
      self.cancel_frames += 1
    else:
      self.cancel_frames = 0

    # Longitudinal control
    if self.CP.openpilotLongitudinalControl:
      # Keep the acceleration request at exactly zero whenever the panda would refuse it. The panda
      # drops longitudinal permission the moment it sees the driver's brake, the driver's gas, or the
      # stock ACM clearing its feature status, and from then on it rejects every 0x160 whose request
      # is not exactly zero. It reads all three straight off the bus, a frame or two before carControl
      # can react, so the frames openpilot keeps sending in the meantime never reach the VDM at all.
      # The VDM puts up with roughly 30 ms of missing request; past about 40 ms it reports an
      # implausible command, and the ACM can then shut itself down for the rest of the ignition cycle,
      # leaving the truck with no cruise control until it is restarted. Mirroring the panda's own
      # condition here, against this frame's CarState, gets the request to zero in time so the stream
      # never breaks. Nothing about the safety checks changes; openpilot just agrees with them sooner.
      long_allowed = CC.longActive and CS.out.cruiseState.enabled and not CS.out.gasPressed and not CS.out.brakePressed
      if long_allowed:
        # Cancel the VDM's uncompensated regen/creep drag so the truck delivers the accel we ask for
        # (less over-braking, more willing accel). Speed-scheduled, ramps from 0 at standstill so we
        # still hold the brake at a stop. See CarControllerParams.ACCEL_FF_DRAG_*.
        accel = actuators.accel + float(np.interp(CS.out.vEgo, CarControllerParams.ACCEL_FF_DRAG_BP, CarControllerParams.ACCEL_FF_DRAG_V))
        accel = float(np.clip(accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX))
      else:
        accel = 0.0
      # Record what we asked for next to what the car answered, for the ACC fault snapshot.
      # getattr because the car-level tests drive this controller with a stand-in CarState.
      recorder = getattr(CS, "acc_fault_recorder", None)
      if recorder is not None:
        recorder.record_command(accel, CC.enabled, long_allowed)
      can_sends.append(create_longitudinal(self.packer, self.frame, accel, CC.enabled))

      # Forward VDM_AdasSts to the ACM. If openpilot would refuse to engage (NO_ENTRY), hide the driver's ACC-on stalk
      # request so the ACM never enters ACC: once it is in ACC with nothing accepting its long request, it latches an
      # ACC fault that only clears when the car sleeps (a spoofed cancel does not prevent it). Cancels pass through.
      block_engage = not self.openpilot_engageable and not CS.out.cruiseState.enabled
      if CS.vdm_adas_status:
        engage_request = any(msg["VDM_UserAdasRequest"] in (3, 4) for msg in CS.vdm_adas_status)
        # rising edge of a blocked request: card surfaces it so selfdrived shows the noEntry reason
        if block_engage and engage_request and not self.engage_request_prev:
          self.engage_request_blocked = True
        self.engage_request_prev = engage_request
      for msg in CS.vdm_adas_status:
        can_sends.append(create_adas_status(self.packer, msg, interface_status, block_engage, cancel_request=True))
    else:
      for msg in CS.vdm_adas_status:
        can_sends.append(create_adas_status(self.packer, msg, interface_status))

    new_actuators = actuators.as_builder()
    new_actuators.torque = apply_torque / steer_max
    new_actuators.torqueOutputCan = apply_torque

    self.frame += 1
    return new_actuators, can_sends
