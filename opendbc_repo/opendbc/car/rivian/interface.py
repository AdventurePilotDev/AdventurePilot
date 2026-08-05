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
  def _apply_angle_caps(ret: structs.CarParams) -> None:
    # idempotent, additive: every angle-specific cap lives here so a base/torque truck (Tier A)
    # matches dev exactly. Called only when the xnor extreme angle box (0x1310) is present.
    ret.flags |= RivianFlags.ANGLE_HARNESS.value
    # angle control can hold the wheel at standstill; lateral is gated to drive gear in mads.py
    ret.steerAtStandstill = True
    # speed-scheduled lateral curvature low-pass (delay-compensated in modeld); damps the
    # angle plant's crawl-speed limit cycle, off by 8 m/s
    ret.lateralSmoothSeconds = 0.4
    ret.safetyConfigs[0].safetyParam |= RivianSafetyFlags.ANGLE_CONTROL.value

  @staticmethod
  def _get_params(ret: structs.CarParams, candidate, fingerprint, car_fw, alpha_long, is_release, docs) -> structs.CarParams:
    ret.brand = "rivian"

    ret.safetyConfigs = [get_safety_config(structs.CarParams.SafetyModel.rivian)]

    # GEN2 (2025+) doesn't have SCCM_WheelTouch on the bus
    if 0x321 not in fingerprint[0]:
      ret.flags |= RivianFlags.GEN2.value

    # angle-capable lateral hardware present (xnor extreme box, 0x1310 on bus 1). A base harness
    # is a valid torque-only config, never dashcam; the angle channel + its standstill/smoothing
    # caps are gated to the box (see _apply_angle_caps).
    if 0x1310 in fingerprint[1]:
      CarInterface._apply_angle_caps(ret)

    ret.steerActuatorDelay = 0.15
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

    # Measured command->aEgo lag = 0.26-0.38 s (xcorr across routes c17ea97d 0000000b/00000002, corr 0.98;
    # tools plant_tracking.py). 0.2 was UNDER the plant delay, so the planner under-anticipated the VDM ->
    # commands land ~0.1 s late, which reads as "slow to react" on lead-brake and sluggish on resume. Set to
    # 0.3 to match the measured lag so the command leads the plant correctly. Drop toward 0.25 if it overshoots.
    ret.longitudinalActuatorDelay = 0.3
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
