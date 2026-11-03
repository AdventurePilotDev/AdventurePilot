"""Actual planner selection/publication with a deterministic MPC output boundary.

The generated MPC optimizer and unrelated SP controllers are seams. The production
planner update, cruise limiter, plan interpolation, AccelBoost, AccelerationControl,
min-selection, clipping and both publishers execute unchanged. No road/replay claim.
"""
from types import SimpleNamespace

import numpy as np
import pytest

from opendbc.car import structs
from opendbc.car.interfaces import ACCEL_MIN, ACCEL_MAX
from openpilot.cereal import log, custom
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlanner, T_IDXS_MPC
from openpilot.selfdrive.controls.lib.accel_boost import AccelBoost
from openpilot.sunnypilot.selfdrive.controls.lib.longitudinal_planner import LongitudinalPlannerSP
from openpilot.sunnypilot.selfdrive.controls.lib.tests.test_acceleration_control import _assist, _sm

Source = log.LongitudinalPlan.LongitudinalPlanSource


class DeterministicMPC:
  """Only the solver result boundary is controlled; interpolation is production code."""
  crash_cnt = 0
  solve_time = 0.001
  source = Source.lead0

  def __init__(self, accel):
    self.v_solution = 25 + accel * T_IDXS_MPC
    self.a_solution = np.full(len(T_IDXS_MPC), accel)
    self.j_solution = np.zeros(len(T_IDXS_MPC) - 1)

  def set_weights(self, prev_accel_constraint, personality):
    pass

  def set_cur_state(self, v, a):
    pass

  def update(self, radar, personality):
    pass


class Messages(dict):
  logMonoTime = {'modelV2': 0}

  def all_checks(self, service_list=None):
    return True


class Publisher:
  def __init__(self):
    self.messages = {}

  def send(self, service, message):
    # Real binary serialization at the parent -> SP publisher boundary.
    with log.Event.from_bytes(message.to_bytes()) as decoded:
      self.messages[service] = decoded.to_dict()[service]


def _planner(monkeypatch, raw, cruise, lead, enabled=True):
  # Isolate unrelated controllers without replacing update's selection algorithms.
  monkeypatch.setattr(LongitudinalPlannerSP, 'update', lambda self, sm: None)
  monkeypatch.setattr(LongitudinalPlannerSP, 'update_targets',
                      lambda self, sm, v_ego, a_ego, v_cruise: (v_cruise, a_ego))
  planner = object.__new__(LongitudinalPlanner)
  planner.CP = structs.CarParams(openpilotLongitudinalControl=True, longitudinalActuatorDelay=0.35)
  planner.mpc = DeterministicMPC(lead)
  planner.accel_boost = AccelBoost()
  planner.acceleration_control = _assist(enabled=enabled)
  # curve control's lateral throttle interlock, inactive
  planner.scc = SimpleNamespace(governor=SimpleNamespace(throttle_scale=lambda: 1.0))
  planner.dt = DT_MDL
  planner.v_desired_filter = FirstOrderFilter(25, 2, DT_MDL)
  planner.output_a_target = 0
  planner.a_cruise = cruise
  sm = Messages(_sm(v_ego=25, desired_accel=raw))
  sm['carState'].vCruise = (25 + cruise) * 3.6
  sm['controlsState'] = log.ControlsState.new_message(longControlState='pid')
  sm['radarState'] = log.RadarState.new_message(leadOne={'present': True})
  return planner, sm


def _publication_fields(planner):
  # Explicit neutral SP state; publish_longitudinal_plan_sp runs its real body.
  default = custom.LongitudinalPlanSP.new_message()
  planner.source = custom.LongitudinalPlanSP.LongitudinalPlanSource.cruise
  planner.output_v_target = 30
  planner.events_sp = SimpleNamespace(to_msg=list)
  vision = SimpleNamespace(state=default.smartCruiseControl.vision.state, output_v_target=30, output_a_target=0,
                           current_lat_acc=0, max_pred_lat_acc=0, is_enabled=False, is_active=False)
  map_source = SimpleNamespace(state=default.smartCruiseControl.map.state, output_v_target=30, output_a_target=0,
                               is_enabled=False, is_active=False)
  curve = SimpleNamespace(state=default.smartCruiseControl.curve.state, output_v_target=30, output_a_target=0,
                          lat_accel_target=0, is_enabled=False, is_active=False)
  planner.scc = SimpleNamespace(vision=vision, map=map_source, curve=curve,
                                governor=SimpleNamespace(load=0, ceiling=1, is_active=False))
  planner.resolver = SimpleNamespace(speed_limit=0, speed_limit_last=0, speed_limit_final=0, speed_limit_final_last=0,
                                     speed_limit_valid=False, speed_limit_last_valid=False, speed_limit_offset=0,
                                     distance=0, source=default.speedLimit.resolver.source)
  planner.sla = SimpleNamespace(state=default.speedLimit.assist.state, is_enabled=False, is_active=False,
                                output_v_target=30, output_a_target=0)
  planner.e2e_alerts_helper = SimpleNamespace(green_light_alert=False, lead_depart_alert=False)


@pytest.mark.parametrize('raw,cruise,lead,enabled,expected,source', [
  (0.1, 1, 1, True, 0.3, Source.e2e),
  (0.1, 0.1, 1, True, 0.1, Source.cruise),
  (0.1, 1, 0.05, True, 0.05, Source.lead0),
  # Production tie order: MPC then cruise then model.
  (0, 0, 0, True, 0, Source.lead0),
  # 94.5 kph and 26.25 m/s are exactly representable, so this is a real tie.
  (1.25, 1.25, 2, False, 1.25, Source.cruise),
  (0.5, 1, 0.5, False, 0.5, Source.lead0),
  (4, 4, 4, True, ACCEL_MAX, Source.cruise),
  (-5, 1, 1, True, ACCEL_MIN, Source.e2e),
])
def test_actual_planner_winners_ties_and_limits(monkeypatch, raw, cruise, lead, enabled, expected, source):
  planner, sm = _planner(monkeypatch, raw, cruise, lead, enabled)
  planner.update(sm)
  assert planner.output_a_target == pytest.approx(expected, abs=1e-6)
  assert planner.mpc.source == source


@pytest.mark.parametrize('cruise,lead', [(0.1, 1), (1, 0.05)])
def test_parent_to_sp_requested_telemetry_and_disable(monkeypatch, cruise, lead):
  planner, sm = _planner(monkeypatch, 0.1, cruise, lead)
  planner.update(sm)
  # Real capnp vCruise is float32; its kph -> m/s conversion has sub-micro precision.
  assert planner.output_a_target <= 0.1 + 1e-6
  _publication_fields(planner)
  pm = Publisher()
  planner.publish(sm, pm)
  shared = pm.messages['longitudinalPlan']
  fork = pm.messages['longitudinalPlanSP']
  assert 'accelerationControlBoost' not in shared
  assert fork['accelerationControlBoostRequested'] == pytest.approx(0.2)
  assert fork['accelerationControlBoostRequested'] > shared['aTarget']
  # Same object, positive preceding request: disabled update must publish zero.
  sm['selfdriveState'].enabled = False
  planner.update(sm)
  planner.publish(sm, pm)
  assert pm.messages['longitudinalPlanSP']['accelerationControlBoostRequested'] == 0
