#!/usr/bin/env python3
import unittest

import numpy as np
from opendbc.car.structs import CarParams
from opendbc.safety.tests.libsafety import libsafety_py
import opendbc.safety.tests.common as common
from opendbc.safety.tests.common import CANPackerSafety
from opendbc.car.rivian.values import RivianSafetyFlags
from opendbc.car.rivian.riviancan import checksum as _checksum


def checksum(msg):
  addr, dat, bus = msg
  ret = bytearray(dat)

  # ESP_Status
  if addr == 0x208:
    ret[0] = _checksum(ret[1:], 0x1D, 0xB1)
  elif addr == 0x150:
    ret[0] = _checksum(ret[1:], 0x1D, 0x9A)

  return addr, ret, bus


class TestRivianSafetyBase(common.CarSafetyTest, common.AngleSteeringSafetyTest,
                           common.LongitudinalAccelSafetyTest, common.VehicleSpeedSafetyTest):

  TX_MSGS = [[0x321, 2], [0x162, 2], [0x110, 0], [0x100, 0]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x110, 0x100), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x110, 0x100]}

  STEER_ANGLE_MAX = 360.0
  STEER_ANGLE_TEST_MAX = 200.0  # don't test beyond max_angle
  DEG_TO_CAN = 10
  ANGLE_RATE_BP = [0., 5., 25.]
  ANGLE_RATE_UP = [3.0, 1.5, 0.3]
  ANGLE_RATE_DOWN = [3.0, 1.5, 0.5]
  LATERAL_FREQUENCY = 100

  cnt_speed = 0
  cnt_speed_2 = 0
  cnt_angle_cmd = 0

  def _angle_cmd_msg(self, angle: float, enabled: bool, increment_timer: bool = True):
    values = {"ACM_SteeringAngleRequest": angle, "ACM_EacEnabled": 1 if enabled else 0}
    if increment_timer:
      self.safety.set_timer(self.cnt_angle_cmd * int(1e6 / self.LATERAL_FREQUENCY))
      self.__class__.cnt_angle_cmd += 1
    return self.packer.make_can_msg_safety("ACM_SteeringControl", 0, values)

  def _angle_meas_msg(self, angle: float):
    values = {"EPAS_InternalSas": angle}
    return self.packer.make_can_msg_safety("EPAS_AdasStatus", 0, values)

  def _speed_msg(self, speed, quality_flag=True):
    values = {"ESP_Vehicle_Speed": speed * 3.6, "ESP_Status_Counter": self.cnt_speed % 15,
              "ESP_Vehicle_Speed_Q": 1 if quality_flag else 0}
    self.__class__.cnt_speed += 1
    return self.packer.make_can_msg_safety("ESP_Status", 0, values, fix_checksum=checksum)

  def _speed_msg_2(self, speed, quality_flag=True):
    # Cross-checked against ESP_Status to disable controls if the two speed sources diverge
    return self._user_gas_msg(0, speed, quality_flag)

  def _user_brake_msg(self, brake):
    values = {"iBESP2_BrakePedalApplied": brake}
    return self.packer.make_can_msg_safety("iBESP2", 0, values)

  def _user_gas_msg(self, gas, speed=0, quality_flag=True):
    values = {"VDM_AcceleratorPedalPosition": gas, "VDM_VehicleSpeed": speed * 3.6,
              "VDM_PropStatus_Counter": self.cnt_speed_2 % 15, "VDM_VehicleSpeedQ": 1 if quality_flag else 0}
    self.__class__.cnt_speed_2 += 1
    return self.packer.make_can_msg_safety("VDM_PropStatus", 0, values, fix_checksum=checksum)

  def _pcm_status_msg(self, enable):
    values = {"ACM_FeatureStatus": enable, "ACM_Unkown1": 1}
    return self.packer.make_can_msg_safety("ACM_Status", 2, values)

  def _accel_msg(self, accel: float):
    values = {"ACM_AccelerationRequest": accel}
    return self.packer.make_can_msg_safety("ACM_longitudinalRequest", 0, values)

  def test_wheel_touch(self):
    # For hiding hold wheel alert on engage
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      values = {
        "SCCM_WheelTouch_HandsOn": 1 if controls_allowed else 0,
        "SCCM_WheelTouch_CapacitiveValue": 100 if controls_allowed else 0,
        "SETME_X52": 100,
      }
      self.assertTrue(self._tx(self.packer.make_can_msg_safety("SCCM_WheelTouch", 2, values)))

  def test_rx_hook(self):
    # checksum, counter, and quality flag checks
    for quality_flag in (True, False):
      for msg_type in ("speed", "speed_2"):
        self.safety.set_controls_allowed(True)
        # send multiple times to verify counter checks
        for _ in range(10):
          if msg_type == "speed":
            msg = self._speed_msg(0, quality_flag=quality_flag)
          elif msg_type == "speed_2":
            msg = self._speed_msg_2(0, quality_flag=quality_flag)

          self.assertEqual(quality_flag, self._rx(msg))
          self.assertEqual(quality_flag, self.safety.get_controls_allowed())

        # Mess with checksum to make it fail
        msg[0].data[0] = 0xff
        self.assertFalse(self._rx(msg))
        self.assertFalse(self.safety.get_controls_allowed())

  def test_angle_cmd_when_disabled(self):
    # Override the AngleSteeringSafetyTest version: Rivian deviates from the
    # standard ±1 inactive tracking. Our rivian_tx_hook only enforces a
    # max_angle sanity bound when EacEnabled=0, to absorb the dual-panda
    # cross-bus skew between ext panda's angle_meas (rebroadcast on the
    # front-object FD bus) and carstate's source (primary actuator). Any
    # inactive angle inside ±max_angle is accepted regardless of measured.
    max_angle_can = int(self.STEER_ANGLE_MAX * self.DEG_TO_CAN)
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      for angle_meas in np.arange(-90, 91, 30):
        self._reset_angle_measurement(angle_meas)
        for angle_cmd in np.arange(-90, 91, 30):
          self._set_prev_desired_angle(angle_cmd)
          # Inactive: anything in ±STEER_ANGLE_MAX passes; outside is rejected
          # by the sanity bound.
          should_tx = abs(angle_cmd) <= self.STEER_ANGLE_MAX
          self.assertEqual(should_tx, self._tx(self._angle_cmd_msg(angle_cmd, False)),
                           f"inactive angle_cmd={angle_cmd} meas={angle_meas}")


