import unittest

from opendbc.car import structs
from opendbc.car.rivian.values import RivianFlags, CAR
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP
from opendbc.sunnypilot.car.interfaces import _initialize_rivian


def _aggressive(fingerprint: str, gen2: bool = False, override: int = 0) -> bool:
  CP = structs.CarParams()
  CP.brand = "rivian"
  CP.carFingerprint = fingerprint
  CP.flags = RivianFlags.GEN2.value if gen2 else 0
  CP_SP = structs.CarParamsSP()
  _initialize_rivian(CP, CP_SP, {"RivianAggressiveTune": override})
  return bool(CP_SP.flags & RivianFlagsSP.AGGRESSIVE_TUNE.value)


R1T = CAR.RIVIAN_R1T
R1S = CAR.RIVIAN_R1S


class TestRivianTuneSelection(unittest.TestCase):
  def test_auto_selection(self):
    # Auto (override 0): aggressive ONLY for a Gen1 R1T; everything else stays tame.
    self.assertTrue(_aggressive(R1T), "Gen1 R1T should auto-select aggressive")
    self.assertFalse(_aggressive(R1S), "R1S should stay tame")
    self.assertFalse(_aggressive(R1T, gen2=True), "Gen2 R1T should stay tame (no data)")

  def test_manual_tame_override(self):
    # Aggressive is not user-selectable; override 1 forces tame, even on a Gen1 R1T.
    self.assertFalse(_aggressive(R1T, override=1), "override 1 forces tame on a Gen1 R1T")
    self.assertFalse(_aggressive(R1S, override=1), "override 1 keeps an R1S tame")

  def test_non_rivian_is_noop(self):
    CP = structs.CarParams()
    CP.brand = "toyota"
    CP_SP = structs.CarParamsSP()
    _initialize_rivian(CP, CP_SP, {})
    self.assertFalse(CP_SP.flags & RivianFlagsSP.AGGRESSIVE_TUNE.value)


if __name__ == "__main__":
  unittest.main()
