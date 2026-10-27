"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from types import SimpleNamespace

from opendbc.car import Bus, structs
from opendbc.safety import ALTERNATIVE_EXPERIENCE
from opendbc.sunnypilot.car.interfaces import _initialize_rivian_enhanced_mads
from opendbc.sunnypilot.car.rivian.carstate_ext import CarStateExt
from opendbc.sunnypilot.car.rivian.values import RivianFlagsSP, RivianSafetyFlagsSP

ButtonType = structs.CarState.ButtonEvent.Type

IDLE, UP_1, UP_2 = 0, 1, 2


class TestEnhancedMadsSetting:
  def _init(self, brand, value):
    CP = structs.CarParams(brand=brand)
    CP_SP = structs.CarParamsSP()
    params_dict = {} if value is None else {"RivianEnhancedMads": value}
    _initialize_rivian_enhanced_mads(CP, CP_SP, params_dict)
    return CP_SP

  def test_on_sets_flag_and_safety_bit(self):
    for value in (True, "1"):
      CP_SP = self._init('rivian', value)
      assert CP_SP.flags & RivianFlagsSP.ENHANCED_MADS
      assert CP_SP.safetyParam & RivianSafetyFlagsSP.ENHANCED_MADS

  def test_off_or_unset_leaves_both_clear(self):
    for value in (False, "0", None):
      CP_SP = self._init('rivian', value)
      assert not CP_SP.flags & RivianFlagsSP.ENHANCED_MADS
      assert not CP_SP.safetyParam & RivianSafetyFlagsSP.ENHANCED_MADS

  def test_other_brands_ignored(self):
    CP_SP = self._init('tesla', True)
    assert CP_SP.flags == 0
    assert CP_SP.safetyParam == 0


class _FakeParser:
  def __init__(self):
    self.vl = {"VDM_AdasSts": {"VDM_UserAdasRequest": IDLE}}


class TestStalkControls:
  # Built without __init__, which reads openpilot Params; only the stalk state is needed here.
  def _make(self, enhanced, disengage_on_brake=False):
    CP = structs.CarParams(brand='rivian')
    if disengage_on_brake:
      CP.alternativeExperience |= ALTERNATIVE_EXPERIENCE.MADS_DISENGAGE_LATERAL_ON_BRAKE
    CP_SP = structs.CarParamsSP()
    if enhanced:
      CP_SP.flags |= RivianFlagsSP.ENHANCED_MADS.value
    cs = CarStateExt.__new__(CarStateExt)
    cs.CP, cs.CP_SP = CP, CP_SP
    cs.vdm_user_adas_request = 0
    cs._lkas_pending = False
    cs.acc_fault_recorder = SimpleNamespace(update=lambda *a: None)
    self.parser = _FakeParser()
    self.parsers = {Bus.pt: self.parser, Bus.cam: self.parser}
    return cs

  def _step(self, cs, vdm, cruise=False):
    self.parser.vl["VDM_AdasSts"]["VDM_UserAdasRequest"] = vdm
    ret = structs.CarState()
    ret.cruiseState.enabled = cruise
    cs.update(ret, self.parsers)
    return [(be.type, be.pressed) for be in ret.buttonEvents]

  def test_off_emits_nothing(self):
    cs = self._make(enhanced=False)
    for vdm in (IDLE, UP_1, IDLE, IDLE, UP_2, IDLE, IDLE):
      assert self._step(cs, vdm) == []

  def test_up1_emits_lkas_one_frame_later(self):
    cs = self._make(enhanced=True)
    self._step(cs, IDLE)
    assert self._step(cs, UP_1) == []
    assert self._step(cs, UP_1) == [(ButtonType.lkas, True)]

  def test_up1_on_the_way_to_up2_is_not_lkas(self):
    cs = self._make(enhanced=True)
    self._step(cs, IDLE)
    self._step(cs, UP_1)
    assert self._step(cs, UP_2) == [(ButtonType.altButton2, True)]
    assert self._step(cs, IDLE) == [(ButtonType.altButton2, False)]

  def test_up1_suppressed_in_disengage_mode_with_acc_on(self):
    cs = self._make(enhanced=True, disengage_on_brake=True)
    self._step(cs, IDLE, cruise=True)
    self._step(cs, UP_1, cruise=True)
    assert self._step(cs, UP_1, cruise=True) == []

  def test_up1_not_suppressed_with_acc_on_in_other_modes(self):
    cs = self._make(enhanced=True, disengage_on_brake=False)
    self._step(cs, IDLE, cruise=True)
    self._step(cs, UP_1, cruise=True)
    assert self._step(cs, UP_1, cruise=True) == [(ButtonType.lkas, True)]
