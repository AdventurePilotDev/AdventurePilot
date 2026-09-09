"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
# pyray exposes Texture and Image as cffi constructor functions, not classes, so a runtime
# evaluated annotation like "-> rl.Texture | None" raises TypeError while the module is being
# imported. Deferring annotations keeps them readable without evaluating them.
from __future__ import annotations

import os
import time

import numpy as np
import pyray as rl

from openpilot.common.hardware import HARDWARE
from openpilot.common.params import Params
from openpilot.common.swaglog import cloudlog
from openpilot.system.ui.lib.application import gui_app, FontWeight
from openpilot.system.ui.lib.text_measure import measure_text_cached
from openpilot.system.ui.sunnypilot.lib.logo_store import load_logo_rgba, resolve_logo
from openpilot.system.ui.widgets import Widget

# ScreenSaverText param: index into PRESET_TEXTS, CUSTOM_TEXT_MODE for ScreenSaverCustomText,
# or LOGO_MODE for the image named by ScreenSaverLogo
PRESET_TEXTS = ("AdventurePilot", "sunnypilot")
CUSTOM_TEXT_MODE = 2
LOGO_MODE = 3
MAX_MODE = LOGO_MODE
DEFAULT_TEXT = PRESET_TEXTS[0]

# Highest mode the small UI offers. Custom text and logos are big-UI features, but ScreenSaverText
# is remotely writable, so the small UI has to cope with a mode it has no way to configure.
MICI_MAX_MODE = 1

# Fraction of the screen the logo is allowed to occupy, matching the share the text takes so the
# logo travels rather than filling the screen.
LOGO_BOX_FRACTION = 1.0 / 3.0

# Longest custom text the settings UI accepts. Audiowide is proportional, so this is a length
# proxy for a width budget of ~10.7 em (identical on mici and tici, since the font size already
# scales with screen width). 14 characters of average text fits at full size; only wide-glyph
# strings need shrinking.
MAX_CUSTOM_TEXT_LEN = 14

# Leave some slack so the text visibly travels rather than filling the screen edge to edge
FIT_MARGIN = 0.9


