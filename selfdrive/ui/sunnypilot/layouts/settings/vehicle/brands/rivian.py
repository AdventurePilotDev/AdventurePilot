"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from openpilot.selfdrive.ui.sunnypilot.layouts.settings.vehicle.brands.base import BrandSettings
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.sunnypilot.widgets.list_view import multiple_button_item_sp, toggle_item_sp


DESCRIPTIONS = {
  'aggressive_tune': tr_noop(
    'Steering tune profile. Auto picks by vehicle: a Gen1 R1T gets the aggressive tune (higher torque '
    'cap, low-pass torque filter, faster rate); everything else gets the tame baseline. Tame forces the '
    'baseline. Shapes the cooperative/handoff torque used by angle control. Takes effect after the '
    'vehicle restarts (change while parked to apply immediately).'
  ),
  'coop_steering': tr_noop(
    'Allows the driver to provide steering input (cooperative torque) while openpilot is engaged. '
    'When disabled, driver steering input disengages lateral control.'
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

    self.coop_steering_toggle = toggle_item_sp(
      lambda: tr("Cooperative Steering"),
      description=lambda: tr(DESCRIPTIONS["coop_steering"]),
      callback=self._on_restart_toggle,
      param="RivianCoopSteering",
    )

    self.items = [self.aggressive_tune, self.coop_steering_toggle]

  def _on_tune_select(self, index: int):
    self._request_restart()

  def _on_restart_toggle(self, *args):
    self._request_restart()

  def _request_restart(self):
    # tune / cooperative steering are read at car init -> request an onroad cycle, but only while
    # parked (when engaged it just applies on the next drive).
    if not ui_state.engaged:
      ui_state.params.put_bool("OnroadCycleRequested", True)

  def update_settings(self):
    pass
