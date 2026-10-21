from opendbc.car.structs import car
from openpilot.common.test import OpenpilotTestCase
from openpilot.selfdrive.locationd.torqued import TorqueEstimator


class TestTorqued(OpenpilotTestCase):
  def test_cal_percent(self):
    est = TorqueEstimator(car.CarParams())
    msg = est.get_msg()
    assert msg.lateralTorqueParameters.calPerc == 0

    for (low, high), min_pts in zip(est.filtered_points.buckets.keys(),
                                    est.filtered_points.buckets_min_points.values(), strict=True):
      for _ in range(int(min_pts)):
        est.filtered_points.add_point((low + high) / 2.0, 0.0)

    # enough bucket points, but not enough total points
    msg = est.get_msg()
    assert msg.lateralTorqueParameters.calPerc == (len(est.filtered_points) / est.min_points_total * 100 + 100) / 2

    # add enough points to bucket with most capacity
    key = list(est.filtered_points.buckets)[0]
    for _ in range(est.min_points_total - len(est.filtered_points)):
      est.filtered_points.add_point((key[0] + key[1]) / 2.0, 0.0)

    msg = est.get_msg()
    assert msg.lateralTorqueParameters.calPerc == 100

  def test_bucket_diagnostics(self):
    est = TorqueEstimator(car.CarParams())
    diag = est.get_bucket_diagnostics()
    assert diag['points'] == [0] * len(est.filtered_points.buckets)
    assert diag['min_points'] == list(est.filtered_points.buckets_min_points.values())
    assert diag['min_bucket_perc'] == 0
    assert not diag['buckets_valid']

    # fill every bucket to its minimum except one, which gets half
    keys = list(est.filtered_points.buckets)
    short = 2
    for i, ((low, high), min_pts) in enumerate(zip(keys, est.filtered_points.buckets_min_points.values(), strict=True)):
      n = int(min_pts) // 2 if i == short else int(min_pts)
      for _ in range(n):
        est.filtered_points.add_point((low + high) / 2.0, 0.0)

    diag = est.get_bucket_diagnostics()
    assert diag['min_bucket'] == short
    assert diag['min_bucket_perc'] == 50.0
    assert diag['total_points'] == len(est.filtered_points)
    assert diag['cal_perc'] == est.get_msg().lateralTorqueParameters.calPerc
    assert not diag['buckets_valid']
