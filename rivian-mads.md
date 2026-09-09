# Rivian MADS

**Applies to:** Rivian R1T / R1S. Everything here is Rivian-specific - no other brand is affected. None of it requires the longitudinal upgrade harness.

MADS (Modular Assistive Driving System) runs lane centering independently of cruise control, engaged from the stalk. This guide covers how MADS behaves on a Rivian: the three brake-pedal modes, how to engage and disengage it, and two recent changes to how it starts and stops - a minimum speed before the stalk will engage it, and Reverse gear now switching it fully off.

---

## Engaging and disengaging

**To engage MADS:** tap the stalk up (not past the detent), from either ACC or fully disengaged. Further taps up toggle MADS off and on. (The toggle-on-repeat behaviour may not have been present in earlier Rivian MADS versions.)

**To disengage MADS:** tap or hold the brake pedal. This is the single, consistent disengage action, and it works the same way whichever brake mode you are in.

**Stalk full up** (past the detent), or selecting **Park** or **Reverse**, disengages ACC and MADS together. Do not get into the habit of using stalk full up to disengage: done while stopped or crawling, it can put the car into Reverse by accident. Use the brake.

---

## The three brake-pedal modes

Set under **Settings -> Steering -> Customize MADS -> Steering Mode on Brake Pedal**. Until this update Rivian was locked to **Disengage**; all three options now work. For safety, the design keeps a single disengage action (the brake) regardless of which mode is selected.

### Disengage (Rivian default)

Tapping or holding the brake disengages **both** ACC and MADS. Re-engage MADS with a stalk tap up.

This is the default on first install. If you had previously changed this setting yourself, a fresh install keeps your choice rather than overwriting it. Right after installing, the Settings screen or Sunnylink may still show "Remain Active" (the sunnypilot default, which is overridden for Rivian) even though the car is already using Disengage - the display catches up once you restart the device or the app. Nothing is wrong.

### Remain Active

Tapping or holding the brake disengages ACC if it is running but **leaves MADS active**. This is the main way to engage MADS in this mode. Disengage MADS with a stalk tap up.

### Pause

The same as Remain Active, except that **holding** the brake temporarily pauses MADS; releasing the brake re-engages it. Stalk actions are the same as Remain Active.

---

## Minimum speed to engage MADS

**Setting:** Settings -> Steering -> MADS -> *Minimum Speed to Engage MADS*. Also on the Sunnylink Steering page. **On by default, at 5 mph.**

Below the set speed, engaging MADS **with the stalk alone** is refused, silently - there is no alert. After updating, this can look like a fault when you press the stalk while stopped or crawling. It is not. If you do not want the gate, set it to **0** and it is gone.

**Engaging cruise still brings steering with it at any speed.** The gate only applies to standalone stalk engagement, so the normal way you set off is untouched.

- Range 0 to 20 mph, in steps of 1. Stored in mph; displayed in km/h if your device is set to metric, and converted for you.
- The value is read once when the drive starts. Change it while parked - a mid-drive change takes effect only after the next OffRoad to OnRoad transition.
- Building from source: a new setting key needs one full reboot to appear. Published builds handle this for you.

**Why you would want it:** at walking pace a stalk press is more often a slip or a habit than a real request for the car to start steering - manoeuvring in a car park, creeping in traffic, edging out of a driveway. A floor means the stalk only acts once you are moving fast enough for lane centering to make sense.

---

## Reverse gear now fully disengages MADS

Previously, shifting into **Reverse** put MADS into a *paused* state rather than *disabled*. Selecting Drive again cleared the pause, so MADS switched itself back on in Mode B without you asking - usually noticed during a parking shuffle. Nothing unsafe happened (MADS never steers in Reverse, and shuffle speeds are low), but it was not what the controls told you: you switched MADS off by shifting out of Drive, and it came back on its own. Park already behaved correctly; Reverse now matches it.

**What you will notice:** after reversing, MADS stays off until you re-engage it the normal way. Nothing else about engagement, settings, or driving changes.

