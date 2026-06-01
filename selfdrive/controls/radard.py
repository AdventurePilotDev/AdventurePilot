#!/usr/bin/env python3
import math
import numpy as np
from collections import deque
from dataclasses import dataclass, asdict, replace

import capnp
from cereal import messaging, log, car, custom
from openpilot.common.filter_simple import FirstOrderFilter
from openpilot.common.params import Params
from openpilot.common.realtime import DT_MDL, Priority, config_realtime_process
from openpilot.common.swaglog import cloudlog
from openpilot.common.simple_kalman import KF1D

from opendbc.car import structs
from opendbc.car.hyundai.values import HyundaiFlags
from opendbc.sunnypilot.car.hyundai.values import HyundaiFlagsSP


# Default lead acceleration decay set to 50% at 1s
_LEAD_ACCEL_TAU = 1.5

# radar tracks
SPEED, ACCEL = 0, 1     # Kalman filter states enum

# stationary qualification parameters
V_EGO_STATIONARY = 4.   # no stationary object flag below this speed

RADAR_TO_CENTER = 2.7   # (deprecated) RADAR is ~ 2.7m ahead from center of car
RADAR_TO_CAMERA = 1.52  # RADAR is ~ 1.5m ahead from center of mesh frame

# Rivian Mando radar lateral-scale correction.
# opendbc's rivian/radar_interface.py computes yRel = 0.5 * -sin(azimuth) * LONG_DIST — an
# undocumented 0.5 factor (original to opendbc #1806, never examined) that over-compresses the
# lateral offset. With everything pulled toward the centerline, prob_y can't separate an
# adjacent-lane track from the in-path lead, which feeds the lead-bounce. Measurement
# (scratch_yrel_transform.py, 269k clean corpus samples) shows the radar lateral that best AGREES
# with the camera — the exact quantity match_vision_to_track's prob_y compares — uses a scale of
# ~0.70, not 0.5; and that scale also minimizes lead-bounce brake-jumps through the full pipeline
# (scratch_yrel_validate.py: tier-2 -4.7%, 0 gross-streak regressions corpus-wide). We correct it
# here, brand-gated, rather than in the shared opendbc interface, so it stays Rivian-only and
# adjacent to the lead-tracking tier system. Correction = target / source = 0.70 / 0.5 = 1.4.
RIVIAN_YREL_TARGET_SCALE = 0.70
_RADAR_INTERFACE_YREL_SCALE = 0.5   # the factor baked into opendbc rivian/radar_interface.py
RIVIAN_YREL_CORRECTION = RIVIAN_YREL_TARGET_SCALE / _RADAR_INTERFACE_YREL_SCALE  # 1.4

# Tier 3 "vision-coast".
#
# A phantom is a radar lead whose speed disagrees with the (confident)
# vision lead AND that arrived via an abrupt radar discontinuity the camera does NOT corroborate, either:
#   - the chosen track-id just changed, or
#   - the radar lead jumped laterally while the vision lead held still
#
# On such a frame, hold the last-good lead for up to COAST_MAX_S instead of
# emitting the phantom; then hand back to the radar until a real lead returns.
#
# The corroborating discontinuity is what lets the speed gate stay loose without coasting on clean frames.
# Holding the last-good (Kalman-smoothed) radar lead beats switching to the noisier vision lead.
COAST_V_GROSS = 5.0          # m/s — speed disagreement vs vision
                             #       a corroborating discontinuity (another sus event)
                             #       is also required, so this stays strict
COAST_LAT_RADAR_JUMP = 1.0   # m   — chosen-lead yRel jump (Rivian yRel is halved → ~2 m real)
COAST_LAT_VISION_STILL = 0.5 # m   — vision lead barely moved laterally (so the jump is radar-only)
COAST_MAX_S = 0.3            # s   — hard cap on continuous coasting, then hand back to the radar.
                             #       bounds the worst-case coast distance to v·0.3
                             #       assuming freeway speeds:
                             #       90 mph (~41 m/s) (faster than most cruise)
                             #       that comes out to: 12.3 m coasting
                             #       this does not include the COAST_NEAR_DREL lockout
                             #       so any close range radar lead would immediately cancel the coast
