"""
Shows which way lane centering is nudging the car and how hard.

The correction is a curvature delta, which means nothing to a driver, so it is shown as the
sideways push it produces: push = correction * vEgo^2, in m/s^2. Away from the lookahead
clamps that reduces to 2 * (distance off centre beyond the deadband) * gain, which is why the
reading tracks the strength setting rather than cancelling it out.

The widget runs at two rates. The bar is live, redrawn every frame, so a lane gate dropout of a
few frames still shows. Everything written in words or figures is latched: it is replaced twice a
second by the average of the window just ended, because a number redrawn 20 times a second cannot
be read at all, and near the tolerance the state itself flickers.

Direction is carried by which side of the centre detent the bar fills, and by the L or R after
the distance off centre, so colour is free to carry effort and state. The two numbers, the push
and the distance, share a size and a colour so they read as a pair; every word and unit beside
them is plain white. It is all one face: the units were set in the medium weight and read as an
afterthought beside two bold numbers, so they are bold too, and only size separates them now.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import time

import pyray as rl

from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import gui_app, FontWeight
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.lib.text_measure import measure_text_cached

FULL_SCALE = 0.5      # m/s^2 at the end of the bar, chosen to suit ordinary driving, not the controller's limit
AMBER_ABOVE = 0.5     # m/s^2, past the end of the scale
SMOOTH_TAU = 0.1      # s, just enough to stop the bar shimmering
TEXT_REFRESH = 0.5    # s, how long a figure stands before it is replaced by the next window's average

# The three states a window can be summarised as. Amber and red are shades of CORRECTING.
GATED = "gated"
HOLDING = "holding"
CORRECTING = "correcting"

# Big UI geometry, in the 2160x1080 space. Nothing is drawn behind the widget, so PILL_W and
# PILL_H are a layout box only: the width positions the offset text against its right edge and
# the bar spans it, and the height sets where the bar sits.
PILL_W = 630
PILL_H = 144
PILL_TOP = 340
PAD = 36
VALUE_SIZE = 63
UNIT_SIZE = 36
SMALL_SIZE = 39
UNIT_GAP = 12         # between a value and the unit after it, used by both pairs
BAR_H = 24

# comma 4 geometry, as fractions of the screen, until the canvas is settled
COMPACT_W_FRAC = 0.38
COMPACT_H_FRAC = 0.05
COMPACT_BOTTOM_FRAC = 0.29

GREY = rl.Color(145, 155, 149, 255)
DIM_WHITE = rl.Color(255, 255, 255, 200)
MINT = rl.Color(128, 216, 166, 255)
AMBER = rl.Color(255, 188, 0, 255)
RED = rl.Color(226, 86, 75, 255)

TRACK = rl.Color(255, 255, 255, 70)
GRADUATION = rl.Color(255, 255, 255, 120)
DETENT = rl.Color(255, 255, 255, 235)
WHITE = rl.Color(255, 255, 255, 255)  # every word and unit, so the two numbers are the only coloured text

# Everything is drawn straight onto the road, so it carries its own contrast
SHADOW = rl.Color(0, 0, 0, 150)
SHADOW_OFF = 3.0

M_TO_CM = 100.0
M_TO_IN = 39.3701


def _clamp(v: float, lo: float, hi: float) -> float:
  return max(lo, min(hi, v))


class LaneCenteringIndicator:
  def __init__(self, compact: bool = False):
    self._compact = compact
    self._font_bold: rl.Font = gui_app.font(FontWeight.BOLD)

    # live, every frame: the bar only
    self._push_filter = FirstOrderFilter(0.0, SMOOTH_TAU, 1 / gui_app.target_fps)
    self._visible = False
    self._active = False
    self._colour = MINT

    # latched, replaced whole every TEXT_REFRESH: everything written in words or figures
    self._shown_state = GATED
    self._shown_push = 0.0
    self._shown_offset = 0.0
    self._shown_colour = GREY

    self._counts = {GATED: 0, HOLDING: 0, CORRECTING: 0}
    self._n_valid = 0
    self._n_clipped = 0
    self._sum_push = 0.0
    self._sum_offset = 0.0
    self._next_commit = 0.0

  def update(self) -> None:
    sm = ui_state.sm

    was_visible = self._visible
    self._visible = False
    if not (ui_state.lane_centering_display and ui_state.lane_centering_enabled):
      return
    if sm.recv_frame["carControlSP"] < ui_state.started_frame:
      return
    if not sm['carControl'].latActive:
      return

    self._visible = True
    if not was_visible:
      # nothing from before the widget appeared is worth averaging into the first figure
      self._start_window(0.0)

    lc = sm['carControlSP'].laneCentering
    v_ego = sm['carState'].vEgo

    self._active = bool(lc.active) and not bool(lc.holding)

    push = float(lc.correction) * v_ego * v_ego if lc.active else 0.0
    self._push_filter.update(push)

    # The bar is live every frame. That is deliberate: a lane gate dropout lasting a few frames
    # shows as a flash of grey, and spotting those through bends is one reason the widget exists.
    if not lc.active:
      self._colour = GREY
    elif lc.holding:
      self._colour = DIM_WHITE
    elif lc.clipped:
      self._colour = RED
    elif abs(self._push_filter.x) >= AMBER_ABOVE:
      self._colour = AMBER
    else:
      self._colour = MINT

    self._sample(lc, push)

    now = time.monotonic()
    if now >= self._next_commit:
      self._commit_window()
      self._start_window(now + TEXT_REFRESH)

  # --- the half second window behind every figure on screen ---

  def _start_window(self, next_commit: float) -> None:
    self._counts = dict.fromkeys(self._counts, 0)
    self._n_valid = 0
    self._n_clipped = 0
    self._sum_push = 0.0
    self._sum_offset = 0.0
    self._next_commit = next_commit

  def _sample(self, lc, push: float) -> None:
    if not lc.active:
      self._counts[GATED] += 1
      return

    # Push and offset only mean anything on a frame where the gates passed, so gated frames are
    # counted for the state but kept out of both averages rather than averaged in as zeroes.
    self._counts[HOLDING if lc.holding else CORRECTING] += 1
    self._n_valid += 1
    self._n_clipped += bool(lc.clipped)
    self._sum_push += push
    self._sum_offset += float(lc.offset)

  def _commit_window(self) -> None:
    """Replace everything drawn in words or figures with one summary of the window just ended."""
    self._shown_push = self._sum_push / self._n_valid if self._n_valid else 0.0
    self._shown_offset = self._sum_offset / self._n_valid if self._n_valid else 0.0
    # whichever state held for most of the window, so a flicker across the tolerance settles
    self._shown_state = max(self._counts, key=self._counts.__getitem__)

    if self._shown_state == GATED:
      self._shown_colour = GREY
    elif self._shown_state == HOLDING:
      self._shown_colour = DIM_WHITE
    elif self._n_clipped * 2 > self._n_valid:
      self._shown_colour = RED
    elif abs(self._shown_push) >= AMBER_ABOVE:
      self._shown_colour = AMBER
    else:
      self._shown_colour = MINT

  def render(self, rect: rl.Rectangle) -> None:
    if not self._visible:
      return

    if self._compact:
      self._render_compact(rect)
    else:
      self._render_pill(rect)

  # --- the two forms ---

  def _render_compact(self, rect: rl.Rectangle) -> None:
    w = rect.width * COMPACT_W_FRAC
    h = max(6.0, rect.height * COMPACT_H_FRAC)
    x = rect.x + (rect.width - w) / 2.0
    y = rect.y + rect.height * (1.0 - COMPACT_BOTTOM_FRAC) - h
    self._draw_bar(x, y, w, h)

  def _render_pill(self, rect: rl.Rectangle) -> None:
    x = rect.x + (rect.width - PILL_W) / 2.0
    y = rect.y + PILL_TOP

    text_y = y + 18
    left = x + PAD

    # Everything below is the latched half second summary, never the live frame, so a figure
    # stands long enough to be read. The bar underneath is the live one.
    if self._shown_state == CORRECTING:
      # A drawn triangle rather than an arrow glyph, so this never depends on font coverage
      tri_r = 20.0
      tri_cx = left + tri_r
      self._draw_arrow(tri_cx, text_y + VALUE_SIZE * 0.5, tri_r, self._shown_push >= 0)

      value = f"{abs(self._shown_push):.2f}"
      value_x = tri_cx + tri_r + 18
      self._draw_text(self._font_bold, value, rl.Vector2(value_x, text_y), VALUE_SIZE, 0, self._shown_colour)

      value_w = measure_text_cached(self._font_bold, value, VALUE_SIZE).x
      self._draw_text(self._font_bold, "m/s2", rl.Vector2(value_x + value_w + UNIT_GAP, text_y + VALUE_SIZE - UNIT_SIZE),
                      UNIT_SIZE, 0, WHITE)
    else:
      # holding is a deliberate state so it reads as ordinary white; gated keeps its grey, because
      # "not acting because there are no lines" is the one the driver most needs to tell apart
      holding = self._shown_state == HOLDING
      label = tr("centred") if holding else tr("no lines")
      self._draw_text(self._font_bold, label, rl.Vector2(left, text_y + 6), SMALL_SIZE + 9, 0,
                      WHITE if holding else self._shown_colour)

    self._draw_offset(x + PILL_W - PAD, text_y)

    self._draw_bar(x + PAD, y + PILL_H - 30 - BAR_H, PILL_W - 2 * PAD, BAR_H)

  def _draw_offset(self, right: float, text_y: float) -> None:
    """The distance off centre, right aligned to `right`, in the same bold face, size and colour as
    the push value on the other side, so the two numbers read as a pair. The unit after it is white."""
    amount, unit = self._offset_parts()

    if not amount:
      if unit:
        unit_w = measure_text_cached(self._font_bold, unit, SMALL_SIZE).x
        self._draw_text(self._font_bold, unit, rl.Vector2(right - unit_w, text_y + VALUE_SIZE - SMALL_SIZE),
                        SMALL_SIZE, 0, WHITE)
      return

    amount_w = measure_text_cached(self._font_bold, amount, VALUE_SIZE).x
    unit_w = measure_text_cached(self._font_bold, unit, UNIT_SIZE).x
    amount_x = right - unit_w - UNIT_GAP - amount_w

    self._draw_text(self._font_bold, amount, rl.Vector2(amount_x, text_y), VALUE_SIZE, 0, self._shown_colour)
    self._draw_text(self._font_bold, unit, rl.Vector2(amount_x + amount_w + UNIT_GAP, text_y + VALUE_SIZE - UNIT_SIZE),
                    UNIT_SIZE, 0, WHITE)

  def _draw_arrow(self, cx: float, cy: float, r: float, pointing_right: bool) -> None:
    self._arrow_triangle(cx + SHADOW_OFF, cy + SHADOW_OFF, r, pointing_right, SHADOW)
    self._arrow_triangle(cx, cy, r, pointing_right, self._shown_colour)

  # --- drawing helpers ---

  @staticmethod
  def _arrow_triangle(cx: float, cy: float, r: float, pointing_right: bool, colour: rl.Color) -> None:
    # raylib wants the vertices counter-clockwise on screen, which is what star_icon.py does too
    back_x = cx - r * 0.6 if pointing_right else cx + r * 0.6
    tip = rl.Vector2(cx + r if pointing_right else cx - r, cy)
    upper = rl.Vector2(back_x, cy - r * 0.85)
    lower = rl.Vector2(back_x, cy + r * 0.85)
    if pointing_right:
      rl.draw_triangle(tip, upper, lower, colour)
    else:
      rl.draw_triangle(tip, lower, upper, colour)

  def _draw_text(self, font: rl.Font, text: str, pos: rl.Vector2, size: float, spacing: float, colour: rl.Color) -> None:
    """draw_text_ex with a dark copy behind it. With nothing drawn behind the widget every string
    has to hold its own contrast, over a bright sky or fresh concrete as much as over tarmac."""
    rl.draw_text_ex(font, text, rl.Vector2(pos.x + SHADOW_OFF, pos.y + SHADOW_OFF), size, spacing, SHADOW)
    rl.draw_text_ex(font, text, pos, size, spacing, colour)

  # --- shared bar ---

  def _draw_bar(self, x: float, y: float, w: float, h: float) -> None:
    rl.draw_rectangle_rounded(rl.Rectangle(x + SHADOW_OFF, y + SHADOW_OFF, w, h), 1.0, 8, SHADOW)
    rl.draw_rectangle_rounded(rl.Rectangle(x, y, w, h), 1.0, 8, TRACK)

    # half scale graduations
    grad_w = max(1.0, h * 0.14)
    for frac in (0.25, 0.75):
      rl.draw_rectangle(int(x + w * frac - grad_w / 2), int(y + h * 0.22), int(grad_w), int(h * 0.56), GRADUATION)

    cx = x + w / 2.0
    if self._active:
      span = _clamp(self._push_filter.x / FULL_SCALE, -1.0, 1.0) * (w / 2.0)
      fill_w = abs(span)
      if fill_w >= 1.0:
        fill_x = cx if span >= 0 else cx - fill_w
        rl.draw_rectangle_rounded(rl.Rectangle(fill_x, y, fill_w, h), 1.0, 8, self._colour)
    else:
      stub = max(3.0, h * 0.28)
      rl.draw_rectangle_rounded(rl.Rectangle(cx - stub / 2, y, stub, h), 1.0, 8, self._colour)

    # the detent sits on top so the centre stays readable at any fill
    detent_w = max(2.0, h * 0.14)
    rl.draw_rectangle(int(cx - detent_w / 2), int(y - h * 0.3), int(detent_w), int(h * 1.6), DETENT)

  def _offset_parts(self) -> tuple[str, str]:
    """The distance off centre, split into the part that takes the value colour and the part that
    stays white. An empty first part means there is no number to show and the second stands alone.
    Like the rest of the text this is the window average, not the current frame."""
    if self._shown_state == GATED:
      return "", ""

    if ui_state.is_metric:
      amount, unit = abs(self._shown_offset) * M_TO_CM, "cm"
    else:
      amount, unit = abs(self._shown_offset) * M_TO_IN, "in"

    if amount < 0.5:
      return "", tr("on centre")

    # The reported offset is the error the controller is closing, so the car sits on the far side of it
    side = tr("L") if self._shown_offset > 0 else tr("R")
    return f"{amount:.0f}{side}", unit
