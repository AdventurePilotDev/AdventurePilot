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

  def test_the_model_manager_does_not_fetch_the_extra_catalog(self):
    """jetlink's extra catalog is a live, unpinned third-party URL.

    jetlink.registry.catalog.fetch_catalogs() fetches EXTRA_CATALOG_URL from
    zoompilot/jetlink's moving main branch, and only the extend_catalog() hook
    reaches it. This fork does not call that hook, so no owner path fetches
    the extra catalog; keep it that way until that URL is pinned or the fetch
    is opt-in upstream.
    """
    fetcher = REPO / "openpilot/sunnypilot/models/fetcher.py"
    source = fetcher.read_text()
    assert "extend_catalog" not in source, fetcher
    assert "jetlink_adapter." not in source, fetcher
