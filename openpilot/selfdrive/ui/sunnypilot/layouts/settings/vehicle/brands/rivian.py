"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from openpilot.selfdrive.ui.sunnypilot.layouts.settings.vehicle.brands.base import BrandSettings
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.sunnypilot.widgets.list_view import option_item_sp, toggle_item_sp


DESCRIPTIONS = {
  'offtrack': tr_noop(
    'Widens tight turns so the rear wheels, which cut inside the front wheels\' path on a long wheelbase, are less ' +
    'likely to clip a curb. A geometric estimate from the wheelbase (3.45 m on an R1T, 3.08 m on an R1S, read from ' +
    'the VIN), not curb sensing. Off by default.'
  ),
  'offtrack_gain': tr_noop('How much of the rear wheels\' cut-in to make up. 100% is the full geometric amount.'),
}


def offtrack_gain_label(value: int) -> str:
  if value == 50:
    return tr("50% (default)")
  return f"{value}%"


class RivianSettings(BrandSettings):
  def __init__(self):
    super().__init__()

    self.offtrack = toggle_item_sp(
      lambda: tr("Rear-Axle Off-Tracking Compensation"),
      description=lambda: tr(DESCRIPTIONS["offtrack"]),
      param="RivianOfftrackCompensationEnabled",
    )
    # Stored as a 0-1 float, shown as a percentage
    self.offtrack_gain = option_item_sp(
      param="RivianOfftrackCompensationGain",
      title=lambda: tr("Compensation Gain"),
      min_value=0,
      max_value=100,
      value_change_step=5,
      description=lambda: tr(DESCRIPTIONS["offtrack_gain"]),
      use_float_scaling=True,
      label_callback=offtrack_gain_label,
    )

    self.items = [self.offtrack, self.offtrack_gain]

  def update_settings(self):
    # Offroad only, as in Sunnylink
    offroad = ui_state.is_offroad()
    self.offtrack.action_item.set_enabled(offroad)
    self.offtrack_gain.action_item.set_enabled(offroad)
    self.offtrack_gain.set_visible(self.offtrack.action_item.get_state())
