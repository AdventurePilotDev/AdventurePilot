"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

Screen saver picture upload for the small UI.

The small UI has no Display settings screen and no room for a picker, so this keeps exactly one
picture under a fixed name: every upload replaces the last. That removes the need for a list, a
selector, or any way to browse, which is what made a settings panel awkward here. Uploading also
switches the screen saver into logo mode, so this one screen is the whole feature.

Layout follows the stock pairing dialog: code on the left scaled to the panel height, words on
the right.
"""
from __future__ import annotations

import pyray as rl

from openpilot.common.params import Params
from openpilot.common.qrcode import make_texture
from openpilot.common.swaglog import cloudlog
from openpilot.system.ui.lib.application import gui_app, FontWeight
from openpilot.system.ui.sunnypilot.lib.logo_store import FIXED_LOGO_NAME, delete_logo, resolve_logo
from openpilot.system.ui.sunnypilot.lib.logo_upload_server import LogoUploadServer
from openpilot.system.ui.sunnypilot.widgets.screen_saver import LOGO_MODE
from openpilot.selfdrive.ui.mici.widgets.button import BigButton
from openpilot.system.ui.widgets.label import UnifiedLabel
from openpilot.system.ui.widgets.nav_widget import NavWidget

PADDING = 16
GAP = 20
REMOVE_BUTTON_HEIGHT = 64


class LogoUploadDialogMici(NavWidget):
  def __init__(self):
    super().__init__()
    self.set_rect(rl.Rectangle(0, 0, gui_app.width, gui_app.height))
    self._params = Params()
    self._server = LogoUploadServer(fixed_name=FIXED_LOGO_NAME)
    self._qr_texture: rl.Texture | None = None

    self._title = UnifiedLabel("screen saver picture", font_size=34, font_weight=FontWeight.BOLD, line_height=0.9)
    self._body = UnifiedLabel(self._body_text, font_size=26, line_height=1.1,
                              text_color=rl.Color(255, 255, 255, int(255 * 0.65)))
    self._remove_button = BigButton("remove picture", "", None)
    self._remove_button.set_click_callback(self._remove)
    self._status = ""

  # -- lifetime ------------------------------------------------------------------------------

  def show_event(self):
    super().show_event()
    self._status = ""
    if self._server.start():
      self._build_qr(self._server.url or "")
    else:
      self._unload_qr()

  def hide_event(self):
    super().hide_event()
    self._server.stop()
    self._unload_qr()

  def __del__(self):
    try:
      self._server.stop()
    except Exception:
      pass

  # -- content -------------------------------------------------------------------------------

  def _has_logo(self) -> bool:
    return resolve_logo(FIXED_LOGO_NAME) is not None

  def _body_text(self) -> str:
    if self._status:
      return self._status
    if not self._server.running:
      return "not connected to a network. join wi-fi, or turn on tethering, then open this again."
    url = self._server.url or ""
    if self._has_logo():
      return f"scan to replace the picture\n{url}"
    return f"scan with your phone to add a picture\n{url}"

  def _remove(self):
    delete_logo(FIXED_LOGO_NAME)
    self._params.remove("ScreenSaverLogo")
    self._params.put("ScreenSaverText", 0)
    self._status = "picture removed"

  def _update_state(self):
    super()._update_state()

    result = self._server.take_result()
    if result is None:
      return

    if not result.ok:
      self._status = result.error.lower()
      return

    # One screen is the whole feature here, so arriving is also choosing
    self._params.put("ScreenSaverLogo", FIXED_LOGO_NAME)
    self._params.put("ScreenSaverText", LOGO_MODE)
    self._status = result.warnings[0].lower() if result.warnings else "picture added"

  # -- drawing -------------------------------------------------------------------------------

  def _build_qr(self, url: str):
    self._unload_qr()
    try:
      # inverted matches the small UI's dark ground, the same way the stock pairing dialog does it
      texture = make_texture(url, inverted=True)
      self._qr_texture = texture if texture.id != 0 else None
    except Exception:
      cloudlog.exception("logo upload: QR code generation failed")
      self._qr_texture = None

  def _unload_qr(self):
    if self._qr_texture is not None and self._qr_texture.id != 0:
      rl.unload_texture(self._qr_texture)
    self._qr_texture = None

  def _render(self, _):
    rect = self._rect
    qr_side = 0.0

    if self._qr_texture is not None:
      qr_side = rect.height - 2 * PADDING
      scale = qr_side / self._qr_texture.height
      rl.draw_texture_ex(self._qr_texture, rl.Vector2(round(rect.x + PADDING), round(rect.y + PADDING)), 0.0, scale, rl.WHITE)

    text_x = rect.x + PADDING + (qr_side + GAP if qr_side else 0)
    text_width = int(rect.x + rect.width - PADDING - text_x)
    if text_width <= 0:
      return

    self._title.set_max_width(text_width)
    self._title.set_position(text_x, rect.y + PADDING)
    self._title.render()

    self._body.set_max_width(text_width)
    self._body.set_position(text_x, rect.y + PADDING + 44)
    self._body.render()

    if self._has_logo():
      button_rect = rl.Rectangle(text_x, rect.y + rect.height - PADDING - REMOVE_BUTTON_HEIGHT,
                                 text_width, REMOVE_BUTTON_HEIGHT)
      self._remove_button.render(button_rect)
