"""An unrecognized (mock) car at startup must not delete the driver's longitudinal cruise settings.

Seen on a Rivian: after one drive recorded as MOCK, the Cruise settings screen deleted Rivian Resume,
Following Distance Offset and both Stop Distance settings, and they stayed off on later drives.
"""
from types import SimpleNamespace
from unittest.mock import MagicMock

import pytest

pytest.importorskip("pyray")

from opendbc.car.structs import car
from openpilot.cereal import custom
import openpilot.selfdrive.ui.sunnypilot.layouts.settings.cruise as cruise
from openpilot.selfdrive.ui.sunnypilot.tests.test_alpha_long_mock_car import FakeParams

KEPT = ("RivianResumeEnabled", "SPFollowingTimeOffset", "SPStopDistance", "SPStopDistancePersonality")


def _update(monkeypatch, brand, has_long):
  params = FakeParams(**dict.fromkeys(KEPT, True))
  fake_ui_state = SimpleNamespace(
    CP=car.CarParams.new_message(brand=brand),
    CP_SP=custom.CarParamsSP.new_message(),
    has_icbm=False,
    has_longitudinal_control=has_long,
    params=params,
    is_offroad=lambda: True,
  )
  monkeypatch.setattr(cruise, "ui_state", fake_ui_state)
  monkeypatch.setattr(cruise.Widget, "_update_state", lambda self: None)
  layout = cruise.CruiseLayout.__new__(cruise.CruiseLayout)
  layout.__dict__.update({name: MagicMock() for name in (
    "icbm_toggle", "custom_acc_toggle", "dec_toggle", "scc_v_toggle", "scc_m_toggle", "curve_speed_toggle",
    "following_time_offset", "stop_distance_option", "stop_distance_personality", "rivian_resume_toggle",
    "custom_acc_short_increment", "custom_acc_long_increment")})
  layout._update_state()
  return params


def test_mock_car_keeps_longitudinal_settings(monkeypatch):
  params = _update(monkeypatch, "mock", has_long=False)
  for key in KEPT:
    assert params.get_bool(key), key


def test_recognized_car_without_long_clears_them(monkeypatch):
  params = _update(monkeypatch, "rivian", has_long=False)
  for key in KEPT:
    assert not params.get_bool(key), key


def test_rivian_with_long_keeps_them(monkeypatch):
  params = _update(monkeypatch, "rivian", has_long=True)
  for key in KEPT:
    assert params.get_bool(key), key
