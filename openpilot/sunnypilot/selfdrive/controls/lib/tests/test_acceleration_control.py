"""Raw-request activation and the real gas-override/assist chain.

Messages use the real schemas. These synthetic tests establish continuity and state
invariants; they do not establish road comfort or jerk limits.
"""
import numpy as np
import pytest

from opendbc.car import structs
from openpilot.cereal import log
from openpilot.common.constants import CV
from openpilot.selfdrive.controls.lib.accel_boost import AccelBoost
from openpilot.sunnypilot.selfdrive.controls.lib.acceleration_control import (
  AccelerationControl, ACCELERATION_CONTROL_BOOST, PARAM_REFRESH_FRAMES,
)

Personality = log.LongitudinalPersonality


class FakeParams:
  def __init__(self, enabled=True):
    self.values = {"AccelerationControl": enabled}

  def get_bool(self, key):
    return bool(self.values.get(key, False))


def _sm(v_ego=25.0, personality=Personality.standard, engaged=True, should_stop=False,
        desired_accel=0.1, gas_pressed=False):
  return {
    'carState': structs.CarState(vEgo=v_ego, gasPressed=gas_pressed),
    'selfdriveState': log.SelfdriveState.new_message(enabled=engaged, personality=personality),
    'modelV2': log.ModelDataV2.new_message(action={'desiredAcceleration': desired_accel, 'shouldStop': should_stop}),
  }


def _assist(enabled=True, op_long=True):
  return AccelerationControl(structs.CarParams(openpilotLongitudinalControl=op_long), params=FakeParams(enabled))


# Explicit expected activation points, including float32 neighborhoods of both breakpoints.
REQUESTS = [(-1, 0), (-0.05, 0), (-1e-6, 0), (0, 0), (1e-6, 1e-5), (0.05, 0.5),
            (0.1 - 1e-6, 1 - 1e-5), (0.1, 1), (0.1 + 1e-6, 1), (0.5, 1)]


