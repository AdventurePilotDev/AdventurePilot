"""Tests for the screen saver logo upload page.

Drives the real HTTP server over a real socket, because the point of this server over copyparty
is that it answers, and an answer is only worth having if it is the right one.
"""
import io
import socket
import urllib.error
import urllib.request

import pytest
from PIL import Image

from openpilot.system.ui.sunnypilot.lib import logo_store
from openpilot.system.ui.sunnypilot.lib.logo_upload_server import LogoUploadServer


@pytest.fixture
def branding_dir(tmp_path, monkeypatch):
  monkeypatch.setattr(logo_store.Paths, "branding_root", staticmethod(lambda: str(tmp_path)))
  return tmp_path


@pytest.fixture
def server(branding_dir):
  # Port 0 lets the OS pick a free one, so parallel test runs cannot collide
  srv = LogoUploadServer(port=0)
  assert srv.start(), "no route to the outside world, cannot bind"
  yield srv
  srv.stop()
  assert not srv.running


def _base_url(server):
  return f"http://127.0.0.1:{server._httpd.server_address[1]}/"


def _png(width=800, height=400, color=(255, 255, 255, 255)):
  buffer = io.BytesIO()
  Image.new("RGBA", (width, height), color).save(buffer, format="PNG")
  return buffer.getvalue()


def _post(server, data: bytes, filename: str = "logo.png") -> str:
  boundary = "----test-boundary"
  header = (f"--{boundary}\r\nContent-Disposition: form-data; name=\"logo\"; filename=\"{filename}\"\r\n" +
            "Content-Type: application/octet-stream\r\n\r\n")
  body = header.encode() + data + f"\r\n--{boundary}--\r\n".encode()
  request = urllib.request.Request(_base_url(server), data=body,
                                   headers={"Content-Type": f"multipart/form-data; boundary={boundary}"})
  with urllib.request.urlopen(request, timeout=10) as response:
    return response.read().decode()


class TestPage:
  def test_get_serves_the_form(self, server):
    with urllib.request.urlopen(_base_url(server), timeout=10) as response:
      body = response.read().decode()
    assert response.status == 200
    assert 'type="file"' in body and 'method="post"' in body

  def test_unknown_path_is_not_found(self, server):
    with pytest.raises(urllib.error.HTTPError) as excinfo:
      urllib.request.urlopen(_base_url(server) + "secrets", timeout=10)
    assert excinfo.value.code == 404

  def test_url_names_the_scheme_explicitly(self, server):
    # A bare address makes browsers assume https, which this server does not speak
    assert server.url.startswith("http://")


class TestUpload:
  def test_accepts_a_valid_picture(self, server, branding_dir):
    body = _post(server, _png(), "MyLogo.png")
    assert "Uploaded MyLogo.png" in body
    assert (branding_dir / "MyLogo.png").is_file()

  def test_says_why_a_bad_file_was_refused(self, server, branding_dir):
    body = _post(server, b"this is not an image", "evil.png")
    assert "Not accepted" in body and "PNG" in body
    assert list(branding_dir.iterdir()) == []

  def test_reports_the_reason_for_an_oversized_picture(self, server):
    body = _post(server, _png(5000, 100), "wide.png")
    assert "Not accepted" in body and "4096" in body

  def test_warns_about_a_dark_picture_but_keeps_it(self, server, branding_dir):
    body = _post(server, _png(400, 200, (6, 6, 8, 255)), "dark.png")
    assert "Uploaded dark.png" in body and "dark" in body
    assert (branding_dir / "dark.png").is_file()

  def test_result_reaches_the_ui_thread_once(self, server):
    _post(server, _png(), "Once.png")
    result = server.take_result()
    assert result is not None and result.ok and result.name == "Once.png"
    assert server.take_result() is None, "a result must not be delivered twice"

  def test_escapes_the_filename_in_the_reply(self, server):
    # The filename is attacker-controlled and lands in an HTML page
    body = _post(server, _png(), "<script>x</script>.png")
    assert "<script>x</script>" not in body

  def test_rejects_an_empty_body(self, server):
    request = urllib.request.Request(_base_url(server), data=b"",
                                     headers={"Content-Type": "multipart/form-data; boundary=x"})
    with urllib.request.urlopen(request, timeout=10) as response:
      assert "No picture was attached" in response.read().decode()


class TestLifetime:
  def test_stops_listening_when_closed(self, branding_dir):
    srv = LogoUploadServer(port=0)
    assert srv.start()
    url = _base_url(srv)
    srv.stop()
    assert not srv.running and srv.url is None
    with pytest.raises((urllib.error.URLError, ConnectionError, socket.timeout)):
      urllib.request.urlopen(url, timeout=3)
