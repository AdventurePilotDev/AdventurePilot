from types import SimpleNamespace

import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus, DT_CTRL
from opendbc.car.common.filter_simple import FirstOrderFilter
from opendbc.car.lateral import apply_driver_steer_torque_limits, common_fault_avoidance
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.rivian.riviancan import create_lka_steering, create_longitudinal, create_wheel_touch, create_adas_status
from opendbc.car.rivian.values import CarControllerParams, RivianFlags, RIVIAN_TUNE
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP

from opendbc.sunnypilot.car.rivian.mads import MadsCarController

MAX_ANGLE_DEG = 90
MAX_ANGLE_FRAMES = 89
BLIP_FRAMES = 2
# Above this wheel angle the rack is saturated >75% of the time (route data); cap output so the
# controller can recover from saturation faster when geometry eases
HIGH_ANGLE_THRESHOLD_DEG = 90
HIGH_ANGLE_CAP_FRAC = 0.95


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.apply_torque_last = 0
    self.packer = CANPacker(dbc_names[Bus.pt])
    self.angle_limit_counter = 0
    self.cancel_frames = 0

    # Steering tune profile, fixed at car init from the RivianAggressiveTune param (applied to
    # CP_SP.flags in opendbc/sunnypilot/car/interfaces.py). Default = tame (ap-dev baseline);
    # aggressive = rs-dev R1T tune. Toggling the param takes effect on the next car restart.
    self.tune = RIVIAN_TUNE[bool(CP_SP.flags & RivianFlagsSP.AGGRESSIVE_TUNE)]

    # Low-pass the planner's desired torque before scaling and rate-limiting. The model
    # output chatters at ~5-15 Hz in hard turns (analyze_chatter.py: 17 reversals/s on
    # commanded steering at 5-15 m/s on route 00000034); the driver-torque limiter alone
    # can't damp it without also slowing legitimate inputs. Time constant is scheduled on
    # speed: heavy filter at parking/intersection where lat dynamics are gentle and
    # twitch is most visible, transparent at highway where bandwidth matters.
    self.torque_filter = FirstOrderFilter(0.0, 0.2, DT_CTRL, initialized=False)

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    apply_torque = 0
    lookup = self.tune['steer_max_lookup']
    steer_max = round(float(np.interp(CS.out.vEgoRaw, lookup[0], lookup[1])))
    if self.mads.lat_active:
      if self.tune['use_torque_filter']:
        # Hyundai schedule: rc=0.2s at v<=5 m/s, 0.1s at 10, 0 at v>=20. Heavier filter
        # at the speeds where chatter is most visible and lat-jerk headroom is high.
        self.torque_filter.update_alpha(float(np.interp(CS.out.vEgoRaw, [5., 10., 20.], [0.2, 0.1, 0.0])))
        desired_torque = self.torque_filter.update(CC.actuators.torque)
      else:
        desired_torque = CC.actuators.torque
      new_torque = int(round(desired_torque * steer_max))
      # Rate-up and rate-down come from the active profile (tame 3/5, aggressive 4/7). The EPS
      # faults on di/dt, so rate-down is hardware-capped ~5-7; the aggressive unwind is sped by a
      # lower low-speed cap, not a faster rate. STEER_MAX / driver limits are shared.
      limits = SimpleNamespace(
        STEER_MAX=CarControllerParams.STEER_MAX,
        STEER_DELTA_UP=self.tune['steer_delta_up'],
        STEER_DELTA_DOWN=self.tune['steer_delta_down'],
        STEER_DRIVER_ALLOWANCE=CarControllerParams.STEER_DRIVER_ALLOWANCE,
        STEER_DRIVER_MULTIPLIER=CarControllerParams.STEER_DRIVER_MULTIPLIER,
        STEER_DRIVER_FACTOR=CarControllerParams.STEER_DRIVER_FACTOR,
      )
      apply_torque = apply_driver_steer_torque_limits(new_torque, self.apply_torque_last,
                                                      CS.out.steeringTorque, limits, steer_max)
      if abs(CS.out.steeringAngleDeg) > HIGH_ANGLE_THRESHOLD_DEG:
        cap = int(round(steer_max * HIGH_ANGLE_CAP_FRAC))
        apply_torque = max(-cap, min(cap, apply_torque))
    else:
      # When inactive, keep the filter pinned to zero so re-engagement starts from
      # neutral with no transient.
      self.torque_filter.x = 0.0
      self.torque_filter.initialized = True

    self.angle_limit_counter, lka_act_toi = common_fault_avoidance(
      abs(CS.out.steeringAngleDeg) >= MAX_ANGLE_DEG,
      self.mads.lat_active,
      self.angle_limit_counter,
      MAX_ANGLE_FRAMES,
      BLIP_FRAMES,
    )

    blip = self.mads.lat_active and not lka_act_toi
    send_torque = 0 if blip else apply_torque
    if not blip:
      self.apply_torque_last = apply_torque

    can_sends.append(create_lka_steering(self.packer, self.frame, CS.acm_lka_hba_cmd, send_torque, CC.enabled, CC.latActive, self.mads, lka_act_toi))

    if self.frame % 5 == 0 and not (self.CP.flags & RivianFlags.GEN2):
      can_sends.append(create_wheel_touch(self.packer, CS.sccm_wheel_touch, CC.enabled))

    # Longitudinal control
    if self.CP.openpilotLongitudinalControl:
      accel = float(np.clip(actuators.accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX))
      can_sends.append(create_longitudinal(self.packer, self.frame, accel, CC.enabled))
    else:
      interface_status = None
      if CC.cruiseControl.cancel:
        # if there is a noEntry, we need to send a status of "available" before the ACM will accept "unavailable"
        # send "available" right away as the VDM itself takes a few frames to acknowledge
        interface_status = 1 if self.cancel_frames < 5 else 0
        self.cancel_frames += 1
      else:
        self.cancel_frames = 0

      for msg in CS.vdm_adas_status:
        can_sends.append(create_adas_status(self.packer, msg, interface_status))

    new_actuators = actuators.as_builder()
    new_actuators.torque = apply_torque / steer_max
    new_actuators.torqueOutputCan = apply_torque

    self.frame += 1
    return new_actuators, can_sends
