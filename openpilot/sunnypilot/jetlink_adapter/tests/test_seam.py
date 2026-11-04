"""
The fork's side of jetlink: the modeld seams and the no-checkout fallback.
"""
from pathlib import Path

import pytest

pytest.importorskip("jetlink")

from jetlink.openpilot.testing import check_adapter, check_fallback, check_seam

from openpilot.common.test import OpenpilotTestCase
from openpilot.sunnypilot import jetlink_adapter

REPO = Path(__file__).resolve().parents[4]


class TestJetlinkSeam(OpenpilotTestCase):
  def test_the_adapter_conforms(self):
    heavy = ("numpy", "capnp", "zmq", "cereal", "msgq", "tinygrad", "openpilot.cereal",
             "openpilot.common.params", "openpilot.common.swaglog")
    assert check_adapter(jetlink_adapter, heavy=heavy) == []

  def test_every_modeld_has_the_seam(self):
    for modeld in ("openpilot/selfdrive/modeld/modeld.py", "openpilot/sunnypilot/modeld_v2/modeld.py"):
      assert check_seam(str(REPO / modeld)) == [], modeld

  def test_every_hook_answers_as_the_link_off(self):
    assert check_fallback(jetlink_adapter._Off("why"), jetlink_adapter.HOOK_NAMES,
                          link_on=jetlink_adapter.link_on) == []
