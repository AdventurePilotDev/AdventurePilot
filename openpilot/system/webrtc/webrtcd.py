#!/usr/bin/env python3

from abc import abstractmethod
from collections.abc import Callable
import os
import socket
import time
import traceback
import capnp
import argparse
import asyncio
import contextlib
import json
import uuid
import logging
import signal
import threading
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs
from typing import Any

from openpilot.system.webrtc import opview_slim
from openpilot.system.webrtc.helpers import StreamRequestBody
from openpilot.system.webrtc.schema import generate_field
from openpilot.common.params import Params, UnknownKeyName
from openpilot.common.swaglog import cloudlog
from openpilot.cereal import messaging, log
from opendbc.car.structs import car

SESSION_TIMEOUT_SECONDS = 300

OPVIEW_PARAMS_INTERVAL_S = 1.
# data health is counted in Python only: calling channel.buffered_amount() from the send loop crashed
# webrtcd (route 000000e6). libdatachannel's send() returns False when it queued a message instead of
# sending it straight away, which is counted as "queued"
# outgoing data health in the saved logs: a summary this often, failures at most this often
OPVIEW_STATS_INTERVAL_S = 60.
OPVIEW_FAILURE_LOG_INTERVAL_S = 1.


def _xyz(v) -> dict[str, list[float]]:
  # metres, rounded to the millimetre: far shorter JSON than full float precision
  return {"x": [round(a, 3) for a in v.x], "y": [round(a, 3) for a in v.y], "z": [round(a, 3) for a in v.z]}


def opview_enabled(params: Params) -> bool:
  # a prebuilt whose params library predates this setting does not know the key: treat as off
  try:
    return params.get_bool("OpviewEnabled")
  except UnknownKeyName:
    return False


def incoming_services(requested: list[str], opview: bool) -> list[str]:
  # with opview on this server is open to the whole local network with no login, and the app only ever reads:
  # nothing a client sends may be published into openpilot
  return [] if opview else requested


def opview_model(m) -> dict[str, Any]:
  # only the modelV2 fields opview draws (path, lane lines, road edges, acceleration, confidence);
  # the full message is ~32 kB of JSON and most of webrtcd's work at 20 Hz
  return {
    "position": _xyz(m.position),
    "laneLines": [_xyz(line) for line in m.laneLines],
    "laneLineProbs": [round(p, 3) for p in m.laneLineProbs],
    "roadEdges": [_xyz(edge) for edge in m.roadEdges],
    "roadEdgeStds": [round(sd, 3) for sd in m.roadEdgeStds],
    "acceleration": {"x": [round(a, 3) for a in m.acceleration.x]},
    # for the confidence ball: how likely the model thinks a takeover is
    "meta": {"disengagePredictions": {
      "brakeDisengageProbs": [round(p, 3) for p in m.meta.disengagePredictions.brakeDisengageProbs],
      "steerOverrideProbs": [round(p, 3) for p in m.meta.disengagePredictions.steerOverrideProbs],
    }},
  }


# ice candidate parser for logging
def _ice_candidates(sdp: str) -> list[str]:
  return [line.removeprefix("a=") for line in sdp.splitlines() if line.startswith("a=candidate:")]

# socket trick: route lookup for 8.8.8.8 (nothing is sent or actually connected to)
# return the source interfaces IP which is the default interface of the device
def _default_route_ip() -> str | None:
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  try:
    s.connect(("8.8.8.8", 53))  # selects a route, sends nothing
    return s.getsockname()[0]
  except OSError:
    return None
  finally:
    s.close()

class AsyncTaskRunner:
  def __init__(self):
    self.task = None
    self.logger = logging.getLogger("webrtcd")

  def start(self):
    assert self.task is None
    self.task = asyncio.create_task(self.run())

  async def stop(self):
    if self.task is None:
      return
    task = self.task
    self.task = None
    if task.done():
      return
    task.cancel()
    with contextlib.suppress(asyncio.CancelledError):
      await task

  @abstractmethod
  async def run(self):
    pass


