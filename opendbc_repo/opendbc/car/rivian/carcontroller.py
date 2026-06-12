import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.rivian.ext_controller import ExternalController
from opendbc.car.rivian.riviancan import create_lka_steering, create_angle_steering, create_acm_status, create_longitudinal, create_wheel_touch, create_adas_status
from opendbc.car.rivian.values import CarControllerParams, RivianFlags, RIVIAN_TUNE
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP

from opendbc.sunnypilot.car.rivian.mads import MadsCarController


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.apply_torque_last = 0
    self.packer = CANPacker(dbc_names[Bus.pt])
    self.cancel_frames = 0

    # Active steering tune (tame/aggressive). Angle control doesn't use the torque cap directly, but
    # the ExternalController's cooperative/handoff torque follows this profile, and steer_max below
    # normalizes the reported torque actuator.
    self.tune = RIVIAN_TUNE[bool(CP_SP.flags & RivianFlagsSP.AGGRESSIVE_TUNE)]

    # The ExternalController owns the angle command (ACM_SteeringControl 0x110) and the
    # cooperative/handoff torque (ACM_lkaHbaCmd 0x120) that gets the EPAS into the angle envelope.
    self.erc = ExternalController(CP, CP_SP)

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    lookup = self.tune['steer_max_lookup']
    steer_max = round(float(np.interp(CS.out.vEgoRaw, lookup[0], lookup[1])))

    self.erc.update(CS, self.mads.lat_active, actuators)
    apply_torque = self.erc.apply_torque_last
    self.apply_torque_last = apply_torque

    # ACM_Status: 0 = off, 1 = cooperative torque active, 2 = angle active
    feature_status = (1 if self.erc.torque_active else 2) if self.mads.lat_active else 0
    can_sends.append(create_lka_steering(self.packer, self.frame, CS.acm_lka_hba_cmd, apply_torque,
                                         CC.enabled, self.erc.torque_active, self.mads, self.erc.torque_active))
    can_sends.append(create_angle_steering(self.packer, self.frame, self.erc.apply_angle_last, self.erc.angle_active))
    can_sends.append(create_acm_status(self.packer, self.frame, feature_status))

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
    new_actuators.steeringAngleDeg = self.erc.apply_angle_last

    self.frame += 1
    return new_actuators, can_sends
