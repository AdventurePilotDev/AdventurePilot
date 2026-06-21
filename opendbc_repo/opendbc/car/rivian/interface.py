from opendbc.car import get_safety_config, structs
from opendbc.car.interfaces import CarInterfaceBase
from opendbc.car.rivian.carcontroller import CarController
from opendbc.car.rivian.carstate import CarState
from opendbc.car.rivian.radar_interface import RadarInterface
from opendbc.car.rivian.values import RivianFlags, RivianSafetyFlags
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP


class CarInterface(CarInterfaceBase):
  CarState = CarState
  CarController = CarController
  RadarInterface = RadarInterface

<<<<<<< c488ab5f4fa43d6481f26bf1d97a40cc9a5a4271
=======
  def build_secondary_lateral_controller(self, CP_SP, dt):
    # cooperative torque alongside the primary angle path (handoff / driver override)
    from opendbc.car.rivian.ext_controller import build_torque_controller
    return build_torque_controller(self.CP, CP_SP, self, dt)

>>>>>>> 4210a4aaac1940234ae19cb3f7f0521313993816
  @staticmethod
  def _get_params(ret: structs.CarParams, candidate, fingerprint, car_fw, alpha_long, is_release, docs) -> structs.CarParams:
    ret.brand = "rivian"

    ret.safetyConfigs = [get_safety_config(structs.CarParams.SafetyModel.rivian)]

    # GEN2 (2025+) doesn't have SCCM_WheelTouch on the bus
    if 0x321 not in fingerprint[0]:
      ret.flags |= RivianFlags.GEN2.value

<<<<<<< c488ab5f4fa43d6481f26bf1d97a40cc9a5a4271
    ret.steerActuatorDelay = 0.15
    ret.steerLimitTimer = 0.4
    CarInterfaceBase.configure_torque_tune(candidate, ret.lateralTuning)

    ret.steerControlType = structs.CarParams.SteerControlType.torque
=======
    # no angle upgrade installed
    if 0x1310 not in fingerprint[1]:
      ret.dashcamOnly = True

    ret.steerActuatorDelay = 0.3
    ret.lateralSmoothSeconds = 0.4  # speed-scheduled lateral curvature low-pass (delay-compensated in modeld); off by 8 m/s
    ret.steerAtStandstill = True
    ret.steerLimitTimer = 0.4

    ret.steerControlType = structs.CarParams.SteerControlType.angle
>>>>>>> 4210a4aaac1940234ae19cb3f7f0521313993816
    ret.radarUnavailable = True

    # TODO: pending finding/handling missing set speed
    ret.alphaLongitudinalAvailable = False
    if alpha_long:
      ret.openpilotLongitudinalControl = True
      ret.safetyConfigs[0].safetyParam |= RivianSafetyFlags.LONG_CONTROL.value

<<<<<<< c488ab5f4fa43d6481f26bf1d97a40cc9a5a4271
    # Measured command->aEgo lag ~0.25s (route 00000028, xcorr); was 0.1 = under-modeled, so the
    # planner under-anticipates the VDM. 0.2 tightens anticipation (smoother) while staying well under
    # xnor's conservative 0.5 to keep AP's responsive feel. Fall back to 0.15 if it feels laggy on lead-brake.
    ret.longitudinalActuatorDelay = 0.2
    ret.vEgoStopping = 0.25
    ret.stopAccel = -0.2
=======
    # Our long tuning, used in full — xnor's base long values are intentionally NOT retained (his
    # long tuning isn't good). 0.2 actuator delay (measured cmd->aEgo lag ~0.25s, route 00000028)
    # tightens anticipation vs xnor's conservative 0.5 for AP's responsive feel. vEgoStarting /
    # stoppingDecelRate left at the openpilot defaults, exactly as in our ap-dev config (NOT xnor's 0.5/0.8).
    ret.longitudinalActuatorDelay = 0.2
    ret.stopAccel = -0.2
    ret.vEgoStopping = 0.25
>>>>>>> 4210a4aaac1940234ae19cb3f7f0521313993816
    ret.longitudinalTuning.kiBP = [0.]
    ret.longitudinalTuning.kiV = [0.2]

    return ret

  @staticmethod
  def _get_params_sp(stock_cp: structs.CarParams, ret: structs.CarParamsSP, candidate, fingerprint: dict[int, dict[int, int]],
                     car_fw: list[structs.CarParams.CarFw], alpha_long: bool, is_release_sp: bool, docs: bool) -> structs.CarParamsSP:
    if 0x131a in fingerprint[1]:
      ret.flags |= RivianFlagsSP.LONGITUDINAL_HARNESS_UPGRADE.value
      stock_cp.radarUnavailable = False
      stock_cp.enableBsm = True
      stock_cp.alphaLongitudinalAvailable = True

    if alpha_long and stock_cp.alphaLongitudinalAvailable:
      stock_cp.openpilotLongitudinalControl = True
      stock_cp.safetyConfigs[0].safetyParam |= RivianSafetyFlags.LONG_CONTROL.value

    return ret