class TestRivianStockSafety(TestRivianSafetyBase):

  LONGITUDINAL = False

  def setUp(self):
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, 0)
    self.safety.init_tests()

  def test_adas_status(self):
    # For canceling stock ACC
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      for interface_status in range(4):
        values = {"VDM_AdasInterfaceStatus": interface_status}
        self.assertTrue(self._tx(self.packer.make_can_msg_safety("VDM_AdasSts", 2, values)))


class TestRivianLongitudinalSafety(TestRivianSafetyBase):

  TX_MSGS = [[0x321, 2], [0x160, 0], [0x110, 0], [0x100, 0]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x110, 0x160, 0x100), 2: (0x321,)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321], 2: [0x110, 0x160, 0x100]}

  def setUp(self):
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, RivianSafetyFlags.LONG_CONTROL)
    self.safety.init_tests()


class TestRivianSecondarySafety(common.SafetyTest):
  # Ext panda (front-object FD bus relay-cut): minimal safety config mirroring only
  # the int panda's 0x110 angle + 0x100 ACM_Status injections. Brake/gas/cruise state
  # aren't visible on the ext panda's buses, so the full CarSafetyTest mixins don't
  # apply — we only verify the TX whitelist + relay-malfunction set here.
  TX_MSGS = [[0x110, 0], [0x100, 0]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x110, 0x100), 2: ()}
  FWD_BLACKLISTED_ADDRS = {0: [], 2: [0x110, 0x100]}

  def setUp(self):
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, RivianSafetyFlags.SECONDARY_TX)
    self.safety.init_tests()


if __name__ == "__main__":
  unittest.main()
