"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import pyray as rl

from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.selfdrive.ui.sunnypilot.onroad.chevron_metrics import ChevronMetrics
from openpilot.selfdrive.ui.sunnypilot.onroad.rainbow_path import RainbowPath

LEAD_GLOW_GOLD = rl.Color(218, 202, 37, 255)   # stock lead chevron glow
COAST_CYAN = rl.Color(86, 199, 214, 255)        # vision-coast tint (matches the dev-UI COAST element)


def _lerp_color(a: rl.Color, b: rl.Color, t: float) -> rl.Color:
  inv = 1.0 - t
  return rl.Color(int(inv * a.r + t * b.r), int(inv * a.g + t * b.g),
                  int(inv * a.b + t * b.b), int(inv * a.a + t * b.a))


class ModelRendererSP:
  def __init__(self):
    self.rainbow_path = RainbowPath()
    self.chevron_metrics = ChevronMetrics()

  def _lead_glow_color(self, i: int) -> rl.Color:
    """Per-lead chevron glow color, called by the base _draw_lead_indicator. Stock gold unless the
    ShowVisionCoastOnChevron debug toggle is on AND Tier-3 vision-coast is currently holding lead i
    (from radarStateSP) — then lerp toward cyan so it reads as 'held by vision', per-lead."""
    if not getattr(ui_state, "show_vision_coast", False):
      return LEAD_GLOW_GOLD

    sm = ui_state.sm
    if not sm.valid["radarStateSP"]:
      return LEAD_GLOW_GOLD

    rs = sm["radarStateSP"]
    coasting = rs.leadOneCoasting if i == 0 else rs.leadTwoCoasting
    return _lerp_color(LEAD_GLOW_GOLD, COAST_CYAN, 0.6) if coasting else LEAD_GLOW_GOLD
