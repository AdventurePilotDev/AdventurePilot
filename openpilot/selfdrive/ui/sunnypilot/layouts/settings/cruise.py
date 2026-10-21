"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from enum import IntEnum

from openpilot.selfdrive.ui.sunnypilot.layouts.settings.cruise_sub_layouts.speed_limit_settings import SpeedLimitSettingsLayout
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.multilang import tr, tr_noop
from openpilot.system.ui.sunnypilot.widgets.list_view import toggle_item_sp, option_item_sp, simple_button_item_sp
from openpilot.system.ui.widgets import Widget
from openpilot.system.ui.widgets.scroller_tici import Scroller


class PanelType(IntEnum):
  CRUISE = 0
  SLA = 1


ICBM_DESC = tr_noop("When enabled, sunnypilot will attempt to manage the built-in cruise control buttons " +
                    "by emulating button presses for limited longitudinal control.")
ICMB_UNAVAILABLE = tr_noop("Intelligent Cruise Button Management is currently unavailable on this platform.")
ICMB_UNAVAILABLE_LONG_AVAILABLE = tr_noop("Disable the sunnypilot Longitudinal Control (alpha) toggle to allow Intelligent Cruise Button Management.")
ICMB_UNAVAILABLE_LONG_UNAVAILABLE = tr_noop("sunnypilot Longitudinal Control is the default longitudinal control for this platform.")

ACC_ENABLED_DESCRIPTION = tr_noop("Enable custom Short & Long press increments for cruise speed increase/decrease.")
ACC_NOLONG_DESCRIPTION = tr_noop("This feature can only be used with sunnypilot longitudinal control enabled.")
ACC_PCMCRUISE_DISABLED_DESCRIPTION = tr_noop("This feature is not supported on this platform due to vehicle limitations.")
ONROAD_ONLY_DESCRIPTION = tr_noop("Start the vehicle to check vehicle compatibility.")


def _stop_distance_label(v: int) -> str:
  return f"{v / 10:.1f}m"


def _t_follow_offset_label(v: int) -> str:
  if v == 0:
    return tr("Default")
  return f"+{v}%" if v > 0 else f"{v}%"


