import io
from email.message import Message
from types import SimpleNamespace

from openpilot.system.webrtc.webrtcd import WebrtcdHandler, cors_origin_allowed


def test_cors_origins():
  assert cors_origin_allowed("https://inertiaimpact.github.io")
  assert not cors_origin_allowed("https://inertiaimpact.github.io.example.com")
  assert not cors_origin_allowed("http://inertiaimpact.github.io")
  assert not cors_origin_allowed("https://example.com")
  assert not cors_origin_allowed(None)  # the opview app sends no Origin
  assert not cors_origin_allowed("")


def _response(origin: str | None, method: str = "OPTIONS", path: str = "/stream", private_network: bool = False) -> str:
  handler = WebrtcdHandler.__new__(WebrtcdHandler)
  headers = Message()
  if origin is not None:
    headers["Origin"] = origin
  if private_network:
    headers["Access-Control-Request-Private-Network"] = "true"
  handler.headers = headers
  handler.command, handler.path, handler.request_version = method, path, "HTTP/1.1"
  handler.wfile = io.BytesIO()
  handler.server = SimpleNamespace()
  handler.requestline = f"{method} {path} HTTP/1.1"
  handler.client_address = ("127.0.0.1", 0)
  getattr(handler, f"do_{method}")()
  return handler.wfile.getvalue().decode()


def test_preflight_from_the_web_client_is_allowed():
  resp = _response("https://inertiaimpact.github.io", private_network=True)
  assert resp.startswith("HTTP/1.1 204")
  assert "Access-Control-Allow-Origin: https://inertiaimpact.github.io" in resp
  assert "Access-Control-Allow-Private-Network: true" in resp


def test_other_sites_and_the_app_get_no_cors_headers():
  for origin in ("https://example.com", None):
    assert "Access-Control-Allow-Origin" not in _response(origin)


def test_preflight_on_an_unknown_path_is_not_found():
  assert _response("https://inertiaimpact.github.io", path="/nope").startswith("HTTP/1.1 404")
