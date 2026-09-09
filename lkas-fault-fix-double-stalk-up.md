# LKAS Fault Fix: steering lost after a double stalk-up

**Applies to:** Rivian R1T / R1S. The shared half of the fix benefits every sunnypilot car.

**Where it has landed:**

| Line | Source branch | Commit | First shipped in |
| :---- | :---- | :---- | :---- |
| `stg-a` (angle harness) | `stg-a-src` | `3cb72baa1` | `v2026.08.06-49` |
| `stg` (general) | `stg-src` | `6b655f6b0` | `v2026.08.06-51` |

Those are the builds the fix **first** appeared in, not the ones to flash. It is included in every build published since, so any current `stg` or `stg-a` prebuilt already has it. If you are checking whether your device is covered, the question is whether your build is that version **or newer**.

The `stg` version is a port of the same fix, not a different one: the safety code is identical on both lines. Only the test needed adapting, because `stg` does not carry the angle-steering work that the `stg-a` test relies on.

**Also carries the fix:** the `dev` and `dev-a` development trunks - the same safety code, verified byte-identical to the staging lines. The shared half is additionally proposed upstream as sunnypilot/opendbc draft PR #493, which is its proper long-term home.

**Not yet fixed:** `rel` and `rel-src` (deliberately left for now) and `ap-dev`. Verified directly rather than assumed - none of those has either half of the fix.

---

## In one line

Fixes a real, reproducible fault where engaging MADS twice in quick succession could permanently kill steering for the rest of the drive, with a "TAKE CONTROL IMMEDIATELY" alert.

---

## The fault you might have hit

Symptoms, as reported by a tester and then confirmed frame-by-frame in the logs:

- Driving at speed (the captured case was 55 mph)  
- Stalk up once to cancel cruise, then stalk up again shortly after to engage MADS Mode B  
- Instead of engaging, the car throws a **steering fault**: "TAKE CONTROL IMMEDIATELY"  
- **Steering assist is gone for the remainder of the drive.** Only stopping and restarting clears it.

If you have experienced an unexplained permanent LKAS fault after fiddling with the stalk, this is very likely what you hit.

---

## Where this came from

This was not found by code review or guesswork. A tester hit the fault on the road, and the cause was traced through the actual drive log (route `c17ea97dc5472650/00000006`, segment 3), one CAN frame at a time, until the full chain was proven end to end.

Worth being clear about one thing: **this bug is not new, and the steering-architecture change did not cause it.** The same faulty machinery had been present on `stg-a` and `dev` for a long time before this. The new build simply produced the first clean, readable log of it happening, which is what made the diagnosis possible.

---

## What was actually going wrong

The car has two pieces that must agree on whether MADS is engaged: the software, and the panda safety module. The fault was a disagreement between them that then escalated:

1. **They disagree.** On the first stalk-up, the software read it as "cancel cruise" and fully disengaged. The panda read the same signal as "MADS toggle" and turned lateral control **on**. Now the two disagree about the state of the car.  
2. **A watchdog fires.** Because the panda thought it was engaged while the software did not, a mismatch counter climbed and, three seconds later, forced an exit. Crucially, that counter was left **stuck at its maximum** instead of being cleared.  
3. **The next engage gets shot down.** On the second stalk-up, both sides engaged correctly - but the very next watchdog tick, about a tenth of a second later, saw the stale maxed-out counter and instantly revoked steering again, while the software was still happily engaged.  
4. **The EPAS faults.** During that revoke, the steering messages the car was still sending got rejected by the safety layer. That left **gaps in the message counter** on the CAN bus. The Rivian EPAS treats counter gaps as corruption, raises an angle-control error, and latches a permanent steering fault. Game over until restart.

The nasty part is step 2: a piece of stale state surviving a disengage and then sabotaging the *next* engagement.

---

## What changed

Two small, targeted fixes:

- **Clear the stale counter on disengage.** When MADS exits, the mismatch counter is now reset, so a saturated count can never carry over and kill the next engagement on its first tick.  
- **Stop the two sides disagreeing in the first place.** The panda now only treats a stalk-up as a MADS toggle when stock cruise is **not** already engaged - matching how the software interprets the same input. While cruise is engaged, lateral is already granted through the normal path, so **no capability is lost**; the two state machines simply can no longer drift apart at step 1.

### On safety

Both changes are in the panda safety layer, so they deserve a direct answer: **neither one relaxes any safety constraint.**

- The stalk change makes the panda grant lateral control in **fewer** circumstances, not more.  
- The counter reset happens **inside the disengage path** - after control has already been dropped. It does not extend any window in which control stays allowed. If a genuine mismatch persists, the watchdog simply re-accumulates and exits again exactly as before. It is stale-state cleanup, not a tolerance window.

---

## What you will notice

Mostly, you will notice **nothing** - which is the point. Specifically:

- Stalk-up to engage MADS behaves the same as before in normal use.  
- Engaging twice in quick succession, or engaging shortly after cancelling cruise, no longer risks faulting the steering.  
- No settings changed, nothing new to configure.

---

## Scope and limitations

Two things worth knowing:

- **One of the two fixes is in shared code**, not Rivian-specific. The stale-counter reset lives in the common sunnypilot MADS module, so it applies to every supported brand. That is appropriate - it is the same bug for all of them - but it is not a Rivian-only change.  
- **This is one half of a two-part fix.** It closes the path where a *disengage* leaves the counter stale. There is a matching fix for the *request* side that is in neither build yet. The fault chain proven in the log is closed; full coverage arrives when the second half is promoted.

---

## Verification done before release

Each line was tested independently, on its own tree.

| Check | `stg-a` | `stg` |
| :---- | :---- | :---- |
| Rivian panda safety tests | **151 passed**, 91 skipped, 0 failed (up from 149) | **131 passed**, 89 skipped, 0 failed (up from 129) |
| Rivian car tests | 18 passed, 0 failed | not applicable to this line |

The two new regression tests cover the stalk gate, and run in both the stock and longitudinal-control configurations on each line.

- **Mutation tested on both lines:** the fix was deliberately reverted to confirm the new tests actually catch it. On each line exactly the two new tests failed with the fix removed - and nothing else did - then passed again once restored. The tests are genuinely exercising the compiled safety code, not passing vacuously.  
- Built and published by CI on the comma 3X runner: both lines successful. (The `stg` build took two attempts - the first compiled fine but its publish step was cancelled during a GitHub Actions outage on 2026-08-06. The re-run published cleanly from the same commit.)
