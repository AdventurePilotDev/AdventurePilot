"""The UI must not subscribe to the same service twice.

The UI builds its subscription list from two halves: the stock list in ui_state.py and the
sunnypilot additions in sunnypilot/ui_state.py. When an upstream sync moves a service from one
half to the other and the merge keeps the old line, the service ends up in both.

That is not harmless. SubMaster opens a socket per entry, so a duplicated service is received
twice in the same update, and the second copy records a time gap of exactly zero. On the very
first message the average gap is then zero and the frequency check divides by it. The UI process
dies with ZeroDivisionError, the screen is left on the boot logo, and openpilot disengages with
"Process Not Running". It happened on a road test when the first GPS fix arrived a few minutes
into the drive, because gpsLocation was listed in both halves.

This reads the list from the real ui_state singleton rather than from source text, so it sees
exactly what the running UI subscribes to.
"""
from collections import Counter

from openpilot.selfdrive.ui.ui_state import ui_state


def test_ui_subscribes_to_each_service_once():
  services = ui_state.sm.services
  duplicates = sorted(s for s, n in Counter(services).items() if n > 1)
  assert not duplicates, f"UI subscribes to these services more than once: {duplicates}"
