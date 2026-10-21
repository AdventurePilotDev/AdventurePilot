# Following Distance Offset

**Applies to:** any car where sunnypilot controls the speed (openpilot longitudinal control). On a Rivian R1T / R1S that needs the xnor longitudinal harness (xnor.shop).

Following Distance Offset lets you nudge how far back the car follows a moving vehicle in front, without giving up the driving personality you like. Pick Aggressive, Standard or Relaxed as usual, then move the gap a little closer or a little further to taste.

**Where it has landed:** the `dev` and `dev-a` development trunks. Not in the `stg` or `rel` prebuilts yet.

---

## What it does

Each driving personality keeps a time gap to the car in front: Aggressive about 1.25 seconds, Standard 1.45 seconds and Relaxed 1.75 seconds. This setting scales that gap up or down by a percentage, in whichever personality is active.

- **Negative values** follow closer.
- **Positive values** follow further back.
- **0** (shown as **Default**) changes nothing.

It only changes the gap while both cars are moving. How close the car stops behind a stopped car is a separate setting: see [Minimum Stop Distance](minimum-stop-distance.md).

---

## Turning it on

1. Park the car. The setting can only be changed while parked (OffRoad).
2. Open the **Cruise** menu on the device, or the **Cruise** page in Sunnylink.
3. Set **Following Distance Offset** anywhere from **-20%** to **+20%**, in steps of 5%.

The new value takes effect after changing from OffRoad to OnRoad.

---

## What the numbers mean

| Personality | Default gap | At -20% | At +20% |
| :---- | :---- | :---- | :---- |
| Aggressive | 1.25 s | 1.00 s | 1.50 s |
| Standard | 1.45 s | 1.16 s | 1.74 s |
| Relaxed | 1.75 s | 1.40 s | 2.10 s |

As a rough guide, each 5% step moves the gap by about 1 m (3 ft) at 50 km/h (31 mph), and by about 2 m (6 ft) at 100 km/h (62 mph).

---

## Things to keep in mind

- **Needs openpilot longitudinal control.** The setting is unavailable without it.
- **Parked only, read at the start of a drive.** Changing it mid-drive is not possible, and a new value is picked up after changing from OffRoad to OnRoad.
- **You can still switch personality at any time.** The offset is applied on top of whichever personality is active.
- **Acceleration and braking feel are unchanged.** Only the gap is scaled; how smoothly each personality speeds up and slows down stays the same.
- This assists you; it does not replace attention. A closer gap leaves less room to react if the car ahead brakes hard.
