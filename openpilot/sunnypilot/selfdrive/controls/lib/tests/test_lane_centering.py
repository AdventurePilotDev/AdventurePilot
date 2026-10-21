"""
Tests for lane centering, ported from StarPilot (firestar5683/StarPilot) alongside the feature.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from types import SimpleNamespace

import numpy as np
import pytest

from openpilot.sunnypilot.selfdrive.controls.lib.lane_centering import (CENTER_ERROR_DEADBAND, DEFAULT_GAIN, LaneCenteringController,
                                                                          MAX_CENTER_ERROR_DEADBAND, MAX_GAIN, MAX_RAW_CORRECTION)

V_EGO = 20.0
XS = np.linspace(0.0, 50.0, 52)


class FakeParams:
  """Stands in for Params. Only the two reads the controller makes are supported, and they
  return the same types the real store does: a bool for a BOOL key, a float for a FLOAT key."""
  def __init__(self, enabled=True, offset=0.0, authority=1.0, gain=DEFAULT_GAIN, deadband=CENTER_ERROR_DEADBAND):
    self.values = {"LaneCenteringEnabled": bool(enabled), "LaneCenteringOffset": float(offset),
                   "LaneCenteringE2EAuthority": float(authority), "LaneCenteringGain": float(gain),
                   "LaneCenteringDeadband": float(deadband)}

  def get_bool(self, key):
    return bool(self.values[key])

  def get(self, key, return_default=False):
    return self.values[key]


def _path(y, y_std=0.1):
  return SimpleNamespace(x=XS.copy(), y=np.full_like(XS, float(y)), yStd=np.full_like(XS, float(y_std)))


def _model(left=-1.8, right=1.8, model_y=0.0, lane_prob=0.9, lane_std=0.1, path_std=0.1, lane_change=0):
  return SimpleNamespace(
    laneLines=[_path(0.0), _path(left), _path(right), _path(0.0)],
    laneLineProbs=[0.0, lane_prob, lane_prob, 0.0],
    laneLineStds=[0.0, lane_std, lane_std, 0.0],
    position=_path(model_y, path_std),
    meta=SimpleNamespace(laneChangeState=lane_change),
  )


def _controller(enabled=True, offset=0.0, authority=1.0, gain=DEFAULT_GAIN, deadband=CENTER_ERROR_DEADBAND):
  controller = LaneCenteringController(FakeParams(enabled, offset, authority, gain, deadband))
  controller.get_params()
  return controller


def _update(controller, model, *, blinker=False, active=True, valid=True, speed=V_EGO):
  return controller.update(0.0, model, speed, blinker, active, valid)


def _converge(model, *, offset=0.0, authority=1.0, gain=DEFAULT_GAIN, deadband=CENTER_ERROR_DEADBAND, frames=300, speed=V_EGO):
  controller = _controller(offset=offset, authority=authority, gain=gain, deadband=deadband)
  output = 0.0
  for _ in range(frames):
    output = _update(controller, model, speed=speed)
  return controller, output


# --- the gates ---

def test_disabled_is_noop():
  assert _update(_controller(enabled=False), _model(left=-1.5, right=2.1)) == 0.0


def test_lateral_inactive_is_noop():
  assert _update(_controller(), _model(left=-1.5, right=2.1), active=False) == 0.0


def test_invalid_model_is_noop():
  assert _update(_controller(), _model(left=-1.5, right=2.1), valid=False) == 0.0


def test_below_min_speed_is_noop():
  assert _update(_controller(), _model(left=-1.5, right=2.1), speed=3.0) == 0.0


def test_non_finite_speed_is_noop():
  assert _update(_controller(), _model(left=-1.5, right=2.1), speed=float("nan")) == 0.0


def test_lane_change_is_noop():
  assert _update(_controller(), _model(left=-1.5, right=2.1, lane_change=1)) == 0.0


@pytest.mark.parametrize("field,value", [
  ("lane_prob", 0.2),      # lines the model does not believe in
  ("lane_std", 0.9),       # lines it is not sure where to put
])
def test_low_confidence_is_noop(field, value):
  assert _update(_controller(), _model(left=-1.5, right=2.1, **{field: value})) == 0.0


@pytest.mark.parametrize("left,right", [
  (-1.0, 1.0),             # 2.0 m, too narrow
  (-3.0, 3.0),             # 6.0 m, too wide
])
def test_implausible_lane_width_is_noop(left, right):
  assert _update(_controller(), _model(left=left, right=right)) == 0.0


def test_missing_lane_data_is_noop():
  model = _model()
  model.laneLineProbs = [0.9]
  assert _update(_controller(), model) == 0.0


def test_garbage_model_does_not_raise():
  assert _update(_controller(), SimpleNamespace(meta=SimpleNamespace(laneChangeState=0))) == 0.0


# --- what it does when it is allowed to act ---

def test_steers_toward_lane_center():
  # Lane center sits right of the model path, so the correction should steer right (positive)
  _, right_of_path = _converge(_model(left=-1.5, right=2.1))
  assert right_of_path > 0.0

  # And the mirror image steers left
  _, left_of_path = _converge(_model(left=-2.1, right=1.5))
  assert left_of_path < 0.0


def test_already_centered_does_nothing():
  _, output = _converge(_model())
  assert output == 0.0


def test_small_error_does_not_chatter():
  # inside the deadband, half of it either side of the model path
  controller = _controller()
  edge = controller.deadband * 0.5
  _, output = _converge(_model(left=-1.8 + edge, right=1.8 + edge))
  assert output == 0.0


def test_offset_direction():
  # Positive offset is to the right in the openpilot frame
  _, right = _converge(_model(), offset=0.2)
  _, left = _converge(_model(), offset=-0.2)
  assert right > 0.0 > left
  assert right == pytest.approx(-left, abs=1e-9)


def test_offset_cannot_aim_us_at_a_line():
  # A 2.7 m lane leaves only 0.25 m of room before the 1.1 m keep-off, so 0.3 m gets clamped
  clamped, _ = _converge(_model(left=-1.35, right=1.35), offset=0.30)
  allowed, _ = _converge(_model(left=-1.35, right=1.35), offset=0.25)
  assert clamped.correction > 0.0
  assert clamped.correction == pytest.approx(allowed.correction, abs=1e-9)


def test_correction_is_smoothed_and_capped():
  model = _model(left=-1.5, right=2.1)
  controller = _controller()

  first = _update(controller, model)
  second = _update(controller, model)
  assert 0.0 < first < second, "correction should build up over several frames, not jump"

  _, settled = _converge(model)
  assert 0.0 < settled <= MAX_RAW_CORRECTION * DEFAULT_GAIN


def test_correction_stays_within_cap_at_low_speed():
  # Low speed means a short lookahead, which is where the raw correction wants to run away.
  # An unsure model (high path std) keeps the e2e break-in from swallowing this large an error.
  _, output = _converge(_model(left=-1.2, right=2.4, path_std=0.9), frames=600, speed=6.0)
  assert output == pytest.approx(MAX_RAW_CORRECTION * DEFAULT_GAIN, rel=1e-3), "should be pinned at the cap"


def test_confident_model_keeps_a_large_deliberate_departure():
  # Same geometry, but the model is sure of its path, so it keeps most of its departure
  unsure = _converge(_model(left=-1.7, right=2.1, path_std=0.9))[1]
  confident = _converge(_model(left=-1.7, right=2.1, path_std=0.1))[1]
  assert unsure > confident > 0.0


def test_authority_sets_how_much_the_model_keeps():
  # Same confident model, three authority settings: less authority means more centering
  model = _model(left=-1.7, right=2.3, path_std=0.1)
  full = _converge(model, authority=1.0)[1]
  half = _converge(model, authority=0.5)[1]
  none = _converge(model, authority=0.0)[1]
  assert none > half > full >= 0.0


def test_zero_authority_ignores_model_confidence():
  # With no authority given away, a confident and an unsure model are corrected identically
  confident = _converge(_model(left=-1.7, right=2.3, path_std=0.1), authority=0.0)[1]
  unsure = _converge(_model(left=-1.7, right=2.3, path_std=0.9), authority=0.0)[1]
  assert confident == pytest.approx(unsure, abs=1e-9)


# --- how it lets go ---

def test_confidence_loss_fades_rather_than_snaps():
  model = _model(left=-1.5, right=2.1)
  controller, settled = _converge(model)
  assert settled > 0.0

  faded = _update(controller, _model(left=-1.5, right=2.1, lane_prob=0.2))
  assert 0.0 < faded < settled

  for _ in range(300):
    faded = _update(controller, _model(left=-1.5, right=2.1, lane_prob=0.2))
  assert abs(faded) < 1e-6


def test_turn_signal_fades_rather_than_snaps():
  model = _model(left=-1.5, right=2.1)
  controller, settled = _converge(model)

  faded = _update(controller, model, blinker=True)
  assert 0.0 < faded < settled

  for _ in range(300):
    faded = _update(controller, model, blinker=True)
  assert abs(faded) < 1e-6


def test_disabling_mid_drive_fades_rather_than_snaps():
  model = _model(left=-1.5, right=2.1)
  controller, settled = _converge(model)

  controller.params.values["LaneCenteringEnabled"] = False
  controller.get_params()

  faded = _update(controller, model)
  assert 0.0 < faded < settled

  for _ in range(300):
    faded = _update(controller, model)
  assert abs(faded) < 1e-6


def test_lateral_disengage_drops_the_correction_immediately():
  model = _model(left=-1.5, right=2.1)
  controller, settled = _converge(model)
  assert settled > 0.0

  assert _update(controller, model, active=False) == 0.0
  assert controller.correction == 0.0


# --- the tuning gain ---

def test_gain_scales_the_correction():
  # Same geometry, three gains. The correction is proportional as long as nothing hits the cap.
  model = _model(left=-1.5, right=2.1, path_std=0.9)
  low = _converge(model, gain=0.30)[1]
  high = _converge(model, gain=0.60)[1]
  assert high == pytest.approx(2.0 * low, rel=1e-3)


def test_zero_gain_is_a_noop():
  assert _converge(_model(left=-1.5, right=2.1), gain=0.0)[1] == 0.0


def test_gain_is_clamped_to_the_ceiling():
  # An out of range value must not let the nudge exceed MAX_RAW_CORRECTION
  controller = _controller(gain=99.0)
  assert controller.gain == MAX_GAIN
  _, output = _converge(_model(left=-1.0, right=2.6, path_std=0.9), gain=99.0, frames=600, speed=6.0)
  assert abs(output) <= MAX_RAW_CORRECTION + 1e-12


# --- the deadband, how close to the middle counts as close enough ---

def test_deadband_sets_how_much_of_the_error_is_acted_on():
  # 20 cm off the middle. A 4 cm deadband leaves 16 cm to act on, a 10 cm one leaves 10 cm.
  # The break-in keys off the full error, which is the same either way, so the ratio is exact.
  model = _model(left=-2.0, right=1.6)
  narrow = _converge(model, deadband=0.04)[1]
  wide = _converge(model, deadband=0.10)[1]
  assert narrow == pytest.approx(1.6 * wide, rel=1e-3)


def test_deadband_is_clamped_to_its_range():
  assert _controller(deadband=-1.0).deadband == 0.0
  assert _controller(deadband=99.0).deadband == MAX_CENTER_ERROR_DEADBAND


def test_zero_deadband_leaves_no_holding_zone():
  # 2 mm off the middle, which any ordinary deadband would swallow
  controller, output = _converge(_model(left=-1.802, right=1.798), deadband=0.0)
  assert controller.active
  assert not controller.holding
  assert output != 0.0


# --- telemetry for the on-screen indicator ---

def test_telemetry_reports_the_error_before_the_deadband():
  # More room on the left, so the car sits 20 cm right of the lane middle and is aimed back to the left.
  # The reported error is the full 20 cm, not what is left after the deadband is taken out.
  controller = _controller()
  _update(controller, _model(left=-2.0, right=1.6))

  assert controller.active
  assert not controller.holding
  assert controller.center_error == pytest.approx(-0.2, abs=1e-6)
  assert not controller.clipped


def test_telemetry_flags_holding_inside_the_deadband():
  controller = _controller()
  # 1 cm off, comfortably inside the default tolerance. Keep it well clear of the boundary: a case
  # sitting exactly on it is decided by floating point, not by the rule being tested.
  _update(controller, _model(left=-1.81, right=1.79))

  assert controller.active
  assert controller.holding
  assert abs(controller.center_error) <= controller.deadband
  assert controller.correction == 0.0


def test_telemetry_is_cleared_when_the_lanes_are_not_trusted():
  controller = _controller()
  _update(controller, _model(left=-2.0, right=1.6))
  assert controller.active

  _update(controller, _model(left=-2.0, right=1.6, lane_prob=0.1))
  assert not controller.active
  assert not controller.holding
  assert controller.center_error == 0.0


def test_telemetry_is_cleared_on_lateral_disengage():
  controller = _controller()
  _update(controller, _model(left=-2.0, right=1.6))
  assert controller.active

  _update(controller, _model(left=-2.0, right=1.6), active=False)
  assert not controller.active
  assert controller.center_error == 0.0
  assert not controller.clipped


def test_telemetry_flags_the_cap_only_when_it_bites():
  # A large error at low speed gives a short lookahead, which is the only way to reach the cap.
  # path_std is high so the model keeps no authority and the whole error is acted on.
  model = _model(left=-2.2, right=1.4, path_std=0.9)

  controller = _controller()
  _update(controller, model, speed=6.0)
  assert controller.clipped

  # The same geometry at cruising speed is nowhere near it
  controller = _controller()
  _update(controller, model, speed=30.0)
  assert controller.active
  assert not controller.clipped


def test_telemetry_direction_follows_the_error_sign():
  # Mirror of the case above: more room on the right, so the car sits left of the middle
  # and the error is positive, meaning it is aimed back to the right.
  controller = _controller()
  _update(controller, _model(left=-1.6, right=2.0))
  assert controller.center_error > 0.0
