"""Unit tests for radard lead selection — the RadarLead type, vision↔radar matching, the
tier-2 hysteresis, and the tier-3 vision-coast. These drive the pure-Python functions/objects
directly (no process_replay / msgq), so they run on macOS as well as in CI."""
from dataclasses import asdict

import cereal.messaging as messaging
from cereal import car, log, custom
from openpilot.common.realtime import DT_MDL
from openpilot.selfdrive.controls.radard import (
  RadarLead, Track, KalmanParams, RadarD, get_lead, match_vision_to_track, RADAR_TO_CAMERA,
  RIVIAN_YREL_CORRECTION,
)

V_EGO = 25.0


def vision_msg(x, y, v, prob=1.0, x_std=2.0, y_std=1.0, v_std=1.0, a=0.0):
  """Build a modelV2 message with one lead. Return the message (keep it in scope) — index
  `.modelV2.leadsV3[0]` to get the reader the radard functions consume."""
  msg = messaging.new_message('modelV2')
  msg.modelV2.leadsV3 = [{'prob': prob, 'x': [x], 'xStd': [x_std], 'y': [y], 'yStd': [y_std],
                          'v': [v], 'vStd': [v_std], 'a': [a]}]
  return msg


def make_track(identifier, d_rel, y_rel, v_rel, v_ego=V_EGO):
  t = Track(identifier, v_rel + v_ego, KalmanParams(DT_MDL))
  t.update(d_rel, y_rel, v_rel, v_rel + v_ego, 1.0)
  return t


def make_radard():
  return RadarD(car.CarParams.new_message(), custom.CarParamsSP.new_message(), delay=0.0)


class TestRadarLead:
  def test_default_is_no_lead(self):
    lead = RadarLead()
    assert lead.status is False and lead.radar is False and lead.radarTrackId == -1

  def test_cereal_roundtrip(self):
    # a full radar lead maps onto the cereal LeadData via asdict() with no loss
    lead = RadarLead(status=True, dRel=52.3, yRel=-1.2, vRel=-3.0, vLead=22.0, vLeadK=22.1,
                     aLeadK=0.4, aLeadTau=1.5, fcw=False, modelProb=0.9, radar=True, radarTrackId=7)
    rs = log.RadarState.new_message()
    rs.leadOne = asdict(lead)
    # cereal LeadData floats are Float32, so compare floats with tolerance and the rest exactly
    for f in ("status", "fcw", "radar", "radarTrackId"):
      assert getattr(rs.leadOne, f) == getattr(lead, f), f
    for f in ("dRel", "yRel", "vRel", "vLead", "vLeadK", "aLeadK", "aLeadTau", "modelProb"):
      assert abs(getattr(rs.leadOne, f) - getattr(lead, f)) < 1e-4, f

  def test_no_lead_cereal_matches_defaults(self):
    # the bare RadarLead() must produce the same struct the old {'status': False} dict did
    rs = log.RadarState.new_message()
    rs.leadTwo = asdict(RadarLead())
    assert rs.leadTwo.status is False
    assert rs.leadTwo.radarTrackId == -1   # cereal schema default
    assert rs.leadTwo.dRel == 0.0


