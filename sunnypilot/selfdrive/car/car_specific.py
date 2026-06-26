"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""

from cereal import log, custom
from opendbc.car import structs

from opendbc.car.chrysler.values import RAM_DT
from openpilot.common.params import Params
from openpilot.selfdrive.selfdrived.events import Events
from openpilot.sunnypilot.mads.helpers import MadsSteeringModeOnBrake, read_steering_mode_param
from openpilot.sunnypilot.selfdrive.selfdrived.events import EventsSP

EventName = log.OnroadEvent.EventName
EventNameSP = custom.OnroadEventSP.EventName
ButtonType = structs.CarState.ButtonEvent.Type
GearShifter = structs.CarState.GearShifter


class CarSpecificEventsSP:
  def __init__(self, CP: structs.CarParams, CP_SP: structs.CarParamsSP):
    self.CP = CP
    self.CP_SP = CP_SP

    self.low_speed_alert = False
    self._rivian_up2_active = False
    self._rivian_prev_in_park = False
    self._rivian_park_disable_pending = False
    if self.CP.brand == 'rivian':
      self._rivian_steering_mode_on_brake = read_steering_mode_param(CP, CP_SP, Params())

  def update(self, CS: structs.CarState, events: Events):
    events_sp = EventsSP()

    if self.CP.brand == 'chrysler':
      if self.CP.carFingerprint in RAM_DT:
        # remove belowSteerSpeed event from CarSpecificEvents as RAM_DT uses a different logic
        if events.has(EventName.belowSteerSpeed):
          events.remove(EventName.belowSteerSpeed)

        # TODO-SP: use if/elif to have the gear shifter condition takes precedence over the speed condition
        # TODO-SP: add 1 m/s hysteresis
        if CS.vEgo >= self.CP.minEnableSpeed:
          self.low_speed_alert = False
        if self.CP.minEnableSpeed >= 14.5 and CS.gearShifter != GearShifter.drive:
          self.low_speed_alert = True
      if self.low_speed_alert:
        events.add(EventName.belowSteerSpeed)

    elif self.CP.brand == 'toyota':
      if self.CP.openpilotLongitudinalControl:
        if CS.cruiseState.standstill and not CS.brakePressed and self.CP_SP.enableGasInterceptor:
          if events.has(EventName.resumeRequired):
            events.remove(EventName.resumeRequired)

    elif self.CP.brand == 'rivian':
      in_park = CS.gearShifter == GearShifter.park
      for be in CS.buttonEvents:
        if be.type == ButtonType.altButton2:
          self._rivian_up2_active = be.pressed
          # UP_2 is a full-cancel gesture: disengage MADS lateral so it doesn't
          # persist in Mode B after ACC cancels. lkasDisable is ET.USER_DISABLE
          # and works from any active MADS state (enabled, overriding, etc.).
          if be.pressed:
            events_sp.add(EventNameSP.lkasDisable)
      # Park entry: full MADS disengage, same as UP_2.
      if in_park and not self._rivian_prev_in_park:
        events_sp.add(EventNameSP.lkasDisable)
        self._rivian_park_disable_pending = True
      elif in_park and self._rivian_park_disable_pending:
        # HACK: fire lkasDisable a second time so that State.disabled wins over State.paused.
        # On frame N, wrongGear fires concurrently and mads.update_events() calls
        # transition_paused_state() → silentLkasDisable; the state machine then sees both
        # silentLkasDisable and lkasDisable and (currently) lets silentLkasDisable win →
        # State.paused instead of State.disabled. On frame N+1 the state is already paused
        # so transition_paused_state() is a no-op, silentLkasDisable is not re-emitted, and
        # the lone lkasDisable drives the state to State.disabled as intended.
        #
        # Clean general fix: in sunnypilot/mads/state.py change the USER_DISABLE branch so
        # that lkasDisable (explicit) takes priority over silentLkasDisable (implicit) and
        # always yields State.disabled. Rivian-specific alternative: a new event type (e.g.
        # hardLkasDisable) that unconditionally maps to State.disabled without checking for
        # silentLkasDisable. Both approaches would not be Rivian-specific.
        events_sp.add(EventNameSP.lkasDisable)
        self._rivian_park_disable_pending = False
      if not in_park:
        self._rivian_park_disable_pending = False
      self._rivian_prev_in_park = in_park
      # Suppress pcmEnable while UP_2 is held or in park.
      if self._rivian_up2_active or in_park:
        events.remove(EventName.pcmEnable)

      # PAUSE mode: keep MADS lateral paused for the full duration of a brake press.
      # Emitting silentLkasDisable (ET.USER_DISABLE) every frame beats the
      # silentLkasEnable (ET.ENABLE) that mads.py adds, because ET.USER_DISABLE is
      # checked first in the MADS state machine. This fixes both the standstill
      # case (where pedalPressed event stops firing) and Mode A (ACC+MADS active).
      if CS.brakePressed and self._rivian_steering_mode_on_brake == MadsSteeringModeOnBrake.PAUSE:
        events_sp.add(EventNameSP.silentLkasDisable)

    return events_sp
