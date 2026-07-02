#!/usr/bin/env python3
"""Role-by-type panda classification: external black pandas are the xnor long-kit
bridge (own firmware via flash_rivian_long) and must never be managed as openpilot
pandas; internal + red intercept pandas are managed. Never decided by panda count."""
import unittest
from unittest.mock import MagicMock, patch

from openpilot.selfdrive.pandad.pandad import classify_panda_serials

HW_TYPE_BLACK = b'\x03'
HW_TYPE_RED = b'\x07'
HW_TYPE_TRES = b'\x09'


def _mock_panda(internal=False, hw_type=HW_TYPE_TRES, raise_on_probe=False):
  p = MagicMock()
  if raise_on_probe:
    p.is_internal.side_effect = RuntimeError("usb error")
  else:
    p.is_internal.return_value = internal
  p.get_type.return_value = hw_type
  return p


class TestPandaRoles(unittest.TestCase):
  def _classify(self, pandas_by_serial):
    with patch("openpilot.selfdrive.pandad.pandad.Panda", side_effect=lambda s: pandas_by_serial[s]):
      return classify_panda_serials(list(pandas_by_serial))

  def test_internal_only(self):
    managed, excluded = self._classify({"int": _mock_panda(internal=True)})
    self.assertEqual((managed, excluded), (["int"], []))

  def test_long_bridge_excluded(self):
    # longitudinal-kit truck: black bridge panda never managed (its firmware is xnor's)
    managed, excluded = self._classify({
      "int": _mock_panda(internal=True),
      "bridge": _mock_panda(hw_type=HW_TYPE_BLACK),
    })
    self.assertEqual(managed, ["int"])
    self.assertEqual(excluded, ["bridge"])

  def test_red_intercept_managed(self):
    # dual-intercept truck: red ext panda managed (gets safetyConfigs[1] SECONDARY_TX)
    managed, excluded = self._classify({
      "int": _mock_panda(internal=True),
      "red": _mock_panda(hw_type=HW_TYPE_RED),
    })
    self.assertEqual(managed, ["int", "red"])
    self.assertEqual(excluded, [])

  def test_internal_never_excluded(self):
    # the exclusion rule is external AND black; an internal panda is always managed
    managed, excluded = self._classify({"int": _mock_panda(internal=True, hw_type=HW_TYPE_BLACK)})
    self.assertEqual((managed, excluded), (["int"], []))

  def test_probe_failure_defaults_to_managed(self):
    # a panda we can't probe is managed (flash_panda will sort it out), never dropped
    managed, excluded = self._classify({"weird": _mock_panda(raise_on_probe=True)})
    self.assertEqual((managed, excluded), (["weird"], []))

  def test_order_preserved(self):
    managed, _ = self._classify({
      "int": _mock_panda(internal=True),
      "red1": _mock_panda(hw_type=HW_TYPE_RED),
      "red2": _mock_panda(hw_type=HW_TYPE_RED),
    })
    self.assertEqual(managed, ["int", "red1", "red2"])


if __name__ == "__main__":
  unittest.main()
