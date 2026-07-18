import unittest

from opendbc.car.rivian.angle_toggle import AngleSteerToggle, HOLD_FRAMES
from opendbc.car.rivian.values import RivianAngleSteerPhase as P


def _run(t, frames, tap=False, hands_on=False, torque_active=False, lat_active=True, master_on=True):
  # tap is a one-frame edge; only the first frame of a burst carries it
  ft = None
  for i in range(frames):
    ft = t.update(tap and i == 0, hands_on, torque_active, lat_active, master_on)
  return ft


class TestAngleSteerToggle(unittest.TestCase):
  def test_master_off_pins_torque_and_ignores_taps(self):
    t = AngleSteerToggle()
    ft = _run(t, 5, tap=True, master_on=False)
    self.assertTrue(ft)
    self.assertEqual(t.phase, P.QUIET)
    # even a tap does nothing while master is off
    ft = t.update(True, False, False, True, False)
    self.assertTrue(ft)
    self.assertEqual(t.phase, P.QUIET)

  def test_default_is_angle(self):
    t = AngleSteerToggle()
    ft = _run(t, 3)
    self.assertFalse(ft)
    self.assertEqual(t.phase, P.QUIET)

  def test_deactivate_instant_when_already_torque(self):
    # tap while the driver is already holding (torque channel) -> lock torque immediately
    t = AngleSteerToggle()
    ft = t.update(True, False, True, True, True)
    self.assertTrue(ft)
    self.assertEqual(t.phase, P.DEACTIVATED)
    self.assertEqual(t.state, AngleSteerToggle.TORQUE_FORCED)

  def test_deactivate_hold_to_confirm_success(self):
    t = AngleSteerToggle()
    t.update(True, False, False, True, True)  # tap in angle mode -> pending
    self.assertEqual(t.phase, P.HOLD_TO_DEACTIVATE)
    self.assertFalse(t.update(False, False, False, True, True))  # still angle while waiting
    ft = t.update(False, True, False, True, True)  # driver holds -> deactivate
    self.assertTrue(ft)
    self.assertEqual(t.phase, P.DEACTIVATED)
    self.assertEqual(t.state, AngleSteerToggle.TORQUE_FORCED)

  def test_deactivate_timeout(self):
    t = AngleSteerToggle()
    t.update(True, False, False, True, True)  # pending
    ft = _run(t, HOLD_FRAMES, hands_on=False)  # never hold
    self.assertFalse(ft)
    self.assertEqual(t.phase, P.DEACTIVATE_TIMEOUT)
    self.assertEqual(t.state, AngleSteerToggle.IDLE_ANGLE)

  def test_deactivate_cancel_on_second_tap(self):
    t = AngleSteerToggle()
    t.update(True, False, False, True, True)  # pending
    t.update(False, False, False, True, True)
    ft = t.update(True, False, False, True, True)  # second tap cancels
    self.assertFalse(ft)
    self.assertEqual(t.phase, P.DEACTIVATE_CANCELED)
    self.assertEqual(t.state, AngleSteerToggle.IDLE_ANGLE)

  def _force_torque(self):
    t = AngleSteerToggle()
    t.update(True, False, True, True, True)  # instant deactivate -> TORQUE_FORCED
    _run(t, 30)  # let the transient clear
    self.assertEqual(t.state, AngleSteerToggle.TORQUE_FORCED)
    self.assertEqual(t.phase, P.QUIET)
    return t

  def test_reactivate_hold_to_confirm_success(self):
    t = self._force_torque()
    t.update(True, False, True, True, True)  # tap -> pending reactivate
    self.assertEqual(t.phase, P.HOLD_TO_REACTIVATE)
    self.assertTrue(t.update(False, False, True, True, True))  # still torque while waiting
    ft = t.update(False, True, True, True, True)  # driver holds -> reactivate angle
    self.assertFalse(ft)
    self.assertEqual(t.phase, P.REACTIVATED)
    self.assertEqual(t.state, AngleSteerToggle.IDLE_ANGLE)

  def test_reactivate_timeout(self):
    t = self._force_torque()
    t.update(True, False, True, True, True)  # pending reactivate
    ft = _run(t, HOLD_FRAMES, hands_on=False, torque_active=True)
    self.assertTrue(ft)  # stays torque
    self.assertEqual(t.phase, P.ACTIVATE_TIMEOUT)
    self.assertEqual(t.state, AngleSteerToggle.TORQUE_FORCED)

  def test_reactivate_cancel_on_second_tap(self):
    t = self._force_torque()
    t.update(True, False, True, True, True)  # pending reactivate
    t.update(False, False, True, True, True)
    ft = t.update(True, False, True, True, True)  # second tap cancels
    self.assertTrue(ft)
    self.assertEqual(t.phase, P.ACTIVATE_CANCELED)
    self.assertEqual(t.state, AngleSteerToggle.TORQUE_FORCED)

  def test_lat_inactive_cancels_pending_keeps_mode(self):
    # pending deactivate then lateral drops -> revert to angle, no message
    t = AngleSteerToggle()
    t.update(True, False, False, True, True)  # pending deact
    ft = t.update(False, False, False, False, True)  # lateral inactive
    self.assertFalse(ft)
    self.assertEqual(t.phase, P.QUIET)
    self.assertEqual(t.state, AngleSteerToggle.IDLE_ANGLE)
    # forced torque persists across a lateral-inactive gap
    t2 = self._force_torque()
    ft = t2.update(False, False, False, False, True)
    self.assertTrue(ft)
    self.assertEqual(t2.state, AngleSteerToggle.TORQUE_FORCED)


if __name__ == "__main__":
  unittest.main()
