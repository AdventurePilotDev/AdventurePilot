"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import numpy as np

from openpilot.common.params import Params

# Speed-scheduled lateral curvature smoothing time: `max_seconds` at crawl, fading to 0 by
# LAT_SMOOTH_BP[1]. Driven per-car by CarParams.lateralSmoothSeconds (0 => off, so other cars are
# unchanged). The caller delay-compensates (lat_delay += the returned value), so the smoothing adds
# no net steering lag. Lives here because both modeld entry points need it.
LAT_SMOOTH_BP = [2.0, 8.0]  # m/s


def get_lat_smooth_seconds(v_ego: float, max_seconds: float) -> float:
  return float(np.interp(v_ego, LAT_SMOOTH_BP, [max_seconds, 0.0]))


class ModelStateBase:
  def __init__(self):
    self.lat_delay = Params().get("LagdValueCache", return_default=True)
