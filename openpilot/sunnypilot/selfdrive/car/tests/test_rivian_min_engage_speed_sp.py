#!/usr/bin/env python3
"""Rivian "Minimum Speed to Engage MADS" gate.

Below MadsMinEngageSpeed the standalone MADS stalk must not engage lateral control.
CarSpecificEventsSP enforces this from its Rivian branch by emitting
belowMadsMinEngageSpeed (ET.NO_ENTRY), leaving the shared MADS core untouched.

Three properties are easy to break without noticing and are pinned here: the gate is
Rivian-only, a threshold of 0 disables it entirely, and cruise/UEM engagement
(pcmEnable/buttonEnable on the same frame) is exempt so engaging ACC always brings
lateral regardless of speed.

The threshold is stored in mph and read once in the constructor, so every case below
builds a fresh CarSpecificEventsSP.
"""
from cereal import custom, log
from opendbc.car import structs
from openpilot.common.constants import CV
from openpilot.selfdrive.selfdrived.events import Events
from openpilot.sunnypilot.selfdrive.car import car_specific
from openpilot.sunnypilot.selfdrive.car.car_specific import CarSpecificEventsSP

EventName = log.OnroadEvent.EventName
EventNameSP = custom.OnroadEventSP.EventName


class FakeParams:
  """Stand-in for Params during construction.

  MadsMinEngageSpeed only registers after a rebuild, so a real Params raises
  UnknownKeyName on builds that predate the key. Faking it also lets the threshold be
  parametrised, which is better testing than leaning on whatever the default happens
  to be. MadsSteeringMode is here because read_steering_mode_param() reads it from the
  same object.
  """

  def __init__(self, min_engage_mph):
    self._values = {"MadsMinEngageSpeed": min_engage_mph, "MadsSteeringMode": 0}

  def get(self, key, return_default=False):
    return self._values[key]

  def get_bool(self, key):
    return False


def _blocked(monkeypatch, min_engage_mph, v_ego_mph, enable_events=(), brand='rivian'):
  """Was engagement refused at v_ego_mph, with the gate set to min_engage_mph?"""
  monkeypatch.setattr(car_specific, "Params", lambda: FakeParams(min_engage_mph))

  CP = structs.CarParams.new_message()
  CP.brand = brand
  ev = CarSpecificEventsSP(CP, structs.CarParamsSP())

  CS = structs.CarState.new_message()
  CS.vEgo = v_ego_mph * CV.MPH_TO_MS
  CS.gearShifter = structs.CarState.GearShifter.drive

  base = Events()
  for e in enable_events:
    base.add(e)
  return ev.update(CS, base).has(EventNameSP.belowMadsMinEngageSpeed)


class TestRivianMinEngageSpeed:
  def test_stopped_blocks(self, monkeypatch):
    assert _blocked(monkeypatch, 5, 0)

  def test_below_threshold_blocks(self, monkeypatch):
    assert _blocked(monkeypatch, 5, 3)

  def test_above_threshold_allows(self, monkeypatch):
    assert not _blocked(monkeypatch, 5, 10)

  def test_straddling_the_threshold(self, monkeypatch):
    # deliberately not testing vEgo exactly equal to the threshold: vEgo is a capnp
    # Float32, so the mph -> m/s conversion does not round-trip and equality there is a
    # property of float rounding rather than of this gate.
    assert _blocked(monkeypatch, 5, 4.9)
    assert not _blocked(monkeypatch, 5, 5.1)

  def test_pcm_enable_is_exempt(self, monkeypatch):
    # engaging ACC brings lateral with it, at any speed
    assert not _blocked(monkeypatch, 5, 0, enable_events=[EventName.pcmEnable])

  def test_button_enable_is_exempt(self, monkeypatch):
    assert not _blocked(monkeypatch, 5, 0, enable_events=[EventName.buttonEnable])

  def test_unrelated_event_is_not_an_exemption(self, monkeypatch):
    # only the two engage events exempt; anything else must still be gated
    assert _blocked(monkeypatch, 5, 0, enable_events=[EventName.belowSteerSpeed])

  def test_threshold_zero_disables_the_gate(self, monkeypatch):
    # This pins the user-facing contract, but note it does not pin the "> 0" guard in
    # car_specific.py: vEgo is never negative, so "vEgo < 0" is already always false and
    # deleting that guard changes nothing. The guard is documentation, not logic.
    for speed in (0, 3, 60):
      assert not _blocked(monkeypatch, 0, speed)

  def test_non_rivian_never_blocks(self, monkeypatch):
    assert not _blocked(monkeypatch, 5, 0, brand='toyota')

  def test_non_rivian_never_reads_the_param(self, monkeypatch):
    # the gate is Rivian-only all the way down: the threshold is not even read
    monkeypatch.setattr(car_specific, "Params", lambda: FakeParams(5))
    CP = structs.CarParams.new_message()
    CP.brand = 'toyota'
    ev = CarSpecificEventsSP(CP, structs.CarParamsSP())
    assert not hasattr(ev, '_rivian_min_engage_speed_ms')
