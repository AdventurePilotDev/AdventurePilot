import copy
from opendbc.can import CANParser
from opendbc.car import Bus, structs
from opendbc.car.interfaces import CarStateBase
from opendbc.car.rivian.values import DBC, GEAR_MAP
from opendbc.car.common.conversions import Conversions as CV
from opendbc.sunnypilot.car.rivian.carstate_ext import CarStateExt

GearShifter = structs.CarState.GearShifter


class CarState(CarStateBase, CarStateExt):
  def __init__(self, CP, CP_SP):
    CarStateBase.__init__(self, CP, CP_SP)
    CarStateExt.__init__(self, CP, CP_SP)
    self.last_speed = 30

    self.acm_lka_hba_cmd = None

  def update(self, can_parsers) -> tuple[structs.CarState, structs.CarStateSP]:
    cp = can_parsers[Bus.pt]
    cp_cam = can_parsers[Bus.cam]
    cp_adas = can_parsers[Bus.adas]
    ret = structs.CarState()
    ret_sp = structs.CarStateSP()

    # Vehicle speed
    ret.vEgoRaw = cp.vl["ESP_Status"]["ESP_Vehicle_Speed"] * CV.KPH_TO_MS
    ret.vEgo, ret.aEgo = self.update_speed_kf(ret.vEgoRaw)
    ret.standstill = abs(ret.vEgoRaw) < 0.01
    conversion = CV.KPH_TO_MS if cp.vl["Cluster"]["Cluster_Unit"] == 0 else CV.MPH_TO_MS
    ret.vEgoCluster = cp.vl["Cluster"]["Cluster_VehicleSpeed"] * conversion

    # Gas pedal
    ret.gasPressed = cp.vl["VDM_PropStatus"]["VDM_AcceleratorPedalPosition"] > 0

    # Brake pedal
    ret.brakePressed = cp.vl["ESP_AebFb"]["iB_BrakePedalApplied"] == 1
    ret.brake = 1.0 if ret.brakePressed else 0.0  # use brake pressed flag for pressure

    # Steering wheel
    ret.steeringAngleDeg = cp_adas.vl["EPAS_AdasStatus"]["EPAS_InternalSas"]
    ret.steeringRateDeg = cp_adas.vl["EPAS_AdasStatus"]["EPAS_SteeringAngleSpeed"]
    ret.steeringTorque = cp.vl["EPAS_SystemStatus"]["EPAS_TorsionBarTorque"]
    ret.steeringPressed = self.update_steering_pressed(abs(ret.steeringTorque) > 1.0, 5)

    # EPAS_HandsOnLevel: 1 = normal/hands-on; any other value is a car-reported hands-off fault
    hands_on_level = cp.vl["EPAS_SystemStatus"]["EPAS_HandsOnLevel"]
    ret.steerFaultTemporary = cp.vl["EPAS_SystemStatus"]["H_CAN_EPSS_ToiFlt"] != 0 or hands_on_level != 1

    # Cruise state
    speed = min(int(cp_cam.vl["ACM_tsrCmd"]["ACM_tsrSpdDisClsMain"]), 85)
    self.last_speed = speed if speed != 0 else self.last_speed
    ret.cruiseState.enabled = cp_adas.vl["ACM_Status"]["ACM_FeatureStatus"] == 1
    # TODO: find cruise set speed on CAN
    ret.cruiseState.speed = self.last_speed * CV.MPH_TO_MS  # detected speed limit
    if not self.CP.openpilotLongitudinalControl:
      ret.cruiseState.speed = -1
    ret.cruiseState.available = True  # VDM_AdasSts not available on this tap
    ret.cruiseState.standstill = ret.standstill  # infer from vehicle speed

    # ACM_Status->ACM_FaultSupervisorState normally 1, appears to go to 3 when either:
    # 1. car in park/not in drive (normal)
    # 2. something (message from another ECU) ACM relies on is faulty
    #  * ACM_FaultStatus will stay 0 since ACM itself isn't faulted
    # TODO: ACM_FaultStatus hasn't been seen high yet, but log anyway
    ret.accFaulted = cp_adas.vl["ACM_Status"]["ACM_FaultStatus"] == 1
                      # VDM_AdasSts not available on this tap, removed VDM_AdasFaultStatus check

    # Gear
    ret.gearShifter = GEAR_MAP.get(int(cp.vl["VDM_PropStatus"]["VDM_Prndl_Status"]), GearShifter.unknown)

    # Doors
    ret.doorOpen = any(cp_adas.vl["IndicatorLights"][door] != 2 for door in ("RearDriverDoor", "FrontPassengerDoor", "DriverDoor", "RearPassengerDoor"))

    # Blinkers
    ret.leftBlinker = cp_adas.vl["IndicatorLights"]["TurnLightLeft"] in (1, 2)
    ret.rightBlinker = cp_adas.vl["IndicatorLights"]["TurnLightRight"] in (1, 2)

    # Seatbelt - assume latched (RCM_Status not available)
    ret.seatbeltUnlatched = False

    # Blindspot
    # ret.leftBlindspot = False
    # ret.rightBlindspot = False

    # AEB
    ret.stockAeb = cp_adas.vl["ACM_AebRequest"]["ACM_EnableRequest"] != 0

    # Messages needed by carcontroller
    self.acm_lka_hba_cmd = copy.copy(cp_cam.vl["ACM_lkaHbaCmd"])

    CarStateExt.update(self, ret, can_parsers)

    return ret, ret_sp

  @staticmethod
  def get_can_parsers(CP, CP_SP):
    return {
      Bus.pt: CANParser(DBC[CP.carFingerprint][Bus.pt], [], 0),
      Bus.adas: CANParser(DBC[CP.carFingerprint][Bus.pt], [], 1),
      Bus.cam: CANParser(DBC[CP.carFingerprint][Bus.pt], [], 2),
      **CarStateExt.get_parser(CP, CP_SP),
    }
