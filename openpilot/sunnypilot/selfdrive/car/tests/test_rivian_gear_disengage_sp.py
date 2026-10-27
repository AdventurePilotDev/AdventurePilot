#!/usr/bin/env python3
"""Rivian reverse-gear entry must disengage MADS, not leave it paused.

mads.update_events() rewrites reverseGear into silentReverseGear + silentLkasDisable
(ET.USER_DISABLE), and the MADS state machine's USER_DISABLE branch routes that to
State.paused. paused auto-resumes to Mode B the instant Drive is re-selected, so lateral
silently re-arms mid parking shuffle.

CarSpecificEventsSP works around it by emitting lkasDisable on two consecutive frames:
frame N loses the race against silentLkasDisable, frame N+1 wins because
transition_paused_state() is already a no-op. These tests pin that two-frame shape, since
it is timing-sensitive and easy to break without noticing.
"""
from openpilot.cereal import custom
from opendbc.car import structs
from openpilot.selfdrive.selfdrived.events import Events
from openpilot.sunnypilot.selfdrive.car.car_specific import CarSpecificEventsSP
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP

EventNameSP = custom.OnroadEventSP.EventName
ButtonType = structs.CarState.ButtonEvent.Type
GearShifter = structs.CarState.GearShifter

DRIVE = GearShifter.drive
REVERSE = GearShifter.reverse
PARK = GearShifter.park


def _disables(gears, enhanced=True, up2_on_first_frame=False):
  """Feed a gear sequence through one CarSpecificEventsSP, one update() per gear.

  Returns whether lkasDisable was emitted on each frame.
  """
  CP = structs.CarParams.new_message()
  CP.brand = 'rivian'
  CP_SP = structs.CarParamsSP()
  if enhanced:
    CP_SP.flags |= RivianFlagsSP.ENHANCED_MADS.value
  ev = CarSpecificEventsSP(CP, CP_SP)

  out = []
  for i, gear in enumerate(gears):
    CS = structs.CarState.new_message()
    CS.gearShifter = gear
    if up2_on_first_frame and i == 0:
      CS.buttonEvents = [structs.CarState.ButtonEvent(pressed=True, type=ButtonType.altButton2)]
    out.append(ev.update(CS, Events()).has(EventNameSP.lkasDisable))
  return out


class TestRivianGearDisengage:
  def test_reverse_entry_fires_exactly_two_frames(self):
    # frame N loses to silentLkasDisable -> paused, frame N+1 lands State.disabled
    assert _disables([DRIVE, DRIVE, REVERSE, REVERSE, REVERSE, REVERSE]) == [False, False, True, True, False, False]

  def test_back_to_drive_does_not_refire(self):
    assert _disables([REVERSE, REVERSE, DRIVE, DRIVE]) == [True, True, False, False]

  def test_each_reverse_entry_disengages_again(self):
    # a parking shuffle: every shift into reverse must disengage on its own
    assert _disables([DRIVE, REVERSE, REVERSE, DRIVE, REVERSE, REVERSE]) == [False, True, True, False, True, True]

  def test_single_frame_reverse_blip(self):
    # the pair never completes, but the first lkasDisable is still emitted
    assert _disables([DRIVE, REVERSE, DRIVE]) == [False, True, False]

  def test_drive_never_disengages(self):
    assert _disables([DRIVE] * 6) == [False] * 6

  def test_park_entry_still_fires_two_frames(self):
    # guards the pre-existing park path against interference from the reverse block
    assert _disables([DRIVE, PARK, PARK, PARK]) == [False, True, True, False]

  def test_up2_disengages(self):
    assert _disables([DRIVE, DRIVE], up2_on_first_frame=True) == [True, False]

  def test_enhanced_mads_off_never_disengages(self):
    # with "Use enhanced Rivian MADS" off, MADS is stock: no UP_2, Park or Reverse disengage
    assert _disables([DRIVE, REVERSE, REVERSE, PARK, PARK, DRIVE], enhanced=False, up2_on_first_frame=True) == [False] * 6