class CerealOutgoingMessageProxy(AsyncTaskRunner):
  def __init__(self, services: list[str], enabled: bool = True, opview: bool = False):
    super().__init__()
    self.services = list(services)
    self.sm = messaging.SubMaster(self.services)
    self.channels = []
    self._enabled = enabled
    # opview: cap each service's rate and also send the UI settings opview cannot otherwise see
    self.opview = opview
    self.params = Params()
    self._last_sent: dict[str, float] = {}
    self._last_params_sent = 0.
    self._car: tuple[str, int] | None = None
    # data health, for the saved logs (swaglog): a frozen opview overlay can then be traced to the comma or not
    self._stats = {"sent": 0, "queued": 0, "closed": 0, "failures": 0, "bytes": 0}
    self._last_stats_log = time.monotonic()
    self._last_failure_log = 0.
    self._queued_in_row = 0  # a long run of queued sends means the channel is backed up
    self._max_queued_in_row = 0

  def add_channel(self, channel):
    self.channels.append(channel)

  def enable(self, enable: bool):
    self._enabled = enable

  def to_json(self, msg_content: Any):
    if isinstance(msg_content, capnp._DynamicStructReader):
      msg_dict = msg_content.to_dict()
    elif isinstance(msg_content, capnp._DynamicListReader):
      msg_dict = [self.to_json(msg) for msg in msg_content]
    elif isinstance(msg_content, bytes):
      msg_dict = msg_content.decode()
    else:
      msg_dict = msg_content

    return msg_dict

  def _send(self, encoded_msg: bytes):
    for channel in self.channels:
      if not channel.is_open():
        self._stats["closed"] += 1
        continue
      if channel.send(encoded_msg):
        self._stats["sent"] += 1
        self._queued_in_row = 0
      else:
        self._stats["queued"] += 1
        self._queued_in_row += 1
        self._max_queued_in_row = max(self._max_queued_in_row, self._queued_in_row)
      self._stats["bytes"] += len(encoded_msg)

  def _log_stats(self, now: float):
    if not self.opview or now - self._last_stats_log < OPVIEW_STATS_INTERVAL_S:
      return
    cloudlog.event("webrtcd.opview.data_stats", **self._stats, max_queued_in_row=self._max_queued_in_row,
                   seconds=round(now - self._last_stats_log, 1))
    self._stats = dict.fromkeys(self._stats, 0)
    self._max_queued_in_row = 0
    self._last_stats_log = now

  def _log_failure(self):
    self._stats["failures"] += 1
    now = time.monotonic()
    if now - self._last_failure_log >= OPVIEW_FAILURE_LOG_INTERVAL_S:
      self._last_failure_log = now
      cloudlog.event("webrtcd.opview.send_failure", failures=self._stats["failures"],
                     exception=traceback.format_exc(limit=4), error=True)

  def _car_brand_flags(self) -> tuple[str, int]:
    # carParams is only published every ~50 s, so use the copy the car interface saves at start-up
    if self._car is None:
      cp_bytes = self.params.get("CarParamsPersistent")
      if not cp_bytes:
        return "", 0
      cp = messaging.log_from_bytes(cp_bytes, car.CarParams)
      self._car = (cp.brand, cp.flags)
    return self._car

  def _param_bool(self, key: str) -> bool:
    # not every branch has every setting; an unknown one counts as off
    try:
      return self.params.get_bool(key)
    except UnknownKeyName:
      return False

  def _param_int(self, key: str) -> int:
    try:
      return int(self.params.get(key, return_default=True) or 0)
    except (UnknownKeyName, TypeError, ValueError):
      return 0

  def _opview_params(self) -> dict[str, Any]:
    brand, flags = self._car_brand_flags()
    return {
      "CarBrand": brand,
      "CarFlags": flags,
      "IsMetric": self._param_bool("IsMetric"),
      "SpeedLimitMode": self._param_int("SpeedLimitMode"),
      "RoadNameToggle": self._param_bool("RoadNameToggle"),
      "RivianForceTorqueSteer": self._param_bool("RivianForceTorqueSteer"),
      "TrueVEgoUI": self._param_bool("TrueVEgoUI"),
      "SPLiveSpeedCorrectionEnabled": self._param_bool("SPLiveSpeedCorrectionEnabled"),
      "SPCruiseSpeedOffset": self._param_int("SPCruiseSpeedOffset"),
      "ShowTurnSignals": self._param_bool("ShowTurnSignals"),
      "BlindSpot": self._param_bool("BlindSpot"),
      "LaneCenteringEnabled": self._param_bool("LaneCenteringEnabled"),
    }

  def update(self):
    # this is blocking in async context...
    self.sm.update(0)
    now = time.monotonic()
    for service, updated in self.sm.updated.items():
      if not updated:
        continue
      if self.opview:
        # opview: each service at its own rate, cut down to the fields the app reads (opview_slim.py)
        gap = opview_slim.min_gap_s(service)
        if gap is not None:
          if now - self._last_sent.get(service, 0.) < gap:
            continue
          self._last_sent[service] = now
        msg_dict = opview_model(self.sm[service]) if service == "modelV2" else opview_slim.slim(service, self.sm[service])
      else:
        msg_dict = self.to_json(self.sm[service])
      mono_time, valid = self.sm.logMonoTime[service], self.sm.valid[service]
      outgoing_msg = {"type": service, "logMonoTime": mono_time, "valid": valid, "data": msg_dict}
      self._send(json.dumps(outgoing_msg).encode())

    if self.opview and now - self._last_params_sent >= OPVIEW_PARAMS_INTERVAL_S:
      self._last_params_sent = now
      self._send(json.dumps({"type": "opviewParams", "logMonoTime": time.monotonic_ns(), "valid": True,
                             "data": self._opview_params()}).encode())
    self._log_stats(now)

  async def run(self):
    while True:
      if not self._enabled:
        await asyncio.sleep(0.01)
        continue
      try:
        self.update()
      except Exception:
        self.logger.exception("Cereal outgoing proxy failure")
        if self.opview:
          self._log_failure()
      await asyncio.sleep(0.01)


