"""The half second window behind every word on the lane centering indicator, One-Line Mode in particular.

Drives _sample and _commit_window directly on an indicator built without a window, so nothing is drawn.
"""
from types import SimpleNamespace

import pytest

pytest.importorskip("pyray")

from openpilot.selfdrive.ui.sunnypilot.onroad.lane_centering_indicator import (
  CORRECTING, GATED, HOLDING, LIGHT_BLUE, MINT, RED, LaneCenteringIndicator)


def _indicator():
  ind = LaneCenteringIndicator.__new__(LaneCenteringIndicator)
  ind._counts = {GATED: 0, HOLDING: 0, CORRECTING: 0}
  ind._start_window(0.0)
  return ind


def _frame(active=True, holding=False, clipped=False, single_line=False, line_side=0, offset=0.1):
  return SimpleNamespace(active=active, holding=holding, clipped=clipped, singleLine=single_line, lineSide=line_side,
                         offset=offset)


def _window(frames, push=0.1):
  ind = _indicator()
  for lc in frames:
    ind._sample(lc, push)
  ind._commit_window()
  return ind


@pytest.mark.parametrize("side", [-1, 1])
def test_one_line_window_is_shown_with_its_side(side):
  ind = _window([_frame(single_line=True, line_side=side)] * 10)
  assert ind._shown_state == CORRECTING
  assert ind._shown_single_line
  assert ind._shown_line_side == side
  assert ind._shown_color == LIGHT_BLUE


def test_one_line_holding_is_light_blue():
  ind = _window([_frame(holding=True, single_line=True, line_side=1)] * 10)
  assert ind._shown_state == HOLDING
  assert ind._shown_single_line
  assert ind._shown_color == LIGHT_BLUE


def test_a_flicker_of_one_line_mode_is_not_shown():
  # A line gaining or losing trust for a few frames must not flash the mark or the words
  ind = _window([_frame(single_line=True, line_side=1)] * 3 + [_frame()] * 7)
  assert not ind._shown_single_line
  assert ind._shown_line_side == 0
  assert ind._shown_color == MINT


def test_gated_frames_do_not_count_toward_one_line_mode():
  ind = _window([_frame(active=False)] * 7 + [_frame(single_line=True, line_side=1)] * 3)
  assert ind._shown_state == GATED
  assert not ind._shown_single_line
  assert ind._shown_line_side == 0


def test_clipped_still_shows_red_in_one_line_mode():
  ind = _window([_frame(clipped=True, single_line=True, line_side=-1)] * 10)
  assert ind._shown_single_line
  assert ind._shown_color == RED


def test_ordinary_centering_is_unchanged():
  ind = _window([_frame()] * 10)
  assert not ind._shown_single_line
  assert ind._shown_line_side == 0
  assert ind._shown_color == MINT
