"""An unrecognized (mock) car at startup must not switch off alpha longitudinal for the real car.

Seen on a Rivian: one drive where the car did not answer the VIN queries was recorded as MOCK, the UI
deleted AlphaLongitudinalEnabled, and the next drives ran on stock cruise.
"""
import pytest

pytest.importorskip("pyray")

from opendbc.car.structs import car
from openpilot.cereal import custom
from openpilot.selfdrive.ui.sunnypilot.ui_state import UIStateSP


class FakeParams:
  def __init__(self, **values):
    self.values = dict(values)

  def get_bool(self, key):
    return bool(self.values.get(key, False))

  def put_bool(self, key, value, block=False):
    self.values[key] = value

  def get(self, key):
    return self.values.get(key)

  def put(self, key, value, block=False):
    self.values[key] = value

  def remove(self, key):
    self.values.pop(key, None)


def _ui(brand, alpha_available):
  ui = UIStateSP.__new__(UIStateSP)
  ui.params = FakeParams(AlphaLongitudinalEnabled=True)
  ui.CP = car.CarParams.new_message(brand=brand, alphaLongitudinalAvailable=alpha_available)
  ui.CP_SP = custom.CarParamsSP.new_message()
  ui.has_longitudinal_control = False
  ui.has_icbm = False
  return ui


def test_mock_car_keeps_alpha_long():
  ui = _ui("mock", False)
  ui._enforce_constraints()
  assert ui.params.get_bool("AlphaLongitudinalEnabled")


def test_recognized_car_without_alpha_long_clears_it():
  ui = _ui("toyota", False)
  ui._enforce_constraints()
  assert not ui.params.get_bool("AlphaLongitudinalEnabled")


def test_recognized_car_with_alpha_long_keeps_it():
  ui = _ui("rivian", True)
  ui._enforce_constraints()
  assert ui.params.get_bool("AlphaLongitudinalEnabled")
