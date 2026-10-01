import unittest

from opendbc.car.rivian.torque_rt import TorqueRtLimiter, TORQUE_RT_MAX_DELTA, refused_torque_parser
from opendbc.car.rivian.values import DBC, CAR
from opendbc.car import Bus


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


if __name__ == "__main__":
  unittest.main()
