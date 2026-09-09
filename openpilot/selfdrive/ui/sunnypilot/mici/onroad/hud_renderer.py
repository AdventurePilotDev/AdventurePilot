"""
Copyright (c) 2021-, Haibin Wen, sunnypilot, and a number of other contributors.

This file is part of sunnypilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.
"""
import pyray as rl

from openpilot.selfdrive.ui.mici.onroad.hud_renderer import HudRenderer
from openpilot.selfdrive.ui.sunnypilot.onroad.blind_spot_indicators import BlindSpotIndicators
from openpilot.selfdrive.ui.sunnypilot.onroad.lane_centering_indicator import LaneCenteringIndicator


class HudRendererSP(HudRenderer):
  def __init__(self):
    super().__init__()
    self.blind_spot_indicators = BlindSpotIndicators()
    self.lane_centering_indicator = LaneCenteringIndicator(compact=True)

  def _update_state(self) -> None:
    super()._update_state()
    self.blind_spot_indicators.update()
    self.lane_centering_indicator.update()

  def _render(self, rect: rl.Rectangle) -> None:
    super()._render(rect)
    self.blind_spot_indicators.render(rect)
    self.lane_centering_indicator.render(rect)

  def _has_blind_spot_detected(self) -> bool:

    return self.blind_spot_indicators.detected
