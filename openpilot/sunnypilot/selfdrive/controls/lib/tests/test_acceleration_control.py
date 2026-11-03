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
from openpilot.selfdrive.controls.lib.accel_boost import AccelBoost
from openpilot.sunnypilot.selfdrive.controls.lib.acceleration_control import AccelerationControl, ACCELERATION_CONTROL_BOOST, PARAM_REFRESH_FRAMES

Personality = log.LongitudinalPersonality


class FakeParams:
  def __init__(self, enabled=True):
    self.values = {"AccelerationControl": enabled}

  def get_bool(self, key):
    return bool(self.values.get(key, False))


def _model_v2(desired_accel=0.0, should_stop=False):
  """Real cereal ModelDataV2 message with an explicit raw desiredAcceleration action field."""
  return log.ModelDataV2.new_message(
    action=log.ModelDataV2.Action.new_message(desiredAcceleration=desired_accel, shouldStop=should_stop))


def _sm(v_ego=25.0, personality=Personality.standard, engaged=True, should_stop=False,
        desired_accel=0.0, gas_pressed=False):
  """Build a message dict with real cereal message types so _DynamicEnum hashing is exercised."""
  selfdrive_state = log.SelfdriveState.new_message(
    enabled=engaged,
    personality=personality,
  )
  return {
    'carState': MagicMock(vEgo=v_ego, gasPressed=gas_pressed),
    'selfdriveState': selfdrive_state,
    'modelV2': _model_v2(desired_accel, should_stop),
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


# --- Raw model braking request (modelV2.action.desiredAcceleration) -------------------------
#
# The planner applies Gas Override Boost before AccelerationControl, so the incoming candidate can
# be non-negative even when the model itself is braking. The boost must be withheld on any raw
# model braking request, with the incoming post-gas candidate passing through unchanged.

RAW_NEGATIVE_CASES = [
  # (raw model request, incoming post-gas candidate)
  (-0.05, 0.05),   # the reported bug: latched gas override lifts a small brake request to positive
  (-0.05, 0.0),    # post-gas candidate exactly at zero
  (-0.1, 0.0),
  (-0.5, 0.05),
  (-1.0, 0.1),
  (-1e-6, 0.0),    # raw request just below zero
]


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize("raw, incoming", RAW_NEGATIVE_CASES)
def test_raw_braking_request_withholds_boost(personality, raw, incoming):
  # no extra fixed boost on a raw model braking request, whatever the post-gas candidate is
  assist = _assist()
  out = assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=personality, desired_accel=raw), incoming)
  assert out == pytest.approx(incoming, abs=1e-12)
  assert assist.boost == 0.0


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize("raw, incoming", [
  (0.0, -0.05),    # non-negative raw, negative incoming: the existing incoming guard still applies
  (0.05, -0.05),
  (0.1, -0.5),
  (0.0, -1e-6),
])
def test_negative_incoming_nonnegative_raw(personality, raw, incoming):
  assist = _assist()
  out = assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=personality, desired_accel=raw), incoming)
  assert out == pytest.approx(incoming, abs=1e-12)
  assert assist.boost == 0.0


def test_raw_negative_to_positive_transition_after_preceding_boost():
  # a negative raw request right after an enabled fixed boost passes the post-gas candidate through
  # unchanged and leaves no boost state behind; the taper then resumes from the current request
  assist = _assist()
  base = ACCELERATION_CONTROL_BOOST[Personality.standard]
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS), 0.0) == pytest.approx(base, abs=1e-12)
  assert assist.boost == pytest.approx(base, abs=1e-12)
  # raw model braking request with a positive post-gas candidate: no boost, passthrough
  out = assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, desired_accel=-0.05), 0.05)
  assert out == pytest.approx(0.05, abs=1e-12)
  assert assist.boost == 0.0
  # raw request back to non-negative: the taper resumes from the current request, not remembered state
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, desired_accel=0.0), 0.05) == pytest.approx(0.05 + base / 2, abs=1e-12)
  assert assist.boost == pytest.approx(base / 2, abs=1e-12)
  assert assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, desired_accel=0.0), 0.0) == pytest.approx(base, abs=1e-12)
  assert assist.boost == pytest.approx(base, abs=1e-12)


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize("a", [0.0, 0.05, 0.1, 0.5])
def test_positive_taper_unchanged_for_nonnegative_raw(personality, a):
  # with a non-negative raw request the exact current taper is retained: full at 0, half at 0.05,
  # none at or above 0.1 m/s^2 (at full speed)
  assist = _assist()
  out = assist.update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=personality, desired_accel=0.0), a)
  base = ACCELERATION_CONTROL_BOOST[personality]
  if a == 0.0:
    expected = base
  elif a == 0.05:
    expected = base / 2
  else:
    expected = 0.0
  assert out == pytest.approx(a + expected, abs=1e-12)
  assert assist.boost == pytest.approx(expected, abs=1e-12)


# --- Integration: ACTUAL AccelBoost (gas override) feeding ACTUAL AccelerationControl ---------
#
# Replicates the planner order (longitudinal_planner.py): the raw model request goes through
# AccelBoost first, and the post-gas result is what AccelerationControl receives. The latched
# 0.10 total boost is produced by the real AccelBoost state machine, not assigned.

