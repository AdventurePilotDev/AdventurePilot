# AdventurePilot dev and dev-a: what's new (October 2026)

This update brings dev (torque steering) and dev-a (angle steering, needs the angle harness) up to the
latest sunnypilot and openpilot, and adds several Rivian fixes and two new cruise settings. Nothing
changes how the car drives until you go looking for the new settings: every new option defaults to the
old behavior.

## Rivian fixes (dev and dev-a)

- **Shifting gear no longer upsets the steering.** Going D -> R -> D while parking could switch steering
  assist on by itself and end in "Take Control Immediately / Controls Mismatch". The ACC stalk is also the
  gear selector, and pushing it down to leave Reverse was being read as an ACC press. Now only a push
  while already in Drive counts.
- **You are told why the truck refused ACC.** If you press for ACC and the truck's own cruise module
  declines, openpilot now explains, about half a second later:
  - "Rivian ACC Not Ready Yet" for roughly the first minute after the truck wakes, and in Park
  - "ACC Needs 20 mph or a Lead Car" below 20 mph with nothing ahead

  This only shows a message. It never engages anything.
- **Steering comes back by itself after you push against the wheel.** If you push hard against the
  wheel while openpilot is steering with torque, the steering rack can decide you have taken over and quietly stop
  accepting openpilot's steering, with no fault and no warning. Until now it could stay that way for
  minutes while openpilot still showed it was steering. openpilot now notices within a third of a
  second and re-arms the rack. If that does not bring steering back, "Steering Temporarily Unavailable"
  stays on screen for at least 3 seconds, so you know to steer yourself.
- **Fewer cruise and steering faults when the device briefly stalls.** If the device freezes for a
  fraction of a second, the steering and cruise messages to the truck could stop long enough to latch
  an "LKAS Fault" or "ACC Fault" that only cleared when the truck slept. The panda link now repeats the
  last command through short gaps, so a brief stall no longer faults the car.
- **openpilot cruise only takes over when the longitudinal harness is fitted.** A leftover "openpilot
  longitudinal control" setting (for example from restored settings) could hand braking and acceleration
  to openpilot on a truck without the harness, where nothing would act on it. It is now ignored unless
  the harness is detected.

## Angle steering only (dev-a)

- **No more "LKAS Fault" when braking with angle steering.** With MADS set to pause or disengage steering
  on the brake, braking at low speed could fault the steering rack ("LKAS Fault: Restart the car to
  engage"). Steering now stops on the very frame the brake is pressed.

## New cruise settings (dev and dev-a)

Both settings need openpilot longitudinal control and can only be changed while parked, on the device or
in Sunnylink. Changes take effect after changing from OffRoad to OnRoad.

- **Following Distance Offset.** Nudge the gap to the car in front from -20% to +20% in steps of 5%, on
  top of whichever driving personality you use. Default 0 (no change).
  Guide: [Following Distance Offset](https://github.com/AdventurePilotDev/AdventurePilot/blob/instructions/following-distance-offset.md)
- **Minimum Stop Distance.** Stop closer behind the car in front in slow traffic: anywhere from 4.5 m up
  to the standard 6.0 m. It only applies at set speeds of 50 kph / 31 mph or below. An optional switch
  ties it to driving personality (aggressive gets the full reduction, standard half, relaxed none).
  Default 6.0 m (no change).
  Guide: [Minimum Stop Distance](https://github.com/AdventurePilotDev/AdventurePilot/blob/instructions/minimum-stop-distance.md)

## Kept: Dynamic Experimental Control

sunnypilot retired Dynamic Experimental Control (DEC) upstream. AdventurePilot keeps it, so the toggle
stays on the cruise page and works as before.

## From sunnypilot and openpilot

- Newer driving model build: models are now compiled ahead of time, including a precompiled model for
  the comma four's external GPU.
- Duplicate MADS "can't engage" alerts are fixed, and the comma four shows a MADS icon when steering is
  engaged.
- Smoother recovery after the device briefly stalls: control loops pick up from the current time instead
  of rushing through missed frames.
- Settings now have descriptions. Alerts have custom icons.
- comma four: buttons to refresh and clear downloaded models, plus eSIM profile management.
- The driver camera infrared light can be switched off while parked.
- Crash reporting to Sentry has been removed upstream.
- Many developer tool improvements (cabana rewritten without Qt, faster route loading in replay).

## After updating

- The first start after the update rebuilds the software and takes longer than usual.
- If something looks wrong, please send the route ID along with a short description of what happened
  and when.