@pytest.mark.parametrize('personality', list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize('raw,activation', REQUESTS)
def test_request_matrix(personality, raw, activation):
  assist = _assist()
  sm = _sm(personality=personality, desired_accel=raw)
  expected = ACCELERATION_CONTROL_BOOST[personality] * activation
  assert assist.update(sm, raw) == pytest.approx(raw + expected, abs=1e-7)
  assert assist.boost == pytest.approx(expected, abs=1e-7)


@pytest.mark.parametrize('personality', list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize('override', [0, 0.05, 0.10])
def test_monotone_continuous_dense_sweep(personality, override):
  assist = _assist()
  raw = np.linspace(-0.2, 0.3, 2001)
  outputs = [assist.update(_sm(personality=personality, desired_accel=float(a)), float(a + override)) for a in raw]
  differences = np.diff(outputs)
  # Maximum full-speed slope is 1 + personality_base / 0.1, with float32 input tolerance.
  assert np.min(differences) >= -1e-7
  assert np.max(differences) <= (raw[1] - raw[0]) * (1 + ACCELERATION_CONTROL_BOOST[personality] / 0.1) + 1e-7
  for boundary in (0, 0.1):
    left, center, right = [assist.update(_sm(personality=personality, desired_accel=a), a + override)
                           for a in (boundary - 1e-6, boundary, boundary + 1e-6)]
    assert 0 <= center - left <= 4.1e-6
    assert 0 <= right - center <= 4.1e-6


def test_defensive_negative_incoming():
  # Positive raw/negative incoming is unreachable through real AccelBoost, but
  # remains an explicit defensive API contract for future callers.
  assist = _assist()
  assert assist.update(_sm(desired_accel=0.1), -0.05) == -0.05
  assert assist.boost == 0


@pytest.mark.parametrize('case', ['disabled', 'unavailable', 'disengaged', 'stop', 'low_speed'])
def test_gates_clear_previous_boost(case):
  assist = _assist()
  assert assist.update(_sm(), 0.1) > 0.1
  sm = _sm(engaged=case != 'disengaged', should_stop=case == 'stop',
           v_ego=5 * CV.MPH_TO_MS if case == 'low_speed' else 25)
  if case in ('disabled', 'unavailable'):
    assist.params.values['AccelerationControl'] = case != 'disabled'
    assist.available = case != 'unavailable'
    assist._frame = PARAM_REFRESH_FRAMES
  assert assist.update(sm, 0.1) == 0.1
  assert assist.boost == 0


def test_speed_ramp_and_determinism():
  assist = _assist()
  for mph, expected in ((5, 0.1), (10, 0.1), (15, 0.2), (20, 0.3), (50, 0.3)):
    assert assist.update(_sm(v_ego=mph * CV.MPH_TO_MS), 0.1) == pytest.approx(expected)
  assert len({assist.update(_sm(), 0.1) for _ in range(200)}) == 1


def test_setting_refresh_and_availability():
  for enabled, available in ((False, True), (True, False)):
    assert _assist(enabled, available).update(_sm(), 0.1) == 0.1
  assist = _assist(enabled=False)
  assist.update(_sm(), 0.1)
  assist.params.values['AccelerationControl'] = True
  outputs = [assist.update(_sm(), 0.1) for _ in range(PARAM_REFRESH_FRAMES)]
  assert outputs[-1] == pytest.approx(0.3)
  assist.params.values['AccelerationControl'] = False
  for _ in range(PARAM_REFRESH_FRAMES):
    output = assist.update(_sm(), 0.1)
  assert output == 0.1
  assert assist.boost == 0


def _latch_gas_override(gas, personality, cycles):
  sm = _sm(personality=personality, desired_accel=0)
  gas.update(sm, 0, 1, 1)
  assert gas.boost_eligible
  for _ in range(cycles):
    for _ in range(40):
      gas.update(_sm(personality=personality, desired_accel=0, gas_pressed=True), 0, 1, 1)
    gas.update(sm, 0, 1, 1)
  assert gas.total_boost == pytest.approx(0.05 * cycles)
  assert gas.boost_this_override == 0


@pytest.mark.parametrize('personality', list(ACCELERATION_CONTROL_BOOST))
@pytest.mark.parametrize('cycles', [1, 2])
def test_real_override_latch_raw_sweep_release_and_reset(personality, cycles):
  # Production order: assist on the raw request, then the gas override on the lifted candidate.
  gas, assist = AccelBoost(), _assist()
  _latch_gas_override(gas, personality, cycles)
  previous = None
  for raw in np.linspace(-0.1, 0.15, 501):
    sm = _sm(personality=personality, desired_accel=float(raw))
    lifted = assist.update(sm, float(raw))
    output = gas.update(sm, lifted, 1, 1)
    assert gas.total_boost == pytest.approx(cycles * 0.05)  # the latched override is kept, not grown
    assert output == pytest.approx(raw + assist.boost + cycles * 0.05)
    if raw <= 0:
      assert lifted == raw
      assert assist.boost == 0
    if previous is not None:
      assert -1e-7 <= output - previous <= 0.002001
    previous = output
  # Successive sign changes must recompute requested boost without losing the override.
  for raw in (0.1, -0.05, 0, 0.05, -1e-6, 1e-6):
    sm = _sm(personality=personality, desired_accel=raw)
    lifted = assist.update(sm, raw)
    gas.update(sm, lifted, 1, 1)
    assert gas.total_boost == pytest.approx(cycles * 0.05)
    assert (lifted == raw) == (raw <= 0)
  # Disengagement releases the real override and assist state.
  sm = _sm(personality=personality, desired_accel=0.1, engaged=False)
  assert gas.update(sm, assist.update(sm, 0.1), 1, 1) == 0.1
  assert gas.total_boost == 0
  assert assist.boost == 0


def _override_cycles(gas, assist, raw, lead, cruise=1.0, presses=4):
  # Real chain in production order: 2 s hands-off, then 3 s of gas, repeated.
  for _ in range(presses):
    for pressed, frames in ((False, 40), (True, 60)):
      for _ in range(frames):
        sm = _sm(desired_accel=raw, gas_pressed=pressed)
        gas.update(sm, assist.update(sm, raw), lead, cruise)


def test_gas_override_does_not_learn_when_assist_already_cleared_the_model():
  # Model mildly timid (0.12), lead allows 0.30. With the assist, the e2e candidate (0.12 + 0.2)
  # is above the lead, so the lead is the limit and the driver's gas presses say nothing about
  # the model. The override must not latch eligibility from the un-lifted request.
  gas, assist = AccelBoost(), _assist()
  _override_cycles(gas, assist, raw=0.12, lead=0.30)
  assert gas.total_boost == 0
  # Without the assist the model really is the limit, so the override learns as upstream intends.
  gas, assist = AccelBoost(), _assist(enabled=False)
  _override_cycles(gas, assist, raw=0.12, lead=0.30)
  # two overrides of 0.05 each; then 0.12 + 0.10 is no longer 0.1 under the lead, so it stops
  assert gas.total_boost == pytest.approx(0.1)


@pytest.mark.parametrize('scale,expected', [(1.0, 0.3), (0.5, 0.2), (0.0, 0.1), (-1.0, 0.1), (2.0, 0.3)])
def test_lateral_throttle_interlock_fades_only_the_assist(scale, expected):
  assist = _assist()
  assert assist.update(_sm(), 0.1, throttle_scale=scale) == pytest.approx(expected)
  assert assist.boost == pytest.approx(expected - 0.1)


def test_unknown_real_personality_uses_standard_fallback():
  assist = _assist()
  assert assist.update(_sm(personality=99), 0.1) == pytest.approx(0.3)
