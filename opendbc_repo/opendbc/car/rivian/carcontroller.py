import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus, DT_CTRL
from opendbc.car.common.filter_simple import FirstOrderFilter
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.lateral import apply_steer_angle_limits_vm
from opendbc.car.rivian.riviancan import (
  create_acm_status,
  create_adas_status,
  create_angle_steering,
  create_longitudinal,
  create_wheel_touch,
)
from opendbc.car.rivian.values import CarControllerParams, RivianFlags
from opendbc.car.vehicle_model import VehicleModel

from opendbc.sunnypilot.car.rivian.mads import MadsCarController

# openpilot bus indices: int panda CAN0/2 = 0/2, ext panda CAN0/2 = 4/6
ANGLE_TX_BUSES = (0, 4)       # car-side: int + ext panda → EPAS / front-object FD car side
ACM_STATUS_TX_BUSES = (0, 4)  # car-side: HWP injection must reach EPAS, not be echoed to the ACM


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.packer = CANPacker(dbc_names[Bus.pt])

    self.apply_angle_last = 0.0
    self.cancel_frames = 0

    # VehicleModel for VM-based angle/rate limiting (mirrors RIVIAN_STEERING_PARAMS
    # in safety/modes/rivian.h). Static CP — paramsd's live steerRatio/stiffness
    # corrections aren't applied here so controller and safety compute matching bounds.
    self.VM = VehicleModel(CP)

    # Low-pass the planner's desired angle before the rate limiter. The model output
    # chatters at ~5-15 Hz in hard turns (analyze_chatter.py: 17 reversals/s on
    # commanded angle at 5-15 m/s on route 00000034); the rate limiter alone can't
    # damp it without also slowing legitimate inputs. Time constant is scheduled on
    # speed: heavy filter at parking/intersection where lat dynamics are gentle and
    # twitch is most visible, transparent at highway where bandwidth matters.
    self.angle_filter = FirstOrderFilter(0.0, 0.2, DT_CTRL, initialized=False)

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    # EPAS external angle control: send 0x110 ACM_SteeringControl and 0x100 ACM_Status
    # on the car-side bus of both pandas so the EPAS sees both the HWP enable and the
    # angle stream. Always stream while onroad — when the relay is open, stock 0x110
    # / 0x100 are cut off, so we replace them at all times and only flip EacEnabled +
    # Hwp FeatureStatus when actively steering. Rate-limit using the same lookup the
    # safety enforces (RIVIAN_STEERING_LIMITS in rivian.h) so the int panda always
    # accepts our TX — otherwise rejected frames create counter gaps and EPAS faults
    # with AngleControlCntr. When inactive, the helper resets to measured angle.
    if self.mads.lat_active:
      # Hyundai schedule: rc=0.2s at v<=5 m/s, 0.1s at 10, 0 at v>=20. Heavier filter
      # at the speeds where chatter is most visible and lat-jerk headroom is high.
      self.angle_filter.update_alpha(float(np.interp(CS.out.vEgoRaw, [5., 10., 20.], [0.2, 0.1, 0.0])))
      desired_angle = self.angle_filter.update(actuators.steeringAngleDeg)
    else:
      # When inactive, keep the filter pinned to measured angle so re-engagement
      # starts from current wheel position with no transient.
      self.angle_filter.x = CS.out.steeringAngleDeg
      self.angle_filter.initialized = True
      desired_angle = actuators.steeringAngleDeg
    self.apply_angle_last = apply_steer_angle_limits_vm(desired_angle, self.apply_angle_last,
                                                        CS.out.vEgoRaw, CS.out.steeringAngleDeg,
                                                        self.mads.lat_active, CarControllerParams, self.VM)
    angle_deg = self.apply_angle_last
    if self.mads.lat_active:
      feature_status = 2  # Hwp
    else:
      feature_status = 1 if CS.out.cruiseState.enabled else 0  # mirror stock cruise state

    for bus in ANGLE_TX_BUSES:
      can_sends.append(create_angle_steering(self.packer, self.frame, angle_deg, self.mads.lat_active, bus))
    for bus in ACM_STATUS_TX_BUSES:
      can_sends.append(create_acm_status(self.packer, self.frame, feature_status, bus))

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
    new_actuators.steeringAngleDeg = angle_deg

    self.frame += 1
    return new_actuators, can_sends
