"""Tests for screen saver logo storage and upload validation.

Only needs Pillow and numpy, both core dependencies, so this runs anywhere. Every test points
logo_store at a tmp_path so nothing touches the real branding directory.
"""
import io

import pytest
from PIL import Image

from openpilot.system.ui.sunnypilot.lib import logo_store


@pytest.fixture(autouse=True)
def branding_dir(tmp_path, monkeypatch):
  monkeypatch.setattr(logo_store.Paths, "branding_root", staticmethod(lambda: str(tmp_path)))
  return tmp_path


def encode(width, height, color=(255, 255, 255, 255), fmt="PNG"):
  image = Image.new("RGBA", (width, height), color)
  if fmt == "JPEG":
    image = image.convert("RGB")
  buffer = io.BytesIO()
  image.save(buffer, format=fmt)
  return buffer.getvalue()


class TestImportLogo:
  def test_accepts_a_plain_png(self, branding_dir):
    result = logo_store.import_logo(encode(800, 400), "MyLogo.png")
    assert result.ok and result.name == "MyLogo.png" and result.warnings == []
    assert (branding_dir / "MyLogo.png").is_file()

  def test_accepts_jpeg(self):
    assert logo_store.import_logo(encode(400, 300, (200, 200, 200, 255), "JPEG"), "photo.jpg").ok

  def test_rejects_an_oversized_upload(self):
    data = b"\x89PNG\r\n\x1a\n" + b"x" * (logo_store.MAX_UPLOAD_BYTES + 1)
    assert not logo_store.import_logo(data, "big.png").ok

  def test_rejects_a_non_image(self):
    assert not logo_store.import_logo(b"this is not an image at all", "evil.png").ok

  def test_trusts_magic_bytes_not_the_extension(self):
    # A .png extension on something that is not a PNG must not get through
    assert not logo_store.import_logo(b"GIF87", "actually_a_gif.png").ok

  def test_rejects_dimensions_over_the_cap(self):
    assert not logo_store.import_logo(encode(logo_store.MAX_SOURCE_DIM + 100, 100), "wide.png").ok

  def test_rejects_too_many_pixels(self):
    assert not logo_store.import_logo(encode(4000, 3000), "huge.png").ok

  def test_rejects_something_too_small_to_see(self):
    assert not logo_store.import_logo(encode(32, 16), "tiny.png").ok

  def test_rejects_a_fully_transparent_image(self):
    assert not logo_store.import_logo(encode(200, 100, (0, 0, 0, 0)), "blank.png").ok

  def test_warns_but_accepts_a_very_dark_logo(self):
    result = logo_store.import_logo(encode(200, 100, (8, 8, 10, 255)), "dark.png")
    assert result.ok and any("dark" in w for w in result.warnings)

  def test_warns_about_a_saturated_logo_that_would_vanish(self):
    # Bright enough by any brightness measure, but it has no green or blue, so a cyan tint
    # multiplies it to solid black. This is the case the old mean-luminance check let through.
    result = logo_store.import_logo(encode(200, 100, (255, 0, 0, 255)), "red.png")
    assert result.ok, result.error
    assert any("one colour" in w for w in result.warnings), result.warnings

  def test_does_not_warn_about_white(self):
    assert logo_store.import_logo(encode(200, 100, (255, 255, 255, 255)), "white.png").warnings == []

  def test_does_not_warn_about_mid_grey(self):
    assert logo_store.import_logo(encode(200, 100, (128, 128, 128, 255)), "grey.png").warnings == []

  def test_warns_about_animation_and_keeps_the_first_frame(self, branding_dir):
    frames = [Image.new("RGBA", (300, 200), (255, 0, 0, 255)), Image.new("RGBA", (300, 200), (0, 255, 0, 255))]
    buffer = io.BytesIO()
    frames[0].save(buffer, format="GIF", save_all=True, append_images=frames[1:])

    result = logo_store.import_logo(buffer.getvalue(), "spin.gif")
    assert result.ok and any("animated" in w.lower() for w in result.warnings)
    assert result.name == "spin.png" and (branding_dir / "spin.png").is_file()

  def test_crops_transparent_margins(self, branding_dir):
    image = Image.new("RGBA", (1000, 1000), (0, 0, 0, 0))
    image.paste((255, 0, 0, 255), (400, 400, 600, 600))
    buffer = io.BytesIO()
    image.save(buffer, format="PNG")

    assert logo_store.import_logo(buffer.getvalue(), "padded.png").ok
    # Without the crop the bounce would be computed against the padding
    assert Image.open(branding_dir / "padded.png").size == (200, 200)

  def test_normalizes_down_to_the_stored_size(self, branding_dir):
    assert logo_store.import_logo(encode(3000, 1000), "wide.png").ok
    width, height = Image.open(branding_dir / "wide.png").size
    assert width <= logo_store.STORED_MAX_WIDTH and height <= logo_store.STORED_MAX_HEIGHT

  def test_strips_directories_from_the_supplied_name(self):
    result = logo_store.import_logo(encode(200, 100), "../../etc/passwd.png")
    assert result.ok and result.name == "passwd.png"


