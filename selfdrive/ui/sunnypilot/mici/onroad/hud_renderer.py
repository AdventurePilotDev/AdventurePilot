"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import pyray as rl

from opendbc.car.rivian.values import RivianFlags
from openpilot.common.params import Params
from openpilot.selfdrive.ui.mici.onroad.hud_renderer import HudRenderer
from openpilot.selfdrive.ui.sunnypilot.onroad.blind_spot_indicators import BlindSpotIndicators
from openpilot.selfdrive.ui.sunnypilot.onroad.lateral_mode import lateral_mode
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import MousePos


class HudRendererSP(HudRenderer):
  def __init__(self):
    super().__init__()
    self.blind_spot_indicators = BlindSpotIndicators()
    # comma 4 angle/torque tap toggle on the wheel icon (backend is device-agnostic; this is the trigger)
    self._params = Params()
    self._angle_tap_armed = False
    self._angle_tap_consumed = False

  def _update_state(self) -> None:
    super()._update_state()
    self.blind_spot_indicators.update()
    lateral_mode.update()
    self.wheel_tint = lateral_mode.wheel_tint

  def _render(self, rect: rl.Rectangle) -> None:
    super()._render(rect)
    self.blind_spot_indicators.render(rect)

  def _has_blind_spot_detected(self) -> bool:

    return self.blind_spot_indicators.detected

  def _torque_toggle_ctx(self) -> bool:
    # Mirror of ExpButton._torque_toggle_ctx (duplicated so the big-UI path stays untouched):
    # Experimental off, angle-harness Rivian, master switch on (RivianAnglePrimary), and MADS steering.
    if ui_state.sm["selfdriveState"].experimentalMode:
      return False
    cp = ui_state.CP
    if cp is None or cp.brand != "rivian" or not (cp.flags & RivianFlags.ANGLE_HARNESS):
      return False
    if not self._params.get_bool("RivianAnglePrimary"):
      return False
    sm = ui_state.sm
    return sm.recv_frame["carControl"] >= ui_state.started_frame and sm["carControl"].latActive

  def _handle_mouse_press(self, mouse_pos: MousePos) -> None:
    # arm a wheel tap only if the press lands on the wheel and the toggle is currently allowed
    self._angle_tap_armed = (self._wheel_hit_rect is not None and
                             rl.check_collision_point_rec(mouse_pos, self._wheel_hit_rect) and
                             self._torque_toggle_ctx())

  def _handle_mouse_release(self, mouse_pos: MousePos) -> None:
    # a genuine tap = press + release both on the wheel; flip the request and consume the tap so the
    # road view does not also navigate to the home screen (checked via angle_tap_consumed()).
    if self._angle_tap_armed and self._wheel_hit_rect is not None and rl.check_collision_point_rec(mouse_pos, self._wheel_hit_rect):
      self._params.put_bool("RivianForceTorqueSteerReq", not self._params.get_bool("RivianForceTorqueSteerReq"))
      self._angle_tap_consumed = True
    self._angle_tap_armed = False

  def angle_tap_consumed(self) -> bool:
    consumed, self._angle_tap_consumed = self._angle_tap_consumed, False
    return consumed