class CerealIncomingMessageProxy:
  def __init__(self, pm: messaging.PubMaster):
    self.pm = pm

  def send(self, message: bytes):
    msg_json = json.loads(message)
    msg_type, msg_data = msg_json["type"], msg_json["data"]
    size = None
    if not isinstance(msg_data, dict):
      size = len(msg_data)

    msg = messaging.new_message(msg_type, size=size)
    setattr(msg, msg_type, msg_data)
    self.pm.send(msg_type, msg)


class DynamicPubMaster(messaging.PubMaster):
  def __init__(self, *args, **kwargs):
    super().__init__(*args, **kwargs)
    self.lock = asyncio.Lock()

  async def add_services_if_needed(self, services):
    async with self.lock:
      for service in services:
        if service not in self.sock:
          self.sock[service] = messaging.pub_sock(service)


class LivestreamBitrateController(AsyncTaskRunner):
  bitrates = [500_000, 1_500_000, int(os.environ.get("STREAM_BITRATE", 5_000_000))]
  label_to_bitrate = { "high": bitrates[2], "med": bitrates[1], "low": bitrates[0]}
  sample_interval = 0.2
  high_level = 0.1 # drop immediately
  med_level = 0.05 # drop after # of samples
  low_level = 0 # raise after # of samples
  down_samples = 5
  param_name = "LivestreamEncoderBitrate"

  def __init__(self, get_stats: Callable[[], dict[str, Any]], params: Params, enabled: bool = True):
    super().__init__()
    self.get_stats = get_stats
    self.params = params

    self.level = 2
    self._publish(self.bitrates[self.level])
    self.prev_stats: tuple[Any, ...] | None = None
    self.counter = 0
    self.up_samples = 5 # 1s
    self._auto = True
    self._enabled = enabled

  def enable(self, enable: bool):
    self._enabled = enable

  async def run(self):
    while True:
      await asyncio.sleep(self.sample_interval)
      if not self._enabled:
        continue
      if not self._auto:
        continue

      loss_rate = self._sample()
      if loss_rate is None:
        continue
      if loss_rate >= self.med_level and self.level > 0:
        self.counter += 1
        if self.counter >= self.down_samples or loss_rate >= self.high_level:
          self.level -= 1
          self.up_samples *= 2 # exponential backoff before raising again
          self.counter = 0
          self._publish(self.bitrates[self.level])
      elif loss_rate <= self.low_level and self.level < len(self.bitrates) - 1:
        self.counter -= 1
        if -self.counter >= self.up_samples:
          self.level += 1
          self.counter = 0
          self._publish(self.bitrates[self.level])

  def _sample(self) -> float | None:
    report = next(iter(self.get_stats().values()), None)
    if report is None:
      return None

    current = (report.ssrc, report.fraction_lost, report.packets_lost, report.highest_seq_no, report.jitter, report.lsr, report.dlsr)
    if self.prev_stats == current:
      return None
    self.prev_stats = current

    loss_rate = report.fraction_lost / 256
    return loss_rate

  def _publish(self, bitrate: float):
    self.params.put(self.param_name, bitrate)

  def set_quality(self, quality):
    if quality in self.label_to_bitrate:
      self._publish(self.label_to_bitrate[quality])
      self._auto = False
    elif quality == "auto":
      self._auto = True


