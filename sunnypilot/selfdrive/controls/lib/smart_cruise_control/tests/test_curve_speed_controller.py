"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import numpy as np

import cereal.messaging as messaging
from cereal import custom, log
from openpilot.common.params import Params
from openpilot.selfdrive.car.cruise import V_CRUISE_UNSET
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.sunnypilot.selfdrive.controls.lib.smart_cruise_control.curve_speed_controller import (
  CurveSpeedController, A_LAT_MAX)
from openpilot.sunnypilot.selfdrive.controls.lib.smart_cruise_control.lateral_load_governor import (
  LateralLoadGovernor, A_LAT_CEILING)

CurveState = custom.LongitudinalPlanSP.SmartCruiseControl.CurveState
N = len(ModelConstants.T_IDXS)


def make_model(yaw_rate: float, speed: float):
  model = messaging.new_message('modelV2')
  orientationRate = log.XYZTData.new_message()
  orientationRate.z = [float(yaw_rate) for _ in range(N)]
  model.modelV2.orientationRate = orientationRate
  velocity = log.XYZTData.new_message()
  velocity.x = [float(speed) for _ in range(N)]
  model.modelV2.velocity = velocity
  return model


def make_controls_state(curvature=0.0, saturated=False, actual=0.0, desired=0.0):
  cs = messaging.new_message('controlsState')
  cs.controlsState.curvature = float(curvature)
  ts = cs.controlsState.lateralControlState.init('torqueState')
  ts.saturated = bool(saturated)
  ts.actualLateralAccel = float(actual)
  ts.desiredLateralAccel = float(desired)
  return cs


def make_car_state(v_ego=20.0, yaw_rate=0.0):
  cs = messaging.new_message('carState')
  cs.carState.vEgo = float(v_ego)
  cs.carState.yawRate = float(yaw_rate)
  return cs


def sm_for(yaw_rate, speed, curvature=0.0, saturated=False, actual=0.0, desired=0.0, v_ego=20.0):
  return {
    'modelV2': make_model(yaw_rate, speed).modelV2,
    'controlsState': make_controls_state(curvature, saturated, actual, desired).controlsState,
    'carState': make_car_state(v_ego, actual / max(v_ego, 1.0)).carState,
  }


class TestCurveSpeedController:
  def setup_method(self):
    Params().put_bool("CurveSpeedControl", True)
    self.scc = CurveSpeedController()
    self.scc.enabled = True

  def test_initial_state(self):
    assert self.scc.state == CurveState.disabled
    assert self.scc.output_v_target == V_CRUISE_UNSET

  def test_straight_inactive(self):
    sm = sm_for(yaw_rate=0.001, speed=25.0, v_ego=25.0)
    self.scc.update(sm, True, False, 25.0, 0.0, 30.0)
    assert not self.scc.is_active
    assert self.scc.output_v_target == V_CRUISE_UNSET

  def test_curve_slows(self):
    # sustained yaw rate -> a curve through the horizon -> target below set speed, slowing
    sm = sm_for(yaw_rate=0.07, speed=15.0, curvature=0.0047, v_ego=24.0)
    self.scc.update(sm, True, False, 24.0, 0.0, 30.0)
    assert self.scc.is_active
    assert self.scc.output_v_target < 30.0
    # physics check: v_target ~= sqrt(a_lat_max / kappa) for the sustained curve
    kappa = 0.07 / 15.0
    assert np.isclose(self.scc.v_target, (A_LAT_MAX / kappa) ** 0.5, rtol=0.05)
    assert self.scc.state in (CurveState.slowing, CurveState.curve)

  def test_toggle_off_disables(self):
    self.scc.enabled = False
    sm = sm_for(yaw_rate=0.07, speed=15.0, curvature=0.0047, v_ego=24.0)
    self.scc.update(sm, True, False, 24.0, 0.0, 30.0)
    assert self.scc.state == CurveState.disabled
    assert self.scc.output_v_target == V_CRUISE_UNSET

  def test_long_disabled_inactive(self):
    sm = sm_for(yaw_rate=0.07, speed=15.0, curvature=0.0047, v_ego=24.0)
    self.scc.update(sm, False, False, 24.0, 0.0, 30.0)
    assert not self.scc.is_active


class TestLateralLoadGovernor:
  def setup_method(self):
    Params().put_bool("CurveSpeedControl", True)
    self.gov = LateralLoadGovernor()
    self.gov.enabled = True

  def test_low_load_passthrough(self):
    sm = sm_for(yaw_rate=0.02, speed=20.0, actual=1.0, desired=1.0, v_ego=20.0)
    self.gov.update(sm, True, False, 20.0)
    assert self.gov.output_v_target == V_CRUISE_UNSET
    assert self.gov.throttle_scale() == 1.0

  def test_overload_caps_speed(self):
    over = A_LAT_CEILING * 1.1
    sm = sm_for(yaw_rate=0.05, speed=24.0, actual=over, desired=over, v_ego=24.0)
    for _ in range(20):  # let the EMA converge
      self.gov.update(sm, True, False, 24.0)
    assert self.gov.is_active
    assert self.gov.output_v_target < 24.0

  def test_interlock_zeros_throttle_when_running_wide(self):
    sm = sm_for(yaw_rate=0.05, speed=24.0, saturated=True, actual=2.0, desired=3.0, v_ego=24.0)
    self.gov.update(sm, True, False, 24.0)
    assert self.gov.throttle_scale() == 0.0  # saturated + desired>actual => running wide => no throttle
