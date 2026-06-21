"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from collections import namedtuple

from opendbc.car import structs
from opendbc.car.interfaces import CarStateBase

MadsDataSP = namedtuple("MadsDataSP",
                        ["lka_icon_states", "lat_active"])


class MadsCarController:
  def __init__(self):
    self.mads = MadsDataSP(False, False)

    self.lka_icon_states = False
    self.lat_active = False

  def mads_status_update(self, CC: structs.CarControl, CC_SP: structs.CarControlSP, CS: CarStateBase) -> MadsDataSP:
    if CC_SP.mads.available:
<<<<<<< c488ab5f4fa43d6481f26bf1d97a40cc9a5a4271
      self.lka_icon_states = CC.latActive
=======
      self.lka_icon_states = self.lat_active
>>>>>>> 4210a4aaac1940234ae19cb3f7f0521313993816
      self.lat_active = CC.latActive
    else:
      self.lka_icon_states = CC.enabled
      self.lat_active = CC.latActive

    return MadsDataSP(self.lka_icon_states, self.lat_active)

  def update(self, CC: structs.CarControl, CC_SP: structs.CarControlSP, CS: CarStateBase) -> None:
    self.mads = self.mads_status_update(CC, CC_SP, CS)
