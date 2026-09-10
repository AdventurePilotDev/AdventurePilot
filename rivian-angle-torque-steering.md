# Rivian Angle / Torque Steering - Driver Guide

**Applies to:** Rivian R1T / R1S **with the angle-steering harness fitted**. On other vehicles, and on Rivians without the harness, none of these controls appear and the car steers with torque steering exactly as before.

**Where it has landed:**

| Line | Source branch | Commits | First shipped in |
| :---- | :---- | :---- | :---- |
| `stg-a` (angle harness) | `stg-a-src` | `17d87aea3`, `77a21378a`, `a29373622`, `aeca4d34a` | `v2026.08.11-59` |

Also on `dev-a`, the angle-harness development trunk. Angle steering needs the angle harness, so it is not on `stg`, `dev`, `rel` or `ap-dev`.

---

## In one line

Adds a master switch, an on-screen tap toggle, and a low-speed threshold, so you can choose between angle steering and torque steering - and adds back the "take control" warning in angle mode.

---

## Background: two ways to steer

When lateral control (MADS or ACC) is active, an angle-harness Rivian can steer in one of two ways:

- **Angle steering** - the default. The car steers itself to a commanded wheel angle. You can drive hands-off (within the usual attention and hands-on-wheel requirements for your region).  
- **Torque steering** - a cooperative mode. The car applies steering torque and allows your hands to be sharing the wheel. This is the same mode used on Rivians without the angle harness.

Normally the system moves between the two on its own: put your hands on the wheel and it cooperates with torque; take them off (once the wheel is settled) and it hands back to angle steering.

The controls below let **you** decide to run in torque steering full-time, and to switch back, without disengaging.

---

## 1. Settings switch: "Rivian: Enable angle steering"

Location: **Settings -> Steering** (main steering menu). Default: **ON**.

- Only appears on angle-harness Rivians.  
- Can only be changed **off-road** (with the vehicle parked / off). Changes take effect after changing from OffRoad to OnRoad. The setting is remembered between drives.

| Setting | Behavior |
| :---- | :---- |
| **ON** (default) | Angle steering is available. The on-screen tap toggle (below) is active. |
| **OFF** | The car uses **torque steering only** for the whole drive. The on-screen tap toggle is disabled - tapping the wheel icon does nothing. |

Use OFF if you simply prefer torque steering all the time and do not want the on-road toggle.

The setting is also available remotely through **Sunnylink** (Steering page), subject to the same off-road-only rule.

---

## 2. Settings option: "Rivian: Always-Torque Below Speed"

Location: **Settings -> Steering**, directly under "Rivian: Enable angle steering". Default: **0 (off)**.

- Only appears when "Rivian: Enable angle steering" is ON (it is meaningless otherwise).  
- Sets a speed below which the car **always uses torque steering**, even though angle steering is enabled. Angle-mode steering can be poor at very low speed, so this lets you keep the smoother torque behavior for slow driving and let angle steering take over once you speed up.  
- The value is in **mph** (shown in **km/h** when your device is set to metric), adjustable from **0 to 40** mph. **0 means off** - no low-speed forcing, exactly as before.  
- The value is **true vehicle speed**. Your dash speedometer may read about **1 mph higher** than this (it is deliberately optimistic), so the threshold matches the speedo exactly when you have "Always Display True Speed" turned on, and reads about 1 mph low against the dash otherwise.  
- Also available remotely through **Sunnylink** (Steering page).

How it behaves on the road:

- **Below the set speed:** the car switches to torque **as soon as** you drop below it, and the wheel icon shows the torque tint (blue), the same as when you force torque yourself. This happens quietly - there are **no** "hold wheel to..." confirmation messages, because nothing is being switched by you.  
- **Above the set speed:** angle steering resumes on its own in the normal hands-off / settled way, and the wheel tint returns to the angle state - but only once you are about **3 mph (about 5 km/h) above** the set speed. This single-sided margin is deliberate: torque engages the instant you drop below the threshold, and hands back to angle only after a clear rise past it, so ordinary cruise ripple never flip-flops the tint. A practical consequence - if you set the threshold at (or very near) a speed you actually hold on cruise, the first dip below it latches torque and you will **stay in torque** at that speed rather than toggling; that is intended.  
- **Wheel tap is inert below the set speed.** Because steering is already torque-only there, tapping the wheel icon does nothing until you are back above the set speed, where the normal hold-to-confirm toggle (section 3) works again.

