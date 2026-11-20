"""
The fork's side of jetlink: the modeld seams and the no-checkout fallback.
"""
import inspect
import re
from pathlib import Path
from unittest import mock

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

  def test_the_large_model_takes_modeld_v2s_action_call(self):
    """modeld_v2 calls the model's get_action_from_model with its own arguments,
    blendful sixth. The large model runs stock's function, whose sixth is
    lat_smooth_seconds: the partial reached smooth_value() and modeld died
    once the truck moved. Stock's must get modeld_v2's first five, no more.
    """
    from openpilot.sunnypilot.modeld_v2.modeld import ModelState
    names = [name for name in inspect.signature(ModelState.get_action_from_model).parameters if name != 'self']
    with mock.patch('openpilot.selfdrive.modeld.modeld.get_action_from_model') as stock:
      action = jetlink_adapter.adapter().action_function()
      action(*[object() for _ in names])
    args, kwargs = stock.call_args
    assert len(args) == 5 and kwargs == {}

  def test_every_hook_answers_as_the_link_off(self):
    assert check_fallback(jetlink_adapter._Off("why"), jetlink_adapter.HOOK_NAMES,
                          link_on=jetlink_adapter.link_on) == []

  def test_the_model_manager_keeps_its_own_big_model_list(self):
    """Its big_catalog() hook would make jetlink's unpinned model index the list
    the user picks from. Nothing outside the adapter calls the list hooks, so the
    list stays the catalog the model manager fetched from sunnypilot; keep it that
    way unless we choose otherwise.
    """
    hooks = re.compile(r"\b(?:jetlink_adapter|hooks|HOOKS)\.(?:big_catalog|lists_big_models|listed_ref)\b")
    adapter = REPO / "openpilot/sunnypilot/jetlink_adapter"
    callers = [path for path in (REPO / "openpilot").rglob("*.py")
               if adapter not in path.parents and hooks.search(path.read_text(errors="ignore"))]
    assert callers == []
    fetcher = (REPO / "openpilot/sunnypilot/models/fetcher.py").read_text()
    assert "big_catalog" not in fetcher
    assert "lists_big_models" not in fetcher
    assert "listed_ref" not in fetcher
    assert "jetlink_adapter." not in fetcher
