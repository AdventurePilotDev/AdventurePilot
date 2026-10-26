"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from collections.abc import Callable
import pyray as rl

from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.sunnypilot.widgets.list_view import option_item_sp, toggle_item_sp, LineSeparatorSP
from openpilot.system.ui.widgets.network import NavButton
from openpilot.system.ui.widgets.scroller_tici import Scroller
from openpilot.system.ui.widgets import Widget


def lane_centering_offset_label(value: int) -> str:
  # Stored in the openpilot frame where positive is to the right, but say so in words
  if value == 0:
    return tr("centered")
  side = tr("right") if value > 0 else tr("left")
  amount = f"{abs(value)} cm" if ui_state.is_metric else f"{abs(value) / 2.54:.1f} in"
  return f"{amount} {side}"


def centering_strength_label(value: int) -> str:
  if value == 60:
    return tr("60% (default)")
  if value == 0:
    return tr("off")
  return f"{value}%"


def deadband_label(value: int) -> str:
  if value == 0:
    return tr("off, always correcting")
  amount = f"{value} cm" if ui_state.is_metric else f"{value / 2.54:.1f} in"
  if value == 2:
    return amount + " " + tr("(default)")
  return amount


def e2e_authority_label(value: int) -> str:
  if value == 0:
    return tr("always center")
  if value == 100:
    return tr("model decides")
  return f"{value}%"


def single_line_distance_label(value: int) -> str:
  # Stored in the model's own distances, where 1.5 m is the car's normal place in a lane, so it is
  # shown as a step either side of normal rather than as a gap to the paint
  if value == 150:
    return tr("normal")
  step = abs(value - 150)
  amount = f"{step} cm" if ui_state.is_metric else f"{step / 2.54:.1f} in"
  return f"{amount} " + (tr("further from the line") if value > 150 else tr("closer to the line"))


class LaneCenteringSettingsLayout(Widget):
  def __init__(self, back_btn_callback: Callable):
    super().__init__()
    self._back_button = NavButton(tr("Back"))
    self._back_button.set_click_callback(back_btn_callback)

    items = self._initialize_items()
    self._scroller = Scroller(items, line_separator=False, spacing=0)

  def _initialize_items(self):
    self._offset = option_item_sp(
      param="LaneCenteringOffset",
      title=lambda: tr("Position in Lane"),
      min_value=-30,
      max_value=30,
      value_change_step=1,
      description=lambda: (tr("Sit deliberately off the middle of the lane. The car is never aimed closer than ")
                           + ("1.1 m" if ui_state.is_metric else "3.6 ft") + tr(" to either line, so a large offset in a narrow lane is trimmed back.")),
      use_float_scaling=True,
      label_callback=lane_centering_offset_label,
    )
    self._e2e_authority = option_item_sp(
      param="LaneCenteringE2EAuthority",
      title=lambda: tr("Let the Model Sit Off Center"),
      min_value=0,
      max_value=100,
      value_change_step=10,
      description=lambda: tr("How much freedom the driving model keeps to sit well off the middle of the lane when it is sure of " +
                             "itself, which usually means it is avoiding something. At 100% it always gets its way. At 0% the car " +
                             "is centered regardless. This only applies to large departures; small ones are always corrected."),
      use_float_scaling=True,
      label_callback=e2e_authority_label,
    )

    # Permanent. Both this and the tolerance below stay on screen: what suits one car, model and
    # road does not suit the next, and comparing values needs them changeable between legs of a
    # single drive rather than a rebuild per value.
    self._strength = option_item_sp(
      param="LaneCenteringGain",
      title=lambda: tr("Centering Strength"),
      min_value=0,
      max_value=100,
      value_change_step=10,
      description=lambda: tr("Sets how hard the car is pulled back toward the middle of the lane. Higher means it sits closer to " +
                             "the middle but works harder to get there. 60% suits the cars and roads tested so far. Lower feels " +
                             "smoother than it is: 30% is too weak to hold against a road crown and leaves the car sitting off " +
                             "center all day."),
      use_float_scaling=True,
      label_callback=centering_strength_label,
    )

    self._deadband = option_item_sp(
      param="LaneCenteringDeadband",
      title=lambda: tr("Close Enough To Center"),
      min_value=0,
      max_value=15,
      value_change_step=1,
      description=lambda: tr("How far off the middle of the lane the car is allowed to sit before lane centering does " +
                             "anything about it. Smaller sits closer to the middle. It does not make the car calmer: a " +
                             "wide tolerance lets the car drift to the edge and get kicked back, which reverses more " +
                             "often, not less. Road testing settled on 2 cm."),
      use_float_scaling=True,
      label_callback=deadband_label,
    )

    self._single_line = toggle_item_sp(
      param="LaneCenteringSingleLine",
      title=lambda: tr("One-Line Mode"),
      description=lambda: tr("When only one lane line is clearly visible, such as on an on-ramp with a faded edge line or a town " +
                             "street with a center line and a bare curb, keep the car from sitting too close to that line. It only " +
                             "ever moves the car away from the visible line, never past the middle of the lane, and not at all " +
                             "when it cannot see anything on the other side. Position in Lane and Let the Model Sit Off Center " +
                             "do not apply in this mode. Experimental."),
    )
    self._edge_distance = option_item_sp(
      param="LaneCenteringSingleLineEdgeDist",
      title=lambda: tr("One-Line Mode: Edge Line"),
      min_value=120,
      max_value=180,
      value_change_step=5,
      description=lambda: tr("How far One-Line Mode keeps the car from a visible line at the edge of the road, as a step " +
                             "either side of the car's normal place in a lane."),
      use_float_scaling=True,
      label_callback=single_line_distance_label,
    )
    self._lane_distance = option_item_sp(
      param="LaneCenteringSingleLineLaneDist",
      title=lambda: tr("One-Line Mode: Center or Lane Line"),
      min_value=120,
      max_value=180,
      value_change_step=5,
      description=lambda: tr("How far One-Line Mode keeps the car from a visible center line or line between lanes, as a step " +
                             "either side of the car's normal place in a lane. When it cannot tell which kind of line it is, " +
                             "the closer of the two settings is used."),
      use_float_scaling=True,
      label_callback=single_line_distance_label,
    )

    self._display = toggle_item_sp(
      param="LaneCenteringDisplay",
      title=lambda: tr("Show Correction On Screen"),
      description=lambda: tr("Draws a small bar while driving showing which way lane centering is nudging the car and " +
                             "how hard. The bar fills to the side the car is being pulled, stays dim while the car is " +
                             "already close enough to the middle, and turns gray when the lane lines cannot be trusted."),
    )

    return [
      self._offset,
      LineSeparatorSP(40),
      self._e2e_authority,
      LineSeparatorSP(40),
      self._strength,
      LineSeparatorSP(40),
      self._deadband,
      LineSeparatorSP(40),
      self._single_line,
      LineSeparatorSP(40),
      self._edge_distance,
      LineSeparatorSP(40),
      self._lane_distance,
      LineSeparatorSP(40),
      self._display,
    ]

  def _update_state(self):
    super()._update_state()
    single_line = self._single_line.action_item.get_state()
    self._edge_distance.set_visible(single_line)
    self._lane_distance.set_visible(single_line)

  def _render(self, rect):
    self._back_button.set_position(self._rect.x, self._rect.y + 20)
    self._back_button.render()
    # subtract button
    content_rect = rl.Rectangle(rect.x, rect.y + self._back_button.rect.height + 40, rect.width, rect.height - self._back_button.rect.height - 40)
    self._scroller.render(content_rect)

  def show_event(self):
    self._scroller.show_event()
