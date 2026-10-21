"""Replays two real torque lockouts through the torque controller and the panda safety model.

Route 2bba20cd6136cc27/0000007a--9d80d483b8 (dev-a c085d4422db6): twice, a high-angle TOI blip left the panda's real-time
reference stale, the panda refused the next torque ramp (441 and 215 frames), the EPAS latched ToiFlt and ignored torque
for 59 s and 75 s while openpilot showed engaged, and the truck drifted across a lane.

The fixture holds, for about 7-9 s around each lockout, what the controller saw each frame (speed, driver torque, wheel
angle, lateral active, torque demand), what openpilot actually sent, the bus 0 messages the panda reads, and when the
real panda refused. The panda's 250 ms real-time interval phase is not in the log; phase_ms is the one at which the
logged frames reproduce the real refusals exactly.
"""
import gzip
import json
import os
import unittest
from types import SimpleNamespace

from opendbc.car import structs
from opendbc.car.rivian.carcontroller import CarController, TOI_CLEAR_RETRY_FRAMES
from opendbc.car.structs import CarParams
from opendbc.safety.tests.common import CANPackerSafety
from opendbc.safety.tests.libsafety import libsafety_py

FIXTURE = os.path.join(os.path.dirname(__file__), "data", "rivian_toiflt_route_replay.json.gz")
TIMER_BASE_US = 10_000_000
ECHO_DELAY_MAX_US = 25_000  # the real panda's refusal echo is logged a few ms after openpilot's send


class TestRivianRouteReplay(unittest.TestCase):
  @classmethod
  def setUpClass(cls):
    with gzip.open(FIXTURE) as f:
      cls.episodes = json.load(f)["episodes"]
    cls.packer = CANPackerSafety("rivian_primary_actuator")
    cls.safety = libsafety_py.libsafety

  def _tx(self, t_us, torque, req):
    self.safety.set_timer(TIMER_BASE_US + t_us)
    msg = self.packer.make_can_msg_safety("ACM_lkaHbaCmd", 0, {"ACM_lkaStrToqReq": torque, "ACM_lkaActToi": req})
    return self.safety.safety_tx_hook(msg)

  def _replay(self, ep, phase_ms, use_controller):
    """(refused send times in us, frames with the TOI request released to clear a latched ToiFlt).
    use_controller=False sends what openpilot logged; True runs the current controller."""
    self.safety.set_safety_hooks(CarParams.SafetyModel.rivian, 0)
    self.safety.init_tests()
    self.safety.set_controls_allowed_lateral(True)
    controller = CarController({"pt": "rivian_primary_actuator"}, structs.CarParams(), structs.CarParamsSP()) if use_controller else None
    hba = {"ACM_hbaSysState": 0, "ACM_hbaLamp": 0, "ACM_hbaOnOffState": 0, "ACM_slifOnOffState": 0}
    wheel_touch = {"SCCM_WheelTouch_Counter": 0, "SCCM_WheelTouch_HandsOn": 0, "SCCM_WheelTouch_CapacitiveValue": 0,
                   "SCCM_WheelTouch_Calibration": 100, "SCCM_WheelTouch_ResistiveValue": 0}

    rx = ep["rx"]
    rx_i = 0
    refused = []
    releases = 0
    toi_fault = False  # the EPAS's real ToiFlt from the log: drives the controller's latch release
    pending_refusal = False
    for n, (t, v_ego, driver_torque, angle, lat, demand, logged_torque, logged_req) in enumerate(ep["frames"]):
      while rx_i < len(rx) and rx[rx_i][0] <= t:
        rt, addr, dat = rx[rx_i]
        self.safety.set_timer(TIMER_BASE_US + rt)
        self.safety.safety_rx_hook(libsafety_py.make_CANPacket(addr, 0, bytes.fromhex(dat)))
        if addr == 0x380:
          toi_fault = bool((bytes.fromhex(dat)[4] >> 3) & 1)
        rx_i += 1

      if n == 0:
        # the panda was mid-drive: seed its torque memory with what was being sent and start its real-time interval
        # phase_ms before this frame
        self.safety.set_desired_torque_last(logged_torque)
        self.safety.set_rt_torque_last(logged_torque)
        self.assertTrue(self._tx(t - phase_ms * 1000, logged_torque, 1))
        if controller is not None:
          controller.apply_torque_last = logged_torque
          controller.rt_limiter.sent(logged_torque)

      if controller is None:
        ok = self._tx(t, logged_torque, logged_req)
      else:
        out = structs.CarState()
        out.vEgoRaw = v_ego
        out.steeringTorque = driver_torque
        out.steeringAngleDeg = angle
        cs = SimpleNamespace(out=out, acm_lka_hba_cmd=hba, sccm_wheel_touch=wheel_touch, vdm_adas_status=[],
                             toi_fault=toi_fault, torque_tx_refused=pending_refusal)
        cc = structs.CarControl()
        cc.latActive = bool(lat)
        cc.actuators.torque = demand
        _, can_sends = controller.update(cc.as_reader(), structs.CarControlSP(), cs, 0)
        releases += controller.toi_clear_cooldown == TOI_CLEAR_RETRY_FRAMES
        dat = next(d for addr, d, _ in can_sends if addr == 0x120)
        self.safety.set_timer(TIMER_BASE_US + t)
        ok = self.safety.safety_tx_hook(libsafety_py.make_CANPacket(0x120, 0, dat))
      pending_refusal = not ok
      if not ok:
        refused.append(t)
    return refused, releases

  def test_logged_frames_reproduce_the_real_lockout(self):
    # keeps the replay honest: what openpilot actually sent is refused exactly as the real panda refused it
    for name, ep in self.episodes.items():
      with self.subTest(episode=name):
        model, _ = self._replay(ep, ep["phase_ms"], use_controller=False)
        real = ep["real_refused_us"]
        self.assertEqual(len(model), len(real))
        delays = [r - m for m, r in zip(model, real, strict=True)]
        self.assertTrue(all(0 <= d <= ECHO_DELAY_MAX_US for d in delays), f"refusals do not pair up: {delays[:5]}")

  def test_controller_is_never_refused(self):
    # the current controller, driven by the same inputs, at every phase of the panda's real-time interval. The logged
    # ToiFlt latch also makes it release the TOI request on the real data; those releases must pass too.
    for name, ep in self.episodes.items():
      for phase_ms in range(0, 260, 10):
        with self.subTest(episode=name, phase_ms=phase_ms):
          refused, releases = self._replay(ep, phase_ms, use_controller=True)
          self.assertEqual(refused, [])
          self.assertGreater(releases, 0, "the logged ToiFlt latch should have been released")


if __name__ == "__main__":
  unittest.main()
