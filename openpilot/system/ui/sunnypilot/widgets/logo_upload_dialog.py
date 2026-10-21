"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

Shows a QR code that opens the screen saver logo upload page on the user's phone.

This exists so that adding a picture does not require reading an IP address off one settings
screen and typing it into a browser on another device. Point the camera at the screen, pick a
picture, done.
"""
from __future__ import annotations

from collections.abc import Callable

import pyray as rl

from openpilot.common.qrcode import make_texture
from openpilot.common.swaglog import cloudlog
from openpilot.system.ui.lib.application import gui_app, FontWeight
from openpilot.system.ui.lib.multilang import tr
from openpilot.system.ui.sunnypilot.lib.logo_upload_server import LogoUploadServer
from openpilot.system.ui.widgets import Widget
from openpilot.system.ui.widgets.button import Button, ButtonStyle
from openpilot.system.ui.widgets.label import gui_label, UnifiedLabel

MARGIN = 50
TITLE_FONT_SIZE = 70
BODY_FONT_SIZE = 40
STATUS_FONT_SIZE = 36
BUTTON_HEIGHT = 160
QR_SIZE = 460
GAP = 40


class LogoUploadDialog(Widget):
  def __init__(self, on_uploaded: Callable[[str], None] | None = None):
    super().__init__()
    self._server = LogoUploadServer()
    self._on_uploaded = on_uploaded
    self._qr_texture: rl.Texture | None = None
    self._status = ""
    self._status_color = rl.Color(155, 166, 162, 255)
    self._done_button = Button(lambda: tr("Done"), click_callback=self._close, button_style=ButtonStyle.PRIMARY)
    # gui_label elides rather than wraps, so the long offline message needs a wrapping label
    self._no_network_label = UnifiedLabel(
      lambda: tr("This device is not on a network, so there is nowhere to send a picture from. " +
                 "Connect it to Wi-Fi, or turn on Tethering under Network settings and join the " +
                 "device's own network from your phone, then open this screen again."),
      font_size=BODY_FONT_SIZE, line_height=1.3)

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

  def _close(self):
    gui_app.pop_widget()

  def _build_qr(self, url: str):
    self._unload_qr()
    try:
      texture = make_texture(url)
      self._qr_texture = texture if texture.id != 0 else None
    except Exception:
      cloudlog.exception("logo upload: QR code generation failed")
      self._qr_texture = None

  def _unload_qr(self):
    if self._qr_texture is not None and self._qr_texture.id != 0:
      rl.unload_texture(self._qr_texture)
    self._qr_texture = None

  def __del__(self):
    try:
      self._server.stop()
    except Exception:
      pass

  def _update_state(self):
    super()._update_state()

    result = self._server.take_result()
    if result is None:
      return

    if not result.ok:
      self._status = result.error
      self._status_color = rl.Color(229, 105, 95, 255)
    elif result.warnings:
      self._status = f"{result.name}: {result.warnings[0]}"
      self._status_color = rl.Color(237, 171, 76, 255)
    else:
      self._status = tr("Added {}. Choose it below.").format(result.name)
      self._status_color = rl.Color(47, 224, 166, 255)

    if result.ok and self._on_uploaded is not None:
      self._on_uploaded(result.name)

  def _render(self, rect: rl.Rectangle):
    dialog = rl.Rectangle(rect.x + MARGIN, rect.y + MARGIN, rect.width - 2 * MARGIN, rect.height - 2 * MARGIN)
    rl.draw_rectangle_rounded(dialog, 0.02, 20, rl.Color(30, 30, 30, 255))

    content = rl.Rectangle(dialog.x + MARGIN, dialog.y + MARGIN, dialog.width - 2 * MARGIN, dialog.height - 2 * MARGIN)
    gui_label(rl.Rectangle(content.x, content.y, content.width, TITLE_FONT_SIZE),
              tr("Add a Logo"), TITLE_FONT_SIZE, font_weight=FontWeight.BOLD)

    body_y = content.y + TITLE_FONT_SIZE + GAP
    body_height = content.height - TITLE_FONT_SIZE - BUTTON_HEIGHT - 2 * GAP

    if self._qr_texture is not None:
      self._render_upload_prompt(content, body_y, body_height)
    else:
      self._render_no_network(content, body_y, body_height)

    button_rect = rl.Rectangle(content.x, content.y + content.height - BUTTON_HEIGHT, content.width, BUTTON_HEIGHT)
    self._done_button.render(button_rect)

  def _render_upload_prompt(self, content: rl.Rectangle, body_y: float, body_height: float):
    qr_rect = rl.Rectangle(content.x, body_y, QR_SIZE, QR_SIZE)
    source = rl.Rectangle(0, 0, self._qr_texture.width, self._qr_texture.height)
    rl.draw_texture_pro(self._qr_texture, source, qr_rect, rl.Vector2(0, 0), 0, rl.WHITE)

    text_x = content.x + QR_SIZE + GAP
    text_width = content.width - QR_SIZE - GAP
    line = body_y

    for text, size, weight in (
      (tr("Point your phone's camera at this code."), BODY_FONT_SIZE, FontWeight.MEDIUM),
      (tr("Then choose a picture and upload it."), BODY_FONT_SIZE, FontWeight.NORMAL),
      (self._server.url or "", STATUS_FONT_SIZE, FontWeight.NORMAL),
      (tr("Your phone must be on the same Wi-Fi as this device."), STATUS_FONT_SIZE, FontWeight.NORMAL),
    ):
      gui_label(rl.Rectangle(text_x, line, text_width, BODY_FONT_SIZE + 10), text, size, font_weight=weight)
      line += size + 24

    if self._status:
      status_rect = rl.Rectangle(text_x, body_y + body_height - (STATUS_FONT_SIZE + 10), text_width, STATUS_FONT_SIZE + 10)
      gui_label(status_rect, self._status, STATUS_FONT_SIZE, color=self._status_color, font_weight=FontWeight.MEDIUM)

  def _render_no_network(self, content: rl.Rectangle, body_y: float, body_height: float):
    self._no_network_label.render(rl.Rectangle(content.x, body_y, content.width, body_height))
