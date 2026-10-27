#!/usr/bin/env python3
"""Rivian enhanced MADS in Pause mode: Park or Reverse with the brake held must switch MADS off.

The Pause handling in CarSpecificEventsSP adds silentLkasDisable on every frame the brake is
held. The MADS state machine turns lkasDisable into a pause whenever silentLkasDisable is also
present, so without care the Park/Reverse disengage only paused MADS and it resumed once Drive
was selected. These tests run the real MADS state machine on the events CarSpecificEventsSP emits,
plus the gear and pedal events selfdrived would add, and check the resulting MADS state.
"""
from unittest.mock import MagicMock

from openpilot.cereal import custom, log
from opendbc.car import structs
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP
from openpilot.common.constants import CV
from openpilot.selfdrive.selfdrived.events import Events
from openpilot.sunnypilot.mads.helpers import MadsSteeringModeOnBrake
from openpilot.sunnypilot.mads.mads import ModularAssistiveDrivingSystem
from openpilot.sunnypilot.selfdrive.car.car_specific import CarSpecificEventsSP
from openpilot.sunnypilot.selfdrive.selfdrived.events import EventsSP

State = custom.ModularAssistiveDrivingSystem.ModularAssistiveDrivingSystemState
EventName = log.OnroadEvent.EventName
GearShifter = structs.CarState.GearShifter


def _setup(steering_mode):
  CP = structs.CarParams.new_message()
  CP.brand = 'rivian'
  CP_SP = structs.CarParamsSP()
  CP_SP.flags |= RivianFlagsSP.ENHANCED_MADS.value
  ev = CarSpecificEventsSP(CP, CP_SP)
  ev._rivian_steering_mode_on_brake = steering_mode
  ev._rivian_min_engage_speed_ms = 0

  sd = MagicMock()
  sd.CP = structs.CarParams()
  sd.CP.brand = 'rivian'
  sd.CP_SP = CP_SP
  sd.params = MagicMock()
  sd.params.get_bool = MagicMock(side_effect=lambda k: {"Mads": True, "MadsUnifiedEngagementMode": True}.get(k, False))
  sd.params.get = MagicMock(return_value=steering_mode)
  sd.events = Events()
  sd.events_sp = EventsSP()
  sd.enabled = sd.enabled_prev = False
  sd.initialized = True
  sd.CS_prev = structs.CarState.new_message()
  sd.sm = {'pandaStates': []}
  sd.state_machine = MagicMock()
  mads = ModularAssistiveDrivingSystem(sd)
  mads.enabled_toggle = True
  mads.steering_mode_on_brake = steering_mode
  mads.state_machine.state = State.enabled
  mads.enabled = mads.active = True
  return ev, mads, sd


def _frame(ev, mads, sd, gear=GearShifter.drive, brake=False, v_ego_mph=0.):
  CS = structs.CarState.new_message()
  CS.gearShifter = gear
  CS.brakePressed = brake
  CS.vEgo = v_ego_mph * CV.MPH_TO_MS
  CS.standstill = v_ego_mph == 0.
  CS.cruiseState.available = True
  sd.events.clear()
  sd.events_sp.clear()
  # what selfdrived adds before MADS runs
  if gear == GearShifter.park:
    sd.events.add(EventName.wrongGear)
  elif gear == GearShifter.reverse:
    sd.events.add(EventName.reverseGear)
  if brake and (not sd.CS_prev.brakePressed or not CS.standstill):
    sd.events.add(EventName.pedalPressed)
  for name in ev.update(CS, sd.events).names:
    sd.events_sp.add(name)
  mads.update(CS)
  sd.CS_prev = CS
  return mads.state_machine.state


def _brake_to_stop_then_shift(steering_mode, gear):
  ev, mads, sd = _setup(steering_mode)
  _frame(ev, mads, sd, brake=True, v_ego_mph=10.)
  for _ in range(5):
    _frame(ev, mads, sd, brake=True)
  states = [_frame(ev, mads, sd, gear=gear, brake=True) for _ in range(3)]
  return states, ev, mads, sd


class TestRivianPauseGearDisengage:
  def test_pause_mode_brake_held_pauses(self):
    ev, mads, sd = _setup(MadsSteeringModeOnBrake.PAUSE)
    _frame(ev, mads, sd, brake=True, v_ego_mph=10.)
    assert [_frame(ev, mads, sd, brake=True) for _ in range(5)] == [State.paused] * 5
    assert _frame(ev, mads, sd) == State.enabled

  def test_pause_mode_park_with_brake_held_switches_off(self):
    states, ev, mads, sd = _brake_to_stop_then_shift(MadsSteeringModeOnBrake.PAUSE, GearShifter.park)
    assert states[-1] == State.disabled
    # stays off after Drive is selected and the brake is released
    _frame(ev, mads, sd, brake=True)
    assert _frame(ev, mads, sd) == State.disabled

  def test_pause_mode_reverse_with_brake_held_switches_off(self):
    states, ev, mads, sd = _brake_to_stop_then_shift(MadsSteeringModeOnBrake.PAUSE, GearShifter.reverse)
    assert states[-1] == State.disabled
    _frame(ev, mads, sd, brake=True)
    assert _frame(ev, mads, sd) == State.disabled

  def test_remain_active_park_with_brake_held_switches_off(self):
    states, _, _, _ = _brake_to_stop_then_shift(MadsSteeringModeOnBrake.REMAIN_ACTIVE, GearShifter.park)
    assert states[-1] == State.disabled

  def test_pause_mode_park_without_brake_switches_off(self):
    ev, mads, sd = _setup(MadsSteeringModeOnBrake.PAUSE)
    _frame(ev, mads, sd)
    assert [_frame(ev, mads, sd, gear=GearShifter.park) for _ in range(3)][-1] == State.disabled
