"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

Curve-aware longitudinal control (feedforward profile + backward pass).

Unlike SCC-V (which reduces the model's curvature to one 97th-percentile number and reacts with a state machine),
this builds a full speed profile over the distance ahead and back-propagates a comfortable deceleration, so the
car slows *early*, holds a steady speed through the curve, and powers out — keeping lateral accel near a chosen
budget. Vision-only: the model horizon (~100-150 m) is well beyond the ~40 m of lookahead canyon curves need, so
no map dependency. Validated offline in tools/scc_tuner against real route logs.

Pairs with LateralLoadGovernor (lateral_load_governor.py), the reactive backstop that keeps the *measured* load
inside the steering's real limit when this feedforward under-reads a curve.
"""
import numpy as np

import cereal.messaging as messaging
from cereal import custom
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.car.cruise import V_CRUISE_UNSET
from openpilot.selfdrive.modeld.constants import ModelConstants
from openpilot.sunnypilot import PARAMS_UPDATE_PERIOD
from openpilot.sunnypilot.selfdrive.controls.lib.smart_cruise_control import MIN_V

CurveState = custom.LongitudinalPlanSP.SmartCruiseControl.CurveState

T_IDXS = np.array(ModelConstants.T_IDXS)  # 33-pt model horizon, 0..10 s (quadratic spacing)

# Lateral-accel budget the profile plans to. Same value for R1T and R1S (same vehicle family). This is the
# "feel" knob = how hard the car loads the steering in curves; keep it a margin below the EPS lateral ceiling.
# 2.2: dropped 2.4->2.0 on 0000003b for overshoot headroom, then nudged to 2.2 (2026-06-14) — canyon test
# 00000006 showed 2.0 over-slows vs the human's ~4 m/s^2; split the difference (governor still backstops the peak).
A_LAT_MAX = 2.2         # m/s^2
A_DECEL = 1.8           # m/s^2, comfortable decel for the backward pass (braking starts early)
A_ACCEL = 1.2           # m/s^2, comfortable accel-out cap (display a_target only; the MPC does the real tracking)
V_TARGET_FLOOR = 2.0    # m/s, never command a crawl below this
KAPPA_FLOOR = 1e-4      # 1/m, ignore curvature below this (treat as straight)


class CurveSpeedController:
  def __init__(self):
    self.params = Params()
    self.frame = -1
    self.enabled = self.params.get_bool("CurveSpeedControl")

    self.long_enabled = False
    self.long_override = False
    self.is_enabled = False
    self.is_active = False
    self.state = CurveState.disabled

    self.v_ego = 0.
    self.a_ego = 0.
    self.v_cruise_setpoint = V_CRUISE_UNSET
    self.v_target = V_CRUISE_UNSET
    self.a_lat_max = A_LAT_MAX
    self.lat_accel_target = 0.

    self.output_v_target = V_CRUISE_UNSET
    self.output_a_target = 0.

  def _update_params(self) -> None:
    if self.frame % int(PARAMS_UPDATE_PERIOD / DT_MDL) == 0:
      self.enabled = self.params.get_bool("CurveSpeedControl")

  def _plan(self, sm: messaging.SubMaster) -> None:
    """Build kappa(s) ahead from the model, cap speed by a_lat_max, back-propagate a_decel, emit v_target."""
    vel = np.asarray(sm['modelV2'].velocity.x, dtype=np.float64)
    yaw = np.abs(np.asarray(sm['modelV2'].orientationRate.z, dtype=np.float64))
    if len(vel) != len(T_IDXS) or len(yaw) != len(T_IDXS):
      self.v_target = V_CRUISE_UNSET
      return

    # distance to each horizon point: trapezoidal integral of predicted speed over T_IDXS
    s = np.concatenate([[0.0], np.cumsum(0.5 * (vel[:-1] + vel[1:]) * np.diff(T_IDXS))])
    kappa = yaw / np.maximum(vel, 0.1)
    kappa = np.maximum(kappa, KAPPA_FLOOR)

    v_cruise = self.v_cruise_setpoint if self.v_cruise_setpoint > 0 else V_CRUISE_UNSET
    v_allow = np.minimum(np.sqrt(self.a_lat_max / kappa), v_cruise)

    # backward pass (far -> near): only let speed drop as fast as a_decel allows -> brake early, smooth entry
    for i in range(len(v_allow) - 2, -1, -1):
      ds = s[i + 1] - s[i]
      if ds <= 0:
        continue
      v_allow[i] = min(v_allow[i], float(np.sqrt(v_allow[i + 1] ** 2 + 2.0 * A_DECEL * ds)))

    self.v_target = max(float(v_allow[0]), V_TARGET_FLOOR)

  def _update_state(self) -> tuple[bool, bool]:
    v_cruise = self.v_cruise_setpoint if self.v_cruise_setpoint > 0 else V_CRUISE_UNSET
    enabled = self.long_enabled and self.enabled and not self.long_override
    constraining = enabled and self.v_ego > MIN_V and self.v_target < v_cruise - 0.5
    if not self.long_enabled or not self.enabled:
      self.state = CurveState.disabled
    elif self.long_override:
      self.state = CurveState.disabled  # manual override; let the driver have it
    elif not constraining:
      self.state = CurveState.cruise
    elif self.v_target < self.v_ego - 0.5:
      self.state = CurveState.slowing
    else:
      self.state = CurveState.curve
    active = self.state in (CurveState.slowing, CurveState.curve)
    return enabled, active

  def get_v_target_from_control(self) -> float:
    return self.v_target if self.is_active else V_CRUISE_UNSET

  def get_a_target_from_control(self) -> float:
    if self.is_active:
      return float(np.clip((self.v_target - self.v_ego) / 2.0, -A_DECEL, A_ACCEL))
    return self.a_ego

  def update(self, sm: messaging.SubMaster, long_enabled: bool, long_override: bool, v_ego: float, a_ego: float,
             v_cruise: float) -> None:
    self.long_enabled = long_enabled
    self.long_override = long_override
    self.v_ego = v_ego
    self.a_ego = a_ego
    self.v_cruise_setpoint = v_cruise

    self._update_params()
    self.a_lat_max = A_LAT_MAX
    if self.long_enabled and self.enabled:
      self._plan(sm)
    else:
      self.v_target = V_CRUISE_UNSET

    self.is_enabled, self.is_active = self._update_state()
    self.lat_accel_target = self.a_lat_max
    self.output_v_target = self.get_v_target_from_control()
    self.output_a_target = self.get_a_target_from_control()
    self.frame += 1
