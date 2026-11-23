"""
Copyright (c) 2021-, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import re
from pathlib import Path

import pytest

ROOT = Path(__file__).resolve().parents[4]
FLOOR = re.compile(r"desired_curvature = model_output\['action'\]\[0, ?0\] / \(max\(([\d.]+), v_ego\)\)\*\*2")


@pytest.mark.parametrize('path', ['openpilot/selfdrive/modeld/modeld.py', 'openpilot/sunnypilot/modeld_v2/modeld.py'])
def test_action_curvature_floor_is_held_at_one(path):
  """Upstream moved this floor to 2.5 (comma #39182, sunnypilot #2091), with no rationale and no Rivian evidence.

  We hold it at 1.0 on purpose, in both modeld paths. A sync can bring upstream's value back without a conflict,
  so taking 2.5 has to be a choice: change this test with the evidence for it.
  """
  assert FLOOR.findall((ROOT / path).read_text()) == ['1.0']
