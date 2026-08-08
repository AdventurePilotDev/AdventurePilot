"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

The speed-cap invariant in LongitudinalPlannerSP.update_targets().

A source wins the arbitration on the lowest SPEED target, but the a_target that comes back
is assigned straight into the planner's a_desired continuity state. Every source in the
contest except `cruise` is a speed CAP, so none of them may ever ask for MORE acceleration
than the car is already producing. Without that clamp, curve control holding a cap we are
already below (because a lead is braking us under it) injects a positive a_target while the
car decelerates, v_desired_filter drifts above the true speed, and the MPC brakes for a car
going several m/s faster than reality.

update_targets() is arbitration plus arithmetic, so the sub-controllers are faked rather
than driven. That keeps each case pinned to one published (v_target, a_target) pair, which
is what the invariant is actually about; the controllers have their own tests.
"""
from types import SimpleNamespace

from cereal import custom
from openpilot.sunnypilot.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlannerSP

LongitudinalPlanSource = custom.LongitudinalPlanSP.LongitudinalPlanSource

UNSET = 1e3  # far above any real v_target, so an unset source never wins the min-by-speed contest


class FakeSource:
  def __init__(self, v_target=UNSET, a_target=0.0):
    self.output_v_target = v_target
    self.output_a_target = a_target

  def update(self, *args):
    pass


class FakeGovernor(FakeSource):
  def __init__(self, v_target=UNSET, a_target=0.0, throttle_scale=1.0):
    super().__init__(v_target, a_target)
    self._throttle_scale = throttle_scale

  def throttle_scale(self):
    return self._throttle_scale


class FakeSCC:
  def __init__(self, vision, scc_map, curve, governor):
    self.vision = vision
    self.map = scc_map
    self.curve = curve
    self.governor = governor

  def update(self, *args):
    pass


class FakeResolver:
  speed_limit_valid = False
  speed_limit_last_valid = False
  speed_limit = 0.0
  speed_limit_final_last = 0.0
  distance = 0.0

  def update(self, *args):
    pass


def _targets(v_ego, a_ego, v_cruise, vision=None, scc_map=None, curve=None, governor=None):
  """Run update_targets() with the sub-controllers faked. Returns (source, v_target, a_target)."""
  planner = object.__new__(LongitudinalPlannerSP)
  planner.events_sp = SimpleNamespace(clear=lambda: None)
  planner.resolver = FakeResolver()
  planner.sla = FakeSource()
  planner.scc = FakeSCC(vision or FakeSource(), scc_map or FakeSource(),
                        curve or FakeSource(), governor or FakeGovernor())
  planner.source = LongitudinalPlanSource.cruise
  planner.output_v_target = 0.0
  planner.output_a_target = 0.0

  sm = {
    'carState': SimpleNamespace(vCruiseCluster=v_cruise * 3.6),
    'carControl': SimpleNamespace(enabled=True, cruiseControl=SimpleNamespace(override=False)),
  }
  v_target, a_target = planner.update_targets(sm, v_ego, a_ego, v_cruise)
  return planner.source, v_target, a_target


class TestSpeedCapInvariant:
  def test_cap_still_wins_the_speed_contest(self):
    # the clamp must not disturb arbitration: the lowest v_target still selects the source
    source, v_target, _ = _targets(v_ego=25.0, a_ego=0.0, v_cruise=30.0,
                                   curve=FakeSource(v_target=18.0, a_target=-1.0))
    assert source == LongitudinalPlanSource.curveSpeed
    assert v_target == 18.0

  def test_phantom_acceleration_is_dropped(self):
    # the reported bug: curve control wins on speed while asking to accelerate, because a lead
    # has braked us below its cap. That positive a_target must not reach a_desired.
    _, _, a_target = _targets(v_ego=15.0, a_ego=-2.5, v_cruise=30.0,
                              curve=FakeSource(v_target=20.0, a_target=+1.2))
    assert a_target == -2.5

  def test_a_source_own_decel_is_preserved(self):
    # the clamp is one-way. A cap braking harder than the car currently is must pass through
    # untouched, or curve control could no longer slow the car at all.
    _, _, a_target = _targets(v_ego=25.0, a_ego=-0.5, v_cruise=30.0,
                              curve=FakeSource(v_target=18.0, a_target=-1.8))
    assert a_target == -1.8

  def test_accel_out_is_bounded_by_a_ego(self):
    # accelerating out of a curve is the MPC's job; a cap may ask for at most what the car
    # is already doing
    _, _, a_target = _targets(v_ego=20.0, a_ego=0.4, v_cruise=30.0,
                              curve=FakeSource(v_target=22.0, a_target=+1.2))
    assert a_target == 0.4

  def test_cruise_source_is_unaffected(self):
    # cruise publishes a_ego as its own a_target, so min(a_target, a_ego) is a no-op for it.
    # Pins that the clamp did not accidentally cap normal driving.
    source, _, a_target = _targets(v_ego=20.0, a_ego=0.8, v_cruise=25.0)
    assert source == LongitudinalPlanSource.cruise
    assert a_target == 0.8

  def test_governor_overrides_curve_and_cannot_accelerate(self):
    # when the governor wants a lower speed than the curve profile it replaces it, and its
    # a_target is floored at 0 before the clamp ever sees it
    source, v_target, a_target = _targets(v_ego=25.0, a_ego=-0.2, v_cruise=30.0,
                                          curve=FakeSource(v_target=22.0, a_target=+0.5),
                                          governor=FakeGovernor(v_target=17.0, a_target=+0.5))
    assert source == LongitudinalPlanSource.curveSpeed
    assert v_target == 17.0
    assert a_target == -0.2

  def test_throttle_interlock_applies_after_the_clamp(self):
    # the throttle-fade interlock only scales a POSITIVE result, so it must see the clamped
    # value. Here the clamp lets +0.6 through (a_ego is higher), then the interlock halves it.
    _, _, a_target = _targets(v_ego=20.0, a_ego=1.0, v_cruise=30.0,
                              curve=FakeSource(v_target=22.0, a_target=+0.6),
                              governor=FakeGovernor(throttle_scale=0.5))
    assert a_target == 0.3

  def test_throttle_interlock_never_scales_a_clamped_decel(self):
    # a negative result must not be touched by the interlock, or a hard-zero throttle_scale
    # would cancel braking outright
    _, _, a_target = _targets(v_ego=15.0, a_ego=-2.5, v_cruise=30.0,
                              curve=FakeSource(v_target=20.0, a_target=+1.2),
                              governor=FakeGovernor(throttle_scale=0.0))
    assert a_target == -2.5
