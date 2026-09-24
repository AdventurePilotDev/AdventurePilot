#!/usr/bin/env python3
"""stg-a inversion branch harness tests.

This branch requires the xnor extreme angle harness (0x1310): without it the car is
dashcamOnly. Torque is the primary control type (xnor inversion); the angle channel
is derived from curvature in ext_controller. Single panda, angle TX on bus 0 only.
"""
import unittest
from types import SimpleNamespace

import numpy as np

from opendbc.can import CANPacker
from opendbc.car import Bus, structs
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.rivian.carcontroller import CarController, LOW_SPEED_TORQUE_HYST_MS
from opendbc.car.rivian.ext_controller import (ExternalController, EAC_RECOVER_FRAMES, MIN_TORQUE_FRAMES,
                                               TOI_MAX_ANGLE_FRAMES, TOI_BLIP_FRAMES, ANGLE_SAT_FRAMES)
from opendbc.car.rivian.ext_controller import (TORQUE_PREARM_ABORT_LOCKOUT, TORQUE_PREARM_EXIT_FRAC, TORQUE_PREARM_MAX_FRAMES,
                                               TORQUE_PREARM_MIN_HOLD)
from opendbc.car.rivian.interface import CarInterface
from opendbc.car.rivian.values import CAR, CarControllerParams, RivianFlags, RivianSafetyFlags
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


