"""Personality-linked requested lift of the model candidate, before planner min-selection.

Raw model acceleration activates the lift linearly over 0 to 0.1 m/s^2; braking and
zero requests get none. The speed ramp is zero at 10 mph and full at 20 mph.
Gas Override Boost runs first and keeps its own state and output. Competing cruise
and lead candidates and final acceleration limits still constrain the result.

At full speed the combined raw-to-candidate gain in the activation band is 2/3/4
for Relaxed/Standard/Aggressive. This continuous ramp is not a jerk limiter;
recorded replay and product acceptance remain necessary.
"""
import numpy as np

from openpilot.cereal import log
from openpilot.selfdrive.controls.lib.accel_boost import ACCEL_BOOST_MIN_SPEED

Personality = log.LongitudinalPersonality

# m/s^2 added to the model's acceleration request
ACCELERATION_CONTROL_BOOST = {
  Personality.relaxed: 0.1,
  Personality.standard: 0.2,
  Personality.aggressive: 0.3,
}

# Rising activation on the raw model request, independent of the gas override.
ACTIVATION_BP = [0.0, 0.1]
ACTIVATION_V = [0.0, 1.0]

PARAM_REFRESH_FRAMES = 20  # 1 s at the 20 Hz planner rate


class AccelerationControl:
  def __init__(self, CP, params=None):
    self.available = CP.openpilotLongitudinalControl
    if params is None:
      from openpilot.common.params import Params
      params = Params()
    self.params = params
    self.enabled = False
    self.boost = 0.0
    self._frame = 0
    self._refresh()

  def _refresh(self) -> None:
    self.enabled = self.available and self.params.get_bool("AccelerationControl")

  def update(self, sm, output_a_target_e2e: float) -> float:
    if self._frame % PARAM_REFRESH_FRAMES == 0:
      self._refresh()
    self._frame += 1

    self.boost = 0.0
    if not self.enabled or not sm['selfdriveState'].enabled or sm['modelV2'].action.shouldStop:
      return output_a_target_e2e

    # same low-speed ramp as Gas Override Boost: none below 10 mph, full by 20 mph
    speed_scale = np.interp(sm['carState'].vEgo, [ACCEL_BOOST_MIN_SPEED, 2 * ACCEL_BOOST_MIN_SPEED], [0.0, 1.0])

    # Defensive contract for future callers: do not lift a braking incoming candidate.
    # Real AccelBoost adds a nonnegative amount, so positive raw/negative incoming
    # cannot occur on the current planner path.
    if output_a_target_e2e < 0.0:
      return output_a_target_e2e

    activation = np.interp(sm['modelV2'].action.desiredAcceleration, ACTIVATION_BP, ACTIVATION_V)
    base = ACCELERATION_CONTROL_BOOST.get(sm['selfdriveState'].personality.raw, ACCELERATION_CONTROL_BOOST[Personality.standard])

    self.boost = float(base * speed_scale * activation)
    return output_a_target_e2e + self.boost