This is a passive preference - it never engages or disengages driver assistance, it only chooses how the car steers at low speed.

### How the switch itself is made ("make-before-break")

Worth knowing, because an earlier version of this feature could produce a noticeable jolt.

Handing over from angle to torque is not instant. Steering torque has to ramp up from zero, and that takes about half a second. The first version released the steering servo the moment you crossed the threshold, which left the wheel briefly unsupported: on a low-speed bend it would unwind, then the arriving torque would over-correct. That was a real reported jerk.

The car now **keeps the steering servo holding while torque builds up underneath it**, and only lets go once the torque is strong enough to carry the load. If the torque ramp stalls - most likely because you are turning the wheel against it - it **aborts and stays on angle steering** rather than letting go of an under-supported wheel.

You should feel a continuous handover with no kink.

---

## 3. On-screen tap toggle (comma 3 and 3X)

While MADS is **actively steering**, tap the on-screen **steering-wheel icon** to switch between angle and torque steering. The wheel icon tint reflects the current mode (it turns blue for torque steering).

The tap only acts as an angle/torque toggle when **all** of these are true:

- the "Rivian: Enable angle steering" switch is ON,  
- **Experimental Mode is OFF** (with Experimental Mode on, the same icon still toggles Experimental Mode as usual), and  
- lateral control is currently active.

Each switch asks you to **hold the wheel** to confirm, so the change only happens when you are in control. The choice resets to the default (angle steering) at the start of each drive.

### Turning angle steering OFF (switch to torque)

- If you are **already holding the wheel** (the car is momentarily in torque steering), it switches immediately and shows:  
    
  > **Angle steering deactivated.** Using only torque steering.  
    
- If the car is steering hands-off on the angle channel, it shows:  
    
  > **Hold wheel to deactivate angle steering**  
    
  Take hold of the wheel. As soon as your hands are detected the car switches to torque steering and shows the "Angle steering deactivated" message above.  
    
- If you do **not** hold the wheel within **5 seconds**, the switch is cancelled with a chime:  
    
  > **Angle steering not deactivated - timeout**  
    
- Tapping the icon **again** while it is waiting cancels the switch:  
    
  > **Angle steering deactivation canceled**

### Turning angle steering back ON (switch to angle)

- Tap the icon. It shows:  
    
  > **Hold wheel to reactivate angle steering**  
    
  Take hold of the wheel. Once your hands are detected, angle steering is re-armed and it shows:  
    
  > **Angle steering reactivated**  
    
  Angle steering then resumes hands-off in the normal way once your hands come off the wheel and the wheel is settled.  
    
- If you do **not** hold the wheel within **5 seconds**, it is cancelled with a chime:  
    
  > **Angle steering not activated - timeout**  
    
- Tapping the icon **again** while it is waiting cancels the switch:  
    
  > **Angle steering reactivation canceled**

---

## 4. comma 4

The **settings switch** ("Rivian: Enable angle steering") and the **"Always-Torque Below Speed"** option (section 2) both work on comma 4 exactly as described above.

The **on-screen wheel-tap toggle also works on comma 4**: tap the steering-wheel icon (bottom-left of the on-road screen) to run the same hold-to-confirm deactivate/reactivate flow described in section 3, with the same messages. The wheel icon tints blue in torque steering, same as on comma 3/3X.

**One difference from comma 3/3X: on comma 4 the tap works regardless of Experimental Mode.** On comma 4 the wheel icon is display-only for Experimental (there is no shared Experimental button on the small screen - Experimental Mode is changed from the settings menu instead), so there is no icon to share and no need to gate the toggle. The tap acts as the angle/torque toggle whenever the master switch is ON and MADS is actively steering, whether Experimental Mode is on or off. (On comma 3/3X the same icon still doubles as the Experimental toggle, so there the wheel tap only switches angle/torque when Experimental Mode is off.)

