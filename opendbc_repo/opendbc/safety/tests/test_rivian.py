#!/usr/bin/env python3
import unittest
import numpy as np

from opendbc.car.lateral import get_max_angle_delta_vm, get_max_angle_vm
from opendbc.car.rivian.carcontroller import get_safety_CP
from opendbc.car.rivian.values import CarControllerParams, RivianSafetyFlags
from opendbc.car.rivian.riviancan import checksum as _checksum
from opendbc.car.structs import CarParams
from opendbc.car.vehicle_model import VehicleModel
from opendbc.safety.tests.libsafety import libsafety_py
import opendbc.safety.tests.common as common
from opendbc.safety.tests.common import CANPackerSafety


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
  """Torque-lateral config (vanilla Rivian-A / longitudinal harness): the angle channel
  (0x110 stream + 0x100 status spoof) is fully locked without ANGLE_CONTROL."""

  TX_MSGS = [[0x120, 0], [0x321, 2], [0x162, 2]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x120,), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x120]}

  SAFETY_PARAM = 0
  LONGITUDINAL = False

  # Torque limits (dev tune envelope)
  MAX_TORQUE_LOOKUP = [9, 25, 27], [385, 295, 275]
  DYNAMIC_MAX_TORQUE = True
  MAX_RATE_UP = 3
  MAX_RATE_DOWN = 5
  MAX_RT_DELTA = 125
  DRIVER_TORQUE_ALLOWANCE = 100
  DRIVER_TORQUE_FACTOR = 2

  # 2-frame TOI blip tolerance
  MIN_VALID_STEERING_FRAMES = 89
  MAX_INVALID_STEERING_FRAMES = 2

  cnt_speed = 0
  cnt_speed_2 = 0

  @classmethod
  def setUpClass(cls):
    if cls.__name__ == "TestRivianSafetyBase":
      cls.safety = None
      raise unittest.SkipTest
    super().setUpClass()

  def setUp(self):
    self.VM = VehicleModel(get_safety_CP())
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, int(self.SAFETY_PARAM))
    self.safety.init_tests()

  def _torque_driver_msg(self, torque):
    values = {"EPAS_TorsionBarTorque": torque / 100.0}
    return self.packer.make_can_msg_safety("EPAS_SystemStatus", 0, values)

  def _torque_cmd_msg(self, torque, steer_req=1):
    values = {"ACM_lkaStrToqReq": torque, "ACM_lkaActToi": steer_req}
    return self.packer.make_can_msg_safety("ACM_lkaHbaCmd", 0, values)

  def _angle_cmd_msg(self, angle: float, enabled: bool, bus: int = 0):
    values = {"ACM_SteeringAngleRequest": angle, "ACM_EacEnabled": enabled}
    return self.packer.make_can_msg_safety("ACM_SteeringControl", bus, values)

  def _acm_status_msg(self, feature_status: int, bus: int = 0):
    values = {"ACM_FeatureStatus": feature_status}
    return self.packer.make_can_msg_safety("ACM_Status", bus, values)

  def _angle_meas_msg(self, angle: float):
    values = {"EPAS_InternalSas": angle}
    return self.packer.make_can_msg_safety("EPAS_AdasStatus", 0, values)

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

  def test_angle_tx_locked_without_flag(self):
    """Without ANGLE_CONTROL the angle stream and status spoof are rejected regardless of
    content or controls state — a torque-config truck can never actuate the angle channel."""
    if int(self.SAFETY_PARAM) & int(RivianSafetyFlags.ANGLE_CONTROL):
      raise unittest.SkipTest("angle configs allow these by design")
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      for enabled in (True, False):
        self.assertFalse(self._tx(self._angle_cmd_msg(0, enabled)))
      for feature_status in (0, 1, 2):
        self.assertFalse(self._tx(self._acm_status_msg(feature_status)))

  def test_toi_blip_freeze_resume(self):
    """The carcontroller freezes its rate-limiter memory through the 2-frame TOI blip and
    resumes at the pre-blip torque. The panda holds last torque through a tolerated
    steer_req cut, so the instant resume must pass rate checks (dev-shipped behavior)."""
    self.safety.init_tests()
    self.safety.set_timer(self.MIN_VALID_STEERING_RT_INTERVAL)
    self.safety.set_controls_allowed(True)
    self._set_prev_torque(self.MAX_TORQUE)
    for _ in range(self.MIN_VALID_STEERING_FRAMES):
      self.assertTrue(self._tx(self._torque_cmd_msg(self.MAX_TORQUE, steer_req=1)))

    # blip: torque and TOI drop for the tolerated frames
    for _ in range(self.MAX_INVALID_STEERING_FRAMES):
      self.assertTrue(self._tx(self._torque_cmd_msg(0, steer_req=0)))

    # instant resume at the pre-blip value
    self.assertTrue(self._tx(self._torque_cmd_msg(self.MAX_TORQUE, steer_req=1)))

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


