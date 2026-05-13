import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
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

    # Fixed VehicleModel for carcontroller-side lateral limiting. Kept independent
    # of liveParameters so the safety envelope doesn't shift mid-drive.
    self.VM = VehicleModel(CP)

    self.apply_angle_last = 0.0
    self.cancel_frames = 0

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    # EPAS external angle control: send 0x110 ACM_SteeringControl and 0x100 ACM_Status
    # on the car-side bus of both pandas so the EPAS sees both the HWP enable and the
    # angle stream. Always stream while onroad — when the relay is open, stock 0x110
    # / 0x100 are cut off, so we replace them at all times and only flip EacEnabled +
    # Hwp FeatureStatus when actively steering. VM-based limit clamps the commanded
    # angle to an ISO 11270 lateral-accel + jerk envelope (Tesla pattern), so the
    # planner can't ask for sharper turns than the car should physically take at the
    # current speed — and so per-frame slew is bounded by jerk, not a hand-tuned
    # lookup. When inactive, the helper resets to measured angle.
    self.apply_angle_last = apply_steer_angle_limits_vm(actuators.steeringAngleDeg, self.apply_angle_last,
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
