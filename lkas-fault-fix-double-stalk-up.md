# LKAS Fault Fix: steering lost after a double stalk-up

**Applies to:** Rivian R1T / R1S. The shared half of the fix benefits every sunnypilot car.

**Where it has landed:**

| Line | Source branch | Commit | First shipped in |
| :---- | :---- | :---- | :---- |
| `stg-a` (angle harness) | `stg-a-src` | `3cb72baa1` | `v2026.08.06-49` |
| `stg` (general) | `stg-src` | `6b655f6b0` | `v2026.08.06-51` |

Those are the builds the fix **first** appeared in, not the ones to flash. It is included in every build published since, so any current `stg` or `stg-a` prebuilt already has it. If you are checking whether your device is covered, the question is whether your build is that version **or newer**.

The `stg` version is a port of the same fix, not a different one: the safety code is identical on both lines. Only the test needed adapting, because `stg` does not carry the angle-steering work that the `stg-a` test relies on.

**Carries the complete fix, both halves:** the `dev` and `dev-a` development trunks. As of 2026-09-11 these are the only lines that also have the **second half** described under "One half or both" below - `stg` and `stg-a` still carry the first half only. The shared parts are additionally proposed upstream as sunnypilot/opendbc draft PR #493, which is their proper long-term home.

**Not yet fixed:** `rel` and `rel-src` (deliberately left for now) and `ap-dev`. Verified directly rather than assumed, and re-checked on 2026-09-11 - none of those has any part of the fix.

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

And, on `dev` and `dev-a` only, a third:

- **Clear the stale counter when you ask to engage, too.** Resetting only on the way out covers a re-engage that arrives *after* the watchdog has fired. It does nothing for one that arrives *before* it, in the second or two while the counter is still climbing - and that ordering is the worse of the two. Asking to engage now clears the count as well, so every engagement starts with a full grace window however it is timed.

### On safety

All of these changes are in the panda safety layer, so they deserve a direct answer: **none of them relaxes any safety constraint.**

- The stalk change makes the panda grant lateral control in **fewer** circumstances, not more.  
- The counter reset happens **inside the disengage path** - after control has already been dropped. It does not extend any window in which control stays allowed. If a genuine mismatch persists, the watchdog simply re-accumulates and exits again exactly as before. It is stale-state cleanup, not a tolerance window.  
- The third change, on `dev` and `dev-a`, clears the same counter when the driver asks to engage. Being straightforward about it: this is the one that does affect timing, because a fresh request restarts the watchdog's three-second count. It never grants control - only the normal engage path does that - and it cannot hold control through a brake, because the brake and cruise-off exits are checked immediately afterwards in the same pass and still fire. If the software genuinely is not engaged, the count simply builds again from zero and the watchdog withdraws steering as it always did. It mirrors a reset the upstream code already performs on the equivalent counter for cruise.

---

## What you will notice

Mostly, you will notice **nothing** - which is the point. Specifically:

- Stalk-up to engage MADS behaves the same as before in normal use.  
- Engaging twice in quick succession, or engaging shortly after cancelling cruise, no longer risks faulting the steering.  
- No settings changed, nothing new to configure.

---

## Scope and limitations

Two things worth knowing:

- **The counter fixes are in shared code**, not Rivian-specific. They live in the common sunnypilot MADS module, so they apply to every supported brand. That is appropriate - it is the same bug for all of them - but they are not Rivian-only changes. Only the stalk gate is Rivian-specific.  
- **One half or both, depending on your line.** The fault chain proven in the log is closed on every line listed at the top. But the stale counter can strand steering in two different ways, depending on whether your re-engage lands *after* the watchdog fires or *before* it:
  - `stg` and `stg-a` close the *after* case. This is the one captured frame-by-frame in the log, and it is the one that latches the permanent EPAS fault.
  - `dev` and `dev-a` close **both**. The *before* case was found later, on a separate drive, and it fails differently: instead of a steering fault it leaves the panda with steering switched off while the software still believes it is on, which ends about two seconds later in a red **"Controls Mismatch: Lateral"**. If you have seen that alert after working the stalk, this is the likely cause.
  
  Full coverage arrives on the staging lines when the second half is promoted to them.

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

### The second half, on `dev` and `dev-a` (2026-09-11)

| Check | `dev` | `dev-a` |
| :---- | :---- | :---- |
| Rivian panda safety tests | **135 passed**, 91 skipped, 0 failed (up from 131) | **161 passed**, 96 skipped, 0 failed (up from 157) |

Two further regression tests come with it, again running in both the stock and longitudinal-control configurations, which is where the increase of four comes from.

- **Mutation tested the same way.** With the safety change reverted but the new tests left in place, exactly the two new tests failed, on both configurations, with the message *"Fresh engage revoked by a stale mismatch counter; counter not reset on request"*. They passed again once restored. The older pair kept passing throughout, which is the expected result: those cover the first half, which these lines already had.  
- The safety logic now matches the line this fix was originally developed and road-proven on, line for line. No reworked variant of it is being carried anywhere.  
- These lines also picked up the routine nightly sync of sunnypilot master on the same day. The sync touches the same safety file, but only to add a static-analysis comment well away from any of this, so the two sit side by side; that comment is the sole difference between the file here and on the line it came from. The test results above are from after the sync was merged.

**A reflash is needed.** These changes are in the panda firmware, not the phone-side software, so pulling the update alone is not enough for them to take effect.
