from types import SimpleNamespace

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
  return {
    'carState': SimpleNamespace(vEgo=v_ego),
    'selfdriveState': SimpleNamespace(enabled=engaged, personality=personality),
    'modelV2': SimpleNamespace(action=SimpleNamespace(shouldStop=should_stop)),
  }


def _assist(enabled=True, op_long=True):
  return AccelerationControl(SimpleNamespace(openpilotLongitudinalControl=op_long), params=FakeParams(enabled))


@pytest.mark.parametrize("personality", list(ACCELERATION_CONTROL_BOOST))
def test_fixed_boost_per_personality(personality):
  # the stroad case: model asking for almost nothing at 50 mph, nothing to slow for
  out = _assist().update(_sm(v_ego=50 * CV.MPH_TO_MS, personality=personality), 0.05)
  assert out == pytest.approx(0.05 + ACCELERATION_CONTROL_BOOST[personality])


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
  # partial fade just below zero
  assert 0.0 < assist.update(_sm(), -0.05) + 0.05 < ACCELERATION_CONTROL_BOOST[Personality.standard]


def test_predicted_stop_passes_through():
  assert _assist().update(_sm(should_stop=True), 0.1) == 0.1


def test_low_speed_ramp():
  assist = _assist()
  boost = ACCELERATION_CONTROL_BOOST[Personality.standard]
  assert assist.update(_sm(v_ego=5 * CV.MPH_TO_MS), 0.0) == 0.0
  assert assist.update(_sm(v_ego=15 * CV.MPH_TO_MS), 0.0) == pytest.approx(boost / 2)
  assert assist.update(_sm(v_ego=25 * CV.MPH_TO_MS), 0.0) == pytest.approx(boost)


def test_setting_change_is_picked_up():
  params = FakeParams(enabled=False)
  assist = AccelerationControl(SimpleNamespace(openpilotLongitudinalControl=True), params=params)
  assert assist.update(_sm(), 0.0) == 0.0
  params.values["AccelerationControl"] = True
  outs = [assist.update(_sm(), 0.0) for _ in range(PARAM_REFRESH_FRAMES)]
  assert outs[-1] > 0.0
