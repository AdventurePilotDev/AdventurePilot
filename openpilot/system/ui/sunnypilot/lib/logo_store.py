"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

Storage and validation for user-supplied screen saver logos.

Files live in Paths.branding_root() so they survive reboots, OTA updates and branch switches.
They can arrive two ways: through import_logo(), which validates and normalizes, or dropped
straight into the directory over copyparty, which validates nothing. So load_logo_rgba() has to
be safe on its own and must never raise into the render loop.
"""
import io
import os
import re
from dataclasses import dataclass, field
from pathlib import Path

import numpy as np
from PIL import Image

from openpilot.common.hardware.hw import Paths
from openpilot.common.swaglog import cloudlog

# Upload limits. A logo is orders of magnitude smaller than any of these; they are here to bound
# memory and decode time on a file we did not create.
MAX_UPLOAD_BYTES = 5 * 1024 * 1024
MAX_SOURCE_DIM = 4096
MAX_DECODED_PIXELS = 8_000_000
MIN_SOURCE_WIDTH = 64
MIN_SOURCE_HEIGHT = 32

# Stored artifact, sized at 2x the on-screen box so it stays sharp and does not depend on the
# hardware it was imported on.
STORED_MAX_WIDTH = 1440
STORED_MAX_HEIGHT = 720

# The logo is drawn tinted on a black background. The tint is a per channel multiply, so a color
# the logo does not contain wipes it out completely: a pure red logo under a cyan tint is black.
# The weakest channel is therefore what decides whether the logo ever disappears, and a channel
# mean below this earns a warning, never a rejection.
MIN_CHANNEL_MEAN = 32.0
# Pixels fainter than this do not count towards the luminance average.
ALPHA_FLOOR = 32

LOGO_EXTENSIONS = (".png", ".jpg", ".jpeg", ".webp", ".gif")
MAX_STORED_NAME_LEN = 40
TEMP_SUFFIX = ".tmp"

# The small UI keeps a single picture under one name rather than a library it has no way to
# browse, so every upload there replaces the last one.
FIXED_LOGO_NAME = "logo.png"

_UNSAFE_NAME_CHARS = re.compile(r"[^A-Za-z0-9 _-]+")


@dataclass
class ImportResult:
  ok: bool
  name: str = ""
  error: str = ""
  warnings: list[str] = field(default_factory=list)


def branding_dir() -> Path:
  return Path(Paths.branding_root())


def ensure_branding_dir() -> Path | None:
  path = branding_dir()
  try:
    path.mkdir(parents=True, exist_ok=True)
  except OSError:
    cloudlog.exception("logo_store: could not create branding directory")
    return None
  return path


def list_logos() -> list[str]:
  """Basenames of every usable logo file, case-insensitively sorted. Never raises."""
  try:
    entries = branding_dir().iterdir()
    return sorted((p.name for p in entries if p.is_file() and p.suffix.lower() in LOGO_EXTENSIONS), key=str.lower)
  except OSError:
    return []


def resolve_logo(name: str) -> Path | None:
  """Map a stored basename to a real file inside the branding directory, or None.

  The name comes from a param, which is remotely writable, so it is treated as untrusted:
  never joined blindly onto a path, and the result must still be inside the directory after
  symlinks are followed.
  """
  if not name or os.path.basename(name) != name or name in (".", ".."):
    return None

  root = branding_dir()
  candidate = root / name
  try:
    if not candidate.is_file():
      return None
    if candidate.resolve().parent != root.resolve():
      return None
  except OSError:
    return None
  return candidate


def load_logo_rgba(path: Path | str, max_width: int, max_height: int) -> Image.Image | None:
  """Decode a logo and fit it inside the given box. RGBA out, or None on any failure.

  Called from the render path, so it swallows everything. The file may never have been through
  import_logo(), which is why the dimension caps are repeated here.
  """
  try:
    with Image.open(path) as img:
      width, height = img.size
      # Checked before load(), because the header gives us the dimensions without decoding any
      # pixels. This ordering is the decompression bomb defense; doing it later would be pointless.
      if width > MAX_SOURCE_DIM or height > MAX_SOURCE_DIM or width * height > MAX_DECODED_PIXELS:
        cloudlog.warning(f"logo_store: refusing oversized logo {width}x{height}")
        return None
      rgba = _first_frame_rgba(img)

    cropped = _crop_transparent_margins(rgba)
    if cropped is None:
      return None

    cropped.thumbnail((max(1, max_width), max(1, max_height)), Image.LANCZOS)
    return cropped
  except Exception:
    cloudlog.exception("logo_store: failed to load logo")
    return None


def import_logo(data: bytes, filename: str) -> ImportResult:
  """Validate, normalize and store an uploaded image. The order of the checks is what makes it safe."""
  warnings: list[str] = []

  if len(data) > MAX_UPLOAD_BYTES:
    return ImportResult(False, error=f"That file is too big. The limit is {MAX_UPLOAD_BYTES // (1024 * 1024)} MB.")

  if _sniff_container(data) is None:
    return ImportResult(False, error="That does not look like a PNG, JPEG, WebP or GIF image.")

  # verify() catches structural damage and truncation, but it leaves the object unusable, so the
  # image has to be opened a second time to actually read it.
  try:
    with Image.open(io.BytesIO(data)) as probe:
      probe.verify()
  except Exception:
    return ImportResult(False, error="That image file is damaged or incomplete.")

  try:
    with Image.open(io.BytesIO(data)) as img:
      width, height = img.size
      if width > MAX_SOURCE_DIM or height > MAX_SOURCE_DIM:
        return ImportResult(False, error=f"That image is {width}x{height}. The limit is {MAX_SOURCE_DIM} pixels on a side.")
      if width * height > MAX_DECODED_PIXELS:
        return ImportResult(False, error=f"That image has too many pixels. The limit is {MAX_DECODED_PIXELS // 1_000_000} megapixels.")
      if width < MIN_SOURCE_WIDTH or height < MIN_SOURCE_HEIGHT:
        return ImportResult(False, error=f"That image is {width}x{height}, too small to show clearly. " +
                                         f"Use at least {MIN_SOURCE_WIDTH}x{MIN_SOURCE_HEIGHT}.")
      if getattr(img, "n_frames", 1) > 1:
        warnings.append("That image is animated. Only the first frame will be shown.")
      rgba = _first_frame_rgba(img)
  except Exception:
    return ImportResult(False, error="That image could not be read.")

  cropped = _crop_transparent_margins(rgba)
  if cropped is None:
    return ImportResult(False, error="That image is completely transparent, so there would be nothing to show.")

  cropped.thumbnail((STORED_MAX_WIDTH, STORED_MAX_HEIGHT), Image.LANCZOS)

  warning = visibility_warning(cropped)
  if warning:
    warnings.append(warning)

  name = safe_stored_name(filename)
  root = ensure_branding_dir()
  if root is None:
    return ImportResult(False, error="Could not write to the device storage.")

  _sweep_stale_temp_files(root)

  destination = root / name
  temp = root / (name + TEMP_SUFFIX)
  try:
    with open(temp, "wb") as f:
      cropped.save(f, format="PNG")
      f.flush()
      os.fsync(f.fileno())
    os.replace(temp, destination)
  except OSError:
    cloudlog.exception("logo_store: failed to store logo")
    temp.unlink(missing_ok=True)
    return ImportResult(False, error="Could not save the logo to the device.")

  return ImportResult(True, name=name, warnings=warnings)


def _sweep_stale_temp_files(root: Path):
  """Clear part-written uploads left behind by a power cut mid-write.

  They are invisible to list_logos() because of the extension filter, so they never show up as
  a broken logo, but without this they accumulate quietly.
  """
  try:
    for path in root.glob("*" + TEMP_SUFFIX):
      path.unlink(missing_ok=True)
  except OSError:
    cloudlog.exception("logo_store: could not clear stale temporary files")


def delete_logo(name: str) -> bool:
  path = resolve_logo(name)
  if path is None:
    return False
  try:
    path.unlink()
  except OSError:
    cloudlog.exception("logo_store: failed to delete logo")
    return False
  return True


def safe_stored_name(filename: str) -> str:
  """Derive a storable basename from a client-supplied filename. Never trusted, never joined raw."""
  stem = Path(os.path.basename(filename or "")).stem
  stem = _UNSAFE_NAME_CHARS.sub("", stem).strip()
  stem = stem[:MAX_STORED_NAME_LEN].strip()
  return f"{stem or 'logo'}.png"


def display_name(name: str) -> str:
  return Path(name).stem


def channel_means(img: Image.Image) -> tuple[float, float, float]:
  """Mean red, green and blue over the pixels that actually get drawn, each 0-255."""
  array = np.asarray(img, dtype=np.float32)
  if array.ndim != 3 or array.shape[2] < 4:
    return 255.0, 255.0, 255.0
  visible = array[..., 3] >= ALPHA_FLOOR
  if not visible.any():
    return 0.0, 0.0, 0.0
  rgb = array[..., :3][visible]
  return float(rgb[:, 0].mean()), float(rgb[:, 1].mean()), float(rgb[:, 2].mean())


def visibility_warning(img: Image.Image) -> str:
  """Warn if some tint would make this logo vanish. Empty string if it is fine.

  The screen saver draws the logo with a full saturation tint that changes on every bounce, and
  the tint multiplies each color channel. At full saturation there is always a tint that lights
  one channel alone, so the worst case a logo ever faces is its own weakest channel: a pure red
  logo goes to solid black under any cyan, green or blue tint, and vanishes for roughly a third
  of every cycle. That makes the weakest channel mean the whole answer, with no need to sample
  the hue wheel.
  """
  red, green, blue = channel_means(img)
  if min(red, green, blue) >= MIN_CHANNEL_MEAN:
    return ""

  if max(red, green, blue) < MIN_CHANNEL_MEAN:
    return "That logo is very dark, so it will be hard to see against the black screen saver background."

  return ("That logo is nearly all one color, so it will disappear each time the screen saver tints it a " +
          "different one. A white logo on a transparent background avoids this.")


def _sniff_container(data: bytes) -> str | None:
  """Identify the container from its magic bytes. The extension and Content-Type are both lies."""
  if data.startswith(b"\x89PNG\r\n\x1a\n"):
    return "png"
  if data.startswith(b"\xff\xd8\xff"):
    return "jpeg"
  if data.startswith((b"GIF87a", b"GIF89a")):
    return "gif"
  if data[:4] == b"RIFF" and data[8:12] == b"WEBP":
    return "webp"
  return None


def _first_frame_rgba(img: Image.Image) -> Image.Image:
  if getattr(img, "n_frames", 1) > 1:
    img.seek(0)
  # convert() returns a new image that owns its data, so it stays valid after the file is closed
  return img.convert("RGBA")


def _crop_transparent_margins(img: Image.Image) -> Image.Image | None:
  """Trim fully transparent borders, or None if the whole image is transparent.

  Worth doing: user PNGs routinely carry large empty margins, and without the crop the bounce
  would be computed against the padding and the logo would stop short of the screen edges.
  """
  bbox = img.getbbox()
  if bbox is None:
    return None
  return img.crop(bbox)
