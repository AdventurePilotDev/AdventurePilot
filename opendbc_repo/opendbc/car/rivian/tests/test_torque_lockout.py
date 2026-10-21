import sys
import types
import unittest
from types import SimpleNamespace
from unittest import mock

from opendbc.car.rivian.torque_rt import TorqueRtLimiter, TORQUE_RT_MAX_DELTA, refused_torque_parser
from opendbc.car.rivian.values import DBC, CAR
from opendbc.can import CANPacker
from opendbc.car import Bus, structs
from opendbc.car.rivian.carcontroller import (CarController, BLIP_FRAMES, TOI_CLEAR_LATCH_FRAMES, TOI_CLEAR_QUIET_FRAMES,
                                              TOI_CLEAR_RETRY_FRAMES, TOI_DROPOUT_FRAMES, TOI_DROPOUT_WARN_FRAMES,
                                              TOI_DROPOUT_WARN_HOLD_FRAMES, TOI_START_FRAMES)


class TestTorqueRtLimiter(unittest.TestCase):
  """Keeps torque requests inside the panda's real-time check (route 4440a486580ed7c6/00000112 seg 15)."""

  def test_normal_ramp_is_untouched(self):
    # a full-rate ramp (3 per frame) moves 75 per 250 ms, well inside the margin
    lim = TorqueRtLimiter()
    torque = 0
    for _ in range(200):
      torque += 3
      self.assertEqual(lim.limit(torque), torque)
      lim.sent(torque)

  def test_ramp_is_held_after_a_blip_until_the_panda_refreshes(self):
    lim = TorqueRtLimiter()
    torque = 0
    for _ in range(60):
      torque += 3
      lim.sent(lim.limit(torque))
    pre_blip = torque
    lim.blip()
    lim.blip()
    held = []
    for _ in range(40):
      torque += 3
      out = lim.limit(torque)
      held.append(out)
      lim.sent(out)
    self.assertGreaterEqual(held[0], pre_blip, "resume must not drop below the pre-blip torque")
    # never more than the margin above anything the panda could be holding (a torque from ~28 frames before the blip)
    self.assertLessEqual(max(held[:26]), pre_blip - 3 * 27 + TORQUE_RT_MAX_DELTA)
    self.assertEqual(held[-1], torque, "once the panda has refreshed its reference the ramp is free again")

  def test_refusal_restarts_from_zero(self):
    lim = TorqueRtLimiter()
    for t in range(3, 300, 3):
      lim.sent(t)
    lim.refused()
    self.assertEqual(lim.limit(5000), TORQUE_RT_MAX_DELTA)
    self.assertEqual(lim.limit(-5000), -TORQUE_RT_MAX_DELTA)

  def test_negative_direction_mirrors(self):
    lim = TorqueRtLimiter()
    torque = 0
    for _ in range(60):
      torque -= 3
      lim.sent(lim.limit(torque))
    lim.blip()
    lim.blip()
    out = [lim.limit(torque - 3 * i) for i in range(1, 30)]
    self.assertGreaterEqual(min(out), torque - 3 * 28 - TORQUE_RT_MAX_DELTA - 3)


class TestRefusedParser(unittest.TestCase):
  def _parser(self):
    return refused_torque_parser(DBC[CAR.RIVIAN_R1][Bus.pt])

  def test_only_refused_frames_are_reported(self):
    p = self._parser()
    echo = (0x120, bytes(8), 128)     # an accepted frame comes back on bus 128
    refused = (0x120, bytes(8), 192)  # a refused frame comes back on bus 192
    p.update([(1_000_000_000, [echo])])
    self.assertEqual(p.vl_all["ACM_lkaHbaCmd"]["ACM_lkaStrToqReq"], [])
    p.update([(2_000_000_000, [echo, refused])])
    self.assertEqual(len(p.vl_all["ACM_lkaHbaCmd"]["ACM_lkaStrToqReq"]), 1)
    p.update([(3_000_000_000, [echo])])
    self.assertEqual(p.vl_all["ACM_lkaHbaCmd"]["ACM_lkaStrToqReq"], [])

  def test_quiet_bus_does_not_raise_can_errors(self):
    p = self._parser()
    p.update([(1_000_000_000, [])])
    p.update([(30_000_000_000, [(0x120, bytes([0, 5, 0, 0, 0, 0, 0, 0]), 192)])])  # odd counter, long gap
    self.assertTrue(p.can_valid)
    self.assertFalse(p.bus_timeout)


