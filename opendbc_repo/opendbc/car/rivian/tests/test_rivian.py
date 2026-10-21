import sys
import types
import unittest
from types import SimpleNamespace
from unittest import mock

from opendbc.car import gen_empty_fingerprint, structs
from opendbc.car.rivian.carcontroller import ACM_ENGAGE_TIMEOUT_FRAMES, CarController
from opendbc.car.rivian.fingerprints import FW_VERSIONS
from opendbc.car.rivian.values import CAR, FW_QUERY_CONFIG, WMI, ModelLine, ModelYear, RivianSafetyFlags

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
                         toi_fault=False, torque_tx_refused=False, acm_fault_supervisor_state=0)
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


class TestLongitudinalNeedsHarness(unittest.TestCase):
  """A leftover AlphaLongitudinalEnabled must not turn on openpilot longitudinal without the longitudinal harness:
  panda would drop the 0x162 cancel path while nothing on the truck accepts the 0x160 request."""

  @staticmethod
  def _get(fingerprint_bus1):
    # the SP extension imports openpilot params and MADS helpers, which the Mac test venv does not have
    stubs = {name: types.ModuleType(name) for name in ("openpilot", "openpilot.common", "openpilot.common.params",
                                                       "openpilot.sunnypilot", "openpilot.sunnypilot.mads",
                                                       "openpilot.sunnypilot.mads.helpers")}
    stubs["openpilot.common.params"].Params = object
    stubs["openpilot.sunnypilot.mads.helpers"].MadsSteeringModeOnBrake = object
    stubs["openpilot.sunnypilot.mads.helpers"].read_steering_mode_param = lambda *a, **k: None
    with mock.patch.dict(sys.modules, stubs):
      from opendbc.car.rivian.interface import CarInterface
    fingerprint = gen_empty_fingerprint()
    fingerprint[0] = {0x321: 7}
    fingerprint[1] = dict.fromkeys(fingerprint_bus1, 8)
    CP = CarInterface.get_params(CAR.RIVIAN_R1, fingerprint, [], True, False, False)
    CarInterface.get_params_sp(CP, CAR.RIVIAN_R1, fingerprint, [], True, False, False)
    return CP

  def test_no_harness_no_openpilot_long(self):
    CP = self._get([0x1310])
    self.assertFalse(CP.openpilotLongitudinalControl)
    self.assertFalse(CP.safetyConfigs[0].safetyParam & RivianSafetyFlags.LONG_CONTROL)

  def test_harness_allows_openpilot_long(self):
    CP = self._get([0x1310, 0x131a])
    self.assertTrue(CP.openpilotLongitudinalControl)
    self.assertTrue(CP.safetyConfigs[0].safetyParam & RivianSafetyFlags.LONG_CONTROL)


class TestDeclinedEngagePress(unittest.TestCase):
  """An ACC press openpilot passed on but the stock ACM did not take: report the likely reason once, after 0.5 s."""

  ADAS_SIGNALS = TestBlockedEngagePress.ADAS_SIGNALS

  def _run(self, seq, v_ego=5.0, supervisor_state=0, acc_engages_at=None):
    # seq: (gear, VDM_UserAdasRequest) per frame, padded with idle frames in Drive. Returns (frame, reason) reports.
    CP = structs.CarParams(carFingerprint=CAR.RIVIAN_R1, openpilotLongitudinalControl=True)
    CC = CarController({"pt": "rivian_primary_actuator"}, CP, structs.CarParamsSP())
    CS = SimpleNamespace(acm_lka_hba_cmd={"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0, "ACM_slifOnOffState": 0},
                         sccm_wheel_touch={"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0,
                                           "SCCM_WheelTouch_CapacitiveValue": 0, "SCCM_WheelTouch_Calibration": 100,
                                           "SCCM_WheelTouch_ResistiveValue": 0},
                         toi_fault=False, torque_tx_refused=False, acm_fault_supervisor_state=supervisor_state)
    seq = list(seq) + [(GearShifter.drive, 0)] * (ACM_ENGAGE_TIMEOUT_FRAMES + 10)
    reports = []
    for frame, (gear, request) in enumerate(seq):
      engaged = acc_engages_at is not None and frame >= acc_engages_at
      CS.out = structs.CarState(gearShifter=gear, vEgo=v_ego, vEgoRaw=v_ego)
      CS.out.cruiseState.enabled = engaged
      CS.vdm_adas_status = [dict(dict.fromkeys(self.ADAS_SIGNALS, 0), VDM_UserAdasRequest=request)]
      CC.update(structs.CarControl().as_reader(), structs.CarControlSP(), CS, 0)
      if CC.engage_request_refused is not None:
        reports.append((frame, CC.engage_request_refused))
        CC.engage_request_refused = None
    return reports

  def test_low_speed_refusal_is_reported_once(self):
    D = GearShifter.drive
    # press on frame 1, reported ~0.5 s later
    self.assertEqual(self._run([(D, 0), (D, 4), (D, 0)]), [(ACM_ENGAGE_TIMEOUT_FRAMES, 'lowSpeed')])

  def test_not_ready_wins_over_low_speed(self):
    D = GearShifter.drive
    self.assertEqual([r for _, r in self._run([(D, 0), (D, 4), (D, 0)], supervisor_state=3)], ['notReady'])

  def test_acm_takes_the_press(self):
    D = GearShifter.drive
    self.assertEqual(self._run([(D, 0), (D, 4), (D, 0)], acc_engages_at=4), [])

  def test_refused_above_min_speed_for_another_reason_stays_silent(self):
    D = GearShifter.drive
    self.assertEqual(self._run([(D, 0), (D, 4), (D, 0)], v_ego=15.0), [])

  def test_gear_change_out_of_reverse_is_not_watched(self):
    # stalk-down out of R is a gear change, not an ACC press: no "ACC Needs 20 mph" alert after shifting into Drive
    R, D = GearShifter.reverse, GearShifter.drive
    self.assertEqual(self._run([(R, 0), (R, 4), (D, 4), (D, 0)]), [])