class ScreenSaverSP(Widget):
  def __init__(self, params: Params | None = None):
    super().__init__()
    self.set_rect(rl.Rectangle(0, 0, gui_app.width, gui_app.height))
    self._params = params or Params()
    self._is_mici = HARDWARE.get_device_type() == 'mici' or (HARDWARE.get_device_type() == "pc" and os.getenv("BIG") != "1")

    self.x = 0.0
    self.y = 100.0
    self.vx = 120.0 if self._is_mici else 300.0
    self.vy = 70.0 if self._is_mici else 200.0
    self._hue = 150
    self.color = rl.color_from_hsv(self._hue, 1, 1)

    self._base_font_size = 50 if self._is_mici else 200
    self._min_font_size = 20 if self._is_mici else 60

    self.text = DEFAULT_TEXT
    self.font_size = self._base_font_size
    self.logo_texture: rl.Texture | None = None
    self._start_time = None
    self._dismiss = False
    self._screensaver_timeout = 300
    self._hit_last_frame = False

  @property
  def is_active(self) -> bool:
    return self._start_time is not None and not self._dismiss

  @property
  def was_dismissed(self) -> bool:
    return self._dismiss

  def initialize(self):
    self._screensaver_timeout = self._params.get("ScreenSaverTimeout", return_default=True)
    self._unload_logo()

    mode = self._resolve_mode()
    if mode == LOGO_MODE:
      self.logo_texture = self._load_logo_texture()

    # Falling back to text is a normal state, not an error: a restored backup can select a logo
    # this device has never had, since ScreenSaverText carries BACKUP but the file does not.
    if self.logo_texture is None:
      self.text = self._resolve_text(mode)
      self.font_size = self._fit_font_size(self.text)

    if self._start_time is None:
      self._start_time = time.monotonic()
    self._dismiss = False

  def _resolve_mode(self) -> int:
    mode = self._params.get("ScreenSaverText", return_default=True)
    if mode is None or not 0 <= mode <= MAX_MODE:
      return 0
    if self._is_mici and mode > MICI_MAX_MODE:
      return 0
    return mode

  def _resolve_text(self, mode: int) -> str:
    if mode == CUSTOM_TEXT_MODE:
      custom = (self._params.get("ScreenSaverCustomText", return_default=True) or "").strip()
      return custom or DEFAULT_TEXT
    if not 0 <= mode < len(PRESET_TEXTS):
      return DEFAULT_TEXT
    return PRESET_TEXTS[mode]

  def _load_logo_texture(self) -> rl.Texture | None:
    path = resolve_logo(self._params.get("ScreenSaverLogo", return_default=True) or "")
    if path is None:
      return None

    image = load_logo_rgba(path, int(self.rect.width * LOGO_BOX_FRACTION), int(self.rect.height * LOGO_BOX_FRACTION))
    if image is None:
      return None

    try:
      # The array has to outlive load_texture_from_image, which reads through the pointer. It does,
      # because the upload happens before this scope ends. Same shape as pairing_dialog.py.
      pixels = np.asarray(image, dtype=np.uint8)
      rl_image = rl.Image()
      rl_image.data = rl.ffi.cast("void *", pixels.ctypes.data)
      rl_image.width = image.width
      rl_image.height = image.height
      rl_image.mipmaps = 1
      rl_image.format = rl.PixelFormat.PIXELFORMAT_UNCOMPRESSED_R8G8B8A8
      texture = rl.load_texture_from_image(rl_image)
    except Exception:
      cloudlog.exception("screen saver: failed to upload logo texture")
      return None

    return texture if texture.id != 0 else None

  def _unload_logo(self):
    if self.logo_texture is not None and self.logo_texture.id != 0:
      rl.unload_texture(self.logo_texture)
    self.logo_texture = None

  def _fit_font_size(self, text: str) -> int:
    # The bounce logic in _update_state degenerates if the text is wider than the screen, so shrink
    # to fit. MAX_CUSTOM_TEXT_LEN keeps normal input at full size; this is the guard for a longer
    # string reaching the param from outside the settings UI (backup restore, direct params write).
    max_width = self.rect.width * FIT_MARGIN
    width = measure_text_cached(gui_app.font(FontWeight.AUDIOWIDE), text, self._base_font_size, 0).x
    if width <= 0 or width <= max_width:
      return self._base_font_size
    return max(self._min_font_size, int(self._base_font_size * max_width / width))

  def hide_event(self):
    super().hide_event()
    self._unload_logo()
    self._dismiss = False
    self._start_time = None

  def _handle_mouse_release(self, mouse_pos):
    self._dismiss = True
    self._start_time = None
    gui_app.pop_widget()
    return super()._handle_mouse_release(mouse_pos)

  def _update_state(self):
    super()._update_state()

    self.font = gui_app.font(FontWeight.AUDIOWIDE)
    if self.logo_texture is not None:
      content_width, content_height = self.logo_texture.width, self.logo_texture.height
    else:
      text_size = measure_text_cached(self.font, self.text, self.font_size, 0)
      content_width, content_height = text_size.x, text_size.y
    # Clamp so the bounce logic below cannot pin x negative and flip vx every frame
    self.logo_width = min(content_width, self.rect.width)
    self.logo_height = min(content_height, self.rect.height)

    if self._start_time and time.monotonic() - self._start_time > self._screensaver_timeout:
      self._dismiss = True
      self._start_time = None

    dt = rl.get_frame_time()

    self.x += self.vx * dt
    self.y += self.vy * dt

    # Travel room can be zero if the text fills the axis, which is only reachable when the font hit
    # its minimum size. Hold still on that axis instead of flipping direction every frame.
    travel_x = self.rect.width - self.logo_width
    travel_y = self.rect.height - self.logo_height

    hit_x = hit_y = False
    if travel_x <= 0:
      self.x = 0
    elif self.x > travel_x:
      self.vx *= -1
      self.x = travel_x
      hit_x = True
    elif self.x < 0:
      self.vx *= -1
      self.x = 0
      hit_x = True

    if travel_y <= 0:
      self.y = 0
    elif self.y > travel_y:
      self.vy *= -1
      self.y = travel_y
      hit_y = True
    elif self.y < 0:
      self.vy *= -1
      self.y = 0
      hit_y = True

    hit = hit_x or hit_y
    if hit and not self._hit_last_frame:
      while self._hue_dist((new_hue := rl.get_random_value(0, 360)), self._hue) < 120:
        pass
      self._hue = new_hue
      self.color = rl.color_from_hsv(self._hue, 1, 1)
    self._hit_last_frame = hit

  @staticmethod
  def _hue_dist(a, b):
    d = abs(a - b)
    return min(d, 360 - d)

  def _render(self, rect: rl.Rectangle):
    self.set_rect(rect)
    rl.clear_background(rl.BLACK)
    if self.logo_texture is not None:
      # Tinted with the same cycling hue the text uses. The cycling is a burn-in mitigation, not
      # decoration, and a logo is a bigger and brighter target than text is.
      source = rl.Rectangle(0, 0, self.logo_texture.width, self.logo_texture.height)
      destination = rl.Rectangle(int(self.x), int(self.y), self.logo_width, self.logo_height)
      rl.draw_texture_pro(self.logo_texture, source, destination, rl.Vector2(0, 0), 0, self.color)
    else:
      rl.draw_text_ex(self.font, self.text, rl.Vector2(int(self.x), int(self.y)), self.font_size, 0, self.color)
    return -1