COAST_NEAR_DREL = 40.0       # m   — close-cut-in failsafe: never coast past a radar lead nearer than
                             #       this. A close return is a hazard — respond to it, don't hold a
                             #       stale far lead. The inverse of the gross-distance trigger: here
                             #       closeness SUPPRESSES coast. Set from the corpus coast-distance
                             #       distribution: coasts under ~40 m are ~100% vision-confirmed real
                             #       objects (often stopped, stop-and-go), where coasting past = a
                             #       rear-end risk; the phantom benefit lives >60 m. 0 disables.
COAST_VIS_PROB = 0.5         #     — only trust the phantom judgment when the vision lead is confident


@dataclass
class RadarLead:
  """A radarState leadOne/leadTwo estimate. Field names mirror cereal LeadData, so an instance
  maps onto the cereal struct via asdict(); the defaults match cereal's defaults, so a bare
  RadarLead() is the 'no lead' value (status=False) — it replaces the old {'status': False} dict."""
  status: bool = False
  dRel: float = 0.0
  yRel: float = 0.0
  vRel: float = 0.0
  vLead: float = 0.0
  vLeadK: float = 0.0
  aLeadK: float = 0.0
  aLeadTau: float = 0.0
  fcw: bool = False
  modelProb: float = 0.0
  radar: bool = False
  radarTrackId: int = -1


class KalmanParams:
  def __init__(self, dt: float):
    # Lead Kalman Filter params, calculating K from A, C, Q, R requires the control library.
    # hardcoding a lookup table to compute K for values of radar_ts between 0.01s and 0.2s
    assert dt > .01 and dt < .2, "Radar time step must be between .01s and 0.2s"
    self.A = [[1.0, dt], [0.0, 1.0]]
    self.C = [1.0, 0.0]
    #Q = np.matrix([[10., 0.0], [0.0, 100.]])
    #R = 1e3
    #K = np.matrix([[ 0.05705578], [ 0.03073241]])
    dts = [i * 0.01 for i in range(1, 21)]
    K0 = [0.12287673, 0.14556536, 0.16522756, 0.18281627, 0.1988689,  0.21372394,
          0.22761098, 0.24069424, 0.253096,   0.26491023, 0.27621103, 0.28705801,
          0.29750003, 0.30757767, 0.31732515, 0.32677158, 0.33594201, 0.34485814,
          0.35353899, 0.36200124]
    K1 = [0.29666309, 0.29330885, 0.29042818, 0.28787125, 0.28555364, 0.28342219,
          0.28144091, 0.27958406, 0.27783249, 0.27617149, 0.27458948, 0.27307714,
          0.27162685, 0.27023228, 0.26888809, 0.26758976, 0.26633338, 0.26511557,
          0.26393339, 0.26278425]
    self.K = [[np.interp(dt, dts, K0)], [np.interp(dt, dts, K1)]]


class Track:
  def __init__(self, identifier: int, v_lead: float, kalman_params: KalmanParams):
    self.identifier = identifier
    self.cnt = 0
    self.aLeadTau = FirstOrderFilter(_LEAD_ACCEL_TAU, 0.45, DT_MDL)
    self.K_A = kalman_params.A
    self.K_C = kalman_params.C
    self.K_K = kalman_params.K
    self.kf = KF1D([[v_lead], [0.0]], self.K_A, self.K_C, self.K_K)

  def update(self, d_rel: float, y_rel: float, v_rel: float, v_lead: float, measured: float):
    # relative values, copy
    self.dRel = d_rel   # LONG_DIST
    self.yRel = y_rel   # -LAT_DIST
    self.vRel = v_rel   # REL_SPEED
    self.vLead = v_lead
    self.measured = measured   # measured or estimate

    # computed velocity and accelerations
    if self.cnt > 0:
      self.kf.update(self.vLead)

    self.vLeadK = float(self.kf.x[SPEED][0])
    self.aLeadK = float(self.kf.x[ACCEL][0])

    # Learn if constant acceleration
    if abs(self.aLeadK) < 0.5:
      self.aLeadTau.x = _LEAD_ACCEL_TAU
    else:
      self.aLeadTau.update(0.0)

    self.cnt += 1

  def get_RadarState(self, model_prob: float = 0.0) -> RadarLead:
    return RadarLead(
      dRel=float(self.dRel),
      yRel=float(self.yRel),
      vRel=float(self.vRel),
      vLead=float(self.vLead),
      vLeadK=float(self.vLeadK),
      aLeadK=float(self.aLeadK),
      aLeadTau=float(self.aLeadTau.x),
      status=True,
      fcw=self.is_potential_fcw(model_prob),
      modelProb=model_prob,
      radar=True,
      radarTrackId=self.identifier,
    )

  def potential_low_speed_lead(self, v_ego: float):
    # stop for stuff in front of you and low speed, even without model confirmation
    # Radar points closer than 0.75, are almost always glitches on toyota radars
    return abs(self.yRel) < 1.0 and (v_ego < V_EGO_STATIONARY) and (0.75 < self.dRel < 25)

  def is_potential_fcw(self, model_prob: float):
    return model_prob > .9

  def __str__(self):
    ret = f"x: {self.dRel:4.1f}  y: {self.yRel:4.1f}  v: {self.vRel:4.1f}  a: {self.aLeadK:4.1f}"
    return ret


