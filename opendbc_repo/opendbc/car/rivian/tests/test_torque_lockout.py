import unittest
from types import SimpleNamespace

from opendbc.car.rivian.torque_rt import TorqueRtLimiter, TORQUE_RT_MAX_DELTA, refused_torque_parser
from opendbc.car.rivian.values import DBC, CAR
from opendbc.car import Bus, structs
from opendbc.car.rivian.carcontroller import (CarController, BLIP_FRAMES, TOI_CLEAR_LATCH_FRAMES, TOI_CLEAR_QUIET_FRAMES,
                                              TOI_CLEAR_RETRY_FRAMES)


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


if __name__ == "__main__":
  unittest.main()
