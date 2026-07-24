import numpy as np
from opendbc.can import CANPacker
from opendbc.car import Bus
from opendbc.car.interfaces import CarControllerBase
from opendbc.car.rivian.angle_toggle import AngleSteerToggle
from opendbc.car.rivian.ext_controller import ExternalController, get_safety_CP  # noqa: F401
from opendbc.car.rivian.riviancan import create_angle_steering, create_lka_steering, create_longitudinal, create_wheel_touch, create_adas_status, create_acm_status
from opendbc.car.rivian.values import CarControllerParams, RivianFlags

from opendbc.sunnypilot.car.rivian.mads import MadsCarController

# single-panda xnor-box branch: angle stream on the car-side bus only. (The dual-intercept
# variant mirrors these on bus 4 for the EPAS 2-of-2 voter — see archive/unified-4h.)
ANGLE_TX_BUSES = (0,)


class CarController(CarControllerBase, MadsCarController):
  def __init__(self, dbc_names, CP, CP_SP):
    CarControllerBase.__init__(self, dbc_names, CP, CP_SP)
    MadsCarController.__init__(self)
    self.apply_torque_last = 0
    self.packer = CANPacker(dbc_names[Bus.pt])
    self.cancel_frames = 0
    self.erc = ExternalController(CP)
    self.angle_harness = bool(CP.flags & RivianFlags.ANGLE_HARNESS)
    try:
      from openpilot.common.params import Params  # lazy: keep opendbc importable standalone (safety tests)
      self._params = Params()
    except Exception:
      self._params = None
    # user steering-primary selection (angle-harness trucks only): 1 = angle primary (hands-off derived
    # angle, default), 0 = torque primary (torque-only LKA). Serves as the toggle's master switch.
    self._angle_primary = True
    if self._params is not None:
      self._angle_primary = self._params.get_bool("RivianAnglePrimary")
    # onroad wheel-tap hold-to-confirm toggle (angle hardware only)
    self._angle_toggle = AngleSteerToggle()
    self._angle_req_last = False
    self._angle_tap = False
    self._angle_phase_last = 0

  def update_live_params(self, roll, angle_offset_deg):
    self.erc.roll = roll
    self.erc.angle_offset_deg = angle_offset_deg

  def update(self, CC, CC_SP, CS, now_nanos):
    MadsCarController.update(self, CC, CC_SP, CS)
    actuators = CC.actuators
    can_sends = []

    steer_max = round(float(np.interp(CS.out.vEgoRaw, CarControllerParams.STEER_MAX_LOOKUP[0],
                                      CarControllerParams.STEER_MAX_LOOKUP[1])))

    # Rivian angle-steering hold-to-confirm toggle (angle hardware only). The state machine reads the
    # capacitive-touch hands-on signal + current channel from the ExternalController (prior frame), sets
    # force_torque for this frame, and publishes the UI message phase via a param. RivianAnglePrimary is
    # the persistent master (torque primary -> master off -> pinned torque, tap inert).
    if self.angle_harness:
      if self._params is not None:
        if self.frame % 10 == 0:
          req = self._params.get_bool("RivianForceTorqueSteerReq")
          if req != self._angle_req_last:
            self._angle_tap = True  # a tap flips the request bool; consumed as a one-frame edge below
          self._angle_req_last = req
        if self.frame % 50 == 0:
          self._angle_primary = self._params.get_bool("RivianAnglePrimary")
      tap = self._angle_tap
      self._angle_tap = False
      self.erc.force_torque = self._angle_toggle.update(tap, self.erc.hands_on, self.erc.torque_active,
                                                        self.mads.lat_active, self._angle_primary)
      if self._params is not None:
        phase = int(self._angle_toggle.phase)
        if phase != self._angle_phase_last:
          self._params.put("RivianAngleSteerPhase", phase)  # INT param: must be an int, not str
          self._angle_phase_last = phase

    self.erc.update(CS, self.mads.lat_active, actuators)
    apply_torque = self.erc.torque_cmd

    # send steering command; torque is 0 and toi_act_cmd low during a ToiFlt-avoidance blip
    # (erc freezes its rate-limiter memory through the blip so assist resumes instantly)
    self.apply_torque_last = apply_torque
    can_sends.append(create_lka_steering(self.packer, self.frame, CS.acm_lka_hba_cmd, apply_torque, CC.enabled, self.erc.toi_act_cmd, self.mads))

    if self.angle_harness:
      # 0x110 angle stream + 0x100 status: streamed continuously — the harness cuts the stock
      # ACM's copies, so ours replace them; EacEnabled/Hwp only flip while actively steering,
      # otherwise 0x100 mirrors the stock cruise state. Without angle hardware these MUST NOT
      # be sent: the live stock ACM still broadcasts them (counter/checksum collision).
      if self.mads.lat_active:
        feature_status = 1 if self.erc.torque_active else 2  # 1=Acc, 2=Hwp unlocks external 0x110
      else:
        feature_status = 1 if CS.out.cruiseState.enabled else 0  # mirror stock cruise state
      for bus in ANGLE_TX_BUSES:
        can_sends.append(create_angle_steering(self.packer, self.frame, self.erc.apply_angle_last, self.erc.angle_active, bus))
        can_sends.append(create_acm_status(self.packer, self.frame, feature_status, bus))

    if self.frame % 5 == 0 and not (self.CP.flags & RivianFlags.GEN2):
      can_sends.append(create_wheel_touch(self.packer, CS.sccm_wheel_touch, self.mads.lat_active))

    # Longitudinal control
    if self.CP.openpilotLongitudinalControl:
      accel = actuators.accel
      if CC.longActive:
        # Cancel the VDM's uncompensated regen/creep drag so the truck delivers the accel we ask for
        # (less over-braking, more willing accel). Speed-scheduled, ramps from 0 at standstill so we
        # still hold the brake at a stop. See CarControllerParams.ACCEL_FF_DRAG_*.
        accel += float(np.interp(CS.out.vEgo, CarControllerParams.ACCEL_FF_DRAG_BP, CarControllerParams.ACCEL_FF_DRAG_V))
      accel = float(np.clip(accel, CarControllerParams.ACCEL_MIN, CarControllerParams.ACCEL_MAX))
      can_sends.append(create_longitudinal(self.packer, self.frame, accel, CC.enabled))
    else:
      interface_status = None
      if CC.cruiseControl.cancel:
        # if there is a noEntry, we need to send a status of "available" before the ACM will accept "unavailable"
        # send "available" right away as the VDM itself takes a few frames to acknowledge
        interface_status = 1 if self.cancel_frames < 5 else 0
        self.cancel_frames += 1
      else:
        self.cancel_frames = 0

      for msg in CS.vdm_adas_status:
        can_sends.append(create_adas_status(self.packer, msg, interface_status))

    new_actuators = actuators.as_builder()
    # on angle the torque channel is idle, not limited; echo the request so
    # steer_limited_by_safety stays false and the saturation warning keeps working
    new_actuators.torque = apply_torque / steer_max if self.erc.torque_active else float(actuators.torque)
    new_actuators.torqueOutputCan = apply_torque
    new_actuators.steeringAngleDeg = self.erc.apply_angle_last

    self.frame += 1
    return new_actuators, can_sends
