# Minimum Stop Distance

**Applies to:** any car where sunnypilot controls the speed (openpilot longitudinal control). On a Rivian R1T / R1S that needs the xnor longitudinal harness (xnor.shop).

Minimum Stop Distance lets the car stop closer behind a stopped vehicle in slow traffic. As standard, sunnypilot stops 6.0 m (about 20 ft) short of the car in front, which can feel like a lot in town queues. This setting brings that down to as little as 4.5 m (about 15 ft).

**Where it has landed:** the `dev` and `dev-a` development trunks. Not in the `stg` or `rel` prebuilts yet.

---

## What it does

When the car ahead comes to a stop, your car stops behind it at the gap you have chosen instead of the standard 6.0 m.

The shorter gap only applies in slow traffic, when your **cruise set speed is 50 km/h (31 mph) or below**. Above that set speed the car always goes back to stopping 6.0 m short, whatever the setting says. A closer stop is useful in town; it is not wanted on a fast road.

It only changes the gap behind a stopped car. How far back the car follows a moving car is a separate setting: see [Following Distance Offset](following-distance-offset.md).

---

## Turning it on

1. Park the car. The settings can only be changed while parked (OffRoad).
2. Open the **Cruise** menu on the device, or the **Cruise** page in Sunnylink.
3. Set **Minimum Stop Distance** to **4.5 m**, **5.0 m**, **5.5 m** or **6.0 m** (the default, no change).
   In Sunnylink the value is shown in tenths of a meter: 45 means 4.5 m and 60 means 6.0 m.

The new value takes effect after changing from OffRoad to OnRoad.

---

## Linking it to your driving personality

Once the stop distance is set below 6.0 m, a second switch appears: **Link Stop Distance to Personality**. With it on, the gap follows your driving personality instead of being fixed:

| Personality | Gap behind a stopped car |
| :---- | :---- |
| Aggressive | The distance you set (for example 4.5 m) |
| Standard | Halfway between your distance and 6.0 m (for example 5.25 m) |
| Relaxed | Always the standard 6.0 m |

This lets you keep the tight stop for when you want it and get the standard gap back just by switching personality. Setting the stop distance back to 6.0 m hides the switch again.

---

## Things to keep in mind

- **Needs openpilot longitudinal control.** The settings are unavailable without it.
- **Parked only, read at the start of a drive.** A new value is picked up after changing from OffRoad to OnRoad.
- **Only below a 50 km/h (31 mph) set speed.** Above that, the car stops 6.0 m short as standard.
- **There is still a safety margin.** The car will not plan to get closer than about three quarters of the chosen gap, so at 4.5 m it keeps at least about 3.4 m (11 ft) even while braking.
- This assists you; it does not replace attention. Be ready to brake yourself, especially when the car ahead stops suddenly.
