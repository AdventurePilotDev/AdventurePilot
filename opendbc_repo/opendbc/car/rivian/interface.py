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

  @staticmethod
  def _get_params(ret: structs.CarParams, candidate, fingerprint, car_fw, alpha_long, is_release, docs) -> structs.CarParams:
    ret.brand = "rivian"

    ret.safetyConfigs = [get_safety_config(structs.CarParams.SafetyModel.rivian)]

    # GEN2 (2025+) doesn't have SCCM_WheelTouch on the bus
    if 0x321 not in fingerprint[0]:
      ret.flags |= RivianFlags.GEN2.value

    ret.steerActuatorDelay = 0.15
    ret.steerLimitTimer = 0.4
    CarInterfaceBase.configure_torque_tune(candidate, ret.lateralTuning)

    ret.steerControlType = structs.CarParams.SteerControlType.torque
    ret.radarUnavailable = True

    # TODO: pending finding/handling missing set speed
    ret.alphaLongitudinalAvailable = False
    if alpha_long:
      ret.openpilotLongitudinalControl = True
      ret.safetyConfigs[0].safetyParam |= RivianSafetyFlags.LONG_CONTROL.value

    # Measured command->aEgo lag = 0.26-0.38 s (xcorr across routes c17ea97d 0000000b/00000002, corr 0.98;
    # tools plant_tracking.py). 0.2 was UNDER the plant delay, so the planner under-anticipated the VDM ->
    # commands land ~0.1 s late, which reads as "slow to react" on lead-brake and sluggish on resume. Set to
    # 0.3 to match the measured lag so the command leads the plant correctly. Drop toward 0.25 if it overshoots.
    ret.longitudinalActuatorDelay = 0.3
    # vEgoStopping is NOT settable any more: commaai df1663c58d ("the one true car.capnp") moved it into
    # the deprecated block, so assigning it raises at car init (capnp "struct has no such member"). AP had
    # it at 0.25 vs the old 0.5 default; the stop transition is upstream's now. If the truck starts easing
    # off the brake too early on a stop, that is where it went.
    ret.stopAccel = -0.2
    # kp intentionally left at default (0): a proportional term on (a_target - aEgo) amplifies the noisy
    # low-speed aEgo (d/dt of wheel-speed vEgo) into a ~12 Hz command dither ("stutter"), and it only
    # marginally corrected the VDM's decel bias anyway. That bias is now cancelled deterministically by a
    # speed-scheduled feedforward at the actuator (see CarControllerParams.ACCEL_FF_DRAG_* / carcontroller).
    # ki=0.2 still cleans up any steady-state residual, noise-free.
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
