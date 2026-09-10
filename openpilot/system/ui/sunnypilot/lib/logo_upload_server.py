"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

A single page upload form for screen saver logos, served by the UI process.

The point of this over copyparty is that it can answer. copyparty takes whatever you give it and
says nothing, so a file that is too large or is not really an image is only discovered later, as
a screen saver that silently shows text. Here the upload runs through import_logo() and the
result comes straight back to the phone that sent it.

The server only exists while the upload dialog is on screen. It is started in show_event and
stopped in hide_event, so there is no listening port at any other time.
"""
from __future__ import annotations

import socket
import threading
from email.parser import BytesParser
from email.policy import default as default_policy
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

from openpilot.common.swaglog import cloudlog
from openpilot.system.ui.sunnypilot.lib.logo_store import MAX_UPLOAD_BYTES, ImportResult, import_logo

# copyparty already uses 8080
PORT = 8081

# Refuse a body larger than an image we would accept anyway, with a little slack for the
# multipart envelope
MAX_BODY_BYTES = MAX_UPLOAD_BYTES + 64 * 1024

PAGE = """<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Screen saver logo</title>
<style>
  :root { color-scheme: dark; }
  body { margin: 0; padding: 2rem 1.25rem 4rem; background: #101314; color: #e8edea;
         font-family: system-ui, -apple-system, "Segoe UI", Roboto, sans-serif; line-height: 1.55; }
  main { max-width: 32rem; margin: 0 auto; }
  h1 { font-size: 1.5rem; margin: 0 0 .5rem; }
  p { margin: 0 0 1rem; color: #9ba6a2; }
  .card { background: #191d1e; border: 1px solid #272e2f; border-radius: 12px; padding: 1.25rem; margin-bottom: 1.25rem; }
  input[type=file] { width: 100%%; color: #e8edea; margin-bottom: 1rem; }
  button { width: 100%%; padding: .9rem 1rem; font-size: 1rem; font-weight: 600; border: 0;
           border-radius: 8px; background: #2fe0a6; color: #06231b; }
  button:active { background: #23bd8b; }
  ul { margin: 0; padding-left: 1.15rem; color: #9ba6a2; }
  li { margin-bottom: .35rem; }
  .ok   { border-color: #2fe0a6; }
  .bad  { border-color: #e5695f; }
  .warn { border-color: #edab4c; }
  .result h2 { font-size: 1.1rem; margin: 0 0 .5rem; }
  .ok h2 { color: #2fe0a6; } .bad h2 { color: #e5695f; } .warn h2 { color: #edab4c; }
  a { color: #2fe0a6; }
</style>
</head>
<body>
<main>
  <h1>Screen saver logo</h1>
  <p>%(intro)s</p>
  %(result)s
  <form class="card" method="post" enctype="multipart/form-data" action="/">
    <input type="file" name="logo" accept="image/png,image/jpeg,image/webp,image/gif" required>
    <button type="submit">Upload</button>
  </form>
  <div class="card">
    <h2 style="font-size:1rem;margin:0 0 .6rem">What works best</h2>
    <ul>
      <li>A white logo on a transparent background. The screen saver recolors the picture as it
          moves, so a picture that is all one strong color disappears whenever it is tinted with
          the opposite one.</li>
      <li>PNG keeps transparency. JPEG does not.</li>
      <li>Up to 4096 pixels a side and 8 megapixels. Bigger is refused, and there is no benefit:
          it is shown at about a third of the screen.</li>
      <li>Animated pictures show their first frame only.</li>
    </ul>
  </div>
</main>
</body>
</html>
"""

RESULT_BLOCK = """<div class="card result %(kind)s"><h2>%(heading)s</h2>%(detail)s</div>"""


def _escape(text: str) -> str:
  return text.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")


def default_route_ip() -> str | None:
  """The address of whichever interface would reach the outside world. Nothing is sent."""
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  try:
    sock.connect(("8.8.8.8", 53))
    return sock.getsockname()[0]
  except OSError:
    return None
  finally:
    sock.close()


ADD_INTRO = "Send a picture to your device. It appears in the list under Display settings once it arrives."
REPLACE_INTRO = ("Send a picture to your device. This device keeps one picture, so whatever you send " +
                 "<strong>replaces the one already on it</strong>.")


def _render_page(result: ImportResult | None, replaces_existing: bool = False) -> bytes:
  block = ""
  if result is not None:
    if not result.ok:
      block = RESULT_BLOCK % {"kind": "bad", "heading": "Not accepted",
                              "detail": f"<p>{_escape(result.error)}</p>"}
    else:
      warnings = "".join(f"<p>{_escape(w)}</p>" for w in result.warnings)
      kind = "warn" if result.warnings else "ok"
      heading = f"Uploaded {_escape(result.name)}"
      detail = warnings + "<p>Choose it on the device under Display settings.</p>"
      block = RESULT_BLOCK % {"kind": kind, "heading": heading, "detail": detail}
  intro = REPLACE_INTRO if replaces_existing else ADD_INTRO
  return (PAGE % {"result": block, "intro": intro}).encode("utf-8")


class _Handler(BaseHTTPRequestHandler):
  protocol_version = "HTTP/1.1"

  @property
  def _server(self) -> LogoUploadServer:
    return self.server.logo_server  # type: ignore[attr-defined]

  def log_message(self, fmt, *args):
    pass  # BaseHTTPRequestHandler logs to stderr by default, which is noise in the UI process

  def _reply(self, body: bytes, status: int = 200):
    self.send_response(status)
    self.send_header("Content-Type", "text/html; charset=utf-8")
    self.send_header("Content-Length", str(len(body)))
    self.send_header("Cache-Control", "no-store")
    self.end_headers()
    self.wfile.write(body)

  def do_GET(self):
    if self.path not in ("/", "/index.html"):
      self._reply(b"not found", 404)
      return
    self._reply(_render_page(None, self._server.replaces_existing))

  def do_POST(self):
    if self.path != "/":
      self._reply(b"not found", 404)
      return

    result = self._handle_upload()
    self._server.publish(result)
    self._reply(_render_page(result, self._server.replaces_existing))

  def _handle_upload(self) -> ImportResult:
    try:
      length = int(self.headers.get("Content-Length") or 0)
    except ValueError:
      return ImportResult(False, error="That upload was malformed.")

    # Checked before reading, so an enormous body is never buffered
    if length <= 0:
      return ImportResult(False, error="No picture was attached.")
    if length > MAX_BODY_BYTES:
      return ImportResult(False, error=f"That file is too big. The limit is {MAX_UPLOAD_BYTES // (1024 * 1024)} MB.")

    content_type = self.headers.get("Content-Type", "")
    if "multipart/form-data" not in content_type:
      return ImportResult(False, error="That upload was malformed.")

    try:
      # Content-Length is a claim, so the read is capped independently
      body = self.rfile.read(min(length, MAX_BODY_BYTES))
      message = BytesParser(policy=default_policy).parsebytes(
        b"Content-Type: " + content_type.encode("utf-8", "replace") + b"\r\n\r\n" + body
      )
      for part in message.iter_parts():
        filename = part.get_filename()
        if filename:
          payload = part.get_payload(decode=True)
          if not payload:
            return ImportResult(False, error="No picture was attached.")
          # A fixed name means every upload lands on the same file, for a UI with no picker
          return import_logo(payload, self._server.fixed_name or filename)
    except Exception:
      cloudlog.exception("logo upload: failed to read the request")
      return ImportResult(False, error="That upload could not be read.")

    return ImportResult(False, error="No picture was attached.")


class LogoUploadServer:
  """Owns the listening socket. Start it when the dialog opens, stop it when the dialog closes."""

  def __init__(self, port: int = PORT, fixed_name: str | None = None):
    self._port = port
    self.fixed_name = fixed_name
    self._httpd: ThreadingHTTPServer | None = None
    self._thread: threading.Thread | None = None
    self._lock = threading.Lock()
    self._result: ImportResult | None = None
    self._address: str | None = None

  @property
  def running(self) -> bool:
    return self._httpd is not None

  @property
  def replaces_existing(self) -> bool:
    """True when uploads overwrite one another, which the page has to say out loud."""
    return self.fixed_name is not None

  @property
  def url(self) -> str | None:
    if self._address is None:
      return None
    # The scheme is spelled out because a browser given a bare address now assumes https,
    # which this server does not speak
    return f"http://{self._address}:{self._port}/"

  def start(self) -> bool:
    if self._httpd is not None:
      return True

    self._address = default_route_ip()
    if self._address is None:
      return False

    try:
      httpd = ThreadingHTTPServer(("0.0.0.0", self._port), _Handler)
    except OSError:
      cloudlog.exception("logo upload: could not open the port")
      self._address = None
      return False

    httpd.daemon_threads = True
    httpd.logo_server = self  # type: ignore[attr-defined]
    self._httpd = httpd
    self._thread = threading.Thread(target=httpd.serve_forever, kwargs={"poll_interval": 0.2}, daemon=True)
    self._thread.start()
    return True

  def stop(self):
    httpd, thread = self._httpd, self._thread
    self._httpd, self._thread, self._address = None, None, None
    if httpd is None:
      return
    try:
      httpd.shutdown()
      httpd.server_close()
    except Exception:
      cloudlog.exception("logo upload: failed to stop cleanly")
    if thread is not None:
      thread.join(timeout=2.0)

  def publish(self, result: ImportResult):
    with self._lock:
      self._result = result

  def take_result(self) -> ImportResult | None:
    """Hand the last upload to the UI thread, once."""
    with self._lock:
      result, self._result = self._result, None
    return result