V_50MPH = 50 * CV.MPH_TO_MS
# mpc/cruise candidates kept high so they never bind the gas-override eligibility calculation
A_MPC = 1.0
A_CRUISE = 1.0


def _latch_gas_override(accel_boost, personality, cycles=2, frames_pressed=40):
  """Reach a latched total boost the normal way: an eligible released frame, then `cycles` rounds
  of 40 gas-pressed frames each followed by a release (at 50 mph the per-frame increase is
  0.025 * DT_MDL * 1.0 = 0.00125, so 40 pressed frames latch 0.05 per override)."""
  # eligible released frame: model limited by the (high) mpc/cruise candidates
  sm = _sm(v_ego=V_50MPH, personality=personality, desired_accel=0.0, gas_pressed=False)
  assert accel_boost.update(sm, 0.0, A_MPC, A_CRUISE) == pytest.approx(0.0, abs=1e-12)
  assert accel_boost.boost_eligible
  for _ in range(cycles):
    for _ in range(frames_pressed):
      sm = _sm(v_ego=V_50MPH, personality=personality, desired_accel=0.0, gas_pressed=True)
      accel_boost.update(sm, 0.0, A_MPC, A_CRUISE)
    # release: resets the per-override counter and re-latches eligibility
    sm = _sm(v_ego=V_50MPH, personality=personality, desired_accel=0.0, gas_pressed=False)
    accel_boost.update(sm, 0.0, A_MPC, A_CRUISE)
  assert accel_boost.total_boost == pytest.approx(0.05 * cycles, abs=1e-9)


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
def test_gas_override_latch_then_raw_braking(personality):
  # the reported bug: with a latched 0.10 gas override and the pedal released, a raw model request
  # of -0.05 becomes an incoming +0.05; AccelerationControl must not add its own boost on top
  accel_boost = AccelBoost()
  assist = _assist()
  _latch_gas_override(accel_boost, personality)

  sm = _sm(v_ego=V_50MPH, personality=personality, desired_accel=-0.05, gas_pressed=False)
  post_gas = accel_boost.update(sm, -0.05, A_MPC, A_CRUISE)
  # the pre-existing gas override output is preserved: raw -0.05 + latched 0.10 = +0.05
  assert post_gas == pytest.approx(0.05, abs=1e-9)

  out = assist.update(sm, post_gas)
  # only the new fixed boost is suppressed: the post-gas candidate passes through unchanged
  assert out == pytest.approx(0.05, abs=1e-9)
  assert assist.boost == 0.0
  # without the fix this would be 0.05 + base * 0.5 (half taper at 0.05), i.e. 0.15 for standard
  base = ACCELERATION_CONTROL_BOOST[personality]
  assert out != pytest.approx(0.05 + base * 0.5, abs=1e-9)


def test_gas_override_latch_then_raw_braking_zero_post_gas():
  # raw -0.10 with the latched 0.10 override gives an incoming candidate of exactly 0: still no
  # extra boost, and the zero-boundary behavior of the incoming candidate is not reached
  accel_boost = AccelBoost()
  assist = _assist()
  _latch_gas_override(accel_boost, Personality.standard)

  sm = _sm(v_ego=V_50MPH, desired_accel=-0.10, gas_pressed=False)
  post_gas = accel_boost.update(sm, -0.10, A_MPC, A_CRUISE)
  assert post_gas == pytest.approx(0.0, abs=1e-9)
  out = assist.update(sm, post_gas)
  assert out == pytest.approx(0.0, abs=1e-9)
  assert assist.boost == 0.0


def test_gas_override_latch_then_raw_accelerating():
  # control case: with a latched override and a non-negative raw request, the taper is still
  # applied to the post-gas candidate (old behavior unchanged on the positive side)
  accel_boost = AccelBoost()
  assist = _assist()
  _latch_gas_override(accel_boost, Personality.standard, cycles=1)  # latched 0.05
  base = ACCELERATION_CONTROL_BOOST[Personality.standard]

  sm = _sm(v_ego=V_50MPH, desired_accel=0.0, gas_pressed=False)
  post_gas = accel_boost.update(sm, 0.0, A_MPC, A_CRUISE)
  assert post_gas == pytest.approx(0.05, abs=1e-9)
  out = assist.update(sm, post_gas)
  # raw is non-negative: half taper at the 0.05 post-gas candidate
  assert out == pytest.approx(0.05 + base / 2, abs=1e-9)
  assert assist.boost == pytest.approx(base / 2, abs=1e-9)

  sm = _sm(v_ego=V_50MPH, desired_accel=-0.02, gas_pressed=False)
  post_gas = accel_boost.update(sm, -0.02, A_MPC, A_CRUISE)
  assert post_gas == pytest.approx(0.03, abs=1e-9)
  out = assist.update(sm, post_gas)
  # raw is negative: suppressed even though the post-gas candidate (0.03) is inside the taper
  assert out == pytest.approx(0.03, abs=1e-9)
  assert assist.boost == 0.0
