import sys
from pathlib import Path

# the submodule, importable as `jetlink` without the launch script's symlink
repo = Path(__file__).resolve().parents[4]
jetlink_repo = repo / "jetlink_repo"
if jetlink_repo.is_dir() and str(jetlink_repo) not in sys.path:
  sys.path.insert(0, str(jetlink_repo))