class StreamSession:
  shared_pub_master = DynamicPubMaster([])

  def __init__(self, body: StreamRequestBody):
    from openpilot.system.webrtc.device.video import LiveStreamVideoStreamTrack
    from teleoprtc.builder import WebRTCAnswerBuilder

    self.identifier = str(uuid.uuid4())
    self.params = Params()
    builder = WebRTCAnswerBuilder(body.sdp, bind_address=_default_route_ip())

    self.enabled = body.enabled
    # opview viewers sit on the local network for the whole drive, so no session cap
    self.opview = opview_enabled(self.params)
    self.session_timeout = None if self.opview else SESSION_TIMEOUT_SECONDS
    self.video_tracks = []
    for camera in body.cameras:
      track = LiveStreamVideoStreamTrack(camera, self.enabled)
      self.video_tracks.append(track)
      builder.add_video_stream(camera, track)
    self.stream = builder.stream()

    bridge_services_in = incoming_services(body.bridge_services_in, self.opview)
    self.is_body = "testJoystick" in bridge_services_in

    self.incoming_bridge: CerealIncomingMessageProxy | None = None
    self.incoming_bridge_services = bridge_services_in
    self.outgoing_bridge: CerealOutgoingMessageProxy | None = None
    self.bitrate_controller: LivestreamBitrateController | None = None
    if len(bridge_services_in) > 0:
      self.incoming_bridge = CerealIncomingMessageProxy(self.shared_pub_master)
    if len(body.bridge_services_out) > 0:
      self.outgoing_bridge = CerealOutgoingMessageProxy(body.bridge_services_out, self.enabled, opview=self.opview)
    self.bitrate_controller = LivestreamBitrateController(self.stream.get_receiver_report_stats, self.params, self.enabled)

    self.run_task: asyncio.Task | None = None
    self._cleanup_lock = asyncio.Lock()
    self._cleanup_done = False
    self.logger = logging.getLogger("webrtcd")
    cloudlog.warning(
      "New stream session (%s), video cameras %s, video enabled %s, incoming services %s, outgoing services %s",
      self.identifier, [t.id for t in self.video_tracks], body.enabled, body.bridge_services_in, body.bridge_services_out,
    )

  def start(self):
    self.run_task = asyncio.create_task(self.run())

  async def stop(self):
    if self.run_task is not None and not self.run_task.done() and self.run_task is not asyncio.current_task():
      self.run_task.cancel()
      with contextlib.suppress(asyncio.CancelledError):
        await self.run_task
    self.run_task = None
    await self.post_run_cleanup()

  async def get_answer(self):
    return await self.stream.start()

  def message_handler(self, message: bytes):
    try:
      payload = json.loads(message) if isinstance(message, (bytes, str)) else None
      if isinstance(payload, dict):
        msg_type = payload.get("type")

        match msg_type:
          case "livestreamCameraSwitch":
            # only needed for 1 track stream
            if len(self.video_tracks) == 1:
              self.video_tracks[0].switch_camera(payload["data"]["camera"])
          case "livestreamSettings":
            if self.bitrate_controller is not None:
              self.bitrate_controller.set_quality(payload["data"]["quality"])
          case "livestreamVideoEnable":
            enabled = payload["data"]["enabled"]
            self.enabled = enabled
            for track in self.video_tracks:
              track.enable(enabled)
            if self.outgoing_bridge is not None:
              self.outgoing_bridge.enable(enabled)
            if self.bitrate_controller is not None:
              self.bitrate_controller.enable(enabled)
            if not enabled:
              self.params.put("LivestreamRequestKeyframe", True)
          case "clockSync":
            pong = json.dumps({"type": "clockSync", "data": {
              "action": "pong", "browserSendTime": payload["data"]["browserSendTime"], "deviceTime": time.time() * 1000, # noqa: TID251
            }})
            self.stream.get_messaging_channel().send(pong)
          case "enableTimingSei":
            for track in self.video_tracks:
              track.timing_sei_enabled = bool(payload["data"]["enabled"])
          case _:
            if msg_type not in self.incoming_bridge_services:
              return
            if self.incoming_bridge is not None:
              self.incoming_bridge.send(message)
    except Exception:
      self.logger.exception("Cereal incoming proxy failure")

  async def run_normal_session(self):
    try:
      await asyncio.wait_for(self.stream.wait_for_disconnection(), timeout=self.session_timeout)
    except TimeoutError:
      self.logger.warning("Stream session (%s) timed out after %d s", self.identifier, self.session_timeout)
      try:
        self.stream.get_messaging_channel().send(json.dumps({"type": "disconnect", "data": "Session timed out"}))
      except Exception:
        pass

  async def run_body_session(self):
    await self.stream.wait_for_disconnection()

  async def run(self):
    try:
      self.params.put("LivestreamRequestKeyframe", True)

      # avoid datachannel race by adding messange_handler immediately
      self.stream.set_message_handler(self.message_handler)

      await asyncio.wait_for(self.stream.wait_for_connection(), timeout=15)
      if self.stream.has_messaging_channel():
        if self.incoming_bridge is not None:
          await self.shared_pub_master.add_services_if_needed(self.incoming_bridge_services)
        if self.outgoing_bridge is not None:
          channel = self.stream.get_messaging_channel()
          self.outgoing_bridge.add_channel(channel)
          self.outgoing_bridge.start()
      if self.bitrate_controller is not None:
        self.bitrate_controller.start()

      with cloudlog.ctx(session_id=self.identifier):
        cloudlog.warning("webrtcd.session.connected")
      if self.is_body:
        await self.run_body_session()
      else:
        await self.run_normal_session()
      with cloudlog.ctx(session_id=self.identifier):
        cloudlog.warning("webrtcd.session.ended")
    except Exception:
      self.logger.exception("Stream session failure")
      with cloudlog.ctx(session_id=self.identifier):
        cloudlog.exception("webrtcd.session.exception")
    finally:
      await self.post_run_cleanup()

  async def post_run_cleanup(self):
    async with self._cleanup_lock:
      if self._cleanup_done:
        return
      self._cleanup_done = True
      self.params.put("LivestreamRequestKeyframe", False)
      if self.bitrate_controller is not None:
        await self.bitrate_controller.stop()
      if self.outgoing_bridge is not None:
        await self.outgoing_bridge.stop()
      for track in self.video_tracks:
        track.stop()
      self.video_tracks.clear()
      await self.stream.stop()


