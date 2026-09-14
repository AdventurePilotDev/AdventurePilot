"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import time
from collections import deque

from opendbc.car import structs

GearShifter = structs.CarState.GearShifter

# Half a second of history either side of the trigger, at the 100 Hz rate this is sampled at.
# The whole of the 2026-09-13 event, from the last healthy frame to the ACM latching itself off,
# spanned 60 ms, so this is generous. It is bounded because the dump has to fit in one log line.
PRE_FRAMES = 50
POST_FRAMES = 25

# A fault that re-arms every frame would otherwise fill the log with identical dumps.
MAX_DUMPS = 4

# The ACM has to hold its supervisor state long enough to be a latch rather than a blip.
LATCH_FRAMES = 100

# Anything at or above this is a fault code. 0 and 1 are the two the VDM sits at normally
# (1 is the resting value on this truck, and carstate.py already treats 2 and 3 as faults).
VDM_FAULT_MIN = 2


class AccFaultRecorder:
  """Ring buffer of the Rivian ACC command and response channel, dumped when the ACC faults.

  Why this exists: an ACC dropout is currently only diagnosable from rlogs, which means fetching
  12 MB a segment and hand-decoding CAN to find a fault that happened once in 164 segments. The dump
  goes out through cloudlog with error=True, which lands it in errorLogMessage. That stream is not
  decimated, so it survives into the qlog and a whole route can be searched at 490 KB a segment.

  Recording is unconditional and the cost is one tuple append per frame; a dump only happens on a
  fault, so a healthy drive produces nothing at all. Nothing here touches a control path: the
  recorder only reads, and every emit is wrapped so a logging problem can never reach the car.
  """

  def __init__(self):
    self.buf: deque = deque(maxlen=PRE_FRAMES + POST_FRAMES + 2)
    self.dumps = 0
    self.suppressed = 0
    self._reason: str | None = None
    self._countdown = 0
    self._last_t = 0.
    self._prev_fault = False
    self._latch_frames = 0
    self._latched = False
    # lazy openpilot import: opendbc must stay importable standalone (safety test suite)
    try:
      from openpilot.common.swaglog import cloudlog
      self._log = cloudlog
    except Exception:
      self._log = None

  def record_command(self, accel: float, enabled: bool, long_allowed: bool) -> None:
    """Fill in what openpilot asked for this frame.

    Called from the Rivian CarController, which runs after CarState in the same 100 Hz pass, so it
    patches the sample this frame's update() just appended. Without it the dump would show the car's
    answer but not the question, which is the half that decides whether openpilot led or followed.
    """
    if self.buf:
      self.buf[-1][1:4] = [round(accel, 3), int(enabled), int(long_allowed)]

  def update(self, ret: structs.CarState, cp, cp_cam) -> None:
    # Emitting before the new sample is appended means every row in the buffer has already had its
    # command fields filled in by the CarController, so no row in a dump is half written.
    if self._reason is not None and self._countdown <= 0:
      self._emit(ret)

    now = time.monotonic()
    dt_ms = (now - self._last_t) * 1e3 if self._last_t else 0.
    self._last_t = now

    vdm = cp.vl["VDM_AdasSts"]
    acm = cp_cam.vl["ACM_Status"]
    vdm_fault = int(vdm["VDM_AdasFaultStatus"])
    acm_sup = int(acm["ACM_FaultSupervisorState"])

    # One row per frame, kept as a list so record_command can patch it. The three command fields are
    # None until the CarController fills them, and stay None when openpilot is not driving the
    # longitudinal, which is itself worth seeing in a dump.
    self.buf.append([round(dt_ms, 2), None, None, None,
                     vdm_fault, int(vdm["VDM_AdasInterfaceStatus"]), int(vdm["VDM_AdasDriverModeStatus"]),
                     int(vdm["VDM_UserAdasRequest"]), round(float(vdm["VDM_AdasAccelLimit"]), 2),
                     int(acm["ACM_FeatureStatus"]), int(acm["ACM_FaultStatus"]), acm_sup,
                     round(ret.vEgo, 2), int(ret.brakePressed), int(ret.gasPressed)])

    in_drive = ret.gearShifter == GearShifter.drive

    # The ACM holding its supervisor state at 3 in Drive is what leaves the truck with no cruise
    # control at all until it is restarted. In Park the same value is normal, hence the gear check.
    # This is reported, never acted on: the resting values of these signals are not yet pinned down
    # well enough to put an alert in front of the driver or to block an engagement.
    if in_drive and acm_sup == 3:
      self._latch_frames += 1
    else:
      if self._latched:
        self._event("rivian_acc_latch_cleared", held_frames=self._latch_frames)
      self._latch_frames = 0
      self._latched = False

    if self._reason is None and self.dumps < MAX_DUMPS:
      # accFaulted is the rising edge that disengaged the car; the latch is the slower condition that
      # keeps it disengaged. They are separate reasons because they need separate triage.
      if ret.accFaulted and not self._prev_fault:
        self._arm("acc_faulted")
      elif not self._latched and self._latch_frames == LATCH_FRAMES:
        self._latched = True
        self._arm("acc_latched")
    elif self._reason is not None:
      self._countdown -= 1
    elif ret.accFaulted and not self._prev_fault:
      self.suppressed += 1

    if not self._latched and self._latch_frames >= LATCH_FRAMES:
      self._latched = True

    self._prev_fault = ret.accFaulted

  def _arm(self, reason: str) -> None:
    self._reason = reason
    self._countdown = POST_FRAMES

  def _event(self, name: str, **kwargs) -> None:
    if self._log is None:
      return
    try:
      # error=True puts this in errorLogMessage, which is not decimated and so survives into qlogs
      self._log.event(name, error=True, **kwargs)
    except Exception:
      pass

  def _emit(self, ret: structs.CarState) -> None:
    reason, self._reason = self._reason, None
    self.dumps += 1
    rows = list(self.buf)
    # Columnar rather than a list of rows: the same numbers in roughly a third of the characters,
    # and every column reads as a time series, which is how you actually look at this.
    cols = list(zip(*rows, strict=True)) if rows else []
    names = ("dt_ms", "accel", "en", "allowed", "vdm_fault", "vdm_iface", "vdm_mode", "vdm_user",
             "vdm_accel_lim", "acm_feat", "acm_fault", "acm_sup", "v_ego", "brake", "gas")
    self._event("rivian_acc_fault_snapshot", reason=reason, frames=len(rows),
                dump=self.dumps, suppressed=self.suppressed,
                gear=str(ret.gearShifter), cruise_enabled=int(ret.cruiseState.enabled),
                **dict(zip(names, [list(c) for c in cols], strict=True)))
