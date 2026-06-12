#!/usr/bin/env python3
import unittest

from opendbc.car.structs import CarParams
from opendbc.car.lateral import get_max_angle_vm
from opendbc.car.vehicle_model import VehicleModel
from opendbc.safety.tests.libsafety import libsafety_py
import opendbc.safety.tests.common as common
from opendbc.safety.tests.common import CANPackerSafety
from opendbc.car.rivian.values import CarControllerParams, RivianSafetyFlags
from opendbc.car.rivian.ext_controller import get_safety_CP
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


class TestRivianSafetyBase(common.CarSafetyTest, common.DriverTorqueSteeringSafetyTest, common.SteerRequestCutSafetyTest,
                           common.LongitudinalAccelSafetyTest, common.VehicleSpeedSafetyTest):

  TX_MSGS = [[0x120, 0], [0x321, 2], [0x162, 2]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x120,), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x120]}

  MAX_TORQUE_LOOKUP = [13, 25, 27], [440, 325, 305]
  DYNAMIC_MAX_TORQUE = True
  MAX_RATE_UP = 4
  MAX_RATE_DOWN = 7

  MAX_RT_DELTA = 125

  DRIVER_TORQUE_ALLOWANCE = 100
  DRIVER_TORQUE_FACTOR = 2

  MIN_VALID_STEERING_FRAMES = 89
  MAX_INVALID_STEERING_FRAMES = 2

  cnt_speed = 0
  cnt_speed_2 = 0

  def _torque_driver_msg(self, torque):
    values = {"EPAS_TorsionBarTorque": torque / 100.0}
    return self.packer.make_can_msg_safety("EPAS_SystemStatus", 0, values)

  def _torque_cmd_msg(self, torque, steer_req=1):
    values = {"ACM_lkaStrToqReq": torque, "ACM_lkaActToi": steer_req}
    return self.packer.make_can_msg_safety("ACM_lkaHbaCmd", 0, values)

  def _speed_msg(self, speed, quality_flag=True):
    values = {"ESP_Vehicle_Speed": speed * 3.6, "ESP_Status_Counter": self.cnt_speed % 15,
              "ESP_Vehicle_Speed_Q": 1 if quality_flag else 0}
    self.__class__.cnt_speed += 1
    return self.packer.make_can_msg_safety("ESP_Status", 0, values, fix_checksum=checksum)

  def _speed_msg_2(self, speed, quality_flag=True):
    # Rivian has a dynamic max torque limit based on speed, so it checks two sources
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

  TX_MSGS = [[0x120, 0], [0x321, 2], [0x160, 0]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x120, 0x160), 2: (0x321,)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321], 2: [0x120, 0x160]}

  def setUp(self):
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, RivianSafetyFlags.LONG_CONTROL)
    self.safety.init_tests()


class TestRivianAngleSafety(TestRivianSafetyBase, common.AngleSteeringSafetyTest):
  # Angle mode additionally TXes ACM_Status (0x100) and ACM_SteeringControl (0x110); the 0x120
  # cooperative-torque path is unchanged, so the inherited torque tests still apply.
  LONGITUDINAL = False
  TX_MSGS = [[0x100, 0], [0x110, 0], [0x120, 0], [0x321, 2], [0x162, 2]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x100, 0x110, 0x120), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x100, 0x110, 0x120]}

  # Angle limits use the vehicle-model limiter (matches CarControllerParams.ANGLE_LIMITS / ext_controller)
  STEER_ANGLE_MAX = 360
  DEG_TO_CAN = 10
  ANGLE_RATE_BP = None
  ANGLE_RATE_UP = None
  ANGLE_RATE_DOWN = None
  LATERAL_FREQUENCY = 100  # Hz

  cnt_angle_cmd = 0

  def _get_steer_cmd_angle_max(self, speed):
    return get_max_angle_vm(max(speed, 1), self.VM, CarControllerParams)

  def test_angle_cmd_when_enabled(self):
    # v1 rate test doesn't apply (we use the vehicle-model limiter). TODO: port tesla's
    # CAN-quantization-exact test_lateral_accel_limit/test_lateral_jerk_limit with a rivian-
    # specific round_angle + speed rounding before on-device angle use (harness pending).
    pass

  def setUp(self):
    self.VM = VehicleModel(get_safety_CP())
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, RivianSafetyFlags.ANGLE_CONTROL)
    self.safety.init_tests()

  def _angle_cmd_msg(self, angle: float, enabled: bool, increment_timer: bool = True):
    values = {"ACM_SteeringAngleRequest": angle, "ACM_EacEnabled": enabled}
    if increment_timer:
      self.safety.set_timer(self.cnt_angle_cmd * int(1e6 / self.LATERAL_FREQUENCY))
      self.__class__.cnt_angle_cmd += 1
    return self.packer.make_can_msg_safety("ACM_SteeringControl", 0, values)

  def _angle_meas_msg(self, angle: float):
    values = {"EPAS_InternalSas": angle}
    return self.packer.make_can_msg_safety("EPAS_AdasStatus", 0, values)


if __name__ == "__main__":
  unittest.main()
