"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from openpilot.selfdrive.ui.sunnypilot.layouts.settings.vehicle.brands.base import BrandSettings
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.sunnypilot.widgets.list_view import multiple_button_item_sp


DESCRIPTIONS = {
  'aggressive_tune': tr_noop(
    'Steering tune profile. Auto picks by vehicle: a Gen1 R1T gets the aggressive tune (higher torque '
    'cap, low-pass torque filter, faster rate); everything else gets the tame baseline. Tame forces the '
    'baseline. Takes effect after the vehicle restarts (change while parked to apply immediately).'
  ),
}


class RivianSettings(BrandSettings):
  def __init__(self):
    super().__init__()

    self.aggressive_tune = multiple_button_item_sp(
      lambda: tr("Steering Tune (R1T)"),
      description=lambda: tr(DESCRIPTIONS["aggressive_tune"]),
      buttons=[lambda: tr("Auto"), lambda: tr("Tame")],
      callback=self._on_tune_select,
      param="RivianAggressiveTune",
      button_width=280,
    )

    self.items = [self.aggressive_tune]

  def _on_tune_select(self, index: int):
    # The widget writes the param (0=auto, 1=tame). The tune is read at car init, so
    # request an onroad cycle to re-init and pick up the new profile -- but only while parked, to
    # avoid a disruptive mid-drive cycle (when engaged it just applies on the next drive).
    if not ui_state.engaged:
      ui_state.params.put_bool("OnroadCycleRequested", True)

  def update_settings(self):
    pass
