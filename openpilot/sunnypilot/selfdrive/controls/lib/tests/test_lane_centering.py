"""
Tests for lane centering, ported from StarPilot (firestar5683/StarPilot) alongside the feature.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import csv
import os
from types import SimpleNamespace

import numpy as np
import pytest

from openpilot.sunnypilot.selfdrive.controls.lib.lane_centering import (CENTER_ERROR_DEADBAND, DEFAULT_GAIN, LaneCenteringController,
                                                                          MAX_CENTER_ERROR_DEADBAND, MAX_GAIN, MAX_RAW_CORRECTION,
                                                                          SINGLE_LINE_DEFAULT_DISTANCE, SINGLE_LINE_MAX_DISTANCE,
                                                                          SINGLE_LINE_MIN_DISTANCE, SINGLE_LINE_NEAR_X)

V_EGO = 20.0
XS = np.linspace(0.0, 50.0, 52)


class FakeParams:
  """Stands in for Params. Only the two kinds of read the controller makes are supported, and they
  return the same types the real store does: a bool for a BOOL key, a float for a FLOAT key."""
  def __init__(self, enabled=True, offset=0.0, authority=1.0, gain=DEFAULT_GAIN, deadband=CENTER_ERROR_DEADBAND,
               single_line=False, edge_dist=SINGLE_LINE_DEFAULT_DISTANCE, lane_dist=SINGLE_LINE_DEFAULT_DISTANCE):
    self.values = {"LaneCenteringEnabled": bool(enabled), "LaneCenteringOffset": float(offset),
                   "LaneCenteringE2EAuthority": float(authority), "LaneCenteringGain": float(gain),
                   "LaneCenteringDeadband": float(deadband), "LaneCenteringSingleLine": bool(single_line),
                   "LaneCenteringSingleLineEdgeDist": float(edge_dist), "LaneCenteringSingleLineLaneDist": float(lane_dist)}

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
  (-1.0, 1.2),             # 2.2 m, still too narrow
  (-3.0, 3.0),             # 6.0 m, too wide
])
def test_implausible_lane_width_is_noop(left, right):
  assert _update(_controller(), _model(left=left, right=right)) == 0.0


def test_narrow_lane_with_both_lines_sure_is_centered():
  # A 2.45 m country lane, as on route 00000043 seg 7, with the car 0.25 m toward one line
  controller, output = _converge(_model(left=-1.0, right=1.45, path_std=0.9))
  assert controller.active
  assert controller.center_error == pytest.approx(0.225, abs=1e-6)
  assert output > 0.0


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


# --- curves ---

def _bent(model, curvature):
  # bend the lane lines and the path together, as a curve ahead would: y += curvature / 2 * x^2
  for line in (*model.laneLines, model.position):
    line.y = line.y + curvature / 2 * line.x ** 2
  return model


def test_curve_gives_the_model_no_authority():
  # A confident model 0.3 m off centre keeps part of that on a straight, but in a curve it is cutting the corner
  def settled(curvature, authority):
    return _converge(_bent(_model(model_y=0.3, path_std=0.1), curvature), authority=authority)[1]
  assert settled(0.0, 1.0) > settled(0.0, 0.0)
  assert settled(1 / 300, 1.0) == pytest.approx(settled(1 / 300, 0.0), abs=1e-9)


def test_gentle_bend_is_not_a_curve():
  # A 1500 m radius is nearly straight: a model easing over for oncoming traffic there keeps its authority
  bend = 1 / 1500
  assert _converge(_bent(_model(model_y=0.3), bend), authority=1.0)[1] > _converge(_bent(_model(model_y=0.3), bend), authority=0.0)[1]


def test_curve_push_is_capped():
  from openpilot.sunnypilot.selfdrive.controls.lib.lane_centering import CURVE_MAX_LAT_ACCEL
  controller, output = _converge(_bent(_model(model_y=0.6), 1 / 300))
  assert output == pytest.approx(-CURVE_MAX_LAT_ACCEL / V_EGO ** 2, rel=1e-3), "should be pinned at the curve cap"
  assert controller.clipped
  # the same cut on a straight is not limited by the curve cap
  assert abs(_converge(_model(model_y=0.6, path_std=0.9))[1]) > CURVE_MAX_LAT_ACCEL / V_EGO ** 2


def test_curve_cap_leaves_small_corrections_alone():
  controller, output = _converge(_bent(_model(model_y=0.1), 1 / 300))
  straight = _converge(_model(model_y=0.1))[1]
  assert output == pytest.approx(straight, rel=1e-6)
  assert not controller.clipped


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


# --- single-line mode: only one line trusted ---
#
# Every case runs both ways round, trusted line on the car's left (line 1) and on its right (line 2),
# because nothing in this mode may depend on which side of the road the curb is. `near` is the
# trusted line's distance from the model path, `far` the untrusted line's, both positive.

NO_EDGE = None


def _sloped(y_near, y_far):
  """A line at y_near 4 m ahead and y_far at the lookahead, as a line peeling away would be."""
  y = np.interp(XS, [SINGLE_LINE_NEAR_X, V_EGO], [y_near, y_far])
  return SimpleNamespace(x=XS.copy(), y=y, yStd=np.full_like(XS, 0.1))


def _one_line(side, near=1.0, far=2.4, far_prob=0.3, near_edge=NO_EDGE, far_edge=NO_EDGE, near_at_4m=None, model_y=0.0,
              lane_change=0):
  """side 1: the trusted line is on the car's left; side 2: on its right.

  near_edge / far_edge: how far beyond the trusted line / from the path the model puts a road edge it
  is sure of. NO_EDGE gives an edge it is unsure of, which the mode must not use.
  """
  s = -1.0 if side == 1 else 1.0   # direction from the path to the trusted line
  trusted = _path(model_y + s * near) if near_at_4m is None else _sloped(model_y + s * near_at_4m, model_y + s * near)
  other = _path(model_y - s * far)
  lines = [trusted, other] if side == 1 else [other, trusted]
  edges = [_path(model_y + s * (near + (near_edge or 0.0))), _path(model_y - s * (far_edge or 0.0))]
  edge_stds = [0.5 if near_edge is not NO_EDGE else 5.0, 0.5 if far_edge is not NO_EDGE else 5.0]
  if side == 2:
    edges, edge_stds = edges[::-1], edge_stds[::-1]
  probs = [0.9, far_prob] if side == 1 else [far_prob, 0.9]
  return SimpleNamespace(
    laneLines=[_path(0.0), *lines, _path(0.0)],
    laneLineProbs=[0.0, *probs, 0.0],
    laneLineStds=[0.0, 0.1, 0.1, 0.0],
    roadEdges=edges,
    roadEdgeStds=edge_stds,
    position=_path(model_y, 0.1),
    meta=SimpleNamespace(laneChangeState=lane_change),
  )


def _away(side):
  """Sign of a correction that steers away from the trusted line."""
  return 1.0 if side == 1 else -1.0


def _single(frames=300, speed=V_EGO, model=None, **params):
  controller = LaneCenteringController(FakeParams(single_line=True, **params))
  controller.get_params()
  output = 0.0
  for _ in range(frames):
    output = _update(controller, model, speed=speed)
  return controller, output


SIDES = pytest.mark.parametrize("side", [1, 2])


@SIDES
def test_single_line_off_by_default(side):
  assert _converge(_one_line(side))[1] == 0.0


@SIDES
def test_single_line_pushes_away_from_a_close_line(side):
  controller, output = _single(model=_one_line(side, near=1.0))
  assert output * _away(side) > 0.0
  assert controller.active and controller.single_line
  assert controller.center_error == pytest.approx(_away(side) * 0.5, abs=1e-6)


@SIDES
def test_single_line_reports_the_side_of_its_line_for_the_screen(side):
  # The indicator marks the line on the side it is on, which is opposite the push. It is reported
  # while holding too, when there is no push to read the side from.
  for near in (1.0, 1.6):
    controller, _ = _single(model=_one_line(side, near=near))
    assert controller.single_line
    assert controller.line_side == -_away(side)
  controller.reset()
  assert not controller.single_line and controller.line_side == 0
  # Two good lines is ordinary centering, with no side to mark
  controller, _ = _single(model=_model(left=-1.0, right=2.4))
  assert not controller.single_line and controller.line_side == 0


@SIDES
def test_single_line_leaves_a_car_already_far_enough(side):
  controller, output = _single(model=_one_line(side, near=1.6))
  assert output == 0.0
  assert controller.active and controller.holding


@SIDES
def test_single_line_does_not_follow_a_line_peeling_away(side):
  # 1.0 m from the path at the lookahead but 1.5 m at 4 m: the line is running off, as at an exit
  assert _single(model=_one_line(side, near=1.0, near_at_4m=1.5))[1] == 0.0
  # A line that wobbles by less than the tolerance is still followed
  assert _single(model=_one_line(side, near=1.0, near_at_4m=1.2))[1] * _away(side) > 0.0


@SIDES
def test_single_line_stops_at_the_middle_of_the_estimated_lane(side):
  # The untrusted far line makes a 2.8 m lane, so the push stops at 1.4 m, not the 1.5 m asked for
  controller, _ = _single(model=_one_line(side, near=1.0, far=1.8))
  assert controller.center_error == pytest.approx(_away(side) * 0.4, abs=1e-6)


@SIDES
def test_single_line_does_nothing_in_a_lane_too_narrow_to_trust(side):
  controller, output = _single(model=_one_line(side, near=1.0, far=1.4))
  assert output == 0.0
  assert not controller.active


@SIDES
def test_single_line_keeps_clear_of_the_far_road_edge(side):
  # A sure road edge 2.1 m from the car's center on the far side only leaves 0.3 m of the 0.5 m push
  controller, _ = _single(model=_one_line(side, near=1.0, far=3.0, far_edge=2.1))
  assert controller.center_error == pytest.approx(_away(side) * 0.3, abs=1e-6)
  # and one already within reach of the tire allows no push at all
  controller, output = _single(model=_one_line(side, near=1.0, far=3.0, far_edge=1.7))
  assert output == 0.0


@SIDES
def test_single_line_does_nothing_when_the_far_side_is_unknown(side):
  model = _one_line(side, near=1.0)
  model.laneLines[3 - side].y = np.full_like(XS, np.nan)
  controller, output = _single(model=model)
  assert output == 0.0
  assert not controller.active


@SIDES
def test_single_line_uses_the_edge_line_distance_at_the_road_edge(side):
  # Trusted line 1.4 m away. Edge lines want 1.7 m, lane lines 1.3 m.
  dists = {"edge_dist": 1.7, "lane_dist": 1.3}
  at_edge, _ = _single(model=_one_line(side, near=1.4, far=3.0, near_edge=1.0), **dists)
  assert at_edge.center_error == pytest.approx(_away(side) * 0.3, abs=1e-6)
  # A road edge a whole lane beyond it makes it a lane line, already far enough
  in_road, output = _single(model=_one_line(side, near=1.4, far=3.0, near_edge=3.5), **dists)
  assert output == 0.0 and in_road.holding
  # Unsure where the road edge is: the smaller of the two, so the smaller push
  unclear, output = _single(model=_one_line(side, near=1.4, far=3.0), **dists)
  assert output == 0.0 and unclear.holding


def test_single_line_distances_are_clamped():
  controller, _ = _single(frames=1, model=_one_line(1), edge_dist=0.0, lane_dist=9.0)
  assert controller.edge_line_distance == SINGLE_LINE_MIN_DISTANCE
  assert controller.lane_line_distance == SINGLE_LINE_MAX_DISTANCE


@SIDES
def test_single_line_ignores_the_offset_and_model_authority(side):
  model = _one_line(side, near=1.0)
  plain = _single(model=model)[1]
  assert _single(model=model, offset=0.3)[1] == pytest.approx(plain, abs=1e-12)
  assert _single(model=model, offset=-0.3)[1] == pytest.approx(plain, abs=1e-12)
  assert _single(model=model, authority=0.0)[1] == pytest.approx(plain, abs=1e-12)


@SIDES
def test_single_line_steps_aside_like_the_two_line_mode(side):
  model = _one_line(side, near=1.0)
  controller = LaneCenteringController(FakeParams(single_line=True))
  controller.get_params()
  assert _update(controller, model, blinker=True) == 0.0
  assert _update(controller, model, speed=3.0) == 0.0
  assert _update(controller, _one_line(side, near=1.0, lane_change=1)) == 0.0
  assert _update(controller, model, active=False) == 0.0


@SIDES
def test_single_line_is_smoothed_and_capped(side):
  model = _one_line(side, near=0.5, far=3.5)
  controller = LaneCenteringController(FakeParams(single_line=True))
  controller.get_params()
  first = _update(controller, model, speed=6.0)
  second = _update(controller, model, speed=6.0)
  assert 0.0 < abs(first) < abs(second), "correction should build up over several frames, not jump"
  _, settled = _single(model=model, frames=600, speed=6.0)
  assert settled == pytest.approx(_away(side) * MAX_RAW_CORRECTION * DEFAULT_GAIN, rel=1e-3)


def test_single_line_mode_leaves_two_line_centering_alone():
  # With both lines trusted, turning single-line mode on changes nothing
  for model in (_model(left=-1.5, right=2.1), _model(left=-2.1, right=1.5, path_std=0.9), _model()):
    off = _converge(model)[1]
    on = _single(model=model)[1]
    assert on == off


def test_single_line_mode_needs_one_trusted_line():
  assert _single(model=_model(left=-1.0, right=2.4, lane_prob=0.2))[1] == 0.0


def _two_point_line(lookahead, near, far):
  """A line known only at 4 m and at the lookahead, which is all the replay file keeps."""
  return SimpleNamespace(x=np.array([0.0, SINGLE_LINE_NEAR_X, lookahead, 60.0]), y=np.array([near, near, far, far]), yStd=np.full(4, 0.1))


def _ramp_frames():
  path = os.path.join(os.path.dirname(__file__), "lane_centering_on_ramp.csv")
  with open(path) as f:
    rows = [{k: float(v) for k, v in r.items()} for r in csv.DictReader(line for line in f if not line.startswith("#"))]
  for r in rows:
    la = float(np.clip(r["v"], 8.0, 35.0))
    model = SimpleNamespace(
      laneLines=[_two_point_line(la, 0.0, 0.0), _two_point_line(la, r["y1n"], r["y1"]), _two_point_line(la, r["y2n"], r["y2"]),
                 _two_point_line(la, 0.0, 0.0)],
      laneLineProbs=[0.0, r["p1"], r["p2"], 0.0],
      laneLineStds=[0.0, r["s1"], r["s2"], 0.0],
      roadEdges=[_two_point_line(la, r["e0n"], r["e0"]), _two_point_line(la, r["e1n"], r["e1"])],
      roadEdgeStds=[r["es0"], r["es1"]],
      position=_two_point_line(la, r["path_n"], r["path"]),
      meta=SimpleNamespace(laneChangeState=int(r["lc_state"])),
    )
    yield r, model


@pytest.mark.parametrize("single_line", [False, True])
def test_on_ramp_replay(single_line):
  # The drive that started this: a curved single-lane on-ramp where the far (left) edge line is too
  # faint to trust and the car hugged the inside line. The log is at 20 Hz; each frame is held for
  # the 100 Hz control loop, and every 4th frame was kept, so each is held for 20 control frames.
  controller = LaneCenteringController(FakeParams(single_line=single_line))
  controller.get_params()
  on_ramp = []
  for r, model in _ramp_frames():
    for _ in range(20):
      output = controller.update(0.0, model, r["v"], bool(r["blinker"]), True, True)
    if controller.single_line:
      assert controller.center_error <= 0.0, f"aimed towards the inside line at {r['t']:.1f} s"
    if 42.0 <= r["t"] <= 50.0:
      on_ramp.append(output)
  if single_line:
    assert np.median(on_ramp) < -1e-4, "should push away from the inside line on the ramp"
  else:
    assert max(abs(x) for x in on_ramp) < 1e-6, "without single-line mode the ramp gets nothing, as before"