def _mock_cs(cp, gen2=False, v_ego=10.0, eac_status=1):
  out = structs.CarState()
  out.gearShifter = GearShifter.drive
  out.vEgo = v_ego
  out.vEgoRaw = v_ego
  return SimpleNamespace(
    out=out,
    acm_lka_hba_cmd={"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0, "ACM_slifOnOffState": 0},
    sccm_wheel_touch=None if gen2 else {"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0,
                                        "SCCM_WheelTouch_CapacitiveValue": 0, "SCCM_WheelTouch_Calibration": 100,
                                        "SCCM_WheelTouch_ResistiveValue": 0},
    vdm_adas_status=[],
    hands_on_level=1,
    eac_status=eac_status,
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

  def test_base_config_no_angle_tx(self):
    # a base harness (no xnor box) must NOT emit 0x110/0x100: the stock ACM still broadcasts
    # them and our copies would collide on counter/checksum
    sent = self._run_one_frame(_get_cp())
    for addr in ANGLE_TX_ADDRS:
      self.assertNotIn((addr, 0), sent)
    self.assertIn((0x120, 0), sent)  # torque LKA still goes out

  def test_reported_torque_is_applied_not_requested(self):
    # Regression for the angle-mode integrator "steering fight" (route 4440a486580ed7c6/...).
    # Rivian is steerControlType=torque, so controlsd sets
    #   steer_limited_by_safety = abs(CC.actuators.torque - CO.actuatorsOutput.torque) > 1e-2
    # and latcontrol_torque freezes the PID integrator on that flag. In angle mode the torque
    # channel is idle (apply_torque == 0) while the angle channel steers, so carOutput torque
    # MUST report the applied 0 -- reporting the request instead makes the flag false, unfreezes
    # the integrator against a discarded output, and it dumps near-full-scale torque on the first
    # handoff to torque mode. Rivian-only invariant, so the fix never touches shared controls code.
    cp = _get_cp(xnor_box=True)
    controller = CarController({Bus.pt: "rivian_primary_actuator"}, cp, structs.CarParamsSP())
    cc = structs.CarControl()
    cc.latActive = True
    cc.enabled = True
    cc.actuators.torque = 1.0  # full-scale torque REQUEST that must not leak into the report
    cc = cc.as_reader()
    cc_sp = structs.CarControlSP()
    cc_sp.mads.available = True
    cs = _mock_cs(cp)  # eac_status=1 -> angle engages, torque channel idle
    new_actuators, _ = controller.update(cc, cc_sp, cs, 0)
    # nothing was applied on the torque channel...
    self.assertEqual(new_actuators.torqueOutputCan, 0)
    # ...so the reported torque must be the applied 0, never the 1.0 request
    self.assertEqual(new_actuators.torque, 0.0)

  def test_saturation_param_written(self):
    # bridge: erc.angle_saturated -> RivianAngleSaturated param (read by CarSpecificEventsSP).
    # Params() is None in the opendbc test env, so inject a fake and drive real saturation.
    cp = _get_cp(xnor_box=True)
    controller = CarController({Bus.pt: "rivian_primary_actuator"}, cp, structs.CarParamsSP())
    writes = {"RivianEnableAngleSteering": True}  # master switch on, so the angle channel stays live

    class _FakeParams:
      def get_bool(self, k): return bool(writes.get(k, False))
      def put_bool(self, k, v): writes[k] = v
      def get(self, k, return_default=False): return "0"
      def put(self, k, v): writes[k] = v
    controller._params = _FakeParams()
    controller._angle_sat_last = None

    cc = structs.CarControl()
    cc.latActive = True
    cc.enabled = True
    cc.actuators.curvature = 0.05  # tight curve -> commanded angle exceeds the deliverable envelope
    cc = cc.as_reader()
    cc_sp = structs.CarControlSP()
    cc_sp.mads.available = True

    controller.update(cc, cc_sp, _mock_cs(cp, v_ego=15, eac_status=1), 0)  # engage on angle
    for _ in range(ANGLE_SAT_FRAMES + 5):
      controller.update(cc, cc_sp, _mock_cs(cp, v_ego=15, eac_status=2), 0)
    self.assertTrue(writes.get("RivianAngleSaturated"))


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
                                        "SCCM_WheelTouch_CapacitiveValue": 0, "SCCM_WheelTouch_Calibration": 100,
                                        "SCCM_WheelTouch_ResistiveValue": 0},
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
    # instant resume from the frozen value (may continue ramping) - a ramp-from-0
    # sawtooth would drop post_blip to ~DELTA_UP instead
    self.assertGreaterEqual(post_blip, pre_blip)

  def test_gen2_hands_on_no_crash(self):
    # xnor upstream dereferences sccm_wheel_touch unconditionally; GEN2 leaves it None
    erc = ExternalController(_get_cp(gen2=True, xnor_box=True))
    erc.update(_cs_frame(gen2=True, eac_status=1), True, _actuators())
    for _ in range(10):
      erc.update(_cs_frame(gen2=True, eac_status=2), True, _actuators())
    self.assertTrue(erc.angle_active)

  def test_angle_saturation_sets_flag(self):
    # a curve past the deliverable envelope while turning latches angle_saturated after the debounce
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.update(_cs_frame(v_ego=15, eac_status=1), True, _actuators(curvature=0.05))  # engage on angle
    flags = []
    for _ in range(ANGLE_SAT_FRAMES + 5):
      erc.update(_cs_frame(v_ego=15, eac_status=2), True, _actuators(curvature=0.05))
      flags.append(erc.angle_saturated)
    self.assertFalse(flags[0])         # debounce holds at first
    self.assertTrue(flags[-1])         # latches once saturation is sustained
    self.assertTrue(erc.angle_active)  # still on the angle channel, not handed to torque

  def test_no_saturation_in_envelope(self):
    # turning but reachable: never warns
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.update(_cs_frame(v_ego=15, eac_status=1), True, _actuators(curvature=0.008))
    for _ in range(ANGLE_SAT_FRAMES + 5):
      erc.update(_cs_frame(v_ego=15, eac_status=2), True, _actuators(curvature=0.008))
    self.assertFalse(erc.angle_saturated)

  def test_no_saturation_at_parking_speed(self):
    # steerAtStandstill keeps angle live at crawl; a big angle at 2 m/s is < 1 m/s^2 lateral accel,
    # so it must NOT warn (matches the stock turning gate)
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.update(_cs_frame(v_ego=2, eac_status=1), True, _actuators(curvature=0.2))
    for _ in range(ANGLE_SAT_FRAMES + 5):
      erc.update(_cs_frame(v_ego=2, eac_status=2), True, _actuators(curvature=0.2))
    self.assertFalse(erc.angle_saturated)

  def test_saturation_clears_back_in_envelope(self):
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.update(_cs_frame(v_ego=15, eac_status=1), True, _actuators(curvature=0.05))
    for _ in range(ANGLE_SAT_FRAMES + 5):
      erc.update(_cs_frame(v_ego=15, eac_status=2), True, _actuators(curvature=0.05))
    self.assertTrue(erc.angle_saturated)
    erc.update(_cs_frame(v_ego=15, eac_status=2), True, _actuators(curvature=0.001))
    self.assertFalse(erc.angle_saturated)  # one in-envelope frame resets the debounce


class TestLowSpeedTorqueLatch(unittest.TestCase):
  """CarController-level "always torque below speed" latch: single-sided hysteresis driven by
  vEgo - enter torque immediately below the set speed, release only once LOW_SPEED_TORQUE_HYST_MS
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


class TestMakeBeforeBreakHandoff(unittest.TestCase):
  """The forced angle->torque handoff (low-speed threshold or the force-torque toggle).

  Torque ramps up underneath a still-active angle command and the angle servo is only released
  once torque can carry the curve. Four road tests exercised the 'reached', 'hold < MIN_HOLD' and
  'driver_took_over' exits; the stall -> abort-to-angle backstop is signed off here instead,
  because on road it is pre-empted by driver_took_over in 5-27 frames and is only reachable when a
  driver applies counter-torque that trips NEITHER hands-on detector (torsion below 4.0 AND no
  capacitive touch). These tests reproduce exactly that gap.
  """

  V_EGO = 10.0
  # STEER_MAX_LOOKUP interpolated at V_EGO, matching ext_controller's own steer_max
  STEER_MAX = round(float(np.interp(V_EGO, CarControllerParams.STEER_MAX_LOOKUP[0],
                                    CarControllerParams.STEER_MAX_LOOKUP[1])))
  # counter-torque that zeroes the allowed torque via the driver limiter while staying below the
  # 4.0 torsion threshold, so hands_on never latches: driver_max_torque <= 0 needs d <= -2.88
  STALL_DRIVER_TORQUE = -3.0

  def _erc(self):
    erc = ExternalController(_get_cp(xnor_box=True))
    erc.low_speed_force = True
    return erc

  def _hold(self, hold_target):
    """actuators.torque that yields the requested hold_target"""
    return _actuators(torque=hold_target / self.STEER_MAX)

  def _run(self, erc, frames, hold_target=200, driver_torque=0.0, pressed=False,
           eac_status=2, hands_on_level=1, angle=10.0):
    """drive the controller and record the per-frame overlap state"""
    trace = []
    for _ in range(frames):
      erc.update(_cs_frame(angle=angle, torque=driver_torque, pressed=pressed, v_ego=self.V_EGO,
                           eac_status=eac_status, hands_on_level=hands_on_level),
                 True, self._hold(hold_target))
      trace.append(SimpleNamespace(prearm=erc.torque_prearm, torque_active=erc.torque_active,
                                   angle_active=erc.angle_active, torque=erc.apply_torque_last,
                                   toi=erc.toi_act_cmd, lockout=erc.prearm_abort_lockout))
    return trace

  def test_stalled_ramp_aborts_back_to_angle(self):
    # THE ABORT PATH. Driver counter-torque below the torsion threshold with no capacitive touch:
    # the driver limiter pins applied torque at 0, so the ramp never makes a new peak, and at the
    # frame cap the handoff must ABORT back to angle rather than release into an under-torqued wheel.
    erc = self._erc()
    trace = self._run(erc, TORQUE_PREARM_MAX_FRAMES, driver_torque=self.STALL_DRIVER_TORQUE, pressed=True)

    # hands_on must NOT have latched - that is what makes this the abort case and not driver_took_over
    self.assertFalse(erc.hands_on, "torsion/capacitive tripped; this is no longer the stall case")
    # the ramp really did stall
    self.assertTrue(all(f.torque == 0 for f in trace), "driver limiter did not pin the ramp at 0")
    # aborted, not released: back on angle, torque channel dropped, and the outcome recorded
    self.assertEqual(erc.prearm_last_outcome, "abort")
    self.assertFalse(erc.torque_active, "released into an under-torqued wheel instead of aborting")
    self.assertFalse(erc.torque_prearm)
    self.assertEqual(erc.prearm_abort_lockout, TORQUE_PREARM_ABORT_LOCKOUT)
    # and the angle servo held the wheel for every frame of the attempt, including the abort frame
    self.assertTrue(all(f.angle_active for f in trace), "wheel was left unsupported during the overlap")

  def test_abort_holds_angle_through_lockout_then_reattempts(self):
    # after aborting, stay on angle for the lockout before trying again; once the driver stops
    # fighting, the retry completes normally
    erc = self._erc()
    self._run(erc, TORQUE_PREARM_MAX_FRAMES, driver_torque=self.STALL_DRIVER_TORQUE, pressed=True)
    self.assertEqual(erc.prearm_last_outcome, "abort")

    # lockout: still fighting, must stay on angle and must not start a new overlap
    lockout = self._run(erc, TORQUE_PREARM_ABORT_LOCKOUT, driver_torque=self.STALL_DRIVER_TORQUE, pressed=True)
    self.assertTrue(all(f.angle_active and not f.torque_active for f in lockout))
    self.assertTrue(all(not f.prearm for f in lockout), "re-attempted the handoff inside the lockout")
    self.assertEqual(erc.prearm_abort_lockout, 0)

    # driver releases: the next attempt is a healthy ramp and completes
    retry = self._run(erc, TORQUE_PREARM_MAX_FRAMES)
    self.assertTrue(erc.torque_active, "never re-attempted the handoff after the lockout expired")
    self.assertEqual(erc.prearm_last_outcome, "reached")
    self.assertTrue(any(f.prearm for f in retry), "switched without a make-before-break overlap")

  def test_steering_pressed_alone_is_not_driver_took_over(self):
    # seg41 of route 00000004 sat in exactly this gap: steeringPressed true and real counter-torque,
    # but 3.3 Nm is below the 4.0 torsion threshold, so only the capacitive sensor could catch it.
    # With no capacitive touch the handoff must fall through to the stall/abort backstop, NOT switch.
    erc = self._erc()
    trace = self._run(erc, TORQUE_PREARM_MAX_FRAMES - 1, driver_torque=self.STALL_DRIVER_TORQUE, pressed=True)
    self.assertTrue(all(f.prearm for f in trace[1:]), "steeringPressed alone ended the overlap early")
    self.assertFalse(erc.torque_active)

  def test_healthy_ramp_reaches_before_the_frame_cap(self):
    # the design invariant the frame cap depends on: for every achievable hold_target, including the
    # sharpest possible low-speed curve, the relative 0.85 exit fires strictly before the backstop.
    # If this fails, the cap has been tuned into a curve-dependent limit (see the ext_controller note).
    for hold_target in (TORQUE_PREARM_MIN_HOLD, 60, 120, 250, self.STEER_MAX):
      with self.subTest(hold_target=hold_target):
        erc = self._erc()
        trace = self._run(erc, TORQUE_PREARM_MAX_FRAMES, hold_target=hold_target)
        overlap = [i for i, f in enumerate(trace) if f.prearm]
        self.assertEqual(erc.prearm_last_outcome, "reached")
        self.assertTrue(erc.torque_active)
        self.assertLess(erc.prearm_last_frames, TORQUE_PREARM_MAX_FRAMES,
                        "healthy ramp hit the absolute backstop instead of the fractional exit")
        # angle held for the whole overlap, and released the frame it ended
        self.assertTrue(all(trace[i].angle_active for i in overlap))
        self.assertFalse(trace[overlap[-1] + 1].angle_active)
        # torque carried at least the exit fraction of the curve before angle let go
        self.assertGreaterEqual(erc.prearm_last_peak, TORQUE_PREARM_EXIT_FRAC * hold_target)

  def test_overlap_ramp_is_panda_legal(self):
    # both channels are commanded at once during the overlap; the torque ramp underneath must still
    # be the ordinary +STEER_DELTA_UP/frame engagement ramp that the panda torque limits allow
    erc = self._erc()
    trace = self._run(erc, TORQUE_PREARM_MAX_FRAMES, hold_target=self.STEER_MAX)
    overlap = [f.torque for f in trace if f.prearm]
    self.assertGreater(len(overlap), 1)
    # trace samples after update(), so the first overlap frame already holds one step off zero
    self.assertLessEqual(abs(overlap[0]), CarControllerParams.STEER_DELTA_UP,
                         "torque channel warm-started instead of ramping from 0")
    steps = np.diff(np.abs(np.array(overlap)))
    self.assertLessEqual(steps.max(), CarControllerParams.STEER_DELTA_UP)

  def test_near_straight_switches_immediately(self):
    # nothing for the servo to hold: switch without an overlap
    erc = self._erc()
    trace = self._run(erc, 30, hold_target=TORQUE_PREARM_MIN_HOLD - 1)
    self.assertTrue(erc.torque_active)
    self.assertFalse(any(f.prearm for f in trace), "opened an overlap for a straight wheel")

  def test_epas_not_holding_switches_immediately(self):
    # EPAS is not actively steering, so there is no angle hold to preserve
    erc = self._erc()
    trace = self._run(erc, 30, eac_status=1)
    self.assertTrue(erc.torque_active)
    self.assertFalse(any(f.prearm for f in trace))

  def test_driver_took_over_switches_immediately(self):
    # hands_on (here via the EPAS hands-on level) + steeringPressed: the driver already has the
    # wheel, so hand the channel over at once rather than holding the servo against them
    erc = self._erc()
    trace = self._run(erc, 30, pressed=True, hands_on_level=2)
    self.assertTrue(erc.hands_on)
    self.assertTrue(erc.torque_active)
    self.assertFalse(any(f.prearm for f in trace))

  def test_lat_inactive_drops_both_channels(self):
    erc = self._erc()
    self._run(erc, 5)
    self.assertTrue(erc.torque_prearm or erc.torque_active)
    for _ in range(5):
      erc.update(_cs_frame(v_ego=self.V_EGO, eac_status=2), False, self._hold(200))
    self.assertFalse(erc.torque_active)
    self.assertFalse(erc.torque_prearm)
    self.assertFalse(erc.angle_active)
    self.assertEqual(erc.prearm_abort_lockout, 0)


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
