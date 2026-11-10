"""
The fork's side of jetlink: the modeld seams and the no-checkout fallback.
"""
from pathlib import Path

from jetlink.openpilot.testing import check_adapter, check_fallback, check_seam

from openpilot.common.test import OpenpilotTestCase
from openpilot.sunnypilot import jetlink_adapter
from openpilot.sunnypilot.jetlink_adapter.tests.support import GuardedTest

REPO = Path(__file__).resolve().parents[4]


class TestJetlinkSeam(GuardedTest, OpenpilotTestCase):
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

  def test_the_adapter_speaks_api_4(self):
    """v0.9.0 replaced the API-3 extra-catalog hooks with the model index's:
    with no chestnut fitted the big-model list is jetlink's, and a pick a
    recompile moves is carried over to its new ref."""
    assert jetlink_adapter.API == 4
    for name in ('lists_big_models', 'big_catalog', 'listed_ref'):
      assert name in jetlink_adapter.HOOK_NAMES, name
    for name in ('should_extend_catalog', 'extend_catalog'):
      assert not hasattr(jetlink_adapter, name), name
    assert not hasattr(jetlink_adapter.KEYS, 'pointers'), "API 4 dropped the pointers param"