class TestRivianStockSafety(TestRivianSafetyBase):

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

  SAFETY_PARAM = RivianSafetyFlags.LONG_CONTROL
  LONGITUDINAL = True


class TestRivianAngleSafetyBase(TestRivianSafetyBase, common.AngleSteeringSafetyTest):
  """Angle-harness configs (xnor extreme box / dual-intercept): ANGLE_CONTROL unlocks the
  0x110 angle stream + 0x100 status spoof on bus 0, bounded by the VM angle checks."""

  TX_MSGS = [[0x100, 0], [0x110, 0], [0x120, 0], [0x321, 2], [0x162, 2]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x100, 0x110, 0x120), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x100, 0x110, 0x120]}

  SAFETY_PARAM = RivianSafetyFlags.ANGLE_CONTROL

  # Angle limits (VM-based, no simple breakpoint rates)
  STEER_ANGLE_MAX = 500
  DEG_TO_CAN = 10
  ANGLE_RATE_BP = None
  ANGLE_RATE_UP = None
  ANGLE_RATE_DOWN = None
  LATERAL_FREQUENCY = 100

  cnt_angle_cmd = 0

  @classmethod
  def setUpClass(cls):
    if cls.__name__ == "TestRivianAngleSafetyBase":
      cls.safety = None
      raise unittest.SkipTest
    super().setUpClass()

  def _angle_cmd_msg(self, angle: float, enabled: bool, bus: int = 0, increment_timer: bool = True):
    if increment_timer:
      self.safety.set_timer(self.cnt_angle_cmd * int(1e6 / self.LATERAL_FREQUENCY))
      self.__class__.cnt_angle_cmd += 1
    values = {"ACM_SteeringAngleRequest": angle, "ACM_EacEnabled": enabled}
    return self.packer.make_can_msg_safety("ACM_SteeringControl", bus, values)

  def _get_steer_cmd_angle_max(self, speed):
    return get_max_angle_vm(max(speed, 1), self.VM, CarControllerParams)

  def test_angle_cmd_when_enabled(self):
    # VM-based limits tested in test_lateral_accel_limit and test_lateral_jerk_limit
    pass

  def _can_to_deg(self, can_val):
    return can_val / self.DEG_TO_CAN

  @staticmethod
  def _round_speed(speed):
    """Round speed through CAN encoding to match what safety computes after fudge"""
    speed_kph_can = round((speed + 1) * 3.6 / 0.01) * 0.01
    stored = round(speed_kph_can / 3.6 * 1000)
    return max(stored / 1000.0 - 1.0, 1.0)

  def test_lateral_accel_limit(self):
    for speed in np.linspace(0, 40, 100):
      speed = max(self._round_speed(speed), 1)
      for sign in (-1, 1):
        self.safety.set_controls_allowed(True)
        self._reset_speed_measurement(speed + 1)

        # safety: max_angle_can = (max_angle_deg * DEG_TO_CAN) + 1
        max_angle_can = int(get_max_angle_vm(speed, self.VM, CarControllerParams) * self.DEG_TO_CAN) + 1
        max_angle_can = min(max_angle_can, self.STEER_ANGLE_MAX * self.DEG_TO_CAN)

        # at limit
        self.safety.set_desired_angle_last(max_angle_can * sign)
        self.assertTrue(self._tx(self._angle_cmd_msg(self._can_to_deg(max_angle_can) * sign, True)))

        # 1 unit above limit
        above_can = max_angle_can + 1
        above_deg = self._can_to_deg(above_can) * sign
        self._tx(self._angle_cmd_msg(above_deg, True))
        should_tx = above_can > self.STEER_ANGLE_MAX * self.DEG_TO_CAN
        self.assertEqual(should_tx, self._tx(self._angle_cmd_msg(above_deg, True)))

  def test_lateral_jerk_limit(self):
    for speed in np.linspace(0, 40, 100):
      speed = max(self._round_speed(speed), 1)
      for sign in (-1, 1):
        self.safety.set_controls_allowed(True)
        self._reset_speed_measurement(speed + 1)
        self._tx(self._angle_cmd_msg(0, True))

        # safety: max_delta_can = (max_delta_deg * DEG_TO_CAN) + 1
        max_delta_can = int(get_max_angle_delta_vm(speed, self.VM, CarControllerParams) * self.DEG_TO_CAN) + 1

        # within limits
        self.assertTrue(self._tx(self._angle_cmd_msg(self._can_to_deg(max_delta_can) * sign, True)))
        self.assertTrue(self._tx(self._angle_cmd_msg(self._can_to_deg(max_delta_can) * sign, True)))
        self.assertTrue(self._tx(self._angle_cmd_msg(0, True)))

        # too high rate
        above_can = max_delta_can + 1
        self.assertFalse(self._tx(self._angle_cmd_msg(self._can_to_deg(above_can) * sign, True)))

        # recover
        self.safety.set_desired_angle_last(round(self._can_to_deg(above_can) * sign * self.DEG_TO_CAN))
        self.assertTrue(self._tx(self._angle_cmd_msg(self._can_to_deg(above_can) * sign, True)))
        self.assertFalse(self._tx(self._angle_cmd_msg(0, True)))
        self.assertTrue(self._tx(self._angle_cmd_msg(0, True)))

  def test_angle_tx_bus_restriction(self):
    """Even with ANGLE_CONTROL, 0x110/0x100 must never TX on bus 2, where the stock
    0x100 is RX'd as the cruise-state source (the spoof can't masquerade as it)."""
    self.safety.set_controls_allowed(True)
    self.assertFalse(self._tx(self._angle_cmd_msg(0, True, bus=2, increment_timer=False)))
    self.assertFalse(self._tx(self._acm_status_msg(2, bus=2)))


