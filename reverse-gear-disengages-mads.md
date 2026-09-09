# Reverse Gear Now Fully Disengages MADS

**Applies to:** Rivian R1T / R1S. Rivian-specific - no other brand is affected.

**Where it has landed:**

| Line | Source branch | Commits | Prebuilt |
| :---- | :---- | :---- | :---- |
| `stg` (general) | `stg-src` | `5c44026e4` + `ef3eb88d6` | `v2026.08.07-52` - published |
| `stg-a` (angle harness) | `stg-a-src` | `5e7b49a5a` + `5928cbae7` | `v2026.08.07-53` - published |

Both lines carry byte-identical code. The two commits are the fix itself and a regression test.

**Not yet fixed:** `rel`, `rel-src`, `dev` and `ap-dev` all still behave the old way. Verified, not assumed - each of those branches has the park handling but not the reverse handling.

---

## In one line

Shifting into Reverse now switches MADS off properly, instead of leaving it parked in a state that quietly switches itself back on the moment you select Drive again.

---

## The behaviour you might have noticed

Reverse into a parking space, then shift back to Drive to straighten up. As soon as Drive engages, **MADS is active again in Mode B** without you asking for it.

Nothing dangerous happened - the system will not steer while you are in Reverse, and speeds during a parking shuffle are low. But it is not what the controls told you. You switched MADS off by shifting out of Drive, and it came back on its own.

Park already behaved correctly. Reverse did not. Now they match.

---

## What was actually going wrong

MADS has two "off" states, and reverse was landing in the wrong one:

- **Disabled** is properly off. It stays off until you engage it.  
- **Paused** is a temporary hold, designed to resume by itself once the reason for pausing clears.

Shifting into Reverse was putting MADS into **paused**, not **disabled**. Selecting Drive removed the reason for the pause, so MADS did exactly what paused is supposed to do: it resumed.

The reason is a small collision inside the software. When reverse is detected, the MADS module raises its own internal "quietly pause" signal. The Rivian code was also asking for a full disengage at the same moment. Both arrived on the same frame, and the pause signal won.

---

## What changed

The fix asks for the disengage on **two consecutive frames** instead of one.

On the first frame it loses the race, exactly as before, and MADS goes to paused. On the very next frame - about a hundredth of a second later - the pause signal is no longer being raised, because MADS is already paused and there is nothing left to pause. The disengage request is now unopposed, and MADS goes to disabled and stays there.

This is not a new invention. It is the same two-frame pattern the **park** path has been using successfully all along. Reverse now uses it too.

---

## Where this came from

This has a slightly messy history that is worth being straight about, because the commit log looks alarming and the alarm is unfounded.

| What happened | Outcome |
| :---- | :---- |
| Fix originally written and road-tested | worked |
| Reverted, blamed for a drive-logging failure | not actually the cause |
| Re-landed unchanged once the real cause was found | the version shipping now |

The logging failure was an unrelated video library version mismatch, fixed separately. The revert was a misdiagnosis. The code shipping here is byte-for-byte the same code that was road-tested and confirmed working on a test vehicle before any of that happened.

---

## What you will notice

**After reversing, MADS will not come back by itself.** You re-engage it the same way you always engage it. This is the change, and it is deliberate.

If you are used to the old behaviour, a parking shuffle will feel different: previously MADS reappeared on its own each time you selected Drive, and now it does not. Nothing else about engagement, settings, or driving changes.

---

## On safety

**No safety-layer code was touched.** Nothing in the panda safety module changed. This is entirely a question of which MADS state a gear change lands in.

The direction of the change is also worth stating plainly: it moves MADS from *paused* to *disabled*, which is **less** engaged, not more. There is no situation in which this build grants steering control where the previous build would not have.

---

## Verification done before release

| Check | Result |
| :---- | :---- |
| Regression tests (new) | 6 passed on both lines |
| Prebuilt CI build and publish | both jobs successful on both lines |
| Code equivalence between the two lines | byte-identical |

The regression tests are new, and they are the first automated coverage this behaviour has ever had on any branch. Until now the only evidence it worked was a road test - which is part of why an unrelated failure was able to get it reverted without much argument.

They cover six cases: reverse entry disengaging on exactly the two expected frames, returning to Drive not re-triggering, each shift in a repeated parking shuffle disengaging on its own, a single-frame reverse blip still disengaging, Drive alone never disengaging, and the pre-existing park path still working.

**Mutation tested twice**, because a test that cannot fail proves nothing:

- Remove the fix entirely: 4 of the 6 tests fail, and the two that should be unaffected (Drive-only and Park) still pass.  
- Remove **only** the second frame - the subtle half that the whole fix depends on: 3 tests fail.

The second check is the important one. It confirms the tests are pinning the precise two-frame timing rather than merely noticing that something, somewhere, switched off.

Tests were run on a comma 3X, since they need compiled components that are not available on a development laptop.

---

## Known limitation

The two-frame approach is a workaround, not a root-cause fix. The underlying issue is that an explicit "disengage" request does not automatically outrank an implicit "pause" request inside the shared MADS state machine. Correcting that properly would change behaviour for every supported brand, not just Rivian, so it is being kept as a separate decision rather than bundled in here.

The practical consequence for you: none. The behaviour is correct. It is simply achieved by working around the collision instead of removing it.  