class TestToiFaultLatchClear(unittest.TestCase):
  """A latched EPAS ToiFlt never times out on its own (59 s and 75 s on route 2bba20cd6136cc27/0000007a--9d80d483b8)
  but clears ~20 ms after a frame with the TOI request low, so the controller releases the request to clear it."""

  def setUp(self):
    self.controller = CarController({"pt": "rivian_primary_actuator"}, structs.CarParams(), structs.CarParamsSP())
    out = structs.CarState()
    out.vEgoRaw = 10.0
    self.cs = SimpleNamespace(out=out, acm_lka_hba_cmd={"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0,
                                                        "ACM_slifOnOffState": 0},
                              sccm_wheel_touch={"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0,
                                                "SCCM_WheelTouch_CapacitiveValue": 0, "SCCM_WheelTouch_Calibration": 100,
                                                "SCCM_WheelTouch_ResistiveValue": 0},
                              vdm_adas_status=[], toi_fault=False, torque_tx_refused=False)

  def _run(self, faults, refused=(), lat_active=True, torque=0.5):
    out = []
    for i, fault in enumerate(faults):
      self.cs.toi_fault = fault
      self.cs.torque_tx_refused = i in refused
      cc = structs.CarControl()
      cc.latActive = lat_active
      cc.actuators.torque = torque
      _, sends = self.controller.update(cc.as_reader(), structs.CarControlSP(), self.cs, 0)
      dat = next(d for addr, d, _ in sends if addr == 0x120)
      out.append((bool((dat[3] >> 4) & 1), ((dat[2] << 3) | (dat[3] >> 5)) - 1024))
    return out

  def _released(self, out):
    return [i for i, (toi, _) in enumerate(out) if not toi]

  def test_latched_fault_is_released_once_then_torque_resumes(self):
    steady = self._run([False] * 150)[-1][1]
    self.assertGreater(steady, 0)
    out = self._run([True] * TOI_CLEAR_LATCH_FRAMES + [False] * 20)  # the release clears it
    rel = self._released(out)
    self.assertEqual(rel, list(range(TOI_CLEAR_LATCH_FRAMES - 1, TOI_CLEAR_LATCH_FRAMES - 1 + BLIP_FRAMES)))
    for i in rel:
      self.assertEqual(out[i][1], 0, "torque is 0 while the request is released")
    self.assertEqual(out[rel[-1] + 1], (True, steady), "torque resumes at the frozen value")

  def test_brief_fault_is_not_released(self):
    self._run([False] * 150)
    out = self._run([True] * (TOI_CLEAR_LATCH_FRAMES - 1) + [False] * 20)
    self.assertEqual(self._released(out), [])

  def test_persistent_fault_is_retried_every_half_second(self):
    self._run([False] * 150)
    rel = self._released(self._run([True] * 200))
    starts = [i for i in rel if i - 1 not in rel]
    self.assertEqual(len(rel), len(starts) * BLIP_FRAMES)
    self.assertEqual(starts[0], TOI_CLEAR_LATCH_FRAMES - 1)
    self.assertEqual([b - a for a, b in zip(starts, starts[1:], strict=False)], [TOI_CLEAR_RETRY_FRAMES] * (len(starts) - 1))

  def test_no_release_while_the_panda_is_still_refusing(self):
    # a release does not stick while 0x120 frames are still being refused: the EPAS loses them and latches again
    self._run([False] * 150)
    rel = self._released(self._run([True] * 40, refused=range(20)))
    self.assertTrue(rel, "released once the refusals stop")
    self.assertEqual(rel[0], 19 + TOI_CLEAR_QUIET_FRAMES)

  def test_no_release_without_lateral(self):
    self._run([True] * 20, lat_active=False)
    self.assertEqual(self.controller.toi_clear_cooldown, 0)
    self.assertEqual(self.controller.toi_fault_frames, 0)


