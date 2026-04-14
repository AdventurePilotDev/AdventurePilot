import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
from opendbc.car.lateral import apply_driver_steer_torque_limits, common_fault_avoidance
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.rivian.riviancan import create_lka_steering, create_longitudinal
from opendbc.car.rivian.values import CarControllerParams

from opendbc.sunnypilot.car.rivian.mads import MadsCarController

# Fault avoidance: one frame with torque=0 and request=0. Panda allows (0,0) through by skipping rate checks when steer_req=0.
MAX_ANGLE_DEG = 90
MAX_ANGLE_FRAMES = 43
BLIP_FRAMES = 1


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.apply_torque_last = 0
    self.packer = CANPacker(dbc_names[Bus.pt])
    self.angle_limit_counter = 0

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    apply_torque = 0
    steer_max = round(float(np.interp(CS.out.vEgoRaw, CarControllerParams.STEER_MAX_LOOKUP[0],
                                      CarControllerParams.STEER_MAX_LOOKUP[1])))
    if self.mads.lat_active:
      new_torque = int(round(CC.actuators.torque * steer_max))
      apply_torque = apply_driver_steer_torque_limits(new_torque, self.apply_torque_last,
                                                      CS.out.steeringTorque, CarControllerParams, steer_max)

    # Fault avoidance: one frame send torque=0 and request=0 (car requires both to not fault)
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

    # Longitudinal control
    if self.CP.openpilotLongitudinalControl:
      accel = float(np.clip(actuators.accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX))
      can_sends.append(create_longitudinal(self.packer, self.frame, accel, CC.enabled))
    # VDM_AdasSts not available on this tap - cannot cancel stock ACC

    new_actuators = actuators.as_builder()
    new_actuators.torque = apply_torque / steer_max
    new_actuators.torqueOutputCan = apply_torque

    self.frame += 1
    return new_actuators, can_sends