class TestVisibility:
  """The tint is a per channel multiply, so the weakest channel decides the worst case."""

  @pytest.mark.parametrize(("colour", "expected"), [
    ((255, 255, 255), ""),          # white survives every tint
    ((128, 128, 128), ""),          # grey is dimmer but never vanishes
    ((255, 0, 0), "one colour"),    # no green or blue: black under a cyan tint
    ((0, 255, 255), "one colour"),  # no red: black under a red tint
    ((10, 10, 12), "dark"),         # nothing to light up at all
  ])
  def test_warning_matches_the_weakest_channel(self, colour, expected):
    image = Image.new("RGBA", (64, 64), (*colour, 255))
    warning = logo_store.visibility_warning(image)
    assert (expected in warning) if expected else (warning == "")

  def test_transparent_pixels_are_ignored(self):
    # A logo is mostly empty space; only the drawn pixels can affect visibility
    image = Image.new("RGBA", (64, 64), (255, 255, 255, 0))
    image.paste((255, 255, 255, 255), (20, 20, 40, 40))
    assert logo_store.visibility_warning(image) == ""


class TestNames:
  def test_falls_back_when_nothing_survives_sanitizing(self):
    assert logo_store.safe_stored_name("!!!.png") == "logo.png"

  def test_caps_the_length(self):
    name = logo_store.safe_stored_name("a" * 100 + ".png")
    assert len(name) <= logo_store.MAX_STORED_NAME_LEN + len(".png")


class TestResolveLogo:
  @pytest.mark.parametrize("name", ["", "..", "sub/logo.png", "../../etc/passwd", "missing.png"])
  def test_refuses_anything_that_is_not_a_plain_local_file(self, name):
    assert logo_store.resolve_logo(name) is None

  def test_resolves_a_stored_logo(self):
    logo_store.import_logo(encode(200, 100), "good.png")
    assert logo_store.resolve_logo("good.png") is not None


class TestListLogos:
  def test_empty_when_the_directory_does_not_exist(self):
    assert logo_store.list_logos() == []

  def test_sorted_case_insensitively(self):
    for name in ("zebra.png", "Apple.png", "mango.png"):
      logo_store.import_logo(encode(200, 100), name)
    assert logo_store.list_logos() == ["Apple.png", "mango.png", "zebra.png"]


class TestLoadLogoRgba:
  """The render path. A file can arrive over copyparty without ever meeting import_logo()."""

  def test_loads_and_fits_the_box(self, branding_dir):
    logo_store.import_logo(encode(3000, 1000), "wide.png")
    image = logo_store.load_logo_rgba(branding_dir / "wide.png", 720, 360)
    assert image is not None and image.mode == "RGBA"
    assert image.width <= 720 and image.height <= 360

  def test_returns_none_for_garbage(self, branding_dir):
    (branding_dir / "garbage.png").write_bytes(b"not an image")
    assert logo_store.load_logo_rgba(branding_dir / "garbage.png", 720, 360) is None

  def test_returns_none_for_a_missing_file(self, branding_dir):
    assert logo_store.load_logo_rgba(branding_dir / "nothere.png", 720, 360) is None

  def test_refuses_an_unvalidated_oversized_file(self, branding_dir):
    # Dropped straight into the directory over copyparty, so the import checks never ran
    Image.new("RGBA", (logo_store.MAX_SOURCE_DIM + 100, 900)).save(branding_dir / "huge.png")
    assert logo_store.load_logo_rgba(branding_dir / "huge.png", 720, 360) is None
