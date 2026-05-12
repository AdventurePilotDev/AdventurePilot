import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.rivian.riviancan import (
  create_acm_status_hwp,
  create_adas_status,
  create_angle_steering,
  create_longitudinal,
  create_wheel_touch,
)
from opendbc.car.rivian.values import CarControllerParams, RivianFlags

from opendbc.sunnypilot.car.rivian.mads import MadsCarController

# openpilot bus indices: int panda CAN0/2 = 0/2, ext panda CAN0/2 = 4/6
ANGLE_TX_BUSES = (0, 4)      # car-side: int + ext panda → EPAS / front-object FD car side
ACM_STATUS_TX_BUSES = (2, 6)  # ACM-side: int + ext panda → HWP injection


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.packer = CANPacker(dbc_names[Bus.pt])

    self.cancel_frames = 0

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    # EPAS external angle control: send 0x110 ACM_SteeringControl on the car-side
    # bus of both pandas, and 0x100 ACM_Status (FeatureStatus=Hwp) on the ACM-side
    # bus of both pandas. Only send while lat_active so the (closed) relay path
    # is left to stock ACM when openpilot isn't steering.
    angle_deg = float(actuators.steeringAngleDeg) if self.mads.lat_active else 0.0
    if self.mads.lat_active:
      for bus in ANGLE_TX_BUSES:
        can_sends.append(create_angle_steering(self.packer, self.frame, angle_deg, True, bus))
      for bus in ACM_STATUS_TX_BUSES:
        can_sends.append(create_acm_status_hwp(self.packer, self.frame, bus))

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