class ServerState:
  def __init__(self):
    self.streams: dict[str, StreamSession] = {}
    self.stream_lock = asyncio.Lock()
    self.teardown: asyncio.TimerHandle | None = None


# if nothing connects for 5 seconds, tear down livestreaming processes
def schedule_teardown(state: ServerState):
  if state.teardown is not None:
    state.teardown.cancel()

  def clear():
    if not state.streams:
      Params().put_bool("IsLiveStreaming", False)

  state.teardown = asyncio.get_running_loop().call_later(5.0, clear)


def _json_response(obj: Any, status: int = 200) -> tuple[int, bytes, str]:
  return (status, json.dumps(obj).encode(), "application/json; charset=utf-8")


def _text_response(text: str, status: int = 200) -> tuple[int, bytes, str]:
  return (status, text.encode(), "text/plain; charset=utf-8")


async def handle_get_stream(state: ServerState, raw_body: bytes, content_type: str) -> tuple[int, bytes, str]:
  if content_type != "application/json":
    return _json_response({"error": "unsupported media type"}, status=415)

  stream_dict = state.streams
  body = StreamRequestBody(**json.loads(raw_body))

  async with state.stream_lock:
    # don't remove existing connection on prewarm request
    enabled = any(s.run_task and not s.run_task.done() and s.enabled for s in stream_dict.values())
    if enabled and not body.enabled:
      return _json_response({"error": "busy", "message": "someone else is connected."})

    for sid, s in list(stream_dict.items()):
      if s.run_task and not s.run_task.done():
        try:
          ch = s.stream.get_messaging_channel()
          ch.send(json.dumps({"type": "disconnect", "data": "Another device has connected, closing this session."}))
        except Exception:
          pass
      await s.stop()
      stream_dict.pop(sid, None)

    session = StreamSession(body)
    stream_dict[session.identifier] = session
    try:
      answer = await asyncio.wait_for(session.get_answer(), timeout=30)
      cloudlog.event(
        "webrtcd.session.ice_candidates",
        session_id=session.identifier,
        offer_candidates=_ice_candidates(body.sdp),
        answer_candidates=_ice_candidates(answer.sdp),
      )
    except TimeoutError:
      await session.stop()
      stream_dict.pop(session.identifier, None)
      logging.getLogger("webrtcd").exception("Timed out creating stream answer")
      with cloudlog.ctx(session_id=session.identifier):
        cloudlog.warning("webrtcd.session.answer_timeout")
      raise
    except Exception:
      await session.stop()
      stream_dict.pop(session.identifier, None)
      logging.getLogger("webrtcd").exception("Failed to create stream answer")
      with cloudlog.ctx(session_id=session.identifier):
        cloudlog.exception("webrtcd.session.answer_exception")
      raise
    session.start()

    def remove_finished_session(_: asyncio.Task) -> None:
      stream_dict.pop(session.identifier, None)
      schedule_teardown(state)

    session.run_task.add_done_callback(remove_finished_session)

  return _json_response({"sdp": answer.sdp, "type": answer.type})