---

## 5. "Take Control" warning in angle mode

If the car is asking for more steering angle than it is allowed to deliver - a curve tighter than the steering envelope permits - you now get the standard **take control** prompt while in angle mode. Previously that warning was silent in angle mode: a curve past the limit gave you no prompt at all.

---

## Message quick reference

| Message | Sound | Meaning |
| :---- | :---- | :---- |
| Angle steering deactivated. / Using only torque steering. | - | Now in torque steering only. |
| Hold wheel to deactivate angle steering | - | Hold the wheel to confirm the switch to torque. |
| Angle steering not deactivated - timeout | chime | You did not hold the wheel in time; nothing changed. |
| Angle steering deactivation canceled | - | You tapped again; nothing changed. |
| Hold wheel to reactivate angle steering | - | Hold the wheel to confirm the switch back to angle. |
| Angle steering reactivated | - | Angle steering is re-armed; it resumes hands-off once your hands come off. |
| Angle steering not activated - timeout | chime | You did not hold the wheel in time; still torque steering. |
| Angle steering reactivation canceled | - | You tapped again; still torque steering. |

---

## Safety notes

- These controls **do not engage or disengage** driver assistance - they only choose how it steers. Lateral control stays active throughout the switch.  
- The "hold the wheel" steps are deliberate confirmations. Always keep your attention on the road and be ready to steer, exactly as you would at any other time.  
- Switching to torque steering means you are expected to share the wheel. Switching back to angle steering re-arms hands-off steering, which takes over again once your hands come off and the wheel is settled.  
- If you are ever unsure which mode you are in, the on-screen wheel icon tint shows it (blue for torque steering), and you can always take the wheel - torque steering cooperates with your input immediately.  
- **No safety-layer code was relaxed.** The panda safety module still validates the angle and torque channels independently, and the make-before-break handover was designed specifically to stay inside those limits rather than around them.

---

## A bug that was fixed on the way here

Worth recording, because it was serious and because the fix is part of what you are getting.

An earlier attempt to restore the angle-mode "take control" warning did it by having the car report the steering torque it *wanted* rather than the torque it was actually applying. That looked harmless. It was not: a part of the steering controller uses that number to decide when to stop accumulating correction, and the false report let it keep accumulating against an output that was being thrown away. The stored-up correction then dumped out the moment the car switched to torque steering, applying near-full-scale steering effort against the driver, for as long as 12 seconds, with only a disengage to clear it.

That is fixed, and the warning has been rebuilt properly from the angle channel's own limits. The faulty version **never existed on this line at any point** - the whole sequence was combined into a single change specifically so that it could not.

---

## Verification done before release

| Check | Result |
| :---- | :---- |
| Rivian car tests | 53 passed |
| Panda safety tests (Rivian) | 157 passed, 94 skipped |
| Angle toggle + harness config tests | 52 passed |
| On-device tests, real hardware | 19 passed on a comma 3X |
| Sunnylink settings file reproduces from source | byte-for-byte |
| Linting of all changed files | clean |

**Mutation tested**, because a test that cannot fail proves nothing. Disabling the saturation detector failed 3 tests; forcing it always-on failed 1. Most importantly, deliberately reintroducing the steering-torque reporting bug described above **was caught by a test** - so it cannot come back unnoticed.

**Road tested** (on the development branch, before this port):

- 14 angle-to-torque handovers across 6 segments of town driving: every ramp clean, zero steering faults, zero unexpected disengagements. Frame-by-frame review of the two highest-load switches shows the wheel tracking the curve continuously through the handover.  
- A second test specifically on bends beyond 90 degrees: both handovers clean, with a comfortable margin.

---

## Limitations and things to know

- **The wheel tap is inert below the always-torque threshold.** This is intended, but can look like the tap has stopped working. Check your speed against the threshold.  
- **New settings need a full reboot to register.** Handled for you in published builds; only matters if you are building from source.  
- **Setting changes take effect after changing from OffRoad to OnRoad**, not mid-drive.