class TestGetLead:
  def test_returns_matched_radar_lead(self):
    tracks = {5: make_track(5, d_rel=50.0, y_rel=0.0, v_rel=0.0)}
    vm = vision_msg(x=50.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    lead = get_lead(V_EGO, True, tracks, vm.modelV2.leadsV3[0], V_EGO, 1.0,
                    car.CarParams.new_message(), custom.CarParamsSP.new_message())
    assert isinstance(lead, RadarLead)
    assert lead.status and lead.radar and lead.radarTrackId == 5
    assert abs(lead.dRel - 50.0) < 1e-6

  def test_falls_back_to_vision_when_no_track(self):
    vm = vision_msg(x=60.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    lead = get_lead(V_EGO, True, {}, vm.modelV2.leadsV3[0], V_EGO, 1.0,
                    car.CarParams.new_message(), custom.CarParamsSP.new_message())
    assert isinstance(lead, RadarLead)
    assert lead.status and not lead.radar and lead.radarTrackId == -1
    assert abs(lead.dRel - 60.0) < 1e-6


class TestHysteresis:
  def test_prev_track_gets_sticky_bonus(self):
    # two near-equal tracks; the held one should win once it gets the hysteresis bonus
    tracks = {1: make_track(1, d_rel=50.0, y_rel=0.3, v_rel=0.0),
              2: make_track(2, d_rel=50.0, y_rel=-0.3, v_rel=0.0)}
    vm = vision_msg(x=50.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    lead = vm.modelV2.leadsV3[0]
    no_hyst = match_vision_to_track(V_EGO, lead, tracks, prev_track_id=None)
    held = match_vision_to_track(V_EGO, lead, tracks, prev_track_id=2)
    assert no_hyst is not None and held is not None
    assert held.identifier == 2                      # the held track sticks
    assert match_vision_to_track(V_EGO, lead, tracks, prev_track_id=1).identifier == 1


class TestVisionCoast:
  def _setup_held(self, rd):
    rd.v_ego = V_EGO
    rd.prev_lead_track_id[0] = 100
    rd.prev_lead_yRel[0] = 0.0
    rd.prev_vision_y[0] = 0.0
    rd.last_good_lead[0] = RadarLead(status=True, radar=True, dRel=95.0, yRel=0.0,
                                     vLead=V_EGO, radarTrackId=100)

  def test_holds_through_phantom(self):
    rd = make_radard()
    self._setup_held(rd)
    # phantom: new id, much closer & slower, while vision still sees the far/fast lead
    phantom = RadarLead(status=True, radar=True, dRel=77.0, yRel=-3.0, vLead=8.0, radarTrackId=200)
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    out, coasting = rd._vision_coast(0, phantom, vm.modelV2.leadsV3[0])
    assert coasting is True
    assert out.radarTrackId == 100 and abs(out.dRel - 95.0) < 1e-6   # held the last-good lead

  def test_passes_real_lead_and_stores_it(self):
    rd = make_radard()
    self._setup_held(rd)
    real = RadarLead(status=True, radar=True, dRel=96.0, yRel=0.0, vLead=V_EGO, radarTrackId=100)
    vm = vision_msg(x=96.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    out, coasting = rd._vision_coast(0, real, vm.modelV2.leadsV3[0])
    assert coasting is False
    assert out is real                                   # emitted unchanged
    assert rd.last_good_lead[0].dRel == 96.0             # and remembered as last-good

  def test_no_coast_when_vision_agrees_with_close_slow(self):
    # a genuinely close/slow lead the camera ALSO sees must not be coasted through
    rd = make_radard()
    self._setup_held(rd)
    real_slow = RadarLead(status=True, radar=True, dRel=77.0, yRel=0.0, vLead=8.0, radarTrackId=200)
    vm = vision_msg(x=77.0 + RADAR_TO_CAMERA, y=0.0, v=8.0)   # vision agrees: close & slow
    out, coasting = rd._vision_coast(0, real_slow, vm.modelV2.leadsV3[0])
    assert coasting is False and out is real_slow

  def test_close_cut_in_failsafe_suppresses_coast(self):
    # a very close radar return (< coast_near_dRel) that LOOKS like a phantom (new id, slow, vision
    # still sees the far/fast lead) must NEVER be coasted — it's a hazard (e.g. a side cut-in)
    rd = make_radard()
    self._setup_held(rd)
    assert rd.coast_near_dRel == 40.0
    cut_in = RadarLead(status=True, radar=True, dRel=18.0, yRel=-3.0, vLead=8.0, radarTrackId=200)
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)   # vision still sees the far/fast lead
    out, coasting = rd._vision_coast(0, cut_in, vm.modelV2.leadsV3[0])
    assert coasting is False                 # failsafe overrode the phantom verdict
    assert out is cut_in and out.dRel == 18.0   # emitted the close lead, did not hold last-good

  def test_phantom_just_beyond_near_threshold_still_coasts(self):
    # the failsafe must not kill the benefit: a phantom just past coast_near_dRel still coasts
    rd = make_radard()
    self._setup_held(rd)
    far_phantom = RadarLead(status=True, radar=True, dRel=55.0, yRel=-3.0, vLead=8.0, radarTrackId=200)
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    out, coasting = rd._vision_coast(0, far_phantom, vm.modelV2.leadsV3[0])
    assert coasting is True and out.radarTrackId == 100   # held last-good, as before (beyond the floor)

  def test_last_good_refreshes_then_clears(self):
    # last-good must never go stale: refresh it to the current non-phantom lead every frame, and
    # clear it to None when there's no radar lead (so we never coast onto a stale value)
    rd = make_radard()
    rd.v_ego = V_EGO
    rd.prev_lead_track_id[0] = 100   # same id below → no corroborator → not a phantom
    # a non-phantom lead that speed-disagrees with vision is now stored fresh (was skipped before)
    lead = RadarLead(status=True, radar=True, dRel=80.0, yRel=0.0, vLead=8.0, radarTrackId=100)
    vm = vision_msg(x=80.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)   # vision 25 vs lead 8 → speed disagrees
    rd._vision_coast(0, lead, vm.modelV2.leadsV3[0])
    assert rd.last_good_lead[0] is not None and rd.last_good_lead[0].dRel == 80.0
    # next frame has no radar lead → last-good cleared, not left stale
    rd._vision_coast(0, RadarLead(), vm.modelV2.leadsV3[0])
    assert rd.last_good_lead[0] is None


class TestIsPhantom:
  """The pure per-frame phantom predicate (no cap, no state mutation) — what the harnesses label
  frames with. Each case isolates one gate."""
  def _rd(self):
    rd = make_radard()
    rd.v_ego = V_EGO
    rd.prev_lead_track_id[0] = 100
    rd.prev_lead_yRel[0] = 0.0
    rd.prev_vision_y[0] = 0.0
    return rd

  def _lead(self, dRel=80.0, vLead=8.0, radarTrackId=200, yRel=0.0):
    # a far (>coast_near_dRel), speed-disagreeing radar lead with a changed id, by default a phantom
    return RadarLead(status=True, radar=True, dRel=dRel, yRel=yRel, vLead=vLead, radarTrackId=radarTrackId)

  def test_phantom_true_speed_disagree_plus_id_change(self):
    rd = self._rd()
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)   # vision: far, fast
    assert rd.is_phantom(0, self._lead(), vm.modelV2.leadsV3[0]) is True

  def test_not_phantom_when_speed_agrees(self):
    rd = self._rd()
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    assert rd.is_phantom(0, self._lead(vLead=V_EGO), vm.modelV2.leadsV3[0]) is False

  def test_not_phantom_without_a_corroborator(self):
    # speed disagrees but the id is unchanged (100) and no lateral jump → not a phantom
    rd = self._rd()
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    assert rd.is_phantom(0, self._lead(radarTrackId=100), vm.modelV2.leadsV3[0]) is False

  def test_not_phantom_when_close_failsafe(self):
    rd = self._rd()
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO)
    assert rd.is_phantom(0, self._lead(dRel=18.0), vm.modelV2.leadsV3[0]) is False

  def test_not_phantom_when_vision_unconfident(self):
    rd = self._rd()
    vm = vision_msg(x=95.0 + RADAR_TO_CAMERA, y=0.0, v=V_EGO, prob=0.3)
    assert rd.is_phantom(0, self._lead(), vm.modelV2.leadsV3[0]) is False


# --- update() integration: drive the whole RadarD.update path via a minimal fake SubMaster, so
# the per-lead lead_coasting flags (published as radarStateSP for the UI) are exercised. msgq /
# process_replay aborts on macOS, but update() only touches a handful of SubMaster attributes. ---

def model_msg(leads_v3, v_ego=V_EGO):
  msg = messaging.new_message('modelV2')
  msg.modelV2.velocity.x = [v_ego]
  msg.modelV2.leadsV3 = leads_v3
  return msg.modelV2


def lead3(x, y, v, prob=1.0):
  return {'prob': prob, 'x': [x], 'xStd': [2.0], 'y': [y], 'yStd': [1.0], 'v': [v], 'vStd': [1.0], 'a': [0.0]}


def carstate_msg(v_ego=V_EGO):
  msg = messaging.new_message('carState')
  msg.carState.vEgo = v_ego
  return msg.carState


def live_tracks(points):
  """points: list of (trackId, dRel, yRel, vRel) → a liveTracks reader."""
  msg = messaging.new_message('liveTracks')
  msg.liveTracks.points = [{'trackId': t, 'dRel': d, 'yRel': y, 'vRel': v, 'measured': True}
                           for (t, d, y, v) in points]
  return msg.liveTracks


class FakeSM:
  def __init__(self, model, carstate):
    self._data = {'modelV2': model, 'carState': carstate}
    self.seen = {'modelV2': True}
    self.logMonoTime = {'modelV2': 1000, 'carState': 1000}
    self.recv_frame = {'carState': 1}

  def __getitem__(self, key):
    return self._data[key]

  def all_checks(self):
    return True


def make_radard_tier(tier):
  rd = make_radard()
  rd.lead_tracking_mode = tier   # update() never reads Params, so a directly-set tier sticks
  return rd


# vision sees two steady, far, same-speed leads the entire time
VIS_TWO_FAR = [lead3(95.0 + RADAR_TO_CAMERA, 0.0, V_EGO), lead3(120.0 + RADAR_TO_CAMERA, 0.0, V_EGO)]


class TestUpdateCoastFlags:
  def _prime(self, rd):
    # several frames of two real radar leads matching vision (id 100 near, 101 far) so leadOne
    # locks onto id 100 and it is remembered as last-good
    sm = FakeSM(model_msg(VIS_TWO_FAR), carstate_msg())
    for _ in range(8):
      rd.update(sm, live_tracks([(100, 95.0, 0.0, 0.0), (101, 120.0, 0.0, 0.0)]))

  def test_tier3_update_sets_coast_flag_and_holds_lead(self):
    rd = make_radard_tier(3)
    self._prime(rd)
    assert rd.radar_state.leadOne.radarTrackId == 100
    assert rd.lead_coasting == {0: False, 1: False}
    assert rd.last_good_lead[0] is not None

    # phantom frame: the held track (100) drops out and only a close/slow clutter track (200)
    # remains, while vision still sees the far/fast lead → coast must hold the last-good lead
    sm = FakeSM(model_msg(VIS_TWO_FAR), carstate_msg())
    rd.update(sm, live_tracks([(200, 77.0, -3.0, -17.0)]))
    assert rd.lead_coasting[0] is True
    assert rd.lead_coasting[1] is False
    assert rd.radar_state.leadOne.radarTrackId == 100             # held last-good, not the phantom
    assert abs(rd.radar_state.leadOne.dRel - 95.0) < 1e-6

  def test_tier2_update_never_coasts(self):
    # the same phantom under Tier 2 (no vision-coast) must NOT set the coast flags
    rd = make_radard_tier(2)
    self._prime(rd)
    sm = FakeSM(model_msg(VIS_TWO_FAR), carstate_msg())
    rd.update(sm, live_tracks([(200, 77.0, -3.0, -17.0)]))
    assert rd.lead_coasting == {0: False, 1: False}
    assert rd.radar_state.leadOne.radarTrackId == 200            # the phantom is emitted, not held

  def test_radarstatesp_carries_coast_flags(self):
    # contract: the flags RadarD.publish() copies into radarStateSP survive the cereal roundtrip
    sp = messaging.new_message('radarStateSP')
    sp.radarStateSP.leadOneCoasting = True
    sp.radarStateSP.leadTwoCoasting = False
    out = messaging.log_from_bytes(sp.to_bytes()).radarStateSP
    assert out.leadOneCoasting is True and out.leadTwoCoasting is False


class TestYrelCorrection:
  """The Rivian-gated radar lateral-scale correction (the 0.5 in opendbc's interface over-compresses;
  ~0.70 best agrees with the camera). Brand-gated, applied at ingestion so matcher + track + emitted
  lead all see the same corrected lateral."""
  def _rivian_radard(self, tier=2):
    cp = car.CarParams.new_message()
    cp.brand = "rivian"
    rd = RadarD(cp, custom.CarParamsSP.new_message(), delay=0.0)
    rd.lead_tracking_mode = tier
    return rd

  def test_non_rivian_is_noop(self):
    assert make_radard().yrel_correction == 1.0           # bare CarParams → other brand → no-op

  def test_rivian_correction_factor(self):
    assert abs(self._rivian_radard().yrel_correction - RIVIAN_YREL_CORRECTION) < 1e-9
    assert abs(RIVIAN_YREL_CORRECTION - 1.4) < 1e-9        # 0.70 / 0.5

  def test_correction_applied_to_ingested_lateral(self):
    # a raw radar yRel of 1.0 becomes 1.4 in the stored track (what the matcher sees) for Rivian
    rd = self._rivian_radard()
    rd.update(FakeSM(model_msg(VIS_TWO_FAR), carstate_msg()), live_tracks([(100, 50.0, 1.0, 0.0)]))
    assert abs(rd.tracks[100].yRel - 1.4) < 1e-6
    # but an unchanged lateral for a non-Rivian radard (the no-op gate)
    rd2 = make_radard_tier(2)
    rd2.update(FakeSM(model_msg(VIS_TWO_FAR), carstate_msg()), live_tracks([(100, 50.0, 1.0, 0.0)]))
    assert abs(rd2.tracks[100].yRel - 1.0) < 1e-6
