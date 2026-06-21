"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

Reactive lateral-load governor — the closed-loop backstop under the curve-speed feedforward.

The feedforward (curve_speed_controller.py) plans from *predicted* curvature; it's blind to what the steering is
actually doing. This governor watches the real lateral state (controlsState.lateralControlState: saturated,
actual/desired lateral accel; carState.yawRate) and:
  1. caps speed to keep the *measured* lateral load at ~99% of the steering's real limit (emits a v_target that
     competes in the planner's min-of-sources) — catches feedforward under-reads / hot entries;
  2. provides a throttle-fade scale so the planner won't add throttle while the steering is near/at its limit
     (the "don't accelerate into a saturated steer" interlock) — hard-zero when actually running wide.

Reactive control can't undo a brief peak mid-curve (a_lat ∝ v²), so the feedforward is the primary edge-holder;
this is the safety backstop. Validated offline in tools/scc_tuner.
"""
import numpy as np

import cereal.messaging as messaging
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.car.cruise import V_CRUISE_UNSET
from openpilot.selfdrive.controls.lib.drive_helpers import MAX_LATERAL_ACCEL_NO_ROLL
from openpilot.sunnypilot import PARAMS_UPDATE_PERIOD
from openpilot.sunnypilot.selfdrive.controls.lib.smart_cruise_control import MIN_V

# The "ceiling" is the steering's usable lateral-accel limit, resolved per the ACTIVE lateral controller each
# frame (see _measure) so one codebase serves both branches (rs-dev torque / rs-angle angle):
#  - TORQUE control: the EPS torque-saturation ceiling, measured ~2.8 m/s^2 for the Rivian R1T from real logs
#    (range 2.8-3.2 across drives) — torque/force is the binding limit and isn't itself logged as a lateral accel.
#  - ANGLE control: openpilot's curvature clip (drive_helpers.MAX_LATERAL_ACCEL_NO_ROLL = 3.0) is the binding
#    limit; the angle controller commands angle and clips desired curvature there, so there's no torque ceiling.
A_LAT_CEILING_TORQUE = 2.8   # m/s^2
SETPOINT = 0.85         # regulate measured load to this fraction of the ceiling. 0.85 (was 0.99): riding right
                        # at the limit overshot it on-device given reactive lag; back off earlier.
TAPER_START = 0.70      # fade feedforward throttle from full (here) to zero at the ceiling — start easing sooner
LOAD_LP = 0.4           # EMA on the load signal (curvature/torque noise rejection)
UNDERSTEER_TH = 0.05    # m/s^2 desired-minus-actual lateral accel that counts as "running wide"
V_TARGET_FLOOR = 2.0    # m/s


class LateralLoadGovernor:
  def __init__(self):
    self.params = Params()
    self.frame = -1
    self.enabled = self.params.get_bool("CurveSpeedControl")  # shares the curve-speed master toggle
    self.ceiling = A_LAT_CEILING_TORQUE   # resolved per active lateral controller each frame in _measure

    self.long_enabled = False
    self.long_override = False
    self.is_active = False
    self.load = 0.
    self.saturated = False
    self.running_wide = False
    self.output_v_target = V_CRUISE_UNSET

  def _update_params(self) -> None:
    if self.frame % int(PARAMS_UPDATE_PERIOD / DT_MDL) == 0:
      self.enabled = self.params.get_bool("CurveSpeedControl")

  def _measure(self, sm: messaging.SubMaster, v_ego: float) -> tuple[float, bool]:
    """Measured lateral load from the lateral controller's ground truth (+ yaw-rate fallback). Also resolves the
    ceiling for the active controller (torque vs angle) and the 'running wide' flag for the interlock."""
    lcs = sm['controlsState'].lateralControlState
    which = lcs.which()
    sub = getattr(lcs, which)
    angle_mode = which == 'angleState'
    self.ceiling = MAX_LATERAL_ACCEL_NO_ROLL if angle_mode else A_LAT_CEILING_TORQUE
    saturated = bool(getattr(sub, 'saturated', False))
    actual = abs(float(getattr(sub, 'actualLateralAccel', 0.0)))
    desired = abs(float(getattr(sub, 'desiredLateralAccel', 0.0)))
    a_lat = max(actual, abs(v_ego * sm['carState'].yawRate))   # what the car actually feels
    # 'running wide' = steering can't hold the line. Torque control logs it as desired > actual lateral accel;
    # angle control has no lateral-accel signal, so its 'saturated' (angle can't track / curvature clipped) IS
    # the running-wide signal.
    self.running_wide = (angle_mode and saturated) or (saturated and (desired - actual) > UNDERSTEER_TH)
    return a_lat, saturated

  def update(self, sm: messaging.SubMaster, long_enabled: bool, long_override: bool, v_ego: float) -> None:
    self.long_enabled = long_enabled
    self.long_override = long_override
    self._update_params()
    self.frame += 1

    if not (self.enabled and long_enabled) or long_override:
      self.is_active = False
      self.output_v_target = V_CRUISE_UNSET
      self.load = 0.
      self.saturated = False
      self.running_wide = False
      return

    a_lat, self.saturated = self._measure(sm, v_ego)
    load = a_lat / max(self.ceiling, 0.1)
    self.load += LOAD_LP * (load - self.load)   # EMA

    if self.load > SETPOINT and v_ego > MIN_V:
      # speed that brings the measured load back to the setpoint (a_lat ∝ v² -> v_cap = v·sqrt(setpoint/load))
      v_cap = v_ego * float(np.sqrt(SETPOINT / max(self.load, 1e-3)))
      self.output_v_target = max(v_cap, V_TARGET_FLOOR)
      self.is_active = True
    else:
      self.output_v_target = V_CRUISE_UNSET
      self.is_active = False

  def throttle_scale(self) -> float:
    """[0,1] multiplier the planner applies to any *positive* a_target: fade throttle as load nears the limit,
    hard-zero while actually running wide. The 'don't accelerate into a saturated steer' interlock."""
    if not (self.enabled and self.long_enabled) or self.long_override:
      return 1.0
    if self.running_wide:
      return 0.0
    return max(0.0, min(1.0, (1.0 - self.load) / max(1.0 - TAPER_START, 1e-3)))