def laplacian_pdf(x: float, mu: float, b: float):
  b = max(b, 1e-4)
  return math.exp(-abs(x-mu)/b)


def match_vision_to_track(v_ego: float, lead: capnp._DynamicStructReader, tracks: dict[int, Track],
                          prev_track_id: int | None = None, stickiness: float = 5.0) -> Track | None:
  offset_vision_dist = lead.x[0] - RADAR_TO_CAMERA

  def prob(c):
    prob_d = laplacian_pdf(c.dRel, offset_vision_dist, lead.xStd[0])
    prob_y = laplacian_pdf(c.yRel, -lead.y[0], lead.yStd[0])
    prob_v = laplacian_pdf(c.vRel + v_ego, lead.v[0], lead.vStd[0])

    # This isn't exactly right, but it's a good heuristic
    base = prob_d * prob_y * prob_v
    # Hysteresis: bias toward the previously-chosen track so a challenger must beat it
    # by a margin before we switch — prevents frame-to-frame lead flipping when two
    # radar tracks have similar probability. The bonus is scaled by the lateral-agreement
    # probability so the held track releases its bonus when it drifts away from the
    # camera's predicted lateral position (i.e. we only stay sticky while the camera
    # still supports the held track laterally).
    if prev_track_id is not None and c.identifier == prev_track_id:
      base *= 1.0 + (stickiness - 1.0) * prob_y
    return base

  track = max(tracks.values(), key=prob)

  # if no 'sane' match is found return -1
  # stationary radar points can be false positives
  dist_sane = abs(track.dRel - offset_vision_dist) < max([(offset_vision_dist)*.25, 5.0])
  vel_sane = (abs(track.vRel + v_ego - lead.v[0]) < 10) or (v_ego + track.vRel > 3)
  if dist_sane and vel_sane:
    return track
  else:
    return None


def get_RadarState_from_vision(lead_msg: capnp._DynamicStructReader, v_ego: float, model_v_ego: float) -> RadarLead:
  lead_v_rel_pred = lead_msg.v[0] - model_v_ego
  return RadarLead(
    dRel=float(lead_msg.x[0] - RADAR_TO_CAMERA),
    yRel=float(-lead_msg.y[0]),
    vRel=float(lead_v_rel_pred),
    vLead=float(v_ego + lead_v_rel_pred),
    vLeadK=float(v_ego + lead_v_rel_pred),
    aLeadK=float(lead_msg.a[0]),
    aLeadTau=0.3,
    fcw=False,
    modelProb=float(lead_msg.prob),
    status=True,
    radar=False,
    radarTrackId=-1,
  )


