"""Test setup for the fork's side of jetlink.

The submodule must be checked out. A job that forgets it fails here with the
command to fix it, rather than skipping every test with importorskip.

check_fallback()'s link_on() is a plain read of the params store, so every
test gets a store of its own through PARAMS_ROOT: no test may read the
machine's settings.
"""
import sys
from pathlib import Path

import pytest

REPO = Path(__file__).resolve().parents[4]
JETLINK_REPO = REPO / "jetlink_repo"

if not (JETLINK_REPO / "jetlink" / "__init__.py").is_file():
  raise RuntimeError(f"{JETLINK_REPO} is not checked out: run "
                     "`git submodule update --init --recursive` (CI must check out submodules)")

if str(JETLINK_REPO) not in sys.path:
  sys.path.insert(0, str(JETLINK_REPO))

from openpilot.sunnypilot.jetlink_adapter.tests import support  # noqa: E402  (after the submodule is on the path)


@pytest.fixture(autouse=True)
def own_params_store(tmp_path, monkeypatch):
  """Every test reads the params store it made, as the porting guide asks."""
  monkeypatch.setenv("PARAMS_ROOT", str(tmp_path))
  return tmp_path