class TestRivianAngleSafety(TestRivianAngleSafetyBase):

  def test_adas_status(self):
    # For canceling stock ACC
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      for interface_status in range(4):
        values = {"VDM_AdasInterfaceStatus": interface_status}
        self.assertTrue(self._tx(self.packer.make_can_msg_safety("VDM_AdasSts", 2, values)))


class TestRivianAngleLongitudinalSafety(TestRivianAngleSafetyBase):

  TX_MSGS = [[0x100, 0], [0x110, 0], [0x120, 0], [0x321, 2], [0x160, 0]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x100, 0x110, 0x120, 0x160), 2: (0x321,)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321], 2: [0x100, 0x110, 0x120, 0x160]}

  SAFETY_PARAM = RivianSafetyFlags.ANGLE_CONTROL | RivianSafetyFlags.LONG_CONTROL
  LONGITUDINAL = True


class TestRivianSecondaryTx(unittest.TestCase):
  """Ext intercept panda (dual-intercept harness): minimal 0x110/0x100 mirror filter.
  The int panda is the safety gatekeeper (EPAS 2-of-2 voting); the ext panda must stay
  frame-identical to it, so it runs no per-panda-state checks that could desynchronize."""

  # for common's cross-mode TX checker
  TX_MSGS = [[0x110, 0], [0x100, 0]]

  def setUp(self):
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, int(RivianSafetyFlags.SECONDARY_TX))
    self.safety.init_tests()
    self.cnt = 0

  def _tx(self, msg):
    return self.safety.safety_tx_hook(msg)

  def _angle_cmd(self, angle, enabled, step_timer=True):
    if step_timer:
      self.safety.set_timer(self.cnt * 10000)  # 100 Hz cadence, usec
      self.cnt += 1
    values = {"ACM_SteeringAngleRequest": angle, "ACM_EacEnabled": enabled}
    return self.packer.make_can_msg_safety("ACM_SteeringControl", 0, values)

  def _acm_status(self, feature_status):
    return self.packer.make_can_msg_safety("ACM_Status", 0, {"ACM_FeatureStatus": feature_status})

  def test_mirror_allowlist(self):
    # only the angle stream + status mirror pass; every other TX is rejected,
    # regardless of controls state (the int panda gates engagement)
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      self.assertTrue(self._tx(self._angle_cmd(0, True)))
      self.assertTrue(self._tx(self._acm_status(2)))
      self.assertFalse(self._tx(self.packer.make_can_msg_safety("ACM_lkaHbaCmd", 0, {"ACM_lkaStrToqReq": 0})))
      self.assertFalse(self._tx(self.packer.make_can_msg_safety("ACM_longitudinalRequest", 0, {"ACM_AccelerationRequest": 0})))
      self.assertFalse(self._tx(self.packer.make_can_msg_safety("SCCM_WheelTouch", 2, {"SETME_X52": 100})))
      self.assertFalse(self._tx(self.packer.make_can_msg_safety("VDM_AdasSts", 2, {"VDM_AdasInterfaceStatus": 1})))

  def test_out_of_range_bound(self):
    # the mirror runs only a max-angle sanity bound: large frame-to-frame jumps within
    # +/-500 deg pass (no per-panda rate state), beyond the bound is rejected
    for angle, should_tx in ((0, True), (499.9, True), (-499.9, True), (520, False), (-520, False)):
      self.assertEqual(should_tx, self._tx(self._angle_cmd(angle, True)), f"angle {angle}")

  def test_inactive_not_rate_capped(self):
    # an EacEnabled=0 flood must pass: the int panda's rate cap lives inside the active
    # branch, so capping inactive frames here would diverge the two mirrored streams
    self.safety.init_tests()
    self.safety.set_timer(0)
    for _ in range(100):
      self.assertTrue(self._tx(self._angle_cmd(0, False, step_timer=False)))

  def test_active_rt_frequency_cap(self):
    # active frames flooded at one timestamp blow the frequency budget and get rejected
    self.safety.init_tests()
    self.safety.set_timer(0)
    results = [self._tx(self._angle_cmd(0, True, step_timer=False)) for _ in range(100)]
    self.assertTrue(results[0])
    self.assertFalse(results[-1])


