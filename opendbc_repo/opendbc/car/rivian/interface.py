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

    # this branch requires the xnor extreme angle harness (announces 0x1310 on bus 1)
    if 0x1310 in fingerprint[1]:
      ret.flags |= RivianFlags.ANGLE_HARNESS.value
    else:
      ret.dashcamOnly = True

    ret.steerActuatorDelay = 0.15
    # angle control can hold the wheel at standstill; lateral is gated to drive gear in mads.py
    ret.steerAtStandstill = True
    # speed-scheduled lateral curvature low-pass (delay-compensated in modeld); damps the
    # angle plant's crawl-speed limit cycle, off by 8 m/s
    ret.lateralSmoothSeconds = 0.4
    ret.steerLimitTimer = 0.4
    CarInterfaceBase.configure_torque_tune(candidate, ret.lateralTuning)

    # torque is the primary channel (xnor inversion): ext_controller derives the angle
    # from curvature and cooperative torque covers override/handoff
    ret.steerControlType = structs.CarParams.SteerControlType.torque
    ret.radarUnavailable = True

    # TODO: pending finding/handling missing set speed
    ret.alphaLongitudinalAvailable = False
    if alpha_long:
      ret.openpilotLongitudinalControl = True
      ret.safetyConfigs[0].safetyParam |= RivianSafetyFlags.LONG_CONTROL.value

    # Measured command->aEgo lag ~0.25s (route 00000028, xcorr); was 0.1 = under-modeled, so the
    # planner under-anticipates the VDM. 0.2 tightens anticipation (smoother) while staying well under
    # xnor's conservative 0.5 to keep AP's responsive feel. Fall back to 0.15 if it feels laggy on lead-brake.
    ret.longitudinalActuatorDelay = 0.2
    ret.vEgoStopping = 0.25
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