def get_lead(v_ego: float, ready: bool, tracks: dict[int, Track], lead_msg: capnp._DynamicStructReader,
             model_v_ego: float, CP: structs.CarParams, CP_SP: structs.CarParamsSP, low_speed_override: bool = True,
             prev_track_id: int | None = None) -> RadarLead:
  # Determine leads, this is where the essential logic happens
  if len(tracks) > 0 and ready and lead_msg.prob > .5:
    track = match_vision_to_track(v_ego, lead_msg, tracks, prev_track_id=prev_track_id)
  else:
    track = None

  lead = RadarLead()
  if track is not None:
    lead = track.get_RadarState(lead_msg.prob)
    lead = get_custom_yrel(CP, CP_SP, lead, lead_msg)
  elif (track is None) and ready and (lead_msg.prob > .5):
    lead = get_RadarState_from_vision(lead_msg, v_ego, model_v_ego)

  if low_speed_override:
    low_speed_tracks = [c for c in tracks.values() if c.potential_low_speed_lead(v_ego)]
    if len(low_speed_tracks) > 0:
      closest_track = min(low_speed_tracks, key=lambda c: c.dRel)

      # Only choose new track if it is actually closer than the previous one
      if (not lead.status) or (closest_track.dRel < lead.dRel):
        lead = closest_track.get_RadarState()

  return lead


def get_custom_yrel(CP: structs.CarParams, CP_SP: structs.CarParamsSP, lead: RadarLead,
                    lead_msg: capnp._DynamicStructReader) -> RadarLead:
  if CP.brand == "hyundai" and (CP_SP.flags & HyundaiFlagsSP.ENHANCED_SCC or
                                CP.flags & (HyundaiFlags.CANFD_CAMERA_SCC | HyundaiFlags.CAMERA_SCC)):
    lead.yRel = float(-lead_msg.y[0])

  return lead