class TestRivianIgnition(unittest.TestCase):
  TX_MSGS: list = []

  def setUp(self):
    self.safety = libsafety_py.libsafety
    self.safety.init_tests()
    self.packer = CANPackerSafety("rivian_primary_actuator")

  def _msg(self, counter, mode):
    return self.packer.make_can_msg_safety("VDM_OutputSignals", 0,
                                           {"VDM_OutputSigs_Counter": counter,
                                            "VDM_EpasPowerMode": mode})

  # VDM_EpasPowerMode_Drive_On=1
  def test_ignition_on(self):
    for i in range(15):
      self.safety.init_tests()
      self.safety.ignition_can_hook(self._msg(i, 1))
      self.assertFalse(self.safety.get_ignition_can())
      self.safety.ignition_can_hook(self._msg((i + 1) % 15, 1))
      self.assertTrue(self.safety.get_ignition_can())

  def test_ignition_off(self):
    self.safety.ignition_can_hook(self._msg(0, 1))
    self.safety.ignition_can_hook(self._msg(1, 1))
    self.assertTrue(self.safety.get_ignition_can())
    self.safety.ignition_can_hook(self._msg(2, 0))
    self.safety.ignition_can_hook(self._msg(3, 0))
    self.assertFalse(self.safety.get_ignition_can())


if __name__ == "__main__":
  unittest.main()
