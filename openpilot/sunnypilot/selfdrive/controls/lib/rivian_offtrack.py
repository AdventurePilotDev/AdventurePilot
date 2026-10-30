"""
Rear-axle off-tracking compensation for the Rivian R1T and R1S, ported from Gerrylum's rx-wb-stg (ce7e5c82cf).

In a turn the rear axle cuts inside the front's path by r - sqrt(r^2 - L^2). This widens openpilot's turn by
gain times that cut-in, so the rear wheels clear the curb the front wheels cleared. A geometric estimate from
the wheelbase, not curb sensing. Off by default.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import math

import numpy as np


def offtrack_curvature(curvature: float, wheelbase: float, gain: float) -> float:
  radius = 1. / abs(curvature) if curvature else math.inf
  if gain <= 0. or not wheelbase < radius < math.inf:
    return curvature
  return math.copysign(1. / (radius + gain * (radius - math.sqrt(radius ** 2 - wheelbase ** 2))), curvature)


class RivianOfftrack:
  def __init__(self, CP, params):
    # Takes the caller's Params, as lane centering does, so this stays testable off-device
    self.params = params
    self.wheelbase = CP.wheelbase  # the VIN-aware figure: 3.45 m on an R1T, 3.08 m otherwise
    self.is_rivian = CP.brand == 'rivian'
    self.gain = 0.0

  def get_params(self) -> None:
    if not self.is_rivian or not self.params.get_bool("RivianOfftrackCompensationEnabled"):
      self.gain = 0.0
      return
    gain = self.params.get("RivianOfftrackCompensationGain", return_default=True)
    self.gain = float(np.clip(gain, 0.0, 1.0))

  def update(self, desired_curvature: float, lat_active: bool) -> float:
    if not lat_active:
      return desired_curvature
    return offtrack_curvature(desired_curvature, self.wheelbase, self.gain)
