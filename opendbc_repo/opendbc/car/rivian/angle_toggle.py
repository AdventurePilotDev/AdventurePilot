from opendbc.car.rivian.values import RivianAngleSteerPhase

# 100 Hz control loop.
HOLD_FRAMES = 500      # 5s hold-to-confirm window
TRANSIENT_FRAMES = 15  # how long a one-shot phase is held so selfdrived (async param read) catches the edge


class AngleSteerToggle:
  """Pure state machine for the Rivian angle-steering hold-to-confirm toggle.

  Lives on the card side (CarController) because it needs the capacitive-touch hands-on signal and the
  instantaneous steering channel, which only exist inside ExternalController. It owns no params/IO:
  update() takes the raw inputs and returns force_torque; self.phase is the UI message phase.
  """
  IDLE_ANGLE, PENDING_DEACT, TORQUE_FORCED, PENDING_ACT = range(4)

  def __init__(self):
    self.state = self.IDLE_ANGLE
    self.timer = 0
    self.phase = RivianAngleSteerPhase.QUIET
    self._transient = 0
    self._transient_phase = RivianAngleSteerPhase.QUIET

  def _fire(self, phase: RivianAngleSteerPhase) -> None:
    self._transient_phase = phase
    self._transient = TRANSIENT_FRAMES

  def update(self, tap: bool, hands_on: bool, torque_active: bool, lat_active: bool, master_on: bool) -> bool:
    """Advance one frame. Returns force_torque (whether to pin torque-only this frame)."""
    # Master off: torque-only for the whole drive, toggle inert, no messages.
    if not master_on:
      self.state = self.IDLE_ANGLE
      self.timer = 0
      self._transient = 0
      self.phase = RivianAngleSteerPhase.QUIET
      return True

    # Not actively steering: abandon any in-progress hold (stale), keep the committed mode, no messages.
    if not lat_active:
      if self.state == self.PENDING_DEACT:
        self.state = self.IDLE_ANGLE
      elif self.state == self.PENDING_ACT:
        self.state = self.TORQUE_FORCED
      self.timer = 0
      self._transient = 0
      self.phase = RivianAngleSteerPhase.QUIET
      return self.state == self.TORQUE_FORCED

    if self._transient > 0:
      self._transient -= 1

    if self.state == self.IDLE_ANGLE:
      if tap:
        if torque_active:  # driver already holding -> already torque -> lock it now
          self.state = self.TORQUE_FORCED
          self._fire(RivianAngleSteerPhase.DEACTIVATED)
        else:
          self.state = self.PENDING_DEACT
          self.timer = HOLD_FRAMES
    elif self.state == self.PENDING_DEACT:
      self.timer -= 1
      if tap:  # second tap cancels
        self.state = self.IDLE_ANGLE
        self._fire(RivianAngleSteerPhase.DEACTIVATE_CANCELED)
      elif hands_on:
        self.state = self.TORQUE_FORCED
        self._fire(RivianAngleSteerPhase.DEACTIVATED)
      elif self.timer <= 0:
        self.state = self.IDLE_ANGLE
        self._fire(RivianAngleSteerPhase.DEACTIVATE_TIMEOUT)
    elif self.state == self.TORQUE_FORCED:
      if tap:
        self.state = self.PENDING_ACT
        self.timer = HOLD_FRAMES
    elif self.state == self.PENDING_ACT:
      self.timer -= 1
      if tap:  # second tap cancels
        self.state = self.TORQUE_FORCED
        self._fire(RivianAngleSteerPhase.ACTIVATE_CANCELED)
      elif hands_on:
        self.state = self.IDLE_ANGLE
        self._fire(RivianAngleSteerPhase.REACTIVATED)
      elif self.timer <= 0:
        self.state = self.TORQUE_FORCED
        self._fire(RivianAngleSteerPhase.ACTIVATE_TIMEOUT)

    # Phase: pending states show their prompt continuously; otherwise a one-shot transient (if live), else quiet.
    if self.state == self.PENDING_DEACT:
      self.phase = RivianAngleSteerPhase.HOLD_TO_DEACTIVATE
    elif self.state == self.PENDING_ACT:
      self.phase = RivianAngleSteerPhase.HOLD_TO_REACTIVATE
    elif self._transient > 0:
      self.phase = self._transient_phase
    else:
      self.phase = RivianAngleSteerPhase.QUIET

    # Torque is pinned while forced and while waiting to re-enable angle (still torque until confirmed).
    return self.state in (self.TORQUE_FORCED, self.PENDING_ACT)
