"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from collections import namedtuple

from opendbc.car import structs
from opendbc.car.interfaces import CarStateBase

GearShifter = structs.CarState.GearShifter

MadsDataSP = namedtuple("MadsDataSP",
                        ["lka_icon_states", "lat_active"])


class MadsCarController:
  def __init__(self):
    self.mads = MadsDataSP(False, False)

    self.lka_icon_states = False
    self.lat_active = False

  def mads_status_update(self, CC: structs.CarControl, CC_SP: structs.CarControlSP, CS: CarStateBase) -> MadsDataSP:
    # Hard-gate lateral actuation to forward (drive) gear. steerAtStandstill (angle branch) holds
    # the angle while stopped in drive, but it must NOT let lateral act in reverse/park/neutral —
    # otherwise the angle controller moves the wheel during the reverse-parking shuffle, even after
    # a brake disengage. The MADS reverse-pause only holds while gear is *actually* reverse; this
    # covers the shifting / drive-creep gaps where MADS re-arms (cruiseState.available edge).
    lat_active = CC.latActive and CS.out.gearShifter == GearShifter.drive
    if CC_SP.mads.available:
      self.lka_icon_states = CC.latActive
      self.lat_active = lat_active
    else:
      self.lka_icon_states = CC.enabled
      self.lat_active = lat_active

    return MadsDataSP(self.lka_icon_states, self.lat_active)

  def update(self, CC: structs.CarControl, CC_SP: structs.CarControlSP, CS: CarStateBase) -> None:
    self.mads = self.mads_status_update(CC, CC_SP, CS)
