# Rivian R1T Wheelbase and Rear-Axle Off-Tracking

**Applies to:** Rivian R1T and R1S. Rivian-only. Works with or without the angle harness.

**Where it has landed:** the `dev` and `dev-a` development trunks, from 31 October 2026. Not yet on `stg` / `stg-a` or `rel` / `rel-a`.

Two related changes, both from Gerrylum's work on his own R1T:

- **The R1T now steers with its own wheelbase.** Automatic, nothing to set.
- **Rear-Axle Off-Tracking Compensation**, an optional setting that widens tight turns so the rear wheels are less likely to clip a curb. **Off by default.**

---

## The R1T's wheelbase

The R1T and R1S share one openpilot vehicle profile. Until now that profile used the R1S's **3.08 m** wheelbase for both, so on an R1T openpilot worked out its steering as if it were driving the shorter car. The R1T's wheelbase is **3.45 m**.

AdventurePilot now reads the car's VIN when it starts up:

| The VIN reads as | Wheelbase used for steering |
| :---- | :---- |
| R1T | 3.45 m |
| R1S | 3.08 m, as before |
| Anything else, or the VIN could not be read | 3.08 m, as before |

**On an R1S nothing changes.**

**On an R1T** the steering is now worked out for the truck you are actually driving. Most of the time you will not feel a difference, because openpilot's self-learning corrects for most of the old mismatch. The learning settles faster and more accurately when it starts from the right figure.

**How we know 3.45 m is right:** openpilot learns a "steer ratio" for each car as it drives. The R1T and R1S share the same steering rack, so with the right wheelbase each should learn about the same ratio. They do: two R1Ts at 3.45 m learned 15.5 and 15.7, and an R1S at 3.08 m learned 15.8.

**After updating an R1T** the learned steer ratio may drift a little over the next few drives as it adjusts to the new figure. That is expected, not a fault.

**What does not change:** the comma's built-in safety limits on steering stay exactly as they were, worked out for the 3.08 m car on every Rivian. On an R1T that is the cautious side. The most sideways force openpilot can ask for comes out a little lower, at about 3.1 m/s2 instead of 3.6, which is still far more than normal driving needs. openpilot's own limits match the safety limits, so nothing gets blocked.

---

## Rear-Axle Off-Tracking Compensation

In a turn, the rear wheels follow a tighter line than the front wheels. On a long vehicle in a tight turn the difference is large enough for a rear wheel to clip a curb the front wheel cleared. That is why truck drivers swing wide.

With this setting on, openpilot widens its turns a little to make up for it. How much is worked out from the wheelbase and how tight the turn is:

| Turn radius | Rear wheels cut inside by (R1T) | (R1S) |
| :---- | :---- | :---- |
| 10 m (a tight town corner) | 0.61 m | 0.49 m |
| 20 m | 0.30 m | 0.24 m |
| 50 m | 0.12 m | 0.09 m |

At the default gain of 50% openpilot makes up half of that amount. Gentle highway curves are hardly affected, and the effect grows in tight turns. An R1T's turns are widened more than an R1S's, because the R1T is longer.

### Setting it up

- **On the device:** the **Vehicle** screen, under the Rivian items.
- **On Sunnylink:** the **Vehicle** page, under **Rivian Settings**.

Both places change the same setting:

- **Rear-Axle Off-Tracking Compensation** turns it on or off. Off by default.
- **Compensation Gain** sets how much of the cut-in to make up: 0% to 100% in 5% steps, default 50%. 100% is the full geometric amount. On the device it only appears while the switch is on.

Both can only be changed while the car is OffRoad. Changes take effect after changing from OffRoad to OnRoad.

### How it has done so far

- On Gerrylum's R1T, at 80%, the widening matched the calculation exactly, with no faults or warnings.
- nimijneb, on a second R1T, drove much the same town loop with it on and off. With it off, they had to correct by hand to keep from curbing the truck. The logs point the same way: with it off, the truck ran about 0.4-0.5 m closer to the inside edge on right turns. That was over only a handful of turns, so treat it as encouraging rather than proven.

### Things to note

- **It is an estimate, not curb sensing.** It knows the geometry of the truck, not where the curb is. Keep watching the inside of tight turns yourself.
- **It only acts while openpilot is steering.** When you steer, nothing changes.
- **It does not add steering force.** It only reshapes the turn openpilot asks for. In very tight, slow turns the car may already be at its steering limit, especially on torque steering without the angle harness, and this setting cannot change that.
- **Trying a different gain:** if 50% still runs close to curbs, try 70-80%. If turns feel too wide, lower it.

---

## Steering Tune (R1T) has been removed

The device's Vehicle screen used to show a **Steering Tune (R1T)** choice of Auto or Tame. The tuning it chose between was taken out of the code in June 2026, so for months it has done nothing except restart openpilot when tapped. It is now gone from the menu. Steering is the same as it was.

---

## Safety

No safety-layer code was touched. The comma's steering limits are the same as before on every Rivian. The wheelbase change only corrects openpilot's own steering arithmetic for the R1T. Off-tracking is off unless you turn it on, and it only makes turns slightly wider.

---

## Credits

- **Gerrylum** worked out the R1T wheelbase from the VIN and wrote the off-tracking compensation on his `rx-wb-stg` branch, and tested both on his R1T. AdventurePilot uses his work, adapted so that an R1S and an unrecognised VIN keep the 3.08 m figure and the safety limits are unchanged.
- Thanks to **nimijneb** for testing on a second R1T: the highway drive and the on/off comparison.
