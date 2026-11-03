"""
Tests for Acceleration Control personality-linked acceleration boost.

Covers the hard negative gate (every negative request passes through unchanged with zero boost),
the positive taper (full boost at 0, half at 0.05 m/s^2, none at or above 0.1 m/s^2 at full speed),
the zero-boundary jump, boost reset after a preceding positive request, and the disabled,
disengaged, shouldStop, low-speed-ramp and param-refresh behaviors. All personality cases use
real cereal SelfdriveState capnp messages.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from unittest.mock import MagicMock

import pytest

from openpilot.cereal import log
from openpilot.common.constants import CV
from openpilot.sunnypilot.selfdrive.controls.lib.acceleration_control import AccelerationControl, ACCELERATION_CONTROL_BOOST, PARAM_REFRESH_FRAMES

Personality = log.LongitudinalPersonality


class FakeParams:
  def __init__(self, enabled=True):
    self.values = {"AccelerationControl": enabled}

  def get_bool(self, key):
    return bool(self.values.get(key, False))


def _sm(v_ego=25.0, personality=Personality.standard, engaged=True, should_stop=False):
  """Build a message dict with real cereal message types so _DynamicEnum hashing is exercised."""
  selfdrive_state = log.SelfdriveState.new_message(
    enabled=engaged,
    personality=personality,
  )
  return {
    'carState': MagicMock(vEgo=v_ego),
    'selfdriveState': selfdrive_state,
    'modelV2': MagicMock(action=MagicMock(shouldStop=should_stop)),
  }


def _assist(enabled=True, op_long=True):
  return AccelerationControl(MagicMock(openpilotLongitudinalControl=op_long), params=FakeParams(enabled))


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
def test_fixed_boost_per_personality(personality):
  # at a=0.05 the model is slightly accelerating so brake fade is 0.5 (halfway through the fade curve)
  out = _assist().update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=personality), 0.05)
  boost = ACCELERATION_CONTROL_BOOST[personality]
  assert out == pytest.approx(0.05 + boost * 0.5)


def test_same_result_every_time():
  # deterministic: no learning, no state carried between engagements
  assist = _assist()
  outs = [assist.update(_sm(), 0.0) for _ in range(200)]
  assert len(set(outs)) == 1


def test_off_by_default_and_without_op_long():
  assert _assist(enabled=False).update(_sm(), 0.1) == 0.1
  assert _assist(op_long=False).update(_sm(), 0.1) == 0.1


def test_not_engaged():
  assert _assist().update(_sm(engaged=False), 0.1) == 0.1


def test_model_braking_passes_through():
  assist = _assist()
  for a in (-0.1, -0.5, -2.0):
    assert assist.update(_sm(), a) == a


def test_predicted_stop_passes_through():
  assert _assist().update(_sm(should_stop=True), 0.1) == 0.1


def test_low_speed_ramp():
  assist = _assist()
  boost = ACCELERATION_CONTROL_BOOST[Personality.standard]
  assert assist.update(_sm(v_ego=5 * CV.MPH_TO_MS), 0.0) == 0.0
  assert assist.update(_sm(v_ego=15 * CV.MPH_TO_MS), 0.0) == pytest.approx(boost / 2)
  assert assist.update(_sm(v_ego=25 * CV.MPH_TO_MS), 0.0) == pytest.approx(boost)


def test_high_speed_no_accel():
  # at 50 mph, a=0: full speed scale, full fade → full boost
  boost = ACCELERATION_CONTROL_BOOST[Personality.standard]
  assert _assist().update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=Personality.standard), 0.0) == pytest.approx(boost)


# Full regression matrix: every real capnp personality x negative, zero and positive requests.
NEGATIVE_REQUESTS = [-1.0, -0.1, -0.05, -1e-6]
POSITIVE_REQUESTS = [1e-6, 0.05, 0.1, 0.5]


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize("a", NEGATIVE_REQUESTS + [0.0] + POSITIVE_REQUESTS)
def test_request_matrix(personality, a):
  # At full speed (50 mph) the speed scale is 1, so the expected boost is the personality base
  # times the positive taper: full at 0, half at 0.05, none at or above 0.1.
  assist = _assist()
  out = assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=personality), a)
  base = ACCELERATION_CONTROL_BOOST[personality]
  if a < 0.0:
    # hard negative gate: unchanged request, zero boost, no fade on the braking side
    assert out == a
    assert assist.boost == 0.0
  elif a == 0.0:
    assert out == pytest.approx(base, abs=1e-12)
    assert assist.boost == pytest.approx(base, abs=1e-12)
  elif a == 0.05:
    assert out == pytest.approx(a + base / 2, abs=1e-12)
    assert assist.boost == pytest.approx(base / 2, abs=1e-12)
  else:
    # a >= 0.1 (0.1 and 0.5): taper fully faded; 1e-6 is within 1e-5 of full boost
    fade = 0.0 if a >= 0.1 else 1.0
    assert out == pytest.approx(a + base * fade, abs=1e-5)
    assert assist.boost == pytest.approx(base * fade, abs=1e-5)


def test_boost_resets_after_preceding_positive_request():
  # boost is recomputed from the current request every frame: a negative request right after a
  # boosted positive one passes through unchanged and leaves no boost state behind
  assist = _assist()
  base = ACCELERATION_CONTROL_BOOST[Personality.standard]
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), 0.0) == pytest.approx(base, abs=1e-12)
  assert assist.boost == pytest.approx(base, abs=1e-12)
  for a in NEGATIVE_REQUESTS:
    assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), a) == a
    assert assist.boost == 0.0
  # and the taper resumes from the current request, not from any remembered boost
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), 0.05) == pytest.approx(0.05 + base / 2, abs=1e-12)


def test_zero_boundary_jump():
  # the boost is discontinuous at zero: full at exactly 0, zero for any negative request, and the
  # gate is immediate (no ramp) on either side of the boundary
  assist = _assist()
  base = ACCELERATION_CONTROL_BOOST[Personality.standard]
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), 0.0) == pytest.approx(base, abs=1e-12)
  assert assist.boost == pytest.approx(base, abs=1e-12)
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), -1e-6) == -1e-6
  assert assist.boost == 0.0
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), 0.0) == pytest.approx(base, abs=1e-12)
  assert assist.boost == pytest.approx(base, abs=1e-12)


def test_setting_change_is_picked_up():
  params = FakeParams(enabled=False)
  assist = AccelerationControl(MagicMock(openpilotLongitudinalControl=True), params=params)
  assert assist.update(_sm(), 0.0) == 0.0
  params.values["AccelerationControl"] = True
  outs = [assist.update(_sm(), 0.0) for _ in range(PARAM_REFRESH_FRAMES)]
  assert outs[-1] > 0.0