class RadarD:
  def __init__(self, CP: structs.CarParams, CP_SP: structs.CarParams, delay: float = 0.0):
    self.CP = CP
    self.CP_SP = CP_SP

    # brand-gated radar lateral correction (see RIVIAN_YREL_CORRECTION); 1.0 = no-op for other brands
    self.yrel_correction = RIVIAN_YREL_CORRECTION if CP.brand == "rivian" else 1.0

    self.current_time = 0.0

    self.tracks: dict[int, Track] = {}
    self.kalman_params = KalmanParams(DT_MDL)

    self.v_ego = 0.0
    self.v_ego_hist = deque([0.0], maxlen=int(round(delay / DT_MDL))+1)
    self.last_v_ego_frame = -1

    self.radar_state: capnp._DynamicStructBuilder | None = None
    self.radar_state_valid = False

    self.ready = False

    # remember the last selected radar trackId per lead, for the hysteresis bonus in
    # match_vision_to_track (the id is also reused by the Tier-3 id-changed corroborator)
    self.prev_lead_track_id: dict[int, int | None] = {0: None, 1: None}

    # behavior tier: 1=no hysteresis, 2=id-hysteresis (committed Fix A), 3=id-hysteresis +
    # vision-coast. Chosen by the LeadTrackingMode UI selector, which stores a button index
    # (0/1/2 -> tier 1/2/3); main() refreshes it ~1 Hz via the DEC throttled-read pattern (never
    # per-frame disk I/O). update() never reads Params, so offline eval harnesses just set
    # self.lead_tracking_mode directly and it sticks. Defaults to Tier 2 (the validated behavior).
    self.params = Params()
    self.frame = 0
    self.lead_tracking_mode = 2
    # Tier 3 vision-coast state (per lead): last lead we trusted, how long we've coasted, and
    # the previous-frame lateral positions powering the radar-lateral-jump corroborator.
    self.last_good_lead: dict[int, RadarLead | None] = {0: None, 1: None}
    self.coast_frames: dict[int, int] = {0: 0, 1: 0}
    # per-lead coast flag, published as radarStateSP for the onroad UI indicator (chevron tint
    # + developer-UI element). Reset each update(); set by _vision_coast when a lead is held.
    self.lead_coasting: dict[int, bool] = {0: False, 1: False}
    self.prev_lead_yRel: dict[int, float | None] = {0: None, 1: None}
    self.prev_vision_y: dict[int, float | None] = {0: None, 1: None}
    # Tier 3 thresholds — instance attrs so they can be swept at runtime during tuning
    self.coast_v_gross = COAST_V_GROSS
    self.coast_lat_radar_jump = COAST_LAT_RADAR_JUMP
    self.coast_lat_vision_still = COAST_LAT_VISION_STILL
    self.coast_max_s = COAST_MAX_S
    self.coast_vis_prob = COAST_VIS_PROB
    self.coast_near_dRel = COAST_NEAR_DREL

  def update(self, sm: messaging.SubMaster, rr: car.RadarData):
    # clear the per-lead coast flags for this frame (set again below only if Tier 3 holds a lead)
    self.frame += 1
    self.lead_coasting[0] = self.lead_coasting[1] = False

    self.ready = sm.seen['modelV2']
    self.current_time = 1e-9*max(sm.logMonoTime.values())

    if sm.recv_frame['carState'] != self.last_v_ego_frame:
      self.v_ego = sm['carState'].vEgo
      self.v_ego_hist.append(self.v_ego)
      self.last_v_ego_frame = sm.recv_frame['carState']

    # correct the radar lateral at ingestion (brand-gated) so the matcher, the stored Track, and the
    # emitted lead all see the same corrected yRel. dRel/vRel are unaffected — only the lateral scale.
    ar_pts = {pt.trackId: [pt.dRel, pt.yRel * self.yrel_correction, pt.vRel, pt.measured] for pt in rr.points}

    # *** remove missing points from meta data ***
    for ids in list(self.tracks.keys()):
      if ids not in ar_pts:
        self.tracks.pop(ids, None)

    # *** compute the tracks ***
    for ids in ar_pts:
      rpt = ar_pts[ids]

      # align v_ego by a fixed time to align it with the radar measurement
      v_lead = rpt[2] + self.v_ego_hist[0]

      # create the track if it doesn't exist or it's a new track
      if ids not in self.tracks:
        self.tracks[ids] = Track(ids, v_lead, self.kalman_params)
      self.tracks[ids].update(rpt[0], rpt[1], rpt[2], v_lead, rpt[3])

    # *** publish radarState ***
    self.radar_state_valid = sm.all_checks()
    self.radar_state = log.RadarState.new_message()
    self.radar_state.mdMonoTime = sm.logMonoTime['modelV2']
    self.radar_state.radarErrors = rr.errors
    self.radar_state.carStateMonoTime = sm.logMonoTime['carState']

    if len(sm['modelV2'].velocity.x):
      model_v_ego = sm['modelV2'].velocity.x[0]
    else:
      model_v_ego = self.v_ego
    leads_v3 = sm['modelV2'].leadsV3
    if len(leads_v3) > 1:
      tier = self.lead_tracking_mode
      # Tier 1: no hysteresis (stock greedy match). Tier 2/3: id-keyed hysteresis bonus on
      # the previously-held trackId (when it's still present this frame).
      if tier >= 2:
        sticky0 = self.prev_lead_track_id[0]
        sticky1 = self.prev_lead_track_id[1]
      else:
        sticky0 = sticky1 = None
      one = get_lead(self.v_ego, self.ready, self.tracks, leads_v3[0], model_v_ego, self.CP, self.CP_SP,
                     low_speed_override=True, prev_track_id=sticky0)
      two = get_lead(self.v_ego, self.ready, self.tracks, leads_v3[1], model_v_ego, self.CP, self.CP_SP,
                     low_speed_override=False, prev_track_id=sticky1)
      # Tier 3: vision-coast — hold the last-good lead through a gross phantom (e.g. a
      # single-frame radar dropout) instead of emitting it.
      if tier >= 3:
        one, c0 = self._vision_coast(0, one, leads_v3[0])
        two, c1 = self._vision_coast(1, two, leads_v3[1])
        self.lead_coasting[0] = c0
        self.lead_coasting[1] = c1
      else:
        self.coast_frames[0] = self.coast_frames[1] = 0
      self.radar_state.leadOne = asdict(one)
      self.radar_state.leadTwo = asdict(two)
      # remember the chosen radar track id (hysteresis bonus + id-changed corroborator) and the
      # lateral positions (radar-lateral-jump corroborator) for the next frame
      for i, ld in ((0, one), (1, two)):
        if ld.status and ld.radar and ld.radarTrackId >= 0:
          self.prev_lead_track_id[i] = ld.radarTrackId
          self.prev_lead_yRel[i] = ld.yRel
        else:
          self.prev_lead_track_id[i] = None
          self.prev_lead_yRel[i] = None
        self.prev_vision_y[i] = float(leads_v3[i].y[0]) if len(leads_v3[i].y) else None

  def _vision_coast(self, i: int, lead: RadarLead,
                    vis: capnp._DynamicStructReader) -> tuple[RadarLead, bool]:
    """Tier 3. When this frame looks like a phantom (see is_phantom), hold the last-good lead
    instead of emitting it, bounded by COAST_MAX_S, after which we hand back to the radar until a
    real lead returns. Returns (possibly-held lead, coasting?). Holding the last-good (Kalman-
    smoothed) radar lead beats substituting the raw vision lead, which is noisier."""
    phantom = self.is_phantom(i, lead, vis)

    # When it's not a phantom, reset the coast timer and refresh last-good to this frame's lead — or
    # clear it to None when there's no radar lead. Never keep a stale last-good around: a non-phantom
    # lead is safe to hold (it's never a phantom by definition), and if there's nothing to hold we'd
    # rather not coast at all than coast onto a stale value.
    if not phantom:
      self.coast_frames[i] = 0
      self.last_good_lead[i] = replace(lead) if (lead.status and lead.radar) else None
      return lead, False

    # --- BELOW HERE WE DO NOT TRUST THE CURRENT RADAR LEAD (phantom) ---

    # We only want to coast for a max time, so that becomes a max number of frames; if we hit that
    # limit we drop back to returning the potential phantom lead, which is safer than coasting on.
    cap = int(round(self.coast_max_s / DT_MDL))

    # if we have a last-good lead to coast with and are within the coast budget, coast
    if self.last_good_lead[i] is not None and self.coast_frames[i] < cap:
      self.coast_frames[i] += 1
      return replace(self.last_good_lead[i]), True

    # no last-good to coast with, or we hit the coast limit → drop back to the (suspected) phantom
    return lead, False

  def is_phantom(self, i: int, lead: RadarLead, vis: capnp._DynamicStructReader) -> bool:
    """Tier 3 per-frame verdict: do we think this radar lead is a phantom we should NOT trust? It
    is, when its speed grossly disagrees with the confident vision lead AND it arrived via an abrupt
    radar discontinuity the camera does not corroborate (the chosen track-id changed, or the radar
    lead jumped laterally while the vision lead held still). Pure predicate — no coast cap, no state
    mutation — so eval harnesses can label frames with it directly. Cheap checks short-circuit first."""
    if not (lead.status and lead.radar):
      return False
    # Close-cut-in failsafe: a very close radar return is a real hazard (e.g. a side cut-in), never
    # a phantom to coast past — respond to it instead of holding a stale far lead. The inverse of a
    # distance trigger: closeness vetoes the phantom verdict outright.
    if 0.0 < lead.dRel < self.coast_near_dRel:
      return False
    # A phantom's speed grossly disagrees with the vision lead; if it agrees (or we can't tell),
    # it isn't a phantom.
    if self._speed_agrees_with_vision(lead, vis):
      return False

    # --- BELOW HERE WE SUSPECT SOMETHING FISHY ---
    # but we need proof to back that up. So far we know:
    #   - this is a medium-distance object (the close-cut-in failsafe above already let it through)
    #   - it's moving at a dramatically different speed from the vision lead
    # That alone is suspicious but not damning, so we look for a radar discontinuity to corroborate:
    #   - did the radar swap to a new track id?
    #   - did the radar lead hop dramatically to the side (while vision didn't)?
    # Either can mean the radar locked onto something like a stationary object in an adjacent lane —
    # e.g. driving past a row of stopped cars, or reflectors between lanes (toll plaza). There ARE
    # real situations like this where you'd want to brake, which is why we keep the distance gate
    # (nothing fires too close — 40 m default, giving vision time to pick it up) and the vision check.

    # Corroborator 1: the chosen radar track-id just changed (a dropout grabbed a different object).
    if lead.radarTrackId != -1 and lead.radarTrackId != self.prev_lead_track_id[i]:
      return True

    # Corroborator 2: the radar lead jumped laterally while the vision lead held still.
    prev_lead_yRel, prev_vision_y = self.prev_lead_yRel[i], self.prev_vision_y[i]
    if prev_lead_yRel is None or prev_vision_y is None:
      return False

    has_dramatic_lateral_radar_jump = abs(lead.yRel - prev_lead_yRel) > self.coast_lat_radar_jump
    vision_lead_laterally_stable = abs(vis.y[0] - prev_vision_y) < self.coast_lat_vision_still
    return has_dramatic_lateral_radar_jump and vision_lead_laterally_stable

  def _speed_agrees_with_vision(self, lead: RadarLead, vis: capnp._DynamicStructReader) -> bool:
    """True when the radar lead's speed is consistent with the vision lead — i.e. NOT a gross
    disagreement. Also True when there's no basis to call a disagreement (ego ~stopped, or no
    confident vision lead), so those frames default to 'not a phantom'. The negation is the phantom
    speed symptom: a dropout that re-points the radar slot at a slower/closer object craters the
    speed relative to the lead the camera still tracks."""
    if self.v_ego <= V_EGO_STATIONARY:
      return True
    if not len(vis.x) or not len(vis.y) or vis.prob <= self.coast_vis_prob:
      return True
    return abs(lead.vLead - vis.v[0]) <= self.coast_v_gross

  def _read_lead_tracking_mode(self) -> None:
    # The UI selector stores a button index (0/1/2); map it to the tier (1/2/3). Called ~1 Hz from
    # main() only (NOT update()), so a harness that sets lead_tracking_mode directly isn't clobbered.
    # Tolerate the key being absent (e.g. before a params_pyx rebuild) by keeping the current tier.
    try:
      idx = self.params.get("LeadTrackingMode", return_default=True)
      if idx is not None:
        self.lead_tracking_mode = int(idx) + 1
    except Exception:
      pass

  def publish(self, pm: messaging.PubMaster):
    assert self.radar_state is not None

    radar_msg = messaging.new_message("radarState")
    radar_msg.valid = self.radar_state_valid
    radar_msg.radarState = self.radar_state
    pm.send("radarState", radar_msg)

    # sunnypilot: per-lead Tier-3 vision-coast state for the onroad UI indicator
    radar_sp = messaging.new_message("radarStateSP")
    radar_sp.valid = self.radar_state_valid
    radar_sp.radarStateSP.leadOneCoasting = self.lead_coasting[0]
    radar_sp.radarStateSP.leadTwoCoasting = self.lead_coasting[1]
    pm.send("radarStateSP", radar_sp)