Under the hood: reverse detection and the Rivian disengage request used to land on the same frame, and the "quietly pause" signal won the race. The fix repeats the disengage request on the next frame - about a hundredth of a second later - when the pause signal is no longer being raised, so MADS ends up *disabled* and stays there. This is the same two-frame pattern the Park path already used. It is a workaround for a collision in the shared MADS state machine, not a root-cause fix: an explicit disengage request does not automatically outrank an implicit pause request, and correcting that properly would change behaviour for every supported brand, so it is being kept as a separate decision. No practical consequence for you - the behaviour is correct.

---

## Where these have landed

| Change | `stg` (general) | `stg-a` (angle harness) |
| :---- | :---- | :---- |
| Reverse fully disengages MADS | `v2026.08.07-52` | `v2026.08.07-53` |
| Minimum speed to engage | `v2026.08.07-54` | `v2026.08.07-55` |

Both lines carry byte-identical code for both changes. Each pair of commits is the change itself plus its regression tests. These are the builds each change **first** shipped in; every build published since includes them, so any current `stg` / `stg-a` prebuilt is covered. The question for your device is whether its build is that version **or newer**.

Three-mode brake-pedal support is part of the same body of Rivian MADS work on these lines.

**Also on** the `dev` and `dev-a` development trunks, which carry the same code.

**Not yet on** `rel`, `rel-src` or `ap-dev` - these still behave the old way for both changes. Verified directly, not assumed.

---

## On safety

**No safety-layer code was touched by any of this.** Nothing in the panda safety module changed.

Every change here is in the "less engaged, never more" direction:

- The minimum-speed gate can only ever **refuse** engagement. There is no speed, and no combination of settings, at which it grants steering where the previous build would not have.
- The Reverse fix moves MADS from *paused* to *disabled* - further off, not closer to engaged.

All of it is implemented in Rivian-specific code. The shared MADS state machine that every other brand relies on is untouched.

---

## Verification done before release

| Check | Minimum speed | Reverse fix |
| :---- | :---- | :---- |
| New regression tests | 10 passed, both lines | 6 passed, both lines |
| Code equivalence between the two lines | byte-identical | byte-identical |
| Sunnylink settings schema / compiled file | passed; reproduces byte-for-byte | n/a |
| Prebuilt CI build and publish | - | both jobs successful, both lines |

Both were **mutation tested on a comma 3X**, because a test that cannot fail proves nothing:

- **Minimum speed:** removing the cruise exemption failed 2 tests; reversing the speed comparison failed 5. One mutation was not caught - deleting an internal `> 0` guard changed nothing - and that is correct, because vehicle speed is never negative, so the guard cannot change the outcome. The test file now records that it is defensive documentation, not load-bearing logic.
- **Reverse fix:** removing the fix entirely failed 4 of 6 tests (Drive-only and Park correctly unaffected); removing **only** the second frame failed 3, confirming the tests pin the exact two-frame timing rather than merely noticing that something switched off.

Tests run on a comma 3X rather than a laptop, since they need compiled components not available on macOS.

Both changes have revert-and-reland histories in the development branch that look alarming in the commit log but are not. The minimum-speed squash folded in a fix for a text-vs-number bug that would only ever have crashed the settings screen on **non-Rivian** cars, so that defect never exists in this branch's history. The Reverse fix was reverted on a misdiagnosis - an unrelated video-library version mismatch was the real cause of a drive-logging failure - and re-landed byte-for-byte once that was found. The code shipping here is the same code that was road-tested and confirmed working on a test vehicle.

---

## Limitations and things to know

- **Silent refusal is intentional but undiscoverable.** Nothing tells you why the stalk did nothing below the minimum speed. If you have not read this page, the behaviour is genuinely confusing - that is the main cost of the design.
- **The minimum-speed threshold is read once per drive.** Changing it mid-drive has no effect until the next OffRoad to OnRoad transition.
- **A new setting key needs a full reboot** if you build from source; published builds handle it.
- **The Reverse fix is a workaround**, not a root-cause fix, for the reason given above. It has no practical effect on how the car behaves for you.
