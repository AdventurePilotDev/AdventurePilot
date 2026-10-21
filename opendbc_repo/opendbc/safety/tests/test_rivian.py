#!/usr/bin/env python3
import unittest
import numpy as np

from opendbc.car.lateral import get_max_angle_delta_vm, get_max_angle_vm
from opendbc.car.rivian.carcontroller import get_safety_CP
from types import SimpleNamespace
from opendbc.car.rivian.ext_controller import ExternalController, TORQUE_PREARM_ABORT_LOCKOUT, TORQUE_PREARM_MAX_FRAMES
from opendbc.car.rivian.interface import CarInterface
from opendbc.car.rivian.values import CAR, CarControllerParams, RivianSafetyFlags
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
  elif addr == 0x162:
    ret[0] = _checksum(ret[1:], 0x1D, 0xD1)
  elif addr == 0x162:
    ret[0] = _checksum(ret[1:], 0x1D, 0xD1)

  return addr, ret, bus


class TestRivianSafetyBase(common.CarSafetyTest, common.AngleSteeringSafetyTest, common.DriverTorqueSteeringSafetyTest,
                           common.SteerRequestCutSafetyTest, common.LongitudinalAccelSafetyTest):

  TX_MSGS = [[0x100, 0], [0x110, 0], [0x120, 0], [0x321, 2], [0x162, 2]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x100, 0x110, 0x120), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x100, 0x110, 0x120]}

  # Torque limits (cooperative override torque; matches dev's AP envelope + the software 385 tune)
  MAX_TORQUE_LOOKUP = [9, 25, 27], [385, 295, 275]
  DYNAMIC_MAX_TORQUE = True
  MAX_RATE_UP = 3
  MAX_RATE_DOWN = 5
  MAX_RT_DELTA = 125
  DRIVER_TORQUE_ALLOWANCE = 100
  DRIVER_TORQUE_FACTOR = 2
  MIN_VALID_STEERING_FRAMES = 89
  MAX_INVALID_STEERING_FRAMES = 2

  # Angle limits (VM-based, no simple breakpoint rates)
  STEER_ANGLE_MAX = 360
  DEG_TO_CAN = 10
  ANGLE_RATE_BP = None
  ANGLE_RATE_UP = None
  ANGLE_RATE_DOWN = None
  LATERAL_FREQUENCY = 100

  cnt_speed = 0
  cnt_speed_2 = 0
  cnt_angle_cmd = 0
  cnt_adas = 0

  def _get_steer_cmd_angle_max(self, speed):
    return get_max_angle_vm(max(speed, 1), self.VM, CarControllerParams)

  def _torque_driver_msg(self, torque):
    values = {"EPAS_TorsionBarTorque": torque / 100.0}
    return self.packer.make_can_msg_safety("EPAS_SystemStatus", 0, values)

  def _torque_cmd_msg(self, torque, steer_req=1):
    values = {"ACM_lkaStrToqReq": torque, "ACM_lkaActToi": steer_req}
    return self.packer.make_can_msg_safety("ACM_lkaHbaCmd", 0, values)

  def _angle_cmd_msg(self, angle: float, enabled: bool, increment_timer: bool = True):
    values = {"ACM_SteeringAngleRequest": angle, "ACM_EacEnabled": enabled}
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

  def _lkas_button_msg(self, enabled):
    # MADS toggle = Rivian stalk-up (VDM_AdasSts.VDM_UserAdasRequest UP_1=1), the signal
    # carstate_ext maps to ButtonType.lkas. The 0x162 rx_check validates checksum + counter.
    values = {"VDM_UserAdasRequest": 1 if enabled else 0, "VDM_AdasStatus_Counter": self.cnt_adas % 15}
    self.__class__.cnt_adas += 1
    return self.packer.make_can_msg_safety("VDM_AdasSts", 0, values, fix_checksum=checksum)

  def _accel_msg(self, accel: float):
    values = {"ACM_AccelerationRequest": accel}
    return self.packer.make_can_msg_safety("ACM_longitudinalRequest", 0, values)

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

  cnt_stalk = 0

  def _stalk_msg(self, req):
    values = {"VDM_UserAdasRequest": req, "VDM_AdasSts_Counter": self.cnt_stalk % 15}
    self.__class__.cnt_stalk += 1
    return self.packer.make_can_msg_safety("VDM_AdasSts", 0, values, fix_checksum=checksum)

  def test_mads_button_gated_on_cruise(self):
    """UP_1 counts as the MADS button only while stock ACC is NOT engaged: with ACC active
    python treats UP_1 as cancel-only, and counting it in the panda desyncs the two MADS
    state machines (root cause of the EPAS AngleControlCntr fault, route c17ea97d.../6 seg 3)."""
    for cruise in (False, True):
      self._rx(self._pcm_status_msg(1 if cruise else 0))
      self._rx(self._stalk_msg(1))
      expected = 0 if cruise else 1  # MADS_BUTTON_NOT_PRESSED / MADS_BUTTON_PRESSED
      self.assertEqual(self.safety.get_mads_button_press(), expected, f"cruise={cruise}")
      self._rx(self._stalk_msg(0))

  def test_toi_blip_freeze_resume(self):
    """The carcontroller freezes its rate-limiter memory through the 2-frame TOI blip and
    resumes at the pre-blip torque. The panda holds last torque through a tolerated
    steer_req cut, so the instant resume must pass rate checks."""
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

  def _torque_loop_setup(self, angle=150.0, speed=11.4, timer_offset_frames=0):
    """the real cooperative-torque controller driving the panda model frame by frame (10 ms) at a high wheel
    angle, where the controller blips the TOI request about every 0.9 s"""
    fp = {i: {} for i in range(8)}
    fp[0][0x321] = 7
    fp[1][0x1310] = 8
    cp = CarInterface.get_params(CAR.RIVIAN_R1, fp, [], alpha_long=False, is_release=False, docs=False)
    self.erc = ExternalController(cp)
    self.erc.torque_active = True
    self.cs = SimpleNamespace(out=SimpleNamespace(vEgoRaw=speed, steeringTorque=0.0, steeringAngleDeg=angle))
    self.safety.init_tests()
    self.safety.set_controls_allowed(True)
    # the panda's 250 ms real-time interval starts at its first torque message. Send one now and start the
    # controller timer_offset_frames later so the interval timer is out of step with the controller's blip cycle
    self.safety.set_timer(int(1e6))
    self.assertTrue(self._tx(self._torque_cmd_msg(0, steer_req=1)))
    self.frame = max(1, timer_offset_frames)
    self.rx_speed = speed
    for _ in range(10):
      self._torque_loop_rx()

  def _torque_loop_rx(self):
    self._rx(self._speed_msg(self.rx_speed))
    self._rx(self._speed_msg_2(self.rx_speed))
    self._rx(self._torque_driver_msg(0))

  def _torque_loop_frame(self, demand):
    """one 10 ms controller frame sent through the panda; returns whether the panda let it through"""
    self.safety.set_timer(int(1e6) + self.frame * 10000)
    self.frame += 1
    self.erc._update_torque(self.cs, SimpleNamespace(torque=demand))
    ok = self._tx(self._torque_cmd_msg(self.erc.torque_cmd, steer_req=int(self.erc.toi_act_cmd)))
    self._torque_loop_rx()
    return ok

  def test_torque_ramp_through_blip_is_never_blocked(self):
    """A full-rate torque ramp that straddles the TOI blip must never be refused. The panda's real-time check
    only refreshes its reference every 250 ms and a blip frame restarts that timer without refreshing the
    reference, so the reference can be about 0.5 s old. A ramp at the maximum rate (3 counts per frame) then
    climbs more than the allowed 125 counts from it, the panda refuses the frame and zeroes its torque memory,
    and every following frame is refused too (route 4440a486580ed7c6/00000112 seg 15, 5 s lockout)."""
    blocked = []
    for sign in (1.0, -1.0):
      for offset in range(0, 26, 2 if sign > 0 else 5):  # sweep the panda interval timer against the blip cycle ...
        for start in range(0, 100, 4 if sign > 0 else 8):  # ... and the start of the ramp across the cycle
          self._torque_loop_setup(timer_offset_frames=offset)
          for _ in range(start):
            self._torque_loop_frame(0.0)
          for i in range(200):
            if not self._torque_loop_frame(sign):
              blocked.append((sign, offset, start, i))
              break
    self.assertEqual(blocked, [], f"panda refused the controller's torque at (sign, timer offset, demand start, frame): {blocked[:6]}")

  def test_torque_recovers_after_panda_refusal(self):
    """If the panda ever does refuse a frame it zeroes its torque memory. The controller, told the frame was
    refused, must restart from zero instead of repeating a request the panda will keep refusing."""
    self._torque_loop_setup()
    for _ in range(80):
      self.assertTrue(self._torque_loop_frame(0.8))
    # force a refusal: an out-of-range request makes the panda drop its torque memory
    self.safety.set_timer(int(1e6) + self.frame * 10000)
    self.assertFalse(self._tx(self._torque_cmd_msg(self.MAX_TORQUE + 50, steer_req=1)))
    self.erc.notify_torque_refused()
    results = [self._torque_loop_frame(0.8) for _ in range(30)]
    self.assertTrue(all(results), f"controller kept sending requests the panda refuses: {results}")

  def test_toi_fault_release_is_never_refused(self):
    """Releasing the TOI request to clear a latched EPAS ToiFlt (torque 0, request low) must pass the panda wherever it
    lands: mid-ramp, next to a TOI blip, at any phase of the panda's real-time interval. So must the resume, which
    starts from the frozen torque (route 2bba20cd6136cc27/0000007a--9d80d483b8: the latch lasted 59 s and 75 s)."""
    blocked = []
    no_release = []
    for sign in (1.0, -1.0):
      for offset in range(0, 26, 5):  # panda interval timer against the blip cycle
        for start in range(0, 100, 7):  # where in the ramp / blip cycle the latch lands
          self._torque_loop_setup(timer_offset_frames=offset)
          released_at = None
          for i in range(200):
            # like the EPAS: latched from start until a couple of frames after the first release
            self.cs.toi_fault = start <= i and (released_at is None or i < released_at + 2)
            if not self._torque_loop_frame(sign):
              blocked.append((sign, offset, start, i))
              break
            if released_at is None and self.erc.toi_clear_cooldown > 0:
              released_at = i
          if released_at is None:
            no_release.append((sign, offset, start))
    self.assertEqual(blocked, [], f"panda refused around a ToiFlt release at (sign, timer offset, latch start, frame): {blocked[:6]}")
    self.assertEqual(no_release, [], f"latch was never released: {no_release[:6]}")

  def _mbb_setup(self, speed=10.0, angle=20.0):
    """settle the rx state and seed the angle command stream at the measured angle"""
    self.safety.init_tests()
    self.safety.set_controls_allowed(True)
    for _ in range(10):
      self._rx(self._speed_msg(speed))
      self._rx(self._speed_msg_2(speed))
      self._rx(self._angle_meas_msg(angle))
      self._rx(self._torque_driver_msg(0))
    self.safety.set_desired_angle_last(round(angle * self.DEG_TO_CAN))

  def _mbb_rx(self, speed, angle):
    self._rx(self._speed_msg(speed))
    self._rx(self._speed_msg_2(speed))
    self._rx(self._angle_meas_msg(angle))
    self._rx(self._torque_driver_msg(0))

  def test_make_before_break_overlap(self):
    """The make-before-break handoff commands BOTH lateral channels at once: the angle command
    stays live (EacEnabled) while torque ramps up underneath it. 0x110 and 0x120 are validated
    independently with no cross-channel state, so the overlap must pass on both channels, and
    the release (angle inactive, tracking measured) must pass too."""
    speed, angle = 10.0, 20.0
    self._mbb_setup(speed, angle)
    torque = 0
    for frame in range(TORQUE_PREARM_MAX_FRAMES):
      self.assertTrue(self._tx(self._angle_cmd_msg(angle, True)), f"angle blocked during overlap, frame {frame}")
      torque = min(torque + CarControllerParams.STEER_DELTA_UP, self.MAX_TORQUE)
      self.assertTrue(self._tx(self._torque_cmd_msg(torque, steer_req=1)), f"torque {torque} blocked, frame {frame}")
      self._mbb_rx(speed, angle)

    # release: angle goes inactive tracking the measured angle while torque keeps carrying the curve
    for frame in range(50):
      self.assertTrue(self._tx(self._angle_cmd_msg(angle, False)), f"inactive angle blocked, frame {frame}")
      self.assertTrue(self._tx(self._torque_cmd_msg(torque, steer_req=1)), f"torque blocked after release, frame {frame}")
      self._mbb_rx(speed, angle)

  def test_make_before_break_overlap_rate_limited(self):
    """control for the test above: the torque limits stay live during the overlap"""
    speed, angle = 10.0, 20.0
    self._mbb_setup(speed, angle)
    torque = 0
    blocked = False
    for _ in range(20):
      self._tx(self._angle_cmd_msg(angle, True))
      torque += CarControllerParams.STEER_DELTA_UP * 4  # illegal ramp
      if not self._tx(self._torque_cmd_msg(torque, steer_req=1)):
        blocked = True
        break
      self._mbb_rx(speed, angle)
    self.assertTrue(blocked, "over-rate torque was allowed during the overlap")

  def test_make_before_break_abort(self):
    """A stalled ramp aborts back to angle: the torque channel drops to (0, steer_req=0) while the
    angle command keeps steering, then re-arms from 0 after the lockout. Panda holds its last
    torque through the cut, so the re-ramp must pass in either direction."""
    speed, angle = 10.0, 20.0
    self._mbb_setup(speed, angle)
    torque = 0
    for _ in range(60):
      self._tx(self._angle_cmd_msg(angle, True))
      torque = min(torque + CarControllerParams.STEER_DELTA_UP, 180)
      self.assertTrue(self._tx(self._torque_cmd_msg(torque, steer_req=1)))
      self._mbb_rx(speed, angle)

    # abort: torque channel released, angle keeps holding the wheel for the lockout
    for frame in range(TORQUE_PREARM_ABORT_LOCKOUT):
      self.assertTrue(self._tx(self._angle_cmd_msg(angle, True)), f"angle blocked during abort, frame {frame}")
      self.assertTrue(self._tx(self._torque_cmd_msg(0, steer_req=0)), f"torque release blocked, frame {frame}")
      self._mbb_rx(speed, angle)

    # re-attempt after the lockout, ramping from 0 in either direction
    for sign in (1, -1):
      torque = 0
      for frame in range(40):
        self.assertTrue(self._tx(self._angle_cmd_msg(angle, True)))
        torque = sign * min(abs(torque) + CarControllerParams.STEER_DELTA_UP, 120)
        self.assertTrue(self._tx(self._torque_cmd_msg(torque, steer_req=1)), f"re-ramp {torque} blocked, frame {frame}")
        self._mbb_rx(speed, angle)
      for _ in range(5):
        self._tx(self._torque_cmd_msg(0, steer_req=0))
        self._mbb_rx(speed, angle)

  def test_wheel_touch(self):
    # For hiding hold wheel alert on engage
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      values = {
        "SCCM_WheelTouch_HandsOn": 1 if controls_allowed else 0,
        "SCCM_WheelTouch_CapacitiveValue": 100 if controls_allowed else 0,
        "SCCM_WheelTouch_Calibration": 100,
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
    self.VM = VehicleModel(get_safety_CP())
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

  TX_MSGS = [[0x100, 0], [0x110, 0], [0x120, 0], [0x321, 2], [0x160, 0], [0x162, 2]]
  RELAY_MALFUNCTION_ADDRS = {0: (0x100, 0x110, 0x120, 0x160), 2: (0x321, 0x162)}
  FWD_BLACKLISTED_ADDRS = {0: [0x321, 0x162], 2: [0x100, 0x110, 0x120, 0x160]}

  def setUp(self):
    self.VM = VehicleModel(get_safety_CP())
    self.packer = CANPackerSafety("rivian_primary_actuator")
    self.safety = libsafety_py.libsafety
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, RivianSafetyFlags.LONG_CONTROL)
    self.safety.init_tests()

  def test_adas_status(self):
    # VDM_AdasSts is forwarded to the ACM in long mode so openpilot can hide ACC engage requests it would refuse
    for controls_allowed in (True, False):
      self.safety.set_controls_allowed(controls_allowed)
      for user_request in range(5):
        values = {"VDM_UserAdasRequest": user_request}
        self.assertTrue(self._tx(self.packer.make_can_msg_safety("VDM_AdasSts", 2, values)))


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
