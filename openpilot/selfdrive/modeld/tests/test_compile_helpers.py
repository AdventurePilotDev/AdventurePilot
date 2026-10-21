import hashlib
from pathlib import Path

import pytest

from openpilot.common.file_chunker import CHUNK_SIZE, chunk_file, get_chunk_targets, open_file_chunked
from openpilot.selfdrive.modeld.compile_helpers import estimate_pickle_max_size


@pytest.mark.parametrize("onnx_size,variants,compiled_size", [
  (60_881_999, 2, 176_360_000),  # Both failed staging builds, 2026-10-21.
  (60_881_999, 1, 90_990_000),   # Previous successful single-camera build.
  (60_881_999, 3, 270_000_000),  # Reserve per variant, not a hard-coded fourth chunk.
  (8 * 1024 * 1024, 1, 20 * 1024 * 1024),  # Single-JIT/DM caller.
  (CHUNK_SIZE, 1, CHUNK_SIZE),
  (CHUNK_SIZE, 2, CHUNK_SIZE + 1),
])
def test_compiled_model_chunk_roundtrip(tmp_path, onnx_size, variants, compiled_size):
  path = tmp_path / "model.pkl"
  # Sparse payload with markers on both sides of chunk boundaries. These are
  # packaging fixtures; GPU compilation and inference remain build-runner checks.
  with path.open("wb") as f:
    f.truncate(compiled_size)
    for offset in (0, CHUNK_SIZE - 1, CHUNK_SIZE, compiled_size - 1):
      if offset < compiled_size:
        f.seek(offset)
        f.write(bytes([offset % 251 + 1]))
  with path.open("rb") as f:
    expected = hashlib.file_digest(f, "sha256").hexdigest()

  targets = get_chunk_targets(str(path), estimate_pickle_max_size(onnx_size, variants))
  chunk_file(path, targets)
  assert not path.exists()
  assert int(Path(targets[0]).read_text()) == len(targets) - 1
  assert all(Path(chunk).stat().st_size <= CHUNK_SIZE for chunk in targets[1:])
  with open_file_chunked(str(path)) as f:
    assert hashlib.file_digest(f, "sha256").hexdigest() == expected


def test_single_variant_default_is_unchanged():
  assert estimate_pickle_max_size(60_881_999) == 132_249_758


def test_undersized_chunk_reservation_still_fails(tmp_path):
  path = tmp_path / "model.pkl"
  with path.open("wb") as f:
    f.truncate(CHUNK_SIZE + 1)
  with pytest.raises(AssertionError, match="Allowed 1 chunks but needs at least 2"):
    chunk_file(path, get_chunk_targets(str(path), CHUNK_SIZE))
  assert path.exists()
