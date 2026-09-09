"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from enum import IntEnum

from openpilot.common.params import Params
from openpilot.system.ui.lib.application import gui_app
from openpilot.system.ui.widgets import Widget, DialogResult
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.widgets.confirm_dialog import alert_dialog
from openpilot.system.ui.widgets.option_dialog import MultiOptionDialog
from openpilot.system.ui.widgets.scroller_tici import Scroller
from openpilot.system.ui.sunnypilot.lib.logo_store import display_name, list_logos
from openpilot.system.ui.sunnypilot.widgets.input_dialog import InputDialogSP
from openpilot.system.ui.sunnypilot.widgets.list_view import toggle_item_sp, option_item_sp, button_item_sp, multiple_button_item_sp
from openpilot.system.ui.sunnypilot.widgets.screen_saver import CUSTOM_TEXT_MODE, LOGO_MODE, MAX_CUSTOM_TEXT_LEN, PRESET_TEXTS
from openpilot.sunnypilot.system.params_migration import ONROAD_BRIGHTNESS_TIMER_VALUES


class OnroadBrightness(IntEnum):
  AUTO = 0
  AUTO_DARK = 1
  SCREEN_OFF = 2


class DisplayLayout(Widget):
  def __init__(self):
    super().__init__()

    self._params = Params()
    self._logo_dialog: MultiOptionDialog | None = None
    # Cached because the row's value callback runs every frame and a directory scan does not belong
    # in the render loop. Refreshed when the panel opens and whenever the picker is opened.
    self._logos: list[str] = []
    items = self._initialize_items()
    self._scroller = Scroller(items, line_separator=True, spacing=0)

  def _initialize_items(self):
    self._onroad_brightness = option_item_sp(
      param="OnroadScreenOffBrightness",
      title=lambda: tr("Onroad Brightness"),
      description="",
      min_value=0,
      max_value=22,
      value_change_step=1,
      label_callback=lambda value: self.update_onroad_brightness(value),
      inline=True
    )
    self._onroad_brightness_timer = option_item_sp(
      param="OnroadScreenOffTimer",
      title=lambda: tr("Onroad Brightness Delay"),
      description="",
      min_value=0,
      max_value=15,
      value_change_step=1,
      value_map=ONROAD_BRIGHTNESS_TIMER_VALUES,
      label_callback=lambda value: f"{value} s" if value < 60 else f"{int(value/60)} m",
      inline=True
    )
    self._interactivity_timeout = option_item_sp(
      param="InteractivityTimeout",
      title=lambda: tr("Interactivity Timeout"),
      description=lambda: tr("Apply a custom timeout for settings UI." +
                             "<br>This is the time after which settings UI closes automatically " +
                             "if user is not interacting with the screen."),
      min_value=0,
      max_value=120,
      value_change_step=10,
      label_callback=lambda value: (tr("Default") if not value or value == 0 else
                                    f"{value} s" if value < 60 else f"{int(value/60)} m"),
      inline=True
    )
    self._screensaver_toggle = toggle_item_sp(
      param="ScreenSaverEnabled",
      title=lambda: tr("Screen Saver"),
      description=lambda: tr("Show a screen saver when the device is offroad and idle, instead of turning the screen off."),
    )
    self._screensaver_timeout = option_item_sp(
      param="ScreenSaverTimeout",
      title=lambda: tr("Screen Saver Duration"),
      description=lambda: tr("How long the screen saver runs before the screen turns off."),
      min_value=60,
      max_value=600,
      value_change_step=60,
      label_callback=lambda value: f"{int(value/60)} m"
    )
    self._screensaver_text = multiple_button_item_sp(
      param="ScreenSaverText",
      title=lambda: tr("Screen Saver Text"),
      description=lambda: tr("Choose what the screen saver shows."),
      buttons=[PRESET_TEXTS[0], PRESET_TEXTS[1], lambda: tr("Custom"), lambda: tr("Logo")],
      button_width=350,
    )
    self._screensaver_custom_text = button_item_sp(
      title=lambda: tr("Custom Text"),
      button_text=lambda: tr("EDIT"),
      description=lambda: tr("Type the text the screen saver should show, up to {} characters.").format(MAX_CUSTOM_TEXT_LEN),
      callback=self._show_custom_text_dialog,
    )
    self._screensaver_custom_text.action_item.set_value(lambda: self._params.get("ScreenSaverCustomText", return_default=True) or "")
    self._screensaver_logo = button_item_sp(
      title=lambda: tr("Screen Saver Logo"),
      button_text=lambda: tr("SELECT"),
      description=lambda: tr("Choose which of your uploaded images the screen saver shows. " +
                             "The image is tinted and changes colour as it moves, so a white logo on a transparent " +
                             "background works best."),
      callback=self._show_logo_dialog,
    )
    self._screensaver_logo.action_item.set_value(self._current_logo_label)
    items = [
      self._onroad_brightness,
      self._onroad_brightness_timer,
      self._interactivity_timeout,
      self._screensaver_toggle,
      self._screensaver_timeout,
      self._screensaver_text,
      self._screensaver_custom_text,
      self._screensaver_logo,
    ]
    return items

  def _current_logo_label(self) -> str:
    selected = self._params.get("ScreenSaverLogo", return_default=True) or ""
    return display_name(selected) if selected in self._logos else tr("None")

  def _show_logo_dialog(self):
    # Re-read here rather than trusting the cache: an image may have been uploaded since the panel
    # was opened, and this is a tap, not a frame
    self._logos = logos = list_logos()
    if not logos:
      gui_app.push_widget(alert_dialog(tr("No images found. Turn on the copyparty Service in Developer settings, " +
                                          "then upload a PNG or JPEG to the branding folder from your browser.")))
      return

    labels = [display_name(name) for name in logos]
    current = self._params.get("ScreenSaverLogo", return_default=True) or ""
    current_label = display_name(current) if current in logos else ""

    def handle_selection(result: DialogResult):
      if result == DialogResult.CONFIRM and self._logo_dialog is not None and self._logo_dialog.selection:
        # Labels are filename stems, so map back rather than assuming the stored name
        chosen = next((name for name in logos if display_name(name) == self._logo_dialog.selection), None)
        if chosen is not None:
          self._params.put("ScreenSaverLogo", chosen, block=True)
      self._logo_dialog = None

    self._logo_dialog = MultiOptionDialog(tr("Select a logo"), labels, current_label, callback=handle_selection)
    gui_app.push_widget(self._logo_dialog)

  def _show_custom_text_dialog(self):
    InputDialogSP(
      title=tr("Screen Saver Text"),
      sub_title=tr("Leave blank to use {}").format(PRESET_TEXTS[0]),
      current_text=self._params.get("ScreenSaverCustomText", return_default=True) or "",
      param="ScreenSaverCustomText",
      max_text_size=MAX_CUSTOM_TEXT_LEN,
    ).show()

  @staticmethod
  def update_onroad_brightness(val):
    if val == OnroadBrightness.AUTO:
      return tr("Auto (Default)")

    if val == OnroadBrightness.AUTO_DARK:
      return tr("Auto (Dark)")

    if val == OnroadBrightness.SCREEN_OFF:
      return tr("Screen Off")

    return f"{(val - 2) * 5} %"

  def _update_state(self):
    super()._update_state()

    brightness_val = self._onroad_brightness.action_item.current_value
    self._onroad_brightness_timer.action_item.set_enabled(brightness_val not in (OnroadBrightness.AUTO, OnroadBrightness.AUTO_DARK))

    screensaver_on = self._screensaver_toggle.action_item.get_state()
    self._screensaver_timeout.set_visible(screensaver_on)
    self._screensaver_text.set_visible(screensaver_on)
    self._screensaver_custom_text.set_visible(screensaver_on and self._screensaver_text.action_item.selected_button == CUSTOM_TEXT_MODE)
    self._screensaver_logo.set_visible(screensaver_on and self._screensaver_text.action_item.selected_button == LOGO_MODE)

  def _render(self, rect):
    self._scroller.render(rect)

  def show_event(self):
    # MultipleButtonActionSP only reads its param on construction, so re-sync in case sunnylink
    # changed it remotely while the panel was closed
    self._screensaver_text.action_item.set_selected_button(self._params.get("ScreenSaverText", return_default=True))
    self._logos = list_logos()
    self._scroller.show_event()
