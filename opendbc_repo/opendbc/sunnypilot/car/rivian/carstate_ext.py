"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import math
from enum import StrEnum

from opendbc.car import Bus, structs
from openpilot.common.params import Params
from openpilot.sunnypilot.mads.helpers import MadsSteeringModeOnBrake, read_steering_mode_param
from opendbc.can.parser import CANParser
from opendbc.car.common.conversions import Conversions as CV
from opendbc.car.rivian.values import DBC
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP

ButtonType = structs.CarState.ButtonEvent.Type

MAX_SET_SPEED = 85 * CV.MPH_TO_MS
MIN_SET_SPEED = 20 * CV.MPH_TO_MS


class CarStateExt:
  def __init__(self, CP: structs.CarParams, CP_SP: structs.CarParamsSP):
    self.CP = CP
    self.CP_SP = CP_SP

    self.set_speed = 10
    self.increase_button = False
    self.decrease_button = False
    self.distance_button = 0
    self.increase_counter = 0
    self.decrease_counter = 0
    self.vdm_user_adas_request = 0
    self._lkas_pending = False
    self.steering_mode_on_brake = read_steering_mode_param(CP, CP_SP, Params())

    self._resume_enabled: bool = Params().get_bool("RivianResumeEnabled")
    self.last_active_set_speed: float | None = None
    self._prev_cruise_enabled: bool = False
    self._resume_eligible: bool = False
    self._resume_acc_counter: int = 0
    self._prev_stalk_down2: bool = False
    self._prev_stalk_down: bool = False
    self._frames_since_acc_on: int = 0

  def update_stalk_controls(self, ret: structs.CarState, can_parsers: dict[StrEnum, CANParser]) -> list:
    cp = can_parsers[Bus.pt]
    vdm = int(cp.vl["VDM_AdasSts"]["VDM_UserAdasRequest"])

    button_events = []

    # Emit the deferred lkas event only if the current frame is not UP_2.
    # This 1-frame lookahead prevents the CAN transition through UP_1 on the
    # way to UP_2 from accidentally engaging or changing MADS state.
    if self._lkas_pending:
      if vdm != 2:
        button_events.append(structs.CarState.ButtonEvent(pressed=True, type=ButtonType.lkas))
      self._lkas_pending = False

    # UP_1 rising edge (from IDLE or DOWN only, not from UP_2 release).
    # In DISENGAGE mode with ACC active, suppress: UP_1 cancels Rivian ACC natively
    # and pcmDisable is stripped by mads.update_events(), leaving MADS in Mode B.
    # Generating lkas here would also fire manualSteeringRequired and kill MADS.
    if vdm == 1 and self.vdm_user_adas_request not in (1, 2):
      if not (self.steering_mode_on_brake == MadsSteeringModeOnBrake.DISENGAGE and ret.cruiseState.enabled):
        self._lkas_pending = True

    # Signal UP_2 state via altButton2 so car_specific.py can fire lkasDisable
    # and suppress pcmEnable. UP_2 disengages ACC; without this, MADS can persist
    # in Mode B (lateral only) after ACC cancels.
    if vdm == 2 and self.vdm_user_adas_request != 2:
      button_events.append(structs.CarState.ButtonEvent(pressed=True, type=ButtonType.altButton2))
    elif vdm != 2 and self.vdm_user_adas_request == 2:
      button_events.append(structs.CarState.ButtonEvent(pressed=False, type=ButtonType.altButton2))

    self.vdm_user_adas_request = vdm
    return button_events

  def update_longitudinal_upgrade(self, ret: structs.CarState, can_parsers: dict[StrEnum, CANParser]) -> list:
    cp_park = can_parsers[Bus.alt]
    cp_adas = can_parsers[Bus.adas]
    cp = can_parsers[Bus.pt]
    button_events = []

    prev_increase_button = self.increase_button
    prev_decrease_button = self.decrease_button

    if self.CP.openpilotLongitudinalControl:
      # distance scroll wheel
      right_scroll = cp_park.vl["WheelButtons_Fwd"]["RightButton_Scroll"]
      if right_scroll != 255:
        if self.distance_button != right_scroll:
          button_events.append(structs.CarState.ButtonEvent(pressed=False, type=ButtonType.gapAdjustCruise))
        self.distance_button = right_scroll

      # button logic for set-speed
      self.increase_button = cp_park.vl["WheelButtons_Fwd"]["RightButton_RightClick"] == 2
      self.decrease_button = cp_park.vl["WheelButtons_Fwd"]["RightButton_LeftClick"] == 2

      self.increase_counter = self.increase_counter + 1 if self.increase_button else 0
      self.decrease_counter = self.decrease_counter + 1 if self.decrease_button else 0

      metric = cp_adas.vl["Cluster"]["Cluster_Unit"] == 0
      conversion = CV.KPH_TO_MS if metric else CV.MPH_TO_MS
      long_press_step = 10.0 if metric else 5.0
      set_speed_converted = self.set_speed * (CV.MS_TO_KPH if metric else CV.MS_TO_MPH)

      if self.increase_button:
        if self.increase_counter % 66 == 0:
          self.set_speed = (int(math.ceil((set_speed_converted + 1) / long_press_step)) * long_press_step) * conversion
        elif not prev_increase_button:
          self.set_speed += conversion

      if self.decrease_button:
        if self.decrease_counter % 66 == 0:
          self.set_speed = (int(math.floor((set_speed_converted - 1) / long_press_step)) * long_press_step) * conversion
        elif not prev_decrease_button:
          self.set_speed -= conversion

      # VDM_UserAdasRequest: 0=IDLE, 1=UP_1, 2=UP_2, 3=DOWN_1, 4=DOWN_2
      vdm_request = int(cp.vl["VDM_AdasSts"]["VDM_UserAdasRequest"])
      stalk_down2 = vdm_request == 4
      stalk_down = vdm_request in (3, 4)

      # Save set speed on ACC deactivation (before vEgoCluster reset so value is intact)
      if self._resume_enabled:
        if self._prev_cruise_enabled and not ret.cruiseState.enabled:
          self.last_active_set_speed = self.set_speed
          self._resume_eligible = False
          self._resume_acc_counter = 0

      # Arm resume only on ACC rising edge while DOWN_2 is held
      if self._resume_enabled:
        if not self._prev_cruise_enabled and ret.cruiseState.enabled and stalk_down2:
          self._resume_eligible = True
        # Also arm on DOWN_2 rising edge within ~100ms of ACC activation: handles the case
        # where the stalk transitions through DOWN_1 before reaching DOWN_2 (~40-50ms delay
        # observed in logs), so ACC activates while the stalk is still in DOWN_1 detent.
        elif (ret.cruiseState.enabled and not self._prev_stalk_down2 and stalk_down2
              and not self._resume_eligible and self._frames_since_acc_on < 10):
          self._resume_eligible = True
        # Also arm on first DOWN_2 press while ACC is on and speed is below minimum: handles
        # stop-and-go where ACC has been engaged continuously (frames_since_acc_on > 10) and
        # the driver presses DOWN_2 to resume to a previously set higher speed.
        elif (ret.cruiseState.enabled and not self._prev_stalk_down2 and stalk_down2
              and ret.vEgoCluster < MIN_SET_SPEED and not self._resume_eligible):
          self._resume_eligible = True

      if not ret.cruiseState.enabled:
        self.set_speed = ret.vEgoCluster

      if stalk_down and not self._prev_stalk_down and not self._resume_eligible:
        # Mimic Rivian ACC: tapping stalk down snaps set speed to current speed (never decreases)
        self.set_speed = max(self.set_speed, ret.vEgoCluster)

      self._prev_cruise_enabled = ret.cruiseState.enabled
      self._prev_stalk_down2 = stalk_down2
      self._prev_stalk_down = stalk_down
      self._frames_since_acc_on = (self._frames_since_acc_on + 1) if ret.cruiseState.enabled else 0

      # Resume: count consecutive frames where ACC is on and DOWN_2 is held after arming
      if self._resume_enabled and self._resume_eligible and ret.cruiseState.enabled and stalk_down2:
        self._resume_acc_counter += 1
      else:
        self._resume_acc_counter = 0

      if self._resume_acc_counter == 50 and self.last_active_set_speed is not None:
        self.set_speed = self.last_active_set_speed
        self._resume_eligible = False
        self._resume_acc_counter = 0

      self.set_speed = max(MIN_SET_SPEED, min(self.set_speed, MAX_SET_SPEED))
      ret.cruiseState.speed = self.set_speed

    if self.CP.enableBsm:
      ret.leftBlindspot = cp_park.vl["BSM_BlindSpotIndicator_Fwd"]["BSM_BlindSpotIndicator_Left"] != 0
      ret.rightBlindspot = cp_park.vl["BSM_BlindSpotIndicator_Fwd"]["BSM_BlindSpotIndicator_Right"] != 0

    return button_events

  def update(self, ret: structs.CarState, can_parsers: dict[StrEnum, CANParser]) -> None:
    button_events = []

    if self.CP_SP.flags & RivianFlagsSP.LONGITUDINAL_HARNESS_UPGRADE:
      button_events.extend(self.update_longitudinal_upgrade(ret, can_parsers))

    button_events.extend(self.update_stalk_controls(ret, can_parsers))
    ret.buttonEvents = button_events

  @staticmethod
  def get_parser(CP, CP_SP) -> dict[StrEnum, CANParser]:
    messages = {}

    if CP_SP.flags & RivianFlagsSP.LONGITUDINAL_HARNESS_UPGRADE:
      messages[Bus.alt] = CANParser(DBC[CP.carFingerprint][Bus.alt], [], 1)

    return messages
