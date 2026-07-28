#!/usr/bin/env python3
"""stg-a inversion branch harness tests.

This branch requires the xnor extreme angle harness (0x1310): without it the car is
dashcamOnly. Torque is the primary control type (xnor inversion); the angle channel
is derived from curvature in ext_controller. Single panda, angle TX on bus 0 only.
"""
import unittest
from types import SimpleNamespace

from opendbc.can import CANPacker
from opendbc.car import Bus, structs
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.rivian.carcontroller import CarController, LOW_SPEED_TORQUE_HYST_MS
from opendbc.car.rivian.ext_controller import ExternalController, EAC_RECOVER_FRAMES, MIN_TORQUE_FRAMES, TOI_MAX_ANGLE_FRAMES, TOI_BLIP_FRAMES
from opendbc.car.rivian.interface import CarInterface
from opendbc.car.rivian.values import CAR, RivianFlags, RivianSafetyFlags
from opendbc.sunnypilot.car.rivian.mads import MadsCarController
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP

GearShifter = structs.CarState.GearShifter

ANGLE_TX_ADDRS = {0x110, 0x100}


def _fp(gen2=False, long_kit=False, xnor_box=False, dual_intercept=False):
  fp = {i: {} for i in range(8)}
  if not gen2:
    fp[0][0x321] = 7  # SCCM_WheelTouch
  if long_kit:
    fp[1][0x131a] = 8
  if xnor_box:
    fp[1][0x1310] = 8
  if dual_intercept:
    fp[4][0x390] = 7  # EPAS broadcasts visible via the ext intercept panda
  return fp


def _get_cp(**kwargs):
  return CarInterface.get_params(CAR.RIVIAN_R1, _fp(**kwargs), [], alpha_long=False, is_release=False, docs=False)


class TestHarnessDetection(unittest.TestCase):
  def test_no_angle_harness_is_dashcam(self):
    for cfg in (dict(), dict(long_kit=True), dict(gen2=True)):
      with self.subTest(cfg=cfg):
        cp = _get_cp(**cfg)
        self.assertTrue(cp.dashcamOnly)
        self.assertFalse(cp.flags & RivianFlags.ANGLE_HARNESS)

  def test_xnor_box_unlocks_angle(self):
    for cfg in (dict(xnor_box=True), dict(gen2=True, xnor_box=True)):
      with self.subTest(cfg=cfg):
        cp = _get_cp(**cfg)
        self.assertFalse(cp.dashcamOnly)
        # torque stays the primary control type (xnor inversion)
        self.assertEqual(cp.steerControlType, structs.CarParams.SteerControlType.torque)
        self.assertTrue(cp.flags & RivianFlags.ANGLE_HARNESS)
        self.assertTrue(cp.steerAtStandstill)
        self.assertAlmostEqual(cp.lateralSmoothSeconds, 0.4, places=5)

  def test_single_safety_config(self):
    cp = _get_cp(xnor_box=True)
    self.assertEqual(len(cp.safetyConfigs), 1)

  def test_gen2_detection(self):
    self.assertFalse(_get_cp().flags & RivianFlags.GEN2)
    self.assertTrue(_get_cp(gen2=True).flags & RivianFlags.GEN2)

  def test_long_kit_detection_sp(self):
    for long_kit in (False, True):
      cp = _get_cp(long_kit=long_kit)
      cp_sp = CarInterface.get_params_sp(cp, CAR.RIVIAN_R1, _fp(long_kit=long_kit), [], alpha_long=False, is_release_sp=False, docs=False)
      self.assertEqual(bool(cp_sp.flags & RivianFlagsSP.LONGITUDINAL_HARNESS_UPGRADE), long_kit)
      self.assertEqual(cp.alphaLongitudinalAvailable, long_kit)


