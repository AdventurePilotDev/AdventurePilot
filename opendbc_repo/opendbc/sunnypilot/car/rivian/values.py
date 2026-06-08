"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
from enum import IntFlag


class RivianFlagsSP(IntFlag):
  LONGITUDINAL_HARNESS_UPGRADE = 1
  # Selects the aggressive R1T steering tune (higher per-speed cap, torque low-pass, faster
  # rate up/down) vs the tame ap-dev baseline. Set at car init from the "RivianAggressiveTune"
  # param; read by the carcontroller to pick a RIVIAN_TUNE profile (opendbc/car/rivian/values.py).
  AGGRESSIVE_TUNE = 2
