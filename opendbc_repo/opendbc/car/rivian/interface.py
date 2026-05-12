from opendbc.car import get_safety_config, structs
from opendbc.car.interfaces import CarInterfaceBase
from opendbc.car.rivian.carcontroller import CarController
from opendbc.car.rivian.carstate import CarState
from opendbc.car.rivian.radar_interface import RadarInterface
from opendbc.car.rivian.values import RivianFlags, RivianSafetyFlags


class CarInterface(CarInterfaceBase):
  CarState = CarState
  CarController = CarController
  RadarInterface = RadarInterface

  @staticmethod
  def _get_params(ret: structs.CarParams, candidate, fingerprint, car_fw, alpha_long, is_release, docs) -> structs.CarParams:
    ret.brand = "rivian"

    # Dual-panda setup: int panda relay-cuts the primary actuator bus, ext panda
    # relay-cuts the front-object FD bus. Both inject the same 0x110 angle command
    # (car side) and 0x100 ACM_Status HWP override (ACM side) so the EPAS accepts
    # external angle control. The SECONDARY_TX safety flag selects the ext-only
    # TX whitelist on the second panda.
    ret.safetyConfigs = [
      get_safety_config(structs.CarParams.SafetyModel.rivian),
      get_safety_config(structs.CarParams.SafetyModel.rivian, RivianSafetyFlags.SECONDARY_TX.value),
    ]

    # GEN2 (2025+) doesn't have SCCM_WheelTouch on the bus
    if 0x321 not in fingerprint[0]:
      ret.flags |= RivianFlags.GEN2.value

    ret.steerActuatorDelay = 0.15
    ret.steerLimitTimer = 0.4

    ret.steerControlType = structs.CarParams.SteerControlType.angle
    ret.radarUnavailable = False
    ret.enableBsm = True

    ret.alphaLongitudinalAvailable = True
    if alpha_long:
      ret.openpilotLongitudinalControl = True
      ret.safetyConfigs[0].safetyParam |= RivianSafetyFlags.LONG_CONTROL.value

    ret.longitudinalActuatorDelay = 0.35
    ret.vEgoStopping = 0.25
    ret.stopAccel = 0

    return ret

  @staticmethod
  def _get_params_sp(stock_cp: structs.CarParams, ret: structs.CarParamsSP, candidate, fingerprint: dict[int, dict[int, int]],
                     car_fw: list[structs.CarParams.CarFw], alpha_long: bool, is_release_sp: bool, docs: bool) -> structs.CarParamsSP:
    return ret
