"""
Acceleration Control: a fixed, personality-linked lift on the driving model's acceleration.

The driving model does not know the set speed and on some roads asks for barely any acceleration, so
the car settles well under the set speed. openpilot's own Gas Override Boost (accel_boost.py) fixes the
same thing, but only after the driver presses the accelerator, and it starts over on every engagement.
This applies a fixed boost instead, chosen by the driving personality, so it behaves the same on every
drive with no pedal input.

Only the model's candidate is raised. The planner still takes the lowest of the lead, cruise and model
candidates, so the boost can never pass the set speed (cruise candidate) or close on a lead (MPC
candidate). The boost is also withheld entirely when the model predicts a stop (shouldStop).

There are two different inputs here. The raw model request (modelV2.action.desiredAcceleration) is the
model's own acceleration before any override, and it controls only this feature's extra-braking gate.
The planner applies Gas Override Boost to the model's request before it reaches this, so the incoming
post-gas candidate (output_a_target_e2e) can be non-negative even when the raw model request is
negative - a latched gas override can lift a small negative request into positive territory. The
positive taper below is evaluated on that incoming post-gas candidate, not on the raw model request.

The raw-model gate: whenever the raw model request is negative, this feature's extra boost is
suppressed and the incoming post-gas candidate is returned unchanged. That only suppresses this
feature's own boost - the pre-existing Gas Override Boost output is preserved, and no overall planner
or raw-model passthrough is promised.

Two separate rules decide how much boost is applied on top of that gate:

1. Incoming-negative guard (a separate check from the raw-model gate): any negative incoming
   post-gas candidate passes through unchanged with zero boost. There is no fade on the braking side -
   a negative incoming candidate means the model is slowing for a stop, turn, or lead vehicle, and
   that request must not be altered at all.
2. Positive taper: for a non-negative incoming post-gas candidate the boost fades out as the incoming
   candidate grows - full at an incoming candidate of 0, half at 0.05 m/s^2, and none at or above
   0.1 m/s^2 - subject to the non-braking raw model request and the other gates (enabled, engaged,
   not shouldStop, speed ramp).

Note the zero-boundary jump: the taper is full at an incoming candidate of exactly 0 but zero for any
negative incoming candidate, so the applied boost is discontinuous at an incoming candidate of zero.
That is intentional (negative incoming candidates are never modified), but it is a property of the
incoming post-gas candidate, not of the raw model request: a latched gas override means a raw request
of zero does not imply an incoming candidate of zero, so a zero raw request does not promise full
boost. Replay and product acceptance of the zero-boundary behavior is still required.
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

# Positive taper (non-negative requests only): full boost at a request of 0, half at 0.05,
# none at or above 0.1 m/s^2. Negative requests never reach this - see the hard gate below.
BRAKE_FADE_BP = [0.0, 0.1]
BRAKE_FADE_V = [1.0, 0.0]

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

    # The planner applies Gas Override Boost before this, so the incoming candidate can be
    # non-negative even when the model itself is braking. Withhold the boost on any raw model
    # braking request: the post-gas candidate passes through unchanged and no boost is added.
    if sm['modelV2'].action.desiredAcceleration < 0.0:
      self.boost = 0.0
      return output_a_target_e2e

    # same low-speed ramp as Gas Override Boost: none below 10 mph, full by 20 mph
    speed_scale = np.interp(sm['carState'].vEgo, [ACCEL_BOOST_MIN_SPEED, 2 * ACCEL_BOOST_MIN_SPEED], [0.0, 1.0])

    # hard negative gate (not a fade): any negative request passes through unchanged with
    # zero boost - the model is slowing for a stop, turn, or lead vehicle, so its request is
    # never altered. Note the jump: boost is full at exactly 0 and zero for any a < 0.
    if output_a_target_e2e < 0.0:
      self.boost = 0.0
      return output_a_target_e2e

    a_clip = np.clip(output_a_target_e2e, BRAKE_FADE_BP[0], BRAKE_FADE_BP[-1])
    brake_fade = np.interp(a_clip, BRAKE_FADE_BP, BRAKE_FADE_V)
    base = ACCELERATION_CONTROL_BOOST.get(sm['selfdriveState'].personality.raw, ACCELERATION_CONTROL_BOOST[Personality.standard])

    self.boost = float(base * speed_scale * brake_fade)
    return output_a_target_e2e + self.boost
