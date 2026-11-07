"""Schema ownership/default and real binary round trip for requested assist telemetry."""
from openpilot.cereal import log, custom


def test_requested_boost_schema_round_trip_and_default():
  assert 'accelerationControlBoost' not in log.LongitudinalPlan.schema.fields
  assert custom.LongitudinalPlanSP.new_message().accelerationControlBoostRequested == 0
  message = custom.LongitudinalPlanSP.new_message(accelerationControlBoostRequested=0.2)
  with custom.LongitudinalPlanSP.from_bytes(message.to_bytes()) as decoded:
    assert abs(decoded.accelerationControlBoostRequested - 0.2) < 1e-7
