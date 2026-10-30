"""
Tests for the Rivian rear-axle off-tracking compensation.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import math
from types import SimpleNamespace

import pytest

from openpilot.sunnypilot.selfdrive.controls.lib.rivian_offtrack import RivianOfftrack, offtrack_curvature


class FakeParams:
  def __init__(self, enabled, gain):
    self.values = {"RivianOfftrackCompensationEnabled": bool(enabled), "RivianOfftrackCompensationGain": float(gain)}

  def get_bool(self, key):
    return bool(self.values[key])

  def get(self, key, return_default=False):
    return self.values[key]


def _offtrack(enabled=True, gain=0.5, brand="rivian", wheelbase=3.45):
  offtrack = RivianOfftrack(SimpleNamespace(brand=brand, wheelbase=wheelbase), FakeParams(enabled, gain))
  offtrack.get_params()
  return offtrack


@pytest.mark.parametrize("curvature", [1 / 10, -1 / 10, 1 / 50])
def test_widens_the_turn_by_gain_times_the_rear_cut_in(curvature):
  radius, wheelbase, gain = 1 / abs(curvature), 3.45, 0.5
  out = offtrack_curvature(curvature, wheelbase, gain)
  assert math.copysign(1, out) == math.copysign(1, curvature)
  assert 1 / abs(out) == pytest.approx(radius + gain * (radius - math.sqrt(radius ** 2 - wheelbase ** 2)))


def test_leaves_straight_and_impossible_turns_alone():
  assert offtrack_curvature(0.0, 3.45, 0.5) == 0.0
  assert offtrack_curvature(1 / 3.0, 3.45, 0.5) == 1 / 3.0  # tighter than the wheelbase: no real geometry
  assert offtrack_curvature(1 / 10, 3.45, 0.0) == 1 / 10


def test_off_by_default_other_brands_and_lateral_off():
  assert _offtrack(enabled=False).update(1 / 10, True) == 1 / 10
  assert _offtrack(brand="tesla").update(1 / 10, True) == 1 / 10
  assert _offtrack().update(1 / 10, False) == 1 / 10
  assert abs(_offtrack().update(1 / 10, True)) < 1 / 10


def test_gain_is_clamped_and_uses_the_cars_wheelbase():
  assert _offtrack(gain=5.0).gain == 1.0
  # the longer R1T cuts in more than the R1S at the same radius, so it is widened more
  assert abs(_offtrack(wheelbase=3.45).update(1 / 10, True)) < abs(_offtrack(wheelbase=3.08).update(1 / 10, True))
