"""Which screen saver modes each device class honours.

Uses the real pyray rather than a stand-in. An earlier stub for this modelled pyray's Texture as
a class when it is really a constructor function, so the tests passed while the device would not
start. Where pyray is genuinely unavailable, such as a developer laptop, these skip rather than
pretend.
"""
import pytest

pytest.importorskip("pyray", reason="raylib is not available here")

from openpilot.system.ui.sunnypilot.widgets.screen_saver import (
  CUSTOM_TEXT_MODE, DEFAULT_TEXT, LOGO_MODE, PRESET_TEXTS, ScreenSaverSP,
)


class FakeParams:
  def __init__(self, **values):
    self.values = values

  def get(self, key, return_default=False):
    return self.values.get(key)


def make(is_mici: bool, **params) -> ScreenSaverSP:
  saver = ScreenSaverSP(params=FakeParams(**params))
  # The real flag comes from the hardware, which a test cannot change
  saver._is_mici = is_mici
  return saver


class TestBigUI:
  @pytest.mark.parametrize("mode", [0, 1, CUSTOM_TEXT_MODE, LOGO_MODE])
  def test_honours_every_mode(self, mode):
    assert make(False, ScreenSaverText=mode)._resolve_mode() == mode

  @pytest.mark.parametrize("mode", [None, -1, 4, 99])
  def test_falls_back_when_the_mode_makes_no_sense(self, mode):
    assert make(False, ScreenSaverText=mode)._resolve_mode() == 0


class TestSmallUI:
  """The small UI has a picture upload screen but no way to type custom text."""

  @pytest.mark.parametrize("mode", [0, 1, LOGO_MODE])
  def test_honours_presets_and_logos(self, mode):
    assert make(True, ScreenSaverText=mode)._resolve_mode() == mode

  @pytest.mark.parametrize("mode", [CUSTOM_TEXT_MODE, 4, 99, None])
  def test_falls_back_from_anything_else(self, mode):
    assert make(True, ScreenSaverText=mode)._resolve_mode() == 0

  def test_shows_a_preset_rather_than_stored_custom_text(self):
    # The param is remotely writable, so custom text can be present without ever being reachable
    saver = make(True, ScreenSaverText=CUSTOM_TEXT_MODE, ScreenSaverCustomText="Geomglot")
    assert saver._resolve_text(saver._resolve_mode()) == PRESET_TEXTS[0]

  def test_does_not_write_the_clamped_value_back(self):
    saver = make(True, ScreenSaverText=CUSTOM_TEXT_MODE)
    saver._resolve_mode()
    assert saver._params.values["ScreenSaverText"] == CUSTOM_TEXT_MODE


class TestText:
  def test_preset(self):
    assert make(False, ScreenSaverText=1)._resolve_text(1) == PRESET_TEXTS[1]

  def test_custom_text_is_trimmed(self):
    assert make(False, ScreenSaverCustomText="  Hello  ")._resolve_text(CUSTOM_TEXT_MODE) == "Hello"

  @pytest.mark.parametrize("stored", ["   ", "", None])
  def test_blank_custom_text_falls_back(self, stored):
    assert make(False, ScreenSaverCustomText=stored)._resolve_text(CUSTOM_TEXT_MODE) == DEFAULT_TEXT

  def test_logo_mode_falls_back_to_the_default_text(self):
    # Reached whenever the chosen picture is missing, which is normal after restoring a backup
    assert make(False)._resolve_text(LOGO_MODE) == DEFAULT_TEXT
