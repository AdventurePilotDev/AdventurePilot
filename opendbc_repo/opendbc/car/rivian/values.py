from dataclasses import dataclass, field
from enum import StrEnum, IntFlag

from opendbc.car import ACCELERATION_DUE_TO_GRAVITY, Bus, CarSpecs, DbcDict, PlatformConfig, Platforms, structs, uds
from opendbc.car.docs_definitions import CarHarness, CarDocs, CarParts
from opendbc.car.fw_query_definitions import FwQueryConfig, Request, StdQueries, p16
from opendbc.car.lateral import AngleSteeringLimitsVM, ISO_LATERAL_ACCEL
from opendbc.car.vin import Vin

AVERAGE_ROAD_ROLL = 0.06  # ~3.4 degrees, 6% superelevation. higher actual roll lowers lateral acceleration


class WMI(StrEnum):
  RIVIAN_TRUCK = "7FC"
  RIVIAN_MPV = "7PD"


class ModelLine(StrEnum):
  R1T = "T"  # R1T 4-door Pickup Truck
  R1S = "S"  # R1S 4-door MPV


class ModelYear(StrEnum):
  N_2022 = "N"
  P_2023 = "P"
  R_2024 = "R"
  S_2025 = "S"
  T_2026 = "T"


@dataclass
class RivianCarDocs(CarDocs):
  package: str = "All"


@dataclass
class RivianPlatformConfig(PlatformConfig):
  dbc_dict: DbcDict = field(default_factory=lambda: {Bus.pt: 'rivian_primary_actuator', Bus.radar: 'rivian_mando_front_radar_generated',
                                                     Bus.alt: 'rivian_park_assist_can'})
  wmis: set[WMI] = field(default_factory=set)
  lines: set[ModelLine] = field(default_factory=set)
  years: set[ModelYear] = field(default_factory=set)


class RivianFlags(IntFlag):
  GEN2 = 1


class RivianSafetyFlags(IntFlag):
  LONG_CONTROL = 1
  SECONDARY_TX = 2  # ext panda: only allow 0x110 angle + 0x100 ACM_Status TX


class CAR(Platforms):
  RIVIAN_R1 = RivianPlatformConfig(
    [
      RivianCarDocs("Rivian R1S 2022-24", setup_video="https://youtu.be/uaISd1j7Z4U", car_parts=CarParts.common([CarHarness.rivian_a])),
      RivianCarDocs("Rivian R1S 2025", car_parts=CarParts.common([CarHarness.rivian_b])),
      RivianCarDocs("Rivian R1T 2022-24", setup_video="https://youtu.be/uaISd1j7Z4U", car_parts=CarParts.common([CarHarness.rivian_a])),
      RivianCarDocs("Rivian R1T 2025", car_parts=CarParts.common([CarHarness.rivian_b])),
    ],
    CarSpecs(mass=3206., wheelbase=3.08, steerRatio=15.2),
    wmis={WMI.RIVIAN_TRUCK, WMI.RIVIAN_MPV},
    lines={ModelLine.R1T, ModelLine.R1S},
    years={ModelYear.N_2022, ModelYear.P_2023, ModelYear.R_2024, ModelYear.S_2025},
  )


def match_fw_to_car_fuzzy(live_fw_versions, vin, offline_fw_versions) -> set[str]:
  # Rivian VIN reference: https://www.rivianforums.com/forum/threads/rivian-vin-decoder.1546
  vin_obj = Vin(vin)
  line = vin_obj.vds[:1]
  year = vin_obj.vis[:1]

  candidates = set()
  for platform in CAR:
    if vin_obj.wmi in platform.config.wmis and line in platform.config.lines and year in platform.config.years:
      candidates.add(platform)

  return {str(c) for c in candidates}


RIVIAN_VERSION_REQUEST = bytes([uds.SERVICE_TYPE.READ_DATA_BY_IDENTIFIER]) + \
  p16(0xf1a0)
RIVIAN_VERSION_RESPONSE = bytes([uds.SERVICE_TYPE.READ_DATA_BY_IDENTIFIER + 0x40])

FW_QUERY_CONFIG = FwQueryConfig(
  requests=[
    Request(
      [StdQueries.TESTER_PRESENT_REQUEST, StdQueries.SUPPLIER_SOFTWARE_VERSION_REQUEST],
      [StdQueries.TESTER_PRESENT_RESPONSE, StdQueries.SUPPLIER_SOFTWARE_VERSION_RESPONSE],
      rx_offset=0x40,
      bus=0,
    ),
    Request(
      [StdQueries.TESTER_PRESENT_REQUEST, StdQueries.MANUFACTURER_ECU_HARDWARE_NUMBER_REQUEST],
      [StdQueries.TESTER_PRESENT_RESPONSE, StdQueries.MANUFACTURER_ECU_HARDWARE_NUMBER_RESPONSE],
      rx_offset=0x40,
      bus=0,
      logging=True,
    ),
    Request(
      [StdQueries.TESTER_PRESENT_REQUEST, RIVIAN_VERSION_REQUEST],
      [StdQueries.TESTER_PRESENT_RESPONSE, RIVIAN_VERSION_RESPONSE],
      rx_offset=0x40,
      bus=0,
      logging=True,
    ),
  ],
  match_fw_to_car_fuzzy=match_fw_to_car_fuzzy,
)

GEAR_MAP = {
  0: structs.CarState.GearShifter.unknown,
  1: structs.CarState.GearShifter.park,
  2: structs.CarState.GearShifter.reverse,
  3: structs.CarState.GearShifter.neutral,
  4: structs.CarState.GearShifter.drive,
}


class CarControllerParams:
  ACCEL_MIN = -3.5  # m/s^2
  ACCEL_MAX = 2.0  # m/s^2

  # Mirror RIVIAN_STEERING_PARAMS/LIMITS in safety/modes/rivian.h. VM-derived
  # rate from MAX_LATERAL_JERK, comfort cap via MAX_ANGLE_RATE for low-speed
  # feel (~250°/s at 100 Hz TX). Lookups empty — VM math handles it.
  ANGLE_LIMITS: AngleSteeringLimitsVM = AngleSteeringLimitsVM(
    360,        # STEER_ANGLE_MAX (deg)
    MAX_LATERAL_ACCEL=ISO_LATERAL_ACCEL + (ACCELERATION_DUE_TO_GRAVITY * AVERAGE_ROAD_ROLL),  # ~3.6 m/s^2
    MAX_LATERAL_JERK=3.0 + (ACCELERATION_DUE_TO_GRAVITY * AVERAGE_ROAD_ROLL),                 # ~3.6 m/s^3
    MAX_ANGLE_RATE=2.5,  # deg/10ms frame = 250°/s comfort cap. min(VM jerk rate, this) keeps it under safety's jerk limit, so no rivian.h mirror needed
  )

  STEER_STEP = 1  # 100 Hz TX

  def __init__(self, CP):
    pass


DBC = CAR.create_dbc_map()
