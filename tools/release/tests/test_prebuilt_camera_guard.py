import os
import subprocess
from pathlib import Path

import pytest
import yaml


ROOT = Path(__file__).resolve().parents[3]
CAMERAS = ("1928x1208", "1344x760")


@pytest.fixture
def camera_guard():
  workflow = yaml.safe_load((ROOT / ".github/workflows/build-prebuilt.yaml").read_text())
  steps = workflow["jobs"]["build"]["steps"]
  return next(step["run"] for step in steps if step["name"] == "Guard - models for both camera types must be present")


@pytest.mark.parametrize("missing", [(), (CAMERAS[0],), (CAMERAS[1],), CAMERAS])
@pytest.mark.parametrize("empty", [False, True])
def test_camera_guard_checks_staged_files(tmp_path, camera_guard, missing, empty):
  output = tmp_path / "staged output"
  models = output / "openpilot/selfdrive/modeld/models"
  models.mkdir(parents=True)
  for camera in CAMERAS:
    path = models / f"dm_warp_{camera}_tinygrad.pkl"
    if camera not in missing:
      path.write_bytes(b"nonempty camera artifact")
    elif empty:
      path.touch()

  # The workflow must reject incomplete staged output even when a build tree
  # still contains both camera files.
  build = tmp_path / "build"
  build_models = build / "openpilot/selfdrive/modeld/models"
  build_models.mkdir(parents=True)
  for camera in CAMERAS:
    (build_models / f"dm_warp_{camera}_tinygrad.pkl").write_bytes(b"build artifact")
  env = {**os.environ, "OUTPUT_DIR": str(output), "BUILD_DIR": str(build)}
  result = subprocess.run(["bash", "-e", "-c", camera_guard], cwd=build, env=env, text=True, capture_output=True)
  assert result.returncode == (1 if missing else 0), result.stdout + result.stderr
  for camera in missing:
    assert f"Missing or empty camera model: {models}/dm_warp_{camera}_tinygrad.pkl" in result.stdout