# fuses camera and radar data for best lead detection
def main() -> None:
  config_realtime_process(5, Priority.CTRL_LOW)

  # wait for stats about the car to come in from controls
  cloudlog.info("radard is waiting for CarParams")
  CP = messaging.log_from_bytes(Params().get("CarParams", block=True), car.CarParams)
  cloudlog.info("radard got CarParams")

  cloudlog.info("radard is waiting for CarParamsSP")
  CP_SP = messaging.log_from_bytes(Params().get("CarParamsSP", block=True), custom.CarParamsSP)
  cloudlog.info("radard got CarParamsSP")

  # *** setup messaging
  sm = messaging.SubMaster(['modelV2', 'carState', 'liveTracks'], poll='modelV2')
  pm = messaging.PubMaster(['radarState', 'radarStateSP'])

  RD = RadarD(CP, CP_SP, CP.radarDelay)

  while 1:
    sm.update()

    # refresh the selected tier ~1 Hz from the LeadTrackingMode UI param — the same sm.frame-gated
    # main-loop param-read idiom as selfdrived/card/paramsd (cheap; never per-frame disk I/O). sm.frame
    # is 0 on the first pass so the tier is live from boot. Kept out of update() so offline harnesses
    # pin the tier by setting rd.lead_tracking_mode directly.
    if sm.frame % int(1. / DT_MDL) == 0:
      RD._read_lead_tracking_mode()
    RD.update(sm, sm['liveTracks'])
    RD.publish(pm)


if __name__ == "__main__":
  main()