async def handle_get_schema(state: ServerState, services_param: str) -> tuple[int, bytes, str]:
  services = services_param.split(",")
  services = [s for s in services if s]
  assert all(s in log.Event.schema.fields and not s.endswith("DEPRECATED") for s in services), "Invalid service name"
  schema_dict = {s: generate_field(log.Event.schema.fields[s]) for s in services}
  return _json_response(schema_dict)


async def handle_post_notify(state: ServerState, payload: Any) -> tuple[int, bytes, str]:
  for session in list(state.streams.values()):
    try:
      ch = session.stream.get_messaging_channel()
      ch.send(json.dumps(payload))
    except Exception:
      continue

  return _text_response("OK")


async def on_shutdown(state: ServerState):
  for session in list(state.streams.values()):
    try:
      ch = session.stream.get_messaging_channel()
      ch.send(json.dumps({"type": "disconnect", "data": "device streaming has been stopped."}))
    except Exception:
      pass
    await session.stop()
  state.streams.clear()


# Browser pages may use this server (CORS) only from the sites listed here: today the OP WebView web client.
# A listed page can watch the stream and, like the app, can replace the current viewer's session. It cannot send
# anything into openpilot, because incoming_services drops every incoming service while opview is on. The opview
# app sends no Origin and is unaffected. Add a site here only when a web client really needs it.
CORS_ORIGINS = ("https://inertiaimpact.github.io",)