class TestToiDropoutRelease(unittest.TestCase):
  """The EPAS can stop taking torque with no ToiFlt (driver overpowering it, EPAS_HandsOnLevel 2) and then ignores torque
  until the request is released (231 s on route 2bba20cd6136cc27/000000ea--8e6d69adcc), so the controller releases it."""

  setUp = TestToiFaultLatchClear.setUp

  def _released(self, out):
    return [i for i, (toi, _, _) in enumerate(out) if not toi]

  def _run_rack(self, active, lat_active=True):
    out = []
    for a in active:
      self.cs.toi_active = a
      cc = structs.CarControl()
      cc.latActive = lat_active
      cc.actuators.torque = 0.5
      _, sends = self.controller.update(cc.as_reader(), structs.CarControlSP(), self.cs, 0)
      dat = next(d for addr, d, _ in sends if addr == 0x120)
      out.append((bool((dat[3] >> 4) & 1), ((dat[2] << 3) | (dat[3] >> 5)) - 1024, self.cs.toi_dropout_warn))
    return out

  def _settled(self):
    out = self._run_rack([True] * 150)
    self.assertGreater(out[-1][1], 0)
    return out[-1][1]

  def test_dropout_is_released_then_torque_resumes(self):
    steady = self._settled()
    out = self._run_rack([False] * TOI_DROPOUT_FRAMES + [True] * 20)
    rel = self._released(out)
    self.assertEqual(rel, list(range(TOI_DROPOUT_FRAMES - 1, TOI_DROPOUT_FRAMES - 1 + BLIP_FRAMES)))
    for i in rel:
      self.assertEqual(out[i][1], 0, "torque is 0 while the request is released")
    self.assertEqual(out[rel[-1] + 1][:2], (True, steady), "torque resumes at the frozen value")
    self.assertFalse(any(w for _, _, w in out), "a dropout the release cleared must not warn")

  def test_brief_dropout_is_not_released(self):
    self._settled()
    out = self._run_rack([False] * (TOI_DROPOUT_FRAMES - 2) + [True] * 20)
    self.assertEqual(self._released(out), [])

  def test_slow_start_is_not_released(self):
    # the EPAS takes up to 1.1 s to start taking torque after a fresh request: releasing then would restart it
    out = self._run_rack([False] * 115 + [True] * 50)
    self.assertEqual(self._released(out), [])

  def test_epas_that_never_starts_is_released(self):
    out = self._run_rack([False] * (TOI_START_FRAMES + 5))
    self.assertEqual(self._released(out)[0], TOI_START_FRAMES)

  def test_persistent_dropout_is_retried_then_warned(self):
    self._settled()
    out = self._run_rack([False] * 200)
    rel = self._released(out)
    starts = [i for i in rel if i - 1 not in rel]
    self.assertEqual(starts[0], TOI_DROPOUT_FRAMES - 1)
    self.assertEqual([b - a for a, b in zip(starts, starts[1:], strict=False)], [TOI_CLEAR_RETRY_FRAMES] * (len(starts) - 1))
    warn = [i for i, (_, _, w) in enumerate(out) if w]
    # release frames hold the count, so the warning lands a frame per release frame after the nominal point
    first = warn[0]
    self.assertGreaterEqual(first, TOI_DROPOUT_FRAMES + TOI_DROPOUT_WARN_FRAMES - 1)
    self.assertLessEqual(first, TOI_DROPOUT_FRAMES + TOI_DROPOUT_WARN_FRAMES - 1 + 2 * BLIP_FRAMES)
    self.assertTrue(all(w for _, _, w in out[first:]))
    # and it clears as soon as the EPAS takes torque again
    self.assertFalse(self._run_rack([True])[0][2])

  def test_warning_holds_while_it_keeps_lateral_off(self):
    # the warning sets steerFaultTemporary and controlsd turns lateral off the next frame. That ended the torque stint
    # and cleared the warning, so lateral came straight back after 20 ms (route 4440a486580ed7c6/00000038 at 0:28.51)
    self._settled()
    warn, lat_active = [], True
    for _ in range(TOI_DROPOUT_FRAMES + TOI_DROPOUT_WARN_FRAMES + TOI_DROPOUT_WARN_HOLD_FRAMES + 50):
      warn.append(self._run_rack([False], lat_active=lat_active)[0][2])
      lat_active = not warn[-1]  # what controlsd does with steerFaultTemporary
    first = warn.index(True)
    self.assertEqual(warn[first:first + TOI_DROPOUT_WARN_HOLD_FRAMES], [True] * TOI_DROPOUT_WARN_HOLD_FRAMES)
    self.assertFalse(warn[first + TOI_DROPOUT_WARN_HOLD_FRAMES], "then it lets lateral retry with a fresh request")

  def test_held_warning_clears_once_the_epas_takes_torque(self):
    self._settled()
    self.assertTrue(self._run_rack([False] * 200)[-1][2])
    self.assertTrue(all(w for _, _, w in self._run_rack([False] * 20, lat_active=False)), "held while lateral is off")
    self.assertFalse(self._run_rack([True] * 3)[0][2])

  def test_not_counted_without_lateral(self):
    out = self._run_rack([False] * 200, lat_active=False)
    self.assertEqual(self.controller.toi_inactive_frames, 0)
    self.assertFalse(any(w for _, _, w in out))


