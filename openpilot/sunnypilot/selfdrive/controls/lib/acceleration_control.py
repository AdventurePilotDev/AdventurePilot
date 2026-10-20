"""
Acceleration Control: a fixed, personality-linked lift on the driving model's acceleration.

The driving model does not know the set speed and on some roads asks for barely any acceleration, so
the car settles well under the set speed. openpilot's own Gas Override Boost (accel_boost.py) fixes the
same thing, but only after the driver presses the accelerator, and it starts over on every engagement.
This applies a fixed boost instead, chosen by the driving personality, so it behaves the same on every
drive with no pedal input.

Only the model's candidate is raised. The planner still takes the lowest of the lead, cruise and model
candidates, so the boost can never pass the set speed (cruise candidate) or close on a lead (MPC
candidate). It applies only while the model is not slowing: it fades out as soon as the model asks
for any braking or predicts a stop, so the model's own slowdowns for lights, stops and turns pass
through untouched.
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

# Fade the boost out as the model starts to brake: full at a request of 0, none by -0.1 m/s^2.
BRAKE_FADE_BP = [-0.1, 0.0]
BRAKE_FADE_V = [0.0, 1.0]

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
    brake_fade = np.interp(output_a_target_e2e, BRAKE_FADE_BP, BRAKE_FADE_V)
    base = ACCELERATION_CONTROL_BOOST.get(sm['selfdriveState'].personality, ACCELERATION_CONTROL_BOOST[Personality.standard])

    self.boost = float(base * speed_scale * brake_fade)
    return output_a_target_e2e + self.boost
