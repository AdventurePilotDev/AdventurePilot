"""
Lane centering: nudge the model's desired curvature so the car tracks the middle of the
painted lane instead of wherever the model's own path happens to fall.

Ported from StarPilot (firestar5683/StarPilot, commits 9f1066ce8, 3aa1436ff, eac56eea2).

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import math

import numpy as np

from openpilot.cereal import log
from openpilot.common.realtime import DT_CTRL
from openpilot.selfdrive.controls.lib.drive_helpers import smooth_value

LaneChangeState = log.LaneChangeState

MIN_V_EGO = 5.0                # m/s, below this the lane lines are not worth trusting
MIN_LANE_PROB = 0.6            # both lines must be at least this likely to be real
MAX_LANE_STD = 0.3             # m, and no more uncertain than this
MIN_LANE_WIDTH = 2.6           # m, rejects merges, splits and junk detections
MAX_LANE_WIDTH = 4.8           # m
MAX_OFFSET = 0.3               # m, largest deliberate offset from center the driver can ask for
MIN_CENTER_TO_LINE = 1.1       # m, never aim closer than this to either line
MAX_RAW_CORRECTION = 0.004     # 1/m, before the gain
DEFAULT_GAIN = 0.60            # fraction of the geometric demand actually applied, settled by road testing
MAX_GAIN = 1.0                 # hard ceiling on that fraction, so the nudge can never exceed MAX_RAW_CORRECTION
MAX_CENTER_ERROR_DEADBAND = 0.15  # m, ceiling on the deadband below
SMOOTH_TAU = 0.4               # s, how fast the correction is allowed to build
RELEASE_TAU = 0.2              # s, how fast it fades when we stop centering
CENTER_ERROR_DEADBAND = 0.02   # m, default for the deadband param: errors smaller produce nothing at all
LOOKAHEAD_MIN = 8.0            # m
LOOKAHEAD_MAX = 35.0           # m

# How much authority a confident model keeps over a large, deliberate departure from lane center.
# 1.0 defers to it fully, 0.0 always centers. Driver adjustable.
E2E_MAX_PATH_STD = 0.35        # m, above this the model is not confident enough to be deferred to
E2E_BREAK_IN_START = 0.15      # m of error where the model starts getting its way
E2E_BREAK_IN_FULL = 0.50       # m of error where it gets all of it


class LaneCenteringController:
  def __init__(self, params):
    # Takes the caller's Params rather than making its own, so this module stays importable
    # off-device and the controller can be unit tested without the compiled params extension
    self.params = params
    self.enabled = self.params.get_bool("LaneCenteringEnabled")
    self.offset = 0.0
    self.e2e_authority = 1.0
    self.gain = DEFAULT_GAIN
    self.deadband = CENTER_ERROR_DEADBAND
    self.correction = 0.0
    # Reported to the on-screen indicator. Nothing here feeds back into the control path.
    self.active = False
    self.holding = False
    self.center_error = 0.0
    self.clipped = False

  def get_params(self) -> None:
    self.enabled = self.params.get_bool("LaneCenteringEnabled")
    offset = self.params.get("LaneCenteringOffset", return_default=True)
    self.offset = float(np.clip(offset, -MAX_OFFSET, MAX_OFFSET))
    authority = self.params.get("LaneCenteringE2EAuthority", return_default=True)
    self.e2e_authority = float(np.clip(authority, 0.0, 1.0))
    # Driver adjustable, and kept that way, for the same reason as the deadband below.
    gain = self.params.get("LaneCenteringGain", return_default=True)
    self.gain = float(np.clip(gain, 0.0, MAX_GAIN))
    # Driver adjustable, and kept that way: the tolerance that suits a road turns out to depend on
    # the road, and comparing values needs them changeable between legs of a single drive.
    deadband = self.params.get("LaneCenteringDeadband", return_default=True)
    self.deadband = float(np.clip(deadband, 0.0, MAX_CENTER_ERROR_DEADBAND))

  def reset(self) -> None:
    self.correction = 0.0
    self._clear_telemetry()

  def _clear_telemetry(self) -> None:
    self.active = False
    self.holding = False
    self.center_error = 0.0
    self.clipped = False

  def _release(self) -> float:
    """Fade whatever correction is applied back to zero instead of dropping it in one frame."""
    self.correction = float(smooth_value(0.0, self.correction, RELEASE_TAU, dt=DT_CTRL))
    return self.correction

  def update(self, desired_curvature: float, model_v2, v_ego: float, blinker_on: bool,
             lat_active: bool, model_valid: bool) -> float:
    # Nothing is being applied when lateral is off or the model is bad, so there is nothing to fade
    if not lat_active or not model_valid or not math.isfinite(v_ego):
      self.reset()
      return desired_curvature

    self._clear_telemetry()

    if not self.enabled or v_ego < MIN_V_EGO:
      return desired_curvature + self._release()

    # Stay out of the way of a lane change, and of the driver signalling one
    if blinker_on or model_v2.meta.laneChangeState != LaneChangeState.off:
      return desired_curvature + self._release()

    valid, raw_correction = self._raw_correction(model_v2, v_ego)
    if not valid:
      self._clear_telemetry()
      return desired_curvature + self._release()

    self.active = True
    self.clipped = abs(raw_correction) >= MAX_RAW_CORRECTION
    target = float(np.clip(raw_correction, -MAX_RAW_CORRECTION, MAX_RAW_CORRECTION)) * self.gain
    self.correction = float(smooth_value(target, self.correction, SMOOTH_TAU, dt=DT_CTRL))
    return desired_curvature + self.correction

  @staticmethod
  def _valid_path(x, y) -> bool:
    return x.size >= 2 and x.size == y.size and np.isfinite(x).all() and np.isfinite(y).all() and np.all(np.diff(x) > 0)

  @staticmethod
  def _covers(x, distance: float) -> bool:
    return bool(x[0] <= distance <= x[-1])

  def _raw_correction(self, model_v2, v_ego: float) -> tuple[bool, float]:
    """The curvature that would close the gap to lane center over the lookahead distance."""
    try:
      lane_lines = model_v2.laneLines
      probs = np.asarray(model_v2.laneLineProbs, dtype=float)
      stds = np.asarray(model_v2.laneLineStds, dtype=float)
      if len(lane_lines) < 3 or probs.size < 3 or stds.size < 3:
        return False, 0.0
      if not np.isfinite(probs[[1, 2]]).all() or not np.isfinite(stds[[1, 2]]).all():
        return False, 0.0
      if np.any(probs[[1, 2]] < MIN_LANE_PROB) or np.any(probs[[1, 2]] > 1.0):
        return False, 0.0
      if np.any(stds[[1, 2]] < 0.0) or np.any(stds[[1, 2]] > MAX_LANE_STD):
        return False, 0.0

      left_x = np.asarray(lane_lines[1].x, dtype=float)
      left_y = np.asarray(lane_lines[1].y, dtype=float)
      right_x = np.asarray(lane_lines[2].x, dtype=float)
      right_y = np.asarray(lane_lines[2].y, dtype=float)
      pos_x = np.asarray(model_v2.position.x, dtype=float)
      pos_y = np.asarray(model_v2.position.y, dtype=float)
      if not (self._valid_path(left_x, left_y) and self._valid_path(right_x, right_y) and self._valid_path(pos_x, pos_y)):
        return False, 0.0

      lookahead = float(np.clip(v_ego, LOOKAHEAD_MIN, LOOKAHEAD_MAX))
      if not all(self._covers(x, lookahead) for x in (left_x, right_x, pos_x)):
        return False, 0.0

      left = float(np.interp(lookahead, left_x, left_y))
      right = float(np.interp(lookahead, right_x, right_y))
      width = right - left
      if not MIN_LANE_WIDTH <= width <= MAX_LANE_WIDTH:
        return False, 0.0

      # Never let the requested offset aim us closer to a line than MIN_CENTER_TO_LINE
      max_safe_offset = min(MAX_OFFSET, max(0.0, width * 0.5 - MIN_CENTER_TO_LINE))
      target_y = 0.5 * (left + right) + float(np.clip(self.offset, -max_safe_offset, max_safe_offset))
      error = target_y - float(np.interp(lookahead, pos_x, pos_y))

      error_abs = abs(error)
      # Reported before the deadband is taken out, so the indicator shows how far off the
      # aim point the car actually is rather than how much of that is being acted on
      self.center_error = error

      if error_abs <= self.deadband:
        self.holding = True
        return True, 0.0
      error = math.copysign(error_abs - self.deadband, error)

      # A confident model that is deliberately well off center is probably avoiding something,
      # so give it its way in proportion to how far off center it has chosen to be
      pos_y_std = np.asarray(model_v2.position.yStd, dtype=float)
      if self._valid_path(pos_x, pos_y_std):
        path_std = float(np.interp(lookahead, pos_x, pos_y_std))
        if 0.0 <= path_std <= E2E_MAX_PATH_STD:
          break_in = np.clip((error_abs - E2E_BREAK_IN_START) / (E2E_BREAK_IN_FULL - E2E_BREAK_IN_START), 0.0, 1.0)
          error *= 1.0 - self.e2e_authority * float(break_in)

      correction = 2.0 * error / lookahead ** 2
      if not math.isfinite(correction):
        return False, 0.0
      return True, float(correction)
    except (AttributeError, IndexError, TypeError, ValueError):
      return False, 0.0