class CruiseLayout(Widget):
  def __init__(self):
    super().__init__()
    self._current_panel = PanelType.CRUISE
    self._speed_limit_layout = SpeedLimitSettingsLayout(lambda: self._set_current_panel(PanelType.CRUISE))

    items = self._initialize_items()
    self._scroller = Scroller(items, line_separator=True, spacing=0)

  def _initialize_items(self):

    self.icbm_toggle = toggle_item_sp(
      title=tr("Intelligent Cruise Button Management (ICBM) (Alpha)"),
      description="",
      param="IntelligentCruiseButtonManagement")

    self.scc_v_toggle = toggle_item_sp(
      title=tr("Smart Cruise Control - Vision"),
      description=tr("Use vision path predictions to estimate the appropriate speed to drive through turns ahead."),
      param="SmartCruiseControlVision")

    self.scc_m_toggle = toggle_item_sp(
      title=tr("Smart Cruise Control - Map"),
      description=tr("Use map data to estimate the appropriate speed to drive through turns ahead."),
      param="SmartCruiseControlMap")

    self.curve_speed_toggle = toggle_item_sp(
      title=tr("Curve Speed Control"),
      description=tr("Slow down ahead of curves and power smoothly through them, keeping cornering within the "
                     "steering's limits. Uses the vision path; no map data required."),
      param="CurveSpeedControl")

    self.custom_acc_toggle = toggle_item_sp(
      title=tr("Custom ACC Speed Increments"),
      description="",
      param="CustomAccIncrementsEnabled",
      callback=self._on_custom_acc_toggle)

    self.custom_acc_short_increment = option_item_sp(
      title=tr("Short Press Increment"),
      param="CustomAccShortPressIncrement",
      min_value=1, max_value=10, value_change_step=1,
      inline=True)

    self.custom_acc_long_increment = option_item_sp(
      title=tr("Long Press Increment"),
      param="CustomAccLongPressIncrement",
      value_map={1: 1, 2: 5, 3: 10},
      min_value=1, max_value=3, value_change_step=1,
      inline=True)

    self.stop_distance_option = option_item_sp(
      title=tr("Minimum Stop Distance"),
      description=tr("Gap behind a stopped lead car (4.5 to 6.0 m). Default 6.0 m. " +
                     "Only active when cruise set speed is 50 kph / 31 mph or below; " +
                     "reverts to 6.0 m above that speed. " +
                     "Takes effect after changing from OffRoad to OnRoad."),
      param="SPStopDistance",
      min_value=45,
      max_value=60,
      value_change_step=5,
      label_callback=_stop_distance_label,
      on_value_changed=self._on_stop_distance_changed,
      inline=True)

    self.stop_distance_personality = toggle_item_sp(
      title=tr("Link Stop Distance to Personality"),
      description=tr("When enabled, and while the reduced stop distance is active: " +
                     "Aggressive uses the minimum distance; Standard uses the midpoint; " +
                     "Relaxed always uses 6.0 m."),
      param="SPStopDistancePersonality")

    self.following_time_offset = option_item_sp(
      title=tr("Following Distance Offset"),
      description=tr("Scales the time-gap to the lead car across all personality modes. " +
                     "Negative = follow closer; positive = follow further. " +
                     "Takes effect after changing from OffRoad to OnRoad."),
      param="SPFollowingTimeOffset",
      min_value=-20,
      max_value=20,
      value_change_step=5,
      label_callback=_t_follow_offset_label,
      inline=True)

    self.sla_settings_button = simple_button_item_sp(
      button_text=lambda: tr("Speed Limit"),
      button_width=800,
      callback=lambda: self._set_current_panel(PanelType.SLA)
    )

    self.dec_toggle = toggle_item_sp(
      title=tr("Enable Dynamic Experimental Control"),
      description=tr("Enable toggle to allow the model to determine when to use sunnypilot ACC or sunnypilot End to End Longitudinal."),
      param="DynamicExperimentalControl")

    self.rivian_resume_toggle = toggle_item_sp(
      title=tr("Rivian: Enable Resume"),
      description=tr('When enabled a full stalk down action held for at least 0.5s, provided activation of ACC is available on stock Rivian, will set the cruise speed to be equal to that from the last time cruise was deactivated. If cruise has never been activated it will set the cruise speed to the current vehicle speed. It is recommended to disable the stock Rivian feature: "Set to speed limit on divided highways", which uses the same activation mechanism.'),
      param="RivianResumeEnabled")


    items = [
      self.icbm_toggle,
      self.dec_toggle,
      self.scc_v_toggle,
      self.scc_m_toggle,
      self.curve_speed_toggle,
      self.custom_acc_toggle,
      self.custom_acc_short_increment,
      self.custom_acc_long_increment,
      self.stop_distance_option,
      self.stop_distance_personality,
      self.following_time_offset,
      self.rivian_resume_toggle,
      self.sla_settings_button,
    ]
    return items

  def _render(self, rect):
    if self._current_panel == PanelType.SLA:
      self._speed_limit_layout.render(rect)
    else:
      self._scroller.render(rect)

  def show_event(self):
    self._set_current_panel(PanelType.CRUISE)
    self._scroller.show_event()
    self.icbm_toggle.show_description(True)
    self.custom_acc_toggle.show_description(True)

  def _set_current_panel(self, panel: PanelType):
    self._current_panel = panel
    if panel == PanelType.SLA:
      self._speed_limit_layout.show_event()

  def _update_state(self):
    super()._update_state()

    if ui_state.CP is not None and ui_state.CP_SP is not None:
      has_icbm = ui_state.has_icbm
      has_long = ui_state.has_longitudinal_control

      if ui_state.CP_SP.intelligentCruiseButtonManagementAvailable and not has_long:
        self.icbm_toggle.action_item.set_enabled(ui_state.is_offroad())
        self.icbm_toggle.set_description(tr(ICBM_DESC))
      else:
        ui_state.params.remove("IntelligentCruiseButtonManagement")
        self.icbm_toggle.action_item.set_enabled(False)

        long_desc = ICMB_UNAVAILABLE
        if has_long:
          if ui_state.CP.alphaLongitudinalAvailable:
            long_desc += " " + ICMB_UNAVAILABLE_LONG_AVAILABLE
          else:
            long_desc += " " + ICMB_UNAVAILABLE_LONG_UNAVAILABLE

        new_desc = "<b>" + tr(long_desc) + "</b>\n\n" + tr(ICBM_DESC)
        if self.icbm_toggle.description != new_desc:
          self.icbm_toggle.set_description(new_desc)
          self.icbm_toggle.show_description(True)

      if has_long or has_icbm:
        self.custom_acc_toggle.action_item.set_enabled(((has_long and not ui_state.CP.pcmCruise) or has_icbm) and ui_state.is_offroad())
        self.dec_toggle.action_item.set_enabled(has_long)
        self.scc_v_toggle.action_item.set_enabled(True)
        self.scc_m_toggle.action_item.set_enabled(True)
        # Curve Speed Control feeds the longitudinal planner, so it only does anything with openpilot
        # longitudinal active -- gate it like the DEC toggle (and like the mici UI, which hides both on
        # has_longitudinal_control) instead of leaving it always-on here.
        self.curve_speed_toggle.action_item.set_enabled(has_long)
        self.following_time_offset.action_item.set_enabled(has_long and ui_state.is_offroad())
        self.stop_distance_option.action_item.set_enabled(has_long and ui_state.is_offroad())
        self.stop_distance_personality.action_item.set_enabled(has_long and ui_state.is_offroad())
      else:
        ui_state.params.remove("CustomAccIncrementsEnabled")
        ui_state.params.remove("DynamicExperimentalControl")
        ui_state.params.remove("SmartCruiseControlVision")
        ui_state.params.remove("SmartCruiseControlMap")
        ui_state.params.remove("CurveSpeedControl")
        self.custom_acc_toggle.action_item.set_enabled(False)
        self.dec_toggle.action_item.set_enabled(False)
        self.scc_v_toggle.action_item.set_enabled(False)
        self.scc_m_toggle.action_item.set_enabled(False)
        self.curve_speed_toggle.action_item.set_enabled(False)
        self.following_time_offset.action_item.set_enabled(False)
        self.stop_distance_option.action_item.set_enabled(False)
        self.stop_distance_personality.action_item.set_enabled(False)

      if not has_long:
        ui_state.params.remove("SPFollowingTimeOffset")
        ui_state.params.remove("SPStopDistance")
        ui_state.params.remove("SPStopDistancePersonality")

      is_rivian_long = ui_state.CP.brand == "rivian" and has_long
      self.rivian_resume_toggle.action_item.set_enabled(is_rivian_long and ui_state.is_offroad())
      if not is_rivian_long:
        ui_state.params.remove("RivianResumeEnabled")

    else:
      has_icbm = has_long = False
      self.icbm_toggle.action_item.set_enabled(False)
      self.icbm_toggle.set_description(tr(ONROAD_ONLY_DESCRIPTION))
      self.rivian_resume_toggle.action_item.set_enabled(False)
      self.following_time_offset.action_item.set_enabled(False)
      self.stop_distance_option.action_item.set_enabled(False)
      self.stop_distance_personality.action_item.set_enabled(False)

    show_custom_acc_desc = False

    if ui_state.is_offroad():
      new_custom_acc_desc = tr(ONROAD_ONLY_DESCRIPTION)
      show_custom_acc_desc = True
    else:
      if has_long or has_icbm:
        if has_long and ui_state.CP.pcmCruise:
          new_custom_acc_desc = tr(ACC_PCMCRUISE_DISABLED_DESCRIPTION)
          show_custom_acc_desc = True
        else:
          new_custom_acc_desc = tr(ACC_ENABLED_DESCRIPTION)
      else:
        new_custom_acc_desc = tr(ACC_NOLONG_DESCRIPTION)
        show_custom_acc_desc = True
        self.custom_acc_toggle.action_item.set_state(False)

    if self.custom_acc_toggle.description != new_custom_acc_desc:
      self.custom_acc_toggle.set_description(new_custom_acc_desc)
      if show_custom_acc_desc:
        self.custom_acc_toggle.show_description(True)

    self._on_custom_acc_toggle(self.custom_acc_toggle.action_item.get_state())

  def _on_stop_distance_changed(self, value: int) -> None:
    below_max = value < 60
    self.stop_distance_personality.set_visible(below_max)
    if not below_max:
      ui_state.params.remove("SPStopDistancePersonality")

  def _on_custom_acc_toggle(self, state):
    self.custom_acc_short_increment.set_visible(state)
    self.custom_acc_long_increment.set_visible(state)
    self.custom_acc_short_increment.action_item.set_enabled(self.custom_acc_toggle.action_item.enabled)
    self.custom_acc_long_increment.action_item.set_enabled(self.custom_acc_toggle.action_item.enabled)
