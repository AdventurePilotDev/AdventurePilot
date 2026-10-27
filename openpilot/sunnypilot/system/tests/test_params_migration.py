"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""

from openpilot.common.params import Params
from openpilot.common.test import OpenpilotTestCase
from openpilot.sunnypilot.system.params_migration import _migrate_model_bundle_slots, _migrate_rivian_enhanced_mads


class TestModelBundleSlotMigration(OpenpilotTestCase):
  """Pre-split, a chestnut user's big-model selection lived in the single ActiveBundle.
  The migration seeds both slots; per-source validation later drops whichever does not
  match its own manifest."""

  def test_seeds_chestnut_slot_from_active_bundle(self):
    params = Params()
    bundle = {"ref": "big", "minimumSelectorVersion": 18}
    params.put("ModelManager_ActiveBundle", bundle, block=True)
    _migrate_model_bundle_slots(params)
    assert params.get("ModelManager_ActiveBundleChestnut") == bundle
    assert params.get("ModelManager_ActiveBundle") == bundle

  def test_noop_when_chestnut_slot_already_set(self):
    params = Params()
    params.put("ModelManager_ActiveBundle", {"ref": "small"}, block=True)
    params.put("ModelManager_ActiveBundleChestnut", {"ref": "big"}, block=True)
    _migrate_model_bundle_slots(params)
    assert params.get("ModelManager_ActiveBundleChestnut") == {"ref": "big"}

  def test_noop_when_no_selection(self):
    params = Params()
    _migrate_model_bundle_slots(params)
    assert params.get("ModelManager_ActiveBundleChestnut") is None


class _FakeParams:
  # A dict-backed stand-in, so the test does not depend on RivianEnhancedMads being registered in this build.
  def __init__(self, values):
    self.values = dict(values)

  def get(self, key, **kwargs):
    return self.values.get(key)

  def put_bool(self, key, value, block=False):
    self.values[key] = value


class TestRivianEnhancedMadsMigration:
  """Existing Rivian installs keep the enhanced MADS they always had; fresh installs get the default Off."""

  def test_existing_rivian_install_seeded_on(self):
    params = _FakeParams({"CarPlatformBundle": {"brand": "rivian"}})
    _migrate_rivian_enhanced_mads(params)
    assert params.values["RivianEnhancedMads"] is True

  def test_fresh_install_untouched(self):
    params = _FakeParams({})
    _migrate_rivian_enhanced_mads(params)
    assert "RivianEnhancedMads" not in params.values

  def test_other_brand_untouched(self):
    params = _FakeParams({"CarPlatformBundle": {"brand": "tesla"}})
    _migrate_rivian_enhanced_mads(params)
    assert "RivianEnhancedMads" not in params.values

  def test_existing_value_untouched(self):
    # a driver who has turned it off must stay off on every later boot
    params = _FakeParams({"CarPlatformBundle": {"brand": "rivian"}, "RivianEnhancedMads": False})
    _migrate_rivian_enhanced_mads(params)
    assert params.values["RivianEnhancedMads"] is False