def cors_origin_allowed(origin: str | None) -> bool:
  return origin in CORS_ORIGINS


class WebrtcdHandler(BaseHTTPRequestHandler):
  protocol_version = "HTTP/1.1"

  # path -> allowed methods (aiohttp registered POST /stream, POST /notify, GET /schema + its auto HEAD)
  _routes = {
    "/schema": ("GET", "HEAD"),
    "/stream": ("POST",),
    "/notify": ("POST",),
  }

  def _send_cors_headers(self) -> None:
    origin = self.headers.get("Origin")
    if not cors_origin_allowed(origin):
      return
    self.send_header("Access-Control-Allow-Origin", origin)
    self.send_header("Vary", "Origin")
    self.send_header("Access-Control-Allow-Methods", "GET, HEAD, POST, OPTIONS")
    self.send_header("Access-Control-Allow-Headers", "Content-Type")
    # Chrome's Private/Local Network Access: an HTTPS page reaching this device on the LAN
    if self.headers.get("Access-Control-Request-Private-Network") == "true":
      self.send_header("Access-Control-Allow-Private-Network", "true")
    self.send_header("Access-Control-Max-Age", "600")

  def _send(self, status: int, body: bytes, content_type: str) -> None:
    self.send_response(status)
    self.send_header("Content-Type", content_type)
    self._send_cors_headers()
    self.send_header("Content-Length", str(len(body)))
    self.end_headers()
    if self.command != "HEAD":
      self.wfile.write(body)

  def _read_body(self) -> bytes:
    length = int(self.headers.get("Content-Length", 0))
    return self.rfile.read(length) if length else b""

  def _run(self, coro) -> tuple[int, bytes, str]:
    return asyncio.run_coroutine_threadsafe(coro, self.server.loop).result()

  def _dispatch_request(self) -> None:
    parsed = urlparse(self.path)
    allowed = self._routes.get(parsed.path)

    try:
      if allowed is None:
        result = _json_response({"error": "not found"}, status=404)
      elif self.command not in allowed:
        result = _json_response({"error": "method not allowed"}, status=405)
      elif parsed.path == "/schema":
        services = parse_qs(parsed.query).get("services", [""])[0]
        result = self._run(handle_get_schema(self.server.state, services))
      elif parsed.path == "/stream":
        result = self._run(handle_get_stream(self.server.state, self._read_body(), self.headers.get_content_type()))
      else:  # /notify
        try:
          payload = json.loads(self._read_body())
        except Exception:
          result = _json_response({"error": "bad request"}, status=400)
        else:
          result = self._run(handle_post_notify(self.server.state, payload))
    except Exception as e:
      logging.getLogger("webrtcd").exception("Unhandled error handling %s", self.path)
      result = _json_response({"error": "exception", "message": f"{type(e).__name__}: {e}"}, status=500)

    self._send(*result)

  def do_GET(self) -> None:
    self._dispatch_request()

  def do_HEAD(self) -> None:
    self._dispatch_request()

  def do_POST(self) -> None:
    self._dispatch_request()

  def do_PUT(self) -> None:
    self._dispatch_request()

  def do_DELETE(self) -> None:
    self._dispatch_request()

  def do_PATCH(self) -> None:
    self._dispatch_request()

  def do_OPTIONS(self) -> None:
    # CORS preflight: the browser asks before POSTing JSON to /stream
    if urlparse(self.path).path in self._routes:
      self._send(204, b"", "text/plain; charset=utf-8")
    else:
      self._dispatch_request()

  def log_message(self, format: str, *args: object) -> None:  # noqa: A002  # stdlib override
    # silence default access logging; errors are logged explicitly in _dispatch_request
    pass


