import time
import pyray as rl
from opendbc.car.rivian.values import RivianFlags
from openpilot.common.params import Params
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.system.ui.lib.application import gui_app
from openpilot.system.ui.widgets import Widget


class ExpButton(Widget):
  def __init__(self, button_size: int, icon_size: int):
    super().__init__()
    self._params = Params()
    self._experimental_mode: bool = False
    self._engageable: bool = False

    # State hold mechanism
    self._hold_duration = 2.0  # seconds
    self._held_mode: bool | None = None
    self._hold_end_time: float | None = None

    self._white_color: rl.Color = rl.Color(255, 255, 255, 255)
    self._black_bg: rl.Color = rl.Color(0, 0, 0, 166)
    self.wheel_tint: rl.Color | None = None  # optional RGB tint for the button icon
    self._txt_wheel: rl.Texture = gui_app.texture('icons/chffr_wheel.png', icon_size, icon_size)
    self._txt_exp: rl.Texture = gui_app.texture('icons/experimental.png', icon_size, icon_size)
    self._rect = rl.Rectangle(0, 0, button_size, button_size)

  def set_rect(self, rect: rl.Rectangle) -> None:
    self._rect.x, self._rect.y = rect.x, rect.y

  def _update_state(self) -> None:
    selfdrive_state = ui_state.sm["selfdriveState"]
    self._experimental_mode = selfdrive_state.experimentalMode
    self._engageable = selfdrive_state.engageable or selfdrive_state.enabled

  def _handle_mouse_release(self, _):
    super()._handle_mouse_release(_)
    # while MADS is actively steering an angle-capable Rivian with Experimental off, the wheel tap
    # toggles angle/torque steering instead of Experimental mode. We flip the request bool; CarController
    # reads the edge and runs the hold-to-confirm state machine (+ on-screen messages via the phase param).
    # When RivianAnglePrimary (torque primary) the tap is a no-op (already torque-only).
    if self._torque_toggle_ctx():
      self._params.put_bool("RivianForceTorqueSteerReq", not self._params.get_bool("RivianForceTorqueSteerReq"))
    elif self._is_toggle_allowed():
      new_mode = not self._experimental_mode
      self._params.put_bool("ExperimentalMode", new_mode)

      # Hold new state temporarily
      self._held_mode = new_mode
      self._hold_end_time = time.monotonic() + self._hold_duration

  def _torque_toggle_ctx(self) -> bool:
    # Experimental on keeps the tap bound to Experimental mode (v1 shares one icon)
    if self._experimental_mode:
      return False
    cp = ui_state.CP
    if cp is None or cp.brand != "rivian" or not (cp.flags & RivianFlags.ANGLE_HARNESS):
      return False
    # master switch off (torque primary) => torque-only, tap is a no-op
    if not self._params.get_bool("RivianAnglePrimary"):
      return False
    # only while MADS is actively steering (fresh carControl with lateral active)
    sm = ui_state.sm
    return sm.recv_frame["carControl"] >= ui_state.started_frame and sm["carControl"].latActive

  def _render(self, rect: rl.Rectangle) -> None:
    center_x = int(self._rect.x + self._rect.width // 2)
    center_y = int(self._rect.y + self._rect.height // 2)

    self._white_color.a = 180 if self.is_pressed or not self._engageable else 255

    exp_mode = self._held_or_actual_mode()
    texture = self._txt_exp if exp_mode else self._txt_wheel

    color = self._white_color
    tint = None
    if self.wheel_tint is not None:
      tint = rl.Color(self.wheel_tint.r, self.wheel_tint.g, self.wheel_tint.b, self._white_color.a)

    rl.draw_circle(center_x, center_y, self._rect.width / 2, self._black_bg)
    if tint is not None:
      if exp_mode:
        # tinting the colored experimental icon is invisible, show a ring instead
        radius = self._rect.width / 2
        rl.draw_ring(rl.Vector2(center_x, center_y), radius - 8, radius, 0, 360, 0, tint)
      else:
        color = tint
    rl.draw_texture_ex(texture, rl.Vector2(center_x - texture.width / 2, center_y - texture.height / 2), 0.0, 1.0, color)

  def _held_or_actual_mode(self):
    now = time.monotonic()
    if self._hold_end_time and now < self._hold_end_time:
      return self._held_mode

    if self._hold_end_time and now >= self._hold_end_time:
      self._hold_end_time = self._held_mode = None

    return self._experimental_mode

  def _is_toggle_allowed(self):
    if not self._params.get_bool("ExperimentalModeConfirmed"):
      return False

    # Mirror exp mode toggle using persistent car params
    return ui_state.has_longitudinal_control