def _mock_cs(cp, gen2=False):
  out = structs.CarState()
  out.gearShifter = GearShifter.drive
  out.vEgo = 10.0
  out.vEgoRaw = 10.0
  return SimpleNamespace(
    out=out,
    acm_lka_hba_cmd={"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0, "ACM_slifOnOffState": 0},
    sccm_wheel_touch=None if gen2 else {"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0,
                                        "SCCM_WheelTouch_CapacitiveValue": 0, "SETME_X52": 100},
    vdm_adas_status=[],
    hands_on_level=1,
    eac_status=1,
    eac_error_code=0,
  )


class TestCarControllerTxMatrix(unittest.TestCase):
  """The live stock ACM broadcasts 0x110/0x100 on torque configs; TXing our copies there
  is a counter/checksum collision. This pins exactly which configs emit them."""

  def _run_one_frame(self, cp, gen2=False, lat_active=True):
    cc = structs.CarControl()
    cc.latActive = lat_active
    cc.enabled = lat_active
    cc = cc.as_reader()
    cc_sp = structs.CarControlSP()
    controller = CarController({Bus.pt: "rivian_primary_actuator"}, cp, structs.CarParamsSP())
    cs = _mock_cs(cp, gen2=gen2)
    _, can_sends = controller.update(cc, cc_sp, cs, 0)
    return [(msg[0], msg[2]) for msg in can_sends]

  def test_angle_tx_bus0_only(self):
    sent = self._run_one_frame(_get_cp(xnor_box=True))
    for addr in ANGLE_TX_ADDRS:
      self.assertIn((addr, 0), sent)
    # single-panda branch: no bus-4 mirroring
    self.assertFalse([m for m in sent if m[1] == 4], f"unexpected bus-4 TX: {sent}")
    self.assertIn((0x120, 0), sent)

  def test_angle_stream_continuous_while_inactive(self):
    # the harness relays cut the stock 0x110/0x100: our replacements must stream even
    # when openpilot is inactive (EacEnabled low, 0x100 mirroring stock cruise state)
    sent = self._run_one_frame(_get_cp(xnor_box=True), lat_active=False)
    for addr in ANGLE_TX_ADDRS:
      self.assertIn((addr, 0), sent)

  def test_gen2_frame_does_not_crash(self):
    # GEN2 has no SCCM_WheelTouch: carstate leaves it None; the frame must build anyway
    sent = self._run_one_frame(_get_cp(gen2=True, xnor_box=True), gen2=True)
    self.assertNotIn((0x321, 2), sent)  # and no wheel-touch spoof

  def test_gen1_wheel_touch_spoof(self):
    sent = self._run_one_frame(_get_cp(xnor_box=True))
    self.assertIn((0x321, 2), sent)


def _cs_frame(angle=0.0, rate=0.0, torque=0.0, pressed=False, v_ego=10.0, eac_status=1, hands_on_level=1, gen2=False):
  out = structs.CarState()
  out.gearShifter = GearShifter.drive
  out.vEgo = v_ego
  out.vEgoRaw = v_ego
  out.aEgo = 0.0
  out.steeringAngleDeg = angle
  out.steeringRateDeg = rate
  out.steeringTorque = torque
  out.steeringPressed = pressed
  return SimpleNamespace(
    out=out,
    sccm_wheel_touch=None if gen2 else {"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0,
                                        "SCCM_WheelTouch_CapacitiveValue": 0, "SETME_X52": 100},
    hands_on_level=hands_on_level,
    eac_status=eac_status,
    eac_error_code=0,
  )


def _actuators(curvature=0.0, torque=0.0):
  a = structs.CarControl.Actuators()
  a.curvature = curvature
  a.torque = torque
  return a


class TestExternalController(unittest.TestCase):
  def test_torque_only_pinning(self):
    # without angle hardware the controller collapses to plain torque: never hands off
    erc = ExternalController(_get_cp())
    for _ in range(200):
      erc.update(_cs_frame(), True, _actuators())
    self.assertTrue(erc.torque_active)
    self.assertFalse(erc.angle_active)

  def test_angle_engage_when_epas_ready(self):
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.update(_cs_frame(eac_status=1), True, _actuators())
    self.assertFalse(erc.torque_active)
    self.assertTrue(erc.angle_active)

  def test_force_torque_pins_torque(self):
    # driver forced full-time torque: even with the EPAS ready and actively steering on angle,
    # stay pinned to torque and never engage the angle channel (reuses the torque-only path)
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.force_torque = True
    for _ in range(MIN_TORQUE_FRAMES * 2):
      erc.update(_cs_frame(eac_status=2), True, _actuators())
    self.assertTrue(erc.torque_active)
    self.assertFalse(erc.angle_active)

  def test_force_torque_release_returns_to_angle(self):
    # clearing the toggle mid-drive returns to normal cooperative behavior: hands back to angle
    # once hands-off, EPAS ready and the wheel is settled
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.force_torque = True
    for _ in range(MIN_TORQUE_FRAMES):
      erc.update(_cs_frame(eac_status=2), True, _actuators())
    self.assertTrue(erc.torque_active)
    erc.force_torque = False
    handed_back = False
    for _ in range(MIN_TORQUE_FRAMES * 3):
      erc.update(_cs_frame(eac_status=1), True, _actuators())
      if not erc.torque_active:
        handed_back = True
        break
    self.assertTrue(handed_back)
    self.assertTrue(erc.angle_active)

  def test_low_speed_force_pins_torque(self):
    # "always torque below speed": low_speed_force pins torque even with the EPAS ready and actively
    # steering on angle (reuses the torque-only path, like force_torque)
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.low_speed_force = True
    for _ in range(MIN_TORQUE_FRAMES * 2):
      erc.update(_cs_frame(eac_status=2), True, _actuators())
    self.assertTrue(erc.torque_active)
    self.assertFalse(erc.angle_active)

  def test_low_speed_force_release_returns_to_angle(self):
    # rising above the speed clears low_speed_force and normal cooperative behavior resumes: hands
    # back to angle once hands-off, EPAS ready and the wheel is settled
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.low_speed_force = True
    for _ in range(MIN_TORQUE_FRAMES):
      erc.update(_cs_frame(eac_status=2), True, _actuators())
    self.assertTrue(erc.torque_active)
    erc.low_speed_force = False
    handed_back = False
    for _ in range(MIN_TORQUE_FRAMES * 3):
      erc.update(_cs_frame(eac_status=1), True, _actuators())
      if not erc.torque_active:
        handed_back = True
        break
    self.assertTrue(handed_back)
    self.assertTrue(erc.angle_active)

  def test_eac_dead_falls_back_to_torque(self):
    # EPAS never activates the EAC -> torque re-arms it after EAC_RECOVER_FRAMES
    erc = ExternalController(_get_cp(xnor_box=True))
    for _ in range(EAC_RECOVER_FRAMES + 1):
      erc.update(_cs_frame(eac_status=1), True, _actuators())  # ready but never active (2)
    self.assertTrue(erc.torque_active)

  def test_eac_active_stays_on_angle(self):
    erc = ExternalController(_get_cp(xnor_box=True))
    # real EPAS sequence: ready (1) at engage, then active (2) once EacEnabled is honored
    erc.update(_cs_frame(eac_status=1), True, _actuators())
    for _ in range(EAC_RECOVER_FRAMES * 4):
      erc.update(_cs_frame(eac_status=2), True, _actuators())
    self.assertFalse(erc.torque_active)
    self.assertTrue(erc.angle_active)

  def test_handoff_hysteresis(self):
    # driver touch enters torque; hand-back requires MIN_TORQUE_FRAMES + hands-off + settled
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.update(_cs_frame(eac_status=2), True, _actuators())
    # sustained torsion + pressed -> hands-on -> torque
    for _ in range(15):
      erc.update(_cs_frame(torque=6.0, pressed=True, eac_status=2), True, _actuators())
    self.assertTrue(erc.torque_active)
    # release: settled at 0 deg, gap 0 -> hand back only after MIN_TORQUE_FRAMES
    frames_to_handoff = 0
    for _ in range(MIN_TORQUE_FRAMES * 3):
      erc.update(_cs_frame(eac_status=1), True, _actuators())
      frames_to_handoff += 1
      if not erc.torque_active:
        break
    self.assertFalse(erc.torque_active)
    self.assertGreaterEqual(frames_to_handoff, MIN_TORQUE_FRAMES - 15)

  def test_toi_blip_pattern_and_freeze_resume(self):
    # sustained cooperative torque at high angle: ACM_lkaActToi blips low for
    # TOI_BLIP_FRAMES every ~TOI_MAX_ANGLE_FRAMES; torque_cmd drops to 0 during the blip
    # and resumes at the frozen pre-blip value (no ramp-from-0 sawtooth)
    erc = ExternalController(_get_cp(xnor_box=True))
    history = []
    for _ in range(TOI_MAX_ANGLE_FRAMES + TOI_BLIP_FRAMES + 10):
      erc.update(_cs_frame(angle=120.0, torque=6.0, pressed=True, eac_status=2), True, _actuators(torque=1.0))
      history.append((erc.toi_act_cmd, erc.torque_cmd))
    blip_idxs = [i for i, (toi, _) in enumerate(history) if not toi]
    self.assertEqual(len(blip_idxs), TOI_BLIP_FRAMES, f"expected exactly one {TOI_BLIP_FRAMES}-frame blip: {blip_idxs}")
    self.assertEqual(blip_idxs[1], blip_idxs[0] + 1)
    for i in blip_idxs:
      self.assertEqual(history[i][1], 0)  # torque cut during blip
    pre_blip = history[blip_idxs[0] - 1][1]
    post_blip = history[blip_idxs[-1] + 1][1]
    self.assertGreater(pre_blip, 0)
    # instant resume from the frozen value (may continue ramping) — a ramp-from-0
    # sawtooth would drop post_blip to ~DELTA_UP instead
    self.assertGreaterEqual(post_blip, pre_blip)

  def test_gen2_hands_on_no_crash(self):
    # xnor upstream dereferences sccm_wheel_touch unconditionally; GEN2 leaves it None
    erc = ExternalController(_get_cp(gen2=True, xnor_box=True))
    erc.update(_cs_frame(gen2=True, eac_status=1), True, _actuators())
    for _ in range(10):
      erc.update(_cs_frame(gen2=True, eac_status=2), True, _actuators())
    self.assertTrue(erc.angle_active)


class TestLowSpeedTorqueLatch(unittest.TestCase):
  """CarController-level "always torque below speed" latch: single-sided hysteresis driven by
  vEgo — enter torque immediately below the set speed, release only once LOW_SPEED_TORQUE_HYST_MS
  (3 mph) above it, no dwell. Holds its last state inside the band from either direction."""

  T_MPH = 10.0
  HYST_MPH = LOW_SPEED_TORQUE_HYST_MS / CV.MPH_TO_MS

  def setUp(self):
    self.cp = _get_cp(xnor_box=True)

  def _make(self):
    controller = CarController({Bus.pt: "rivian_primary_actuator"}, self.cp, structs.CarParamsSP())
    # the macOS venv has no openpilot Params, so the frame%50 threshold re-read is skipped and a
    # manually-set _angle_min_speed_ms survives; assert that assumption before relying on it.
    self.assertIsNone(controller._params)
    controller._angle_min_speed_ms = self.T_MPH * CV.MPH_TO_MS
    return controller

  def _step(self, controller, v_mph):
    cc = structs.CarControl()
    cc.latActive = True
    cc.enabled = True
    cc = cc.as_reader()
    cc_sp = structs.CarControlSP()
    cs = _mock_cs(self.cp)
    cs.out.vEgo = v_mph * CV.MPH_TO_MS
    controller.update(cc, cc_sp, cs, 0)
    # the latch state is mirrored onto the ExternalController every frame
    self.assertEqual(controller.erc.low_speed_force, controller._low_speed_torque)
    return controller._low_speed_torque

  def test_enters_torque_immediately_below_set_speed(self):
    controller = self._make()
    self.assertFalse(controller._low_speed_torque)  # starts released
    self.assertTrue(self._step(controller, self.T_MPH - 2.0))  # one frame below T -> latched

  def test_releases_only_above_band(self):
    controller = self._make()
    self._step(controller, self.T_MPH - 2.0)
    self.assertTrue(controller._low_speed_torque)
    # inside the band it holds torque; only clearly above T + hyst does it release
    self.assertTrue(self._step(controller, self.T_MPH + self.HYST_MPH - 0.5))
    self.assertFalse(self._step(controller, self.T_MPH + self.HYST_MPH + 2.0))

  def test_band_holds_torque_when_rising_from_below(self):
    controller = self._make()
    self._step(controller, self.T_MPH - 2.0)  # latched True
    self.assertTrue(self._step(controller, self.T_MPH + self.HYST_MPH / 2.0))  # in band -> holds True

  def test_band_holds_released_when_coming_from_above(self):
    controller = self._make()
    self.assertFalse(self._step(controller, self.T_MPH + self.HYST_MPH + 2.0))  # above -> released
    self.assertFalse(self._step(controller, self.T_MPH + self.HYST_MPH / 2.0))  # in band -> holds False

  def test_feature_off_forces_release(self):
    controller = self._make()
    self._step(controller, self.T_MPH - 2.0)
    self.assertTrue(controller._low_speed_torque)
    controller._angle_min_speed_ms = 0.0  # setting turned off mid-drive
    self.assertFalse(self._step(controller, self.T_MPH - 2.0))  # forced released despite low speed


class TestMadsGearGate(unittest.TestCase):
  def test_lat_active_gated_to_drive(self):
    # angle at standstill must never actuate in reverse/park/neutral (reverse-parking shuffle)
    mads = MadsCarController()
    cc = structs.CarControl()
    cc.latActive = True
    cc_sp = structs.CarControlSP()
    cc_sp.mads.available = True
    for gear, expected in ((GearShifter.drive, True), (GearShifter.reverse, False),
                           (GearShifter.park, False), (GearShifter.neutral, False)):
      cs = SimpleNamespace(out=structs.CarState())
      cs.out.gearShifter = gear
      result = mads.mads_status_update(cc, cc_sp, cs)
      self.assertEqual(result.lat_active, expected, f"gear {gear}")
      # symState must accompany actToi from the first active frame (ToiFlt oscillation fix)
      self.assertEqual(result.lka_icon_states, expected, f"gear {gear}")


if __name__ == "__main__":
  unittest.main()
