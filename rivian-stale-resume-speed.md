# Rivian: stale resume speed after resuming

**Applies to:** Rivian R1T / R1S, and only if you use the Resume Set Speed feature. Rivian-only.

**Where it has landed:**

| Line | Source branch | Commit | First shipped in |
| :---- | :---- | :---- | :---- |
| `stg` (general) | `stg-src` | `30bab5814` | `v2026.08.08-56` |
| `stg-a` (angle harness) | `stg-a-src` | `9269f7133` | `v2026.08.08-57` |

Both lines carry identical code.

Those are the builds the fix **first** appeared in, not the ones to flash. It is included in every build published since, so any current `stg` or `stg-a` prebuilt already has it. If you are checking whether your device is covered, the question is whether your build is that version **or newer**.

**Not yet on:** `rel`, `rel-src`, `dev` and `ap-dev`.

---

## In one line

Fixes the resume feature re-applying an old set speed a second time, overwriting a speed you had chosen since.

---

## What the feature does

Resume Set Speed remembers the cruise speed you were using when ACC last switched off, so you can get it back without dialling it in again. Hold the stalk down for about a second with ACC on and the remembered speed is restored.

It is off unless you have turned it on.

---

## What was going wrong

The remembered speed was never forgotten after it had been used.

So the sequence went like this:

1. ACC switches off at, say, 70 mph. That speed is remembered.  
2. You re-engage ACC and hold the stalk down. The speed comes back as 70 mph. Correct.  
3. You then set a different speed - say you drop to 50 mph for a slower road.  
4. Later in **the same ACC session**, you hold the stalk down again.  
5. The old 70 mph comes back, overwriting your 50.

Step 5 is the bug. By that point the remembered speed is stale: it is from before the resume you already performed, and it is not what you last chose.

---

## What changed

The remembered speed is now cleared the moment it has been used. Only a fresh ACC deactivation stores a new one.

That gives the behaviour you would expect: **one deactivation, one resume.** The feature restores the speed from before ACC dropped out, and after that it has nothing left to restore until ACC drops out again.

---

## What you will notice

- Resuming works exactly as before the first time.  
- Holding the stalk down again later in the same drive no longer yanks the speed back to an old value.  
- If you did not have Resume Set Speed enabled, nothing changes for you at all.

---

## On safety

**No safety-layer code was touched.** Nothing in the panda safety module changed.

The change only affects which number the cruise set speed is restored to, and only inside a feature you have to switch on. It cannot cause the car to command a speed you never set - the opposite, since the bug it fixes was precisely the car reapplying a speed you had moved away from.