class TestToiFaultWarning(unittest.TestCase):
  """A latched EPAS ToiFlt is reported only once it persists, so a latch the controller clears within a few frames
  does not flash the warning (the same 0.3 s as the angle-harness branches)."""

  def setUp(self):
    # the SP extension imports openpilot params and MADS helpers, which the Mac test venv does not have; it is not
    # under test, so stub those modules for the import and neutralise the extension
    stubs = {name: types.ModuleType(name) for name in ("openpilot", "openpilot.common", "openpilot.common.params",
                                                       "openpilot.sunnypilot", "openpilot.sunnypilot.mads",
                                                       "openpilot.sunnypilot.mads.helpers")}
    stubs["openpilot.common.params"].Params = object
    stubs["openpilot.sunnypilot.mads.helpers"].MadsSteeringModeOnBrake = object
    stubs["openpilot.sunnypilot.mads.helpers"].read_steering_mode_param = lambda *a, **k: None
    with mock.patch.dict(sys.modules, stubs):
      from opendbc.car.rivian.carstate import CarState, TOI_FAULT_FRAMES
      from opendbc.sunnypilot.car.rivian.carstate_ext import CarStateExt
    for name in ("__init__", "update"):
      patcher = mock.patch.object(CarStateExt, name, lambda *a, **k: None)
      patcher.start()
      self.addCleanup(patcher.stop)
    self.toi_fault_frames_limit = TOI_FAULT_FRAMES
    self.cp = structs.CarParams(carFingerprint=CAR.RIVIAN_R1)
    cp_sp = structs.CarParamsSP()
    self.cs = CarState(self.cp, cp_sp)
    self.parsers = CarState.get_can_parsers(self.cp, cp_sp)
    self.packer = CANPacker(DBC[CAR.RIVIAN_R1][Bus.pt])

  def _frame(self, frame, toi_flt):
    system = self.packer.make_can_msg("EPAS_SystemStatus", 0, {"EPAS_SystemStatus_Counter": frame % 16, "H_CAN_EPSS_ToiFlt": toi_flt,
                                                               "EPAS_HandsOnLevel": 1})
    self.parsers[Bus.pt].update([(frame * 10_000_000, [system])])
    ret, _ = self.cs.update(self.parsers)
    return ret.steerFaultTemporary

  def test_toi_fault_reported_once_it_persists(self):
    for frame in range(1, 4):  # let the parser see a normal hands-on level first; it is reported too
      self._frame(frame, 0)
    flags = [self._frame(frame, 1) for frame in range(4, 63)]
    self.assertFalse(any(flags[:self.toi_fault_frames_limit]), "a brief ToiFlt must not raise the warning")
    self.assertTrue(all(flags[self.toi_fault_frames_limit + 2:]), "a persistent ToiFlt must be reported")
    self.assertTrue(self.cs.toi_fault, "the controller still sees the raw fault at once")
    self.assertFalse(self._frame(63, 0), "the warning clears with the fault")

  def test_dropout_warning_is_reported_and_toi_active_exposed(self):
    for frame in range(1, 4):  # let the parser see a few frames first, as above
      system = self.packer.make_can_msg("EPAS_SystemStatus", 0, {"EPAS_SystemStatus_Counter": frame % 16, "H_CAN_EPSS_ToiActive": 1,
                                                                 "EPAS_HandsOnLevel": 1})
      self.parsers[Bus.pt].update([(frame * 10_000_000, [system])])
      ret, _ = self.cs.update(self.parsers)
    self.assertTrue(self.cs.toi_active)
    self.assertFalse(ret.steerFaultTemporary, "the rack alone does not warn; the controller decides")
    self.assertFalse(self._frame(4, 0))
    self.assertFalse(self.cs.toi_active)
    self.cs.toi_dropout_warn = True
    self.assertTrue(self._frame(5, 0))
    self.cs.toi_dropout_warn = False
    self.assertFalse(self._frame(6, 0))


if __name__ == "__main__":
  unittest.main()
