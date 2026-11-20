"""
Copyright (c) 2026-, Zeph Leggett.

This file is part of AdventurePilot and is licensed under the MIT License.
See the LICENSE.md file in the root directory for more details.

The user's say over the Jetlink setting, shared by the mici and tici models
panels: Off, USB (a Jetson, a Linux PC or a Mac) or iOS (an iPhone), stored as
an index into the adapter's MODES. The small model is picked as ever: manager
runs whichever modeld that bundle needs and the link joins it, so the setting
never changes which modeld runs. Everything else here reads ui_state.jetlink,
the snapshot the params pass takes.
"""
from openpilot.selfdrive.ui.ui_state import ui_state
from openpilot.sunnypilot.jetlink_adapter import KEYS, MODES
from openpilot.system.ui.lib.multilang import tr

LINK_MODE_TITLES = {"off": "Off", "usb": "USB", "ios": "iOS"}


def link_mode() -> str:
  """The setting as stored, one of MODES: the panel shows it even when
  jetlink cannot run, so a link that will not start can be turned off."""
  index = ui_state.params.get(KEYS.link, return_default=True)
  return MODES[index] if isinstance(index, int) and 0 <= index < len(MODES) else "off"


def link_toggle_meaningful() -> bool:
  """Offered wherever jetlink is checked out, as the chestnut slot is offered
  whether or not a board is fitted, except beside a chestnut, which runs the
  big model itself; and while the setting is on, whatever else. Presence
  cannot be the gate: with the link off there is no gadget for a Jetson to
  enumerate, so the toggle that turns it on would wait for the thing it enables."""
  if ui_state.chestnut_present:
    return False
  return ui_state.jetlink is not None or link_mode() != "off"


def link_status() -> str:
  """One line under the toggle: what is on the comma's USB-C port right now."""
  jetlink = ui_state.jetlink
  port = jetlink.port_state if jetlink is not None else None
  if port == "connected":
    return f"{tr('Jetlink connected:')} {jetlink.transport}."
  return {"empty": tr("Nothing on the USB port."), "device": tr("A device is on the USB port.")}.get(port, "")


def big_model_progress() -> tuple[str, float, str] | None:
  """(stage, 0..1, message) while jetlink is working, else None."""
  jetlink = ui_state.jetlink
  progress = jetlink.progress if jetlink is not None else None
  if not progress:
    return None
  stage = str(progress.get('stage', ''))
  if stage in ('', 'ready'):
    return None
  msg = tr(str(progress.get('msg', '')))
  if drops := progress.get('drops'):
    hint = tr("check cable or app") if jetlink.mode == 'ios' else tr("check cable")
    msg = tr("{}, {} ({} drops)").format(msg, hint, drops)
  return stage, float(progress.get('frac', 0.0)), msg


def jetlink_note() -> str:
  """What the models panel says under the Jetlink row."""
  if not link_toggle_meaningful():
    return ""
  if (progress := big_model_progress()) is not None:
    return progress[2]
  return link_status()


def cycle_link_mode() -> None:
  """Off -> USB -> iOS -> Off, stored as the index jetlink reads."""
  if not link_toggle_meaningful():
    return
  index = (MODES.index(link_mode()) + 1) % len(MODES)
  ui_state.params.put(KEYS.link, index, block=True)