class WebrtcdHTTPServer(ThreadingHTTPServer):
  daemon_threads = True
  allow_reuse_address = True
  state: ServerState
  loop: asyncio.AbstractEventLoop


async def _shutdown(server: WebrtcdHTTPServer, state: ServerState, loop: asyncio.AbstractEventLoop) -> None:
  # stop accepting new HTTP connections (blocks until serve_forever returns, so
  # run it off the loop) then tear down active stream sessions.
  await loop.run_in_executor(None, server.shutdown)
  await on_shutdown(state)
  loop.stop()


def prewarm_stream_session_imports() -> None:
  from openpilot.system.webrtc.device.video import LiveStreamVideoStreamTrack
  from teleoprtc.builder import WebRTCAnswerBuilder
  assert LiveStreamVideoStreamTrack
  assert WebRTCAnswerBuilder


def webrtcd_thread(host: str, port: int):
  logging.basicConfig(level=logging.INFO, handlers=[logging.StreamHandler()])
  prewarm_start = time.monotonic()
  prewarm_stream_session_imports()
  prewarm_end = time.monotonic()
  logging.getLogger("webrtcd").info(f"webrtc prewarm finished in {(prewarm_end - prewarm_start) * 1000} ms")

  loop = asyncio.new_event_loop()
  asyncio.set_event_loop(loop)
  state = ServerState()

  server = WebrtcdHTTPServer((host, port), WebrtcdHandler)
  server.state = state
  server.loop = loop

  # serve HTTP on a daemon thread so the asyncio loop can own the main thread
  http_thread = threading.Thread(target=server.serve_forever, name="webrtcd-http", daemon=True)
  http_thread.start()

  shutting_down = False
  shutdown_task = None

  def request_shutdown() -> None:
    nonlocal shutting_down, shutdown_task
    if shutting_down:
      return
    shutting_down = True
    shutdown_task = loop.create_task(_shutdown(server, state, loop))

  for sig in (signal.SIGINT, signal.SIGTERM):
    loop.add_signal_handler(sig, request_shutdown)

  try:
    loop.run_forever()
  finally:
    server.server_close()
    loop.close()


def avoid_keyframe_request_deadlock():
  # libdatachannel-py does not release the GIL in send(). teleoprtc's PliHandler calls back into
  # Python (track.request_keyframe) from a libdatachannel worker while it holds the DTLS lock, so a
  # keyframe request arriving while our loop is inside send() deadlocks webrtcd for good (seen on
  # weak wifi, where viewers send many requests). The stream encoder already sends a keyframe every
  # 5 frames, so for opview swap the handler for a plain pass-through that never enters Python.
  import teleoprtc.stream
  from libdatachannel import MediaHandler
  teleoprtc.stream.PliHandler = lambda _request_keyframe: MediaHandler()


def main():
  parser = argparse.ArgumentParser(description="WebRTC daemon")
  parser.add_argument("--host", type=str, default=None, help="Host to listen on")
  parser.add_argument("--port", type=int, default=5001, help="Port to listen on")
  args = parser.parse_args()

  # opview connects from another device on the local network
  opview = opview_enabled(Params())
  host = args.host or ("0.0.0.0" if opview else "127.0.0.1")
  if opview:
    avoid_keyframe_request_deadlock()
  webrtcd_thread(host, args.port)


if __name__=="__main__":
  main()
