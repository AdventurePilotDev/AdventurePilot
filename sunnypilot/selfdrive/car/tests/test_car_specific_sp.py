#!/usr/bin/env python3
"""Rivian angle-mode saturation -> steerSaturated ("Take Control") alert.

The stock steerSaturated raise (selfdrived) keys off the torque controller's saturation, which is
meaningless/suppressed while Rivian steers on its angle channel. CarSpecificEventsSP re-raises the
same stock alert from the angle channel's own saturation (RivianAngleSaturated param, written by
the carcontroller). Rivian angle-harness only.
"""
import unittest

from cereal import log
from opendbc.car import structs
from opendbc.car.rivian.values import CAR
from opendbc.car.rivian.interface import CarInterface
from openpilot.selfdrive.selfdrived.events import Events
from openpilot.sunnypilot.selfdrive.car.car_specific import CarSpecificEventsSP

EventName = log.OnroadEvent.EventName
GearShifter = structs.CarState.GearShifter


def _fp(xnor_box=False):
  fp = {i: {} for i in range(8)}
  fp[0][0x321] = 7  # SCCM_WheelTouch (not GEN2)
  if xnor_box:
    fp[1][0x1310] = 8
  return fp


def _cp(xnor_box=False):
  fp = _fp(xnor_box)
  cp = CarInterface.get_params(CAR.RIVIAN_R1, fp, [], alpha_long=False, is_release=False, docs=False)
  cp_sp = CarInterface.get_params_sp(cp, CAR.RIVIAN_R1, fp, [], alpha_long=False, is_release_sp=False, docs=False)
  return cp, cp_sp


class _FakeParams:
  def __init__(self, store):
    self.store = store

  def get_bool(self, k):
    return bool(self.store.get(k, False))

  def get(self, k, return_default=False):
    return self.store.get(k, 0)


class TestRivianSaturationEvent(unittest.TestCase):
  def _events_after_poll(self, cp, cp_sp, saturated):
    ev = CarSpecificEventsSP(cp, cp_sp)
    # swap in a fake so the test doesn't depend on the new param key being compiled into the
    # params registry; __init__ already read the (existing) mads params via real Params.
    ev._params = _FakeParams({"RivianAngleSaturated": saturated, "RivianAngleSteerPhase": 0})
    cs = structs.CarState()
    cs.gearShifter = GearShifter.drive
    events = None
    for _ in range(5):  # the angle-harness poll fires on _angle_frame % 5 == 0 (the 5th update)
      events = Events()
      ev.update(cs, events)
    return events

  def test_saturation_emits_steer_saturated(self):
    cp, cp_sp = _cp(xnor_box=True)
    self.assertTrue(self._events_after_poll(cp, cp_sp, True).has(EventName.steerSaturated))

  def test_no_event_when_not_saturated(self):
    cp, cp_sp = _cp(xnor_box=True)
    self.assertFalse(self._events_after_poll(cp, cp_sp, False).has(EventName.steerSaturated))

  def test_base_harness_gate(self):
    # no xnor box -> _rivian_angle_harness False -> the poll block never runs, even with the param set
    cp, cp_sp = _cp(xnor_box=False)
    self.assertFalse(self._events_after_poll(cp, cp_sp, True).has(EventName.steerSaturated))


if __name__ == "__main__":
  unittest.main()
