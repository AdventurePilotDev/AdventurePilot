import math
from collections import deque

from opendbc.can import CANParser

# Panda real-time torque check (lateral.h): the reference torque is refreshed only every 250 ms, and a TOI blip
# frame restarts that interval WITHOUT refreshing the reference, so right after a blip the reference can be
# ~0.5 s old. A full-rate ramp (3 counts/frame) then climbs more than the panda's 125 from it, the panda refuses
# the frame and zeroes its torque memory, and the rest of the ramp is refused too (route 4440a486580ed7c6/00000112
# seg 15: 5 s of refused torque, then a latched EPAS ToiFlt). TorqueRtLimiter keeps every request within a margin
# of every torque the panda could be holding as its reference.
TORQUE_RT_MAX_DELTA = 120   # panda allows 125
HIST_FRAMES = 30            # reference is at most one 250 ms interval (26 frames) old, plus margin
REF_AGE_FRAMES = 28         # how far before a blip the stale reference can have been taken, plus margin
HIST_MAX = HIST_FRAMES + 2 * REF_AGE_FRAMES


class TorqueRtLimiter:
  def __init__(self):
    self.hist: deque[int] = deque(maxlen=HIST_MAX)  # torque sent with the request bit high (not blip frames)
    self.frames_since_blip = HIST_MAX

  def reset(self):
    """torque is not being sent: the panda's memory is zero too"""
    self.hist.clear()
    self.frames_since_blip = HIST_MAX

  def refused(self):
    """the panda refused a frame: it has zeroed its rate-limit and real-time memory"""
    self.hist.clear()
    self.hist.append(0)

  def limit(self, torque: int) -> int:
    # Until the panda refreshes its reference (about 26 frames after a blip) it can be holding any torque sent from
    # REF_AGE_FRAMES before the blip onward; otherwise any torque from the last interval.
    n = HIST_FRAMES
    if self.frames_since_blip < REF_AGE_FRAMES:
      n = self.frames_since_blip + REF_AGE_FRAMES
    hist = list(self.hist)[-n:]
    if not hist:
      return torque
    upper = max(min(hist), 0) + TORQUE_RT_MAX_DELTA
    lower = min(max(hist), 0) - TORQUE_RT_MAX_DELTA
    return max(lower, min(upper, torque))

  def sent(self, torque: int):
    """a frame with the request bit high went out"""
    self.hist.append(torque)
    self.frames_since_blip += 1

  def blip(self):
    """a frame with the request bit low (torque 0) went out"""
    self.frames_since_blip = 0


def refused_torque_parser(dbc_name: str) -> CANParser:
  """Frames the panda refuses to send come back on bus 192 (128 + 64); only ACM_lkaHbaCmd (the torque request) is read
  from it. These frames are rare and not a continuous stream, so exempt the parser from the timeout, counter and
  checksum validity checks that would otherwise raise canError."""
  parser = CANParser(dbc_name, [("ACM_lkaHbaCmd", math.nan)], 192)
  for state in parser.message_states.values():
    state.ignore_counter = True
    state.ignore_checksum = True
  return parser
