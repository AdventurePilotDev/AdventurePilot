#!/usr/bin/env python3
"""Rivian MadsSteeringMode: seed DISENGAGE on a first install, never on an existing one.

CarStateExt seeds the Rivian default of DISENGAGE only when CarParamsPersistent is unset.
card.py writes that key AFTER building the CarInterface (and so this constructor), and it
has no registered default, so manager_init's "fill unset params with their default" loop
never touches it. It is therefore unset only on a device that has never completed a drive.

The whole point of this code is the cases where it does NOT write. An earlier attempt used
a marker param registered with default "0", which manager_init filled for everyone before
it was ever read, so it overwrote every existing user's chosen mode on the first drive
after updating. The existing-install cases below are that regression, pinned.

The second guard - only writing when the value is still the stock REMAIN_ACTIVE - covers a
mode chosen in settings while parked, before the first drive, which CarParamsPersistent
alone cannot distinguish.
"""
import openpilot.common.params as params_module
from opendbc.car import structs
from opendbc.sunnypilot.car.rivian import carstate_ext
from opendbc.sunnypilot.car.rivian.carstate_ext import CarStateExt
from openpilot.sunnypilot.mads.helpers import MadsSteeringModeOnBrake

REMAIN_ACTIVE = MadsSteeringModeOnBrake.REMAIN_ACTIVE
PAUSE = MadsSteeringModeOnBrake.PAUSE
DISENGAGE = MadsSteeringModeOnBrake.DISENGAGE


class FakeParams:
  """Stand-in for Params, recording writes so the "does not write" cases can be asserted.

  CarStateExt constructs Params() more than once, so every construction has to hand back
  the same state; the factory installed by _construct() returns this single instance.
  """

  def __init__(self, car_params_persistent, steering_mode):
    self._values = {
      "MadsSteeringMode": steering_mode,
      "RivianResumeEnabled": False,
    }
    if car_params_persistent is not None:
      self._values["CarParamsPersistent"] = car_params_persistent
    self.writes = []

  def get(self, key, block=False, return_default=False):
    return self._values.get(key)

  def get_bool(self, key, block=False):
    return bool(self._values.get(key, False))

  def put(self, key, value, block=False):
    self.writes.append((key, value))
    self._values[key] = value

  def put_bool(self, key, value, block=False):
    self.writes.append((key, bool(value)))
    self._values[key] = bool(value)


def _construct(monkeypatch, car_params_persistent, steering_mode, fake=None):
  """Build one CarStateExt against a fake Params. Returns that fake.

  Params is imported at module top level on one branch and lazily inside the constructor on
  the other, so both binding sites are patched; raising=False covers whichever is absent.
  """
  fake = fake or FakeParams(car_params_persistent, steering_mode)
  monkeypatch.setattr(params_module, "Params", lambda: fake)
  monkeypatch.setattr(carstate_ext, "Params", lambda: fake, raising=False)

  CP = structs.CarParams.new_message()
  CP.brand = 'rivian'
  ext = CarStateExt.__new__(CarStateExt)
  CarStateExt.__init__(ext, CP, structs.CarParamsSP())
  fake.ext = ext
  return fake


def _mode_writes(fake):
  return [v for k, v in fake.writes if k == "MadsSteeringMode"]


class TestRivianMadsSteeringDefault:
  def test_fresh_install_seeds_disengage(self, monkeypatch):
    # never driven, value still stock: the one case that should write
    fake = _construct(monkeypatch, None, REMAIN_ACTIVE)
    assert _mode_writes(fake) == [DISENGAGE]

  def test_fresh_install_uses_the_seeded_value_this_drive(self, monkeypatch):
    # the seed must also take effect immediately, not only from the next drive
    fake = _construct(monkeypatch, None, REMAIN_ACTIVE)
    assert fake.ext.steering_mode_on_brake == DISENGAGE

  def test_existing_install_on_remain_active_is_untouched(self, monkeypatch):
    # THE regression: an existing user sitting on the stock default must not be moved
    fake = _construct(monkeypatch, b"cp", REMAIN_ACTIVE)
    assert _mode_writes(fake) == []
    assert fake.ext.steering_mode_on_brake == REMAIN_ACTIVE

  def test_existing_install_keeps_a_chosen_pause(self, monkeypatch):
    fake = _construct(monkeypatch, b"cp", PAUSE)
    assert _mode_writes(fake) == []
    assert fake.ext.steering_mode_on_brake == PAUSE

  def test_existing_install_already_disengage_is_not_rewritten(self, monkeypatch):
    # correct value, but still no write: the guard is on the signal, not on the outcome
    fake = _construct(monkeypatch, b"cp", DISENGAGE)
    assert _mode_writes(fake) == []

  def test_fresh_install_keeps_a_mode_chosen_before_the_first_drive(self, monkeypatch):
    # parked, never driven, user picked PAUSE in settings: CarParamsPersistent cannot tell,
    # so the stock-value guard is what protects this
    fake = _construct(monkeypatch, None, PAUSE)
    assert _mode_writes(fake) == []
    assert fake.ext.steering_mode_on_brake == PAUSE

  def test_fresh_install_pre_chosen_disengage_is_not_rewritten(self, monkeypatch):
    fake = _construct(monkeypatch, None, DISENGAGE)
    assert _mode_writes(fake) == []

  def test_seed_happens_once_and_only_once(self, monkeypatch):
    # first drive seeds; card.py then writes CarParamsPersistent, so the second drive must not
    fake = _construct(monkeypatch, None, REMAIN_ACTIVE)
    assert _mode_writes(fake) == [DISENGAGE]
    fake.writes.clear()
    fake._values["CarParamsPersistent"] = b"cp"  # what card.py does after the first drive
    _construct(monkeypatch, None, REMAIN_ACTIVE, fake=fake)
    assert _mode_writes(fake) == []

  def test_a_user_change_after_the_seed_survives(self, monkeypatch):
    # seed, then the driver picks Remain Active back; it must stick, not be re-seeded
    fake = _construct(monkeypatch, None, REMAIN_ACTIVE)
    fake._values["CarParamsPersistent"] = b"cp"
    fake._values["MadsSteeringMode"] = REMAIN_ACTIVE
    fake.writes.clear()
    _construct(monkeypatch, None, REMAIN_ACTIVE, fake=fake)
    assert _mode_writes(fake) == []
    assert fake.ext.steering_mode_on_brake == REMAIN_ACTIVE

  def test_the_seeded_value_is_an_int(self, monkeypatch):
    # MadsSteeringMode is an INT param and params_pyx.put type-checks strictly: a str raises
    # TypeError. This has bitten this branch before, on MadsMinEngageSpeed.
    fake = _construct(monkeypatch, None, REMAIN_ACTIVE)
    assert isinstance(_mode_writes(fake)[0], int)
