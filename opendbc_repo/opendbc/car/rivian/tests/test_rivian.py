import unittest
from types import SimpleNamespace

from opendbc.car import structs
from opendbc.car.rivian.carcontroller import CarController
from opendbc.car.rivian.fingerprints import FW_VERSIONS
from opendbc.car.rivian.values import CAR, FW_QUERY_CONFIG, WMI, ModelLine, ModelYear

GearShifter = structs.CarState.GearShifter


class TestRivian(unittest.TestCase):
  def test_custom_fuzzy_fingerprinting(self):
    for platform in CAR:
      with self.subTest(platform=platform.name):
        for wmi in WMI:
          for line in ModelLine:
            for year in ModelYear:
              for bad in (True, False):
                vin = ["0"] * 17
                vin[:3] = wmi
                vin[3] = line.value
                vin[9] = year.value
                if bad:
                  vin[3] = "Z"
                vin = "".join(vin)

                matches = FW_QUERY_CONFIG.match_fw_to_car_fuzzy({}, vin, FW_VERSIONS)
                should_match = year in platform.config.years and not bad
                assert (matches == {platform}) == should_match, "Bad match"


class TestBlockedEngagePress(unittest.TestCase):
  """The ACC stalk is also the gear selector: only a stalk-down in Drive may be reported as a blocked ACC press
  (route ad521a7f5fe6d46a/00000010--2fee7b61ca: D -> R -> D while parking turned MADS on)."""

  ADAS_SIGNALS = ("VDM_AdasStatus_Checksum", "VDM_AdasStatus_Counter", "VDM_AdasDecelLimit", "VDM_AdasDriverAccelPriorityStatus",
                  "VDM_AdasFaultStatus", "VDM_AdasAccelLimit", "VDM_AdasDriverModeStatus", "VDM_AdasUnkown1",
                  "VDM_AdasInterfaceStatus", "VDM_AdasVehicleHoldStatus", "VDM_UserAdasRequest")

  def _run(self, seq, engageable=False):
    # seq: (gear, VDM_UserAdasRequest) per frame. Returns the frames that raised engage_request_blocked.
    CP = structs.CarParams(carFingerprint=CAR.RIVIAN_R1, openpilotLongitudinalControl=True)
    CC = CarController({"pt": "rivian_primary_actuator"}, CP, structs.CarParamsSP())
    CC.openpilot_engageable = engageable
    CS = SimpleNamespace(acm_lka_hba_cmd={"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0, "ACM_slifOnOffState": 0},
                         sccm_wheel_touch={"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0,
                                           "SCCM_WheelTouch_CapacitiveValue": 0, "SCCM_WheelTouch_Calibration": 100,
                                           "SCCM_WheelTouch_ResistiveValue": 0},
                         toi_fault=False, torque_tx_refused=False)
    raised = []
    for frame, (gear, request) in enumerate(seq):
      CS.out = structs.CarState(gearShifter=gear, vEgo=0.3, vEgoRaw=0.3)
      CS.vdm_adas_status = [dict(dict.fromkeys(self.ADAS_SIGNALS, 0), VDM_UserAdasRequest=request)]
      CC.update(structs.CarControl().as_reader(), structs.CarControlSP(), CS, 0)
      if CC.engage_request_blocked:
        raised.append(frame)
        CC.engage_request_blocked = False
    return raised

  def test_gear_changes_are_not_engage_presses(self):
    D, R, P = GearShifter.drive, GearShifter.reverse, GearShifter.park
    self.assertEqual(self._run([(D, 0), (D, 2), (R, 2), (R, 0)]), [])  # D -> R, stalk up
    self.assertEqual(self._run([(R, 0), (R, 4), (D, 4), (D, 0)]), [])  # R -> D, stalk down
    self.assertEqual(self._run([(P, 0), (P, 3), (D, 3), (D, 4), (D, 0)]), [])  # P -> D
    self.assertEqual(self._run([(D, 0), (D, 1), (D, 0)]), [])  # cancel

  def test_acc_press_in_drive_is_reported(self):
    D = GearShifter.drive
    self.assertEqual(self._run([(D, 0), (D, 3), (D, 4), (D, 0)]), [1])
    self.assertEqual(self._run([(D, 0), (D, 4), (D, 0), (D, 4), (D, 0)]), [1, 3])
    # openpilot can engage: the press goes to the ACM, nothing to report
    self.assertEqual(self._run([(D, 0), (D, 4), (D, 0)], engageable=True), [])

