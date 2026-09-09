# Minimum Speed to Engage MADS

**Applies to:** Rivian R1T / R1S. Rivian-only - the setting is hidden and inert on every other brand.

**Where it has landed:**

| Line | Source branch | Commits | First shipped in |
| :---- | :---- | :---- | :---- |
| `stg` (general) | `stg-src` | `a9f475bf3` + `7a5b0e396` | `v2026.08.07-54` |
| `stg-a` (angle harness) | `stg-a-src` | `266f31c90` + `22fc7e308` | `v2026.08.07-55` |

Both lines carry byte-identical code. The two commits are the feature and its regression tests.

Those are the builds the setting **first** appeared in, not the ones to flash. It is included in every build published since, so any current `stg` or `stg-a` prebuilt already has it. If you are checking whether your device is covered, the question is whether your build is that version **or newer**.

**Not yet on:** `rel`, `rel-src`, `dev` and `ap-dev`.

---

## In one line

Adds a Rivian-only setting that stops the MADS stalk from engaging steering below a chosen speed, defaulting to 5 mph.

---

## Read this part first

**The setting is on by default, at 5 mph.** After updating, engaging MADS with the stalk while stopped or crawling **will not work**, and there is no alert explaining why. Nothing has broken.

This is deliberate: an alert every time you pressed the stalk in a car park would be worse than silence. But it does mean the first time you meet it, it looks like a fault. **If you do not want the gate, set it to 0 and it is gone**.

**Where to find it:** Settings -> Steering -> MADS -> *Minimum Speed to Engage MADS*. Also available remotely through Sunnylink, on the Steering page.

---

## What it does

Below the threshold, engaging MADS **with the stalk alone** is refused. Above it, everything behaves exactly as before.

**Engaging cruise still brings steering with it, at any speed.** That exemption is the important half of the design. If you engage ACC, lateral comes along regardless of how slowly you are moving, so the normal way you set off is untouched. The gate only ever applies to standalone MADS engagement.

Range is 0 to 20, in steps of 1. **0 disables the gate entirely** and restores the previous behaviour exactly.

The value is stored in mph and displayed in whichever units you have selected - it shows km/h if your device is set to metric, and converts for you.

---

## Why you would want it

Standalone MADS engages lateral control on its own, without cruise. At walking pace that is rarely what you meant: manoeuvring in a car park, creeping in traffic, or edging out of a driveway are all situations where a stalk press is more likely to be a mistake, or a habit, than a request for the car to start steering.

Setting a floor means the stalk only does something once you are actually moving at a speed where lane centering makes sense.

---

## What you will notice

- Below the threshold, a stalk press to engage MADS does nothing, silently.  
- Above the threshold, nothing changes at all.  
- Engaging ACC brings steering at any speed, exactly as before.  
- Changing the setting **takes effect after changing from OffRoad to OnRoad** - the value is read once when the drive starts, not continuously. Change it while parked.

---

## On safety

**No safety-layer code was touched.** Nothing in the panda safety module changed.

The direction of the change is worth stating plainly: this setting can only ever **refuse** engagement. There is no speed, and no combination of settings, at which it grants steering where the previous build would not have. Turning it up makes the system engage less readily, never more.

It is also implemented entirely in Rivian-specific code. The shared MADS state machine that every other brand relies on is untouched.

---

## Where this came from

Ported from the development branch, where it has been in daily use since early July. Two commits were combined into one on the way across.

That squash was not just tidiness. The original pair was a feature commit plus a follow-up bug fix: the first version wrote the setting's value as text where the system expected a number, which would have crashed the settings screen **on non-Rivian cars**. Folding the fix in means that defect never exists in this branch's history at all.

---

## Verification done before release

| Check | Result |
| :---- | :---- |
| Regression tests (new) | 10 passed on both lines |
| Sunnylink settings schema test | passed - confirms the Rivian-only gate |
| Code equivalence between the two lines | byte-identical |
| Compiled Sunnylink settings file | reproduces from source byte-for-byte |

The regression tests are new. They cover the gate blocking below the threshold and allowing above it, both engage exemptions, a threshold of 0 disabling the gate, and the feature staying completely inert on non-Rivian cars.

**Mutation tested on a comma 3X**, because a test that cannot fail proves nothing. Removing the cruise exemption made 2 tests fail; reversing the speed comparison made 5 fail. Both were then restored and everything passed again.

One mutation was **not** caught, and it is worth recording honestly rather than quietly: deleting an internal `> 0` guard changed no test result. That turned out to be correct - vehicle speed is never negative, so the guard cannot ever change the outcome. It is defensive documentation rather than working logic, and the test file now says so, in case someone later mistakes its presence for something the tests are protecting.

Tests were run on a comma 3X rather than a laptop, since they need compiled components that are not available on macOS.

---

## Limitations and things to know

- **A new setting key needs a full reboot to register.** This is handled for you in the published builds. It only matters if you are building from source, where the setting will not appear until you reboot.  
- **Silent refusal is intentional but undiscoverable.** There is no message telling you why the stalk did nothing. If you have not read this page, the behaviour is genuinely confusing. That is the main cost of the design.  
- **The threshold is read once per drive.** Changing it mid-drive has no effect until the next OffRoad to OnRoad transition.
