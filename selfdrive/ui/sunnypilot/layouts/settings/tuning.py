"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.sunnypilot.widgets.list_view import option_item_sp
from openpilot.system.ui.widgets.scroller_tici import Scroller
from openpilot.system.ui.widgets import Widget


class TuningLayout(Widget):
  """Tuning panel: Kp Low Speed / Kp High Speed (matches 2a00dafc0 Qt tuning panel)."""

  def __init__(self):
    super().__init__()
    items = self._initialize_items()
    self._scroller = Scroller(items, line_separator=True, spacing=0)

  def _initialize_items(self):
    self._kp_low_speed = option_item_sp(
      title=lambda: tr("Kp Low Speed"),
      param="KpLowSpeed",
      description=lambda: tr("Proportional gain multiplier at low speeds (6.7 m/s). Used in custom error calculation."),
      min_value=50,
      max_value=500,
      value_change_step=5,
      label_callback=(lambda x: f"{x / 100:.2f}"),
      use_float_scaling=True,
    )
    self._kp_high_speed = option_item_sp(
      title=lambda: tr("Kp High Speed"),
      param="KpHighSpeed",
      description=lambda: tr("Proportional gain multiplier at high speeds (33.5 m/s). Used in custom error calculation."),
      min_value=50,
      max_value=500,
      value_change_step=5,
      label_callback=(lambda x: f"{x / 100:.2f}"),
      use_float_scaling=True,
    )
    return [self._kp_low_speed, self._kp_high_speed]

  def _update_state(self):
    super()._update_state()
    # Allow tuning Kp while on road so adjustments can be made during drive
    self._kp_low_speed.action_item.set_enabled(True)
    self._kp_high_speed.action_item.set_enabled(True)

  def _render(self, rect):
    self._scroller.render(rect)

  def show_event(self):
    self._scroller.show_event()
