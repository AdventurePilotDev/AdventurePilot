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
  # Cooperative steering in angle mode: when set, the driver-override branch applies cooperative
  # torque; when clear, driver input disengages instead. Set from "RivianCoopSteering" (default off
  # for AP). Only meaningful when steerControlType==angle.
  COOP_STEERING = 4
  # Angle-control harness (CAN 0x1310 on bus 1) detected at fingerprint time in _get_params_sp;
  # bridges the harness presence to _initialize_rivian, which decides steerControlType.
  ANGLE_HARNESS = 8
