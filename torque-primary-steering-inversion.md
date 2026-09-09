# Torque-Primary Steering (the "inversion")

**Build:** `stg-a` prebuilt `v2026.08.06-48` **Source commit:** `670f76fe9` - "Rivian: xnor torque-primary inversion on the stg-a platform" **Applies to:** Rivian R1T / R1S **fitted with the xnor extreme angle harness**. This build gates on that harness: without it the car is dashcam only and openpilot will not control the vehicle at all. Torque-only support on a standard comma harness is being worked on separately and is not part of this build.

**Where it has landed:** the `stg-a-src` line (and its `stg-a` prebuilts from `v2026.08.06-48` onward) and the `dev-a` angle-harness development trunk. It needs the angle harness, so it is not on `stg`, `dev`, `rel` or `ap-dev`.

---

## In one line

The staging build now steers the way the rest of openpilot does - torque first, with the angle channel as a helper - instead of the other way round. You keep hands-off angle steering; what changed is which control loop is in charge underneath.

---

## Where this came from

This is a port of xnor's inversion work from the `rx-dev` line (upstream commit `c2c832419`), adapted onto the stg-a platform. It is not a new experiment written from scratch: it is an architecture that already existed and had been run elsewhere, brought over deliberately.

Before this change, stg-a used an angle-primary design with cooperative torque bolted on as a secondary path. That worked, but it meant carrying a pile of custom plumbing that had to be maintained against every upstream change. The inversion flips it so the standard openpilot torque controller is the primary loop, and all the Rivian-specific arbitration lives in one Rivian-owned file instead of being sprinkled through shared code.

A useful side effect: this build **removes** more shared openpilot code than it adds. The custom wrappers in the shared control path are gone.

---

## What actually changed

- The car now reports itself as a **torque-steered** car, and openpilot's normal `LatControlTorque` loop drives it.  
- The **angle command is derived** from the path openpilot wants to follow, rather than being the primary output. One Rivian module owns the decision of which channel is steering at any moment.  
- Handoff between the two channels is now explicit and rule-based:  
  - You put a hand on the wheel with intent -> it switches to cooperative torque **immediately**.  
  - It hands back to angle steering only once the wheel has **settled** - within about 15 degrees of where the system wants it, and not being turned faster than about 40 deg/s.  
  - If the car's EPAS drops out of angle mode unexpectedly, it falls back to torque within about 0.15 s to keep the steering alive and let the EPAS re-arm.  
- The **panda safety code was not touched.** The layer that actually constrains what the car can be commanded to do is byte-for-byte the same road-proven code that was already on stg-a. This change is entirely above that line.

---

## What you will notice while driving

**A coloured steering wheel icon.** The wheel on screen is tinted to show which channel is actually steering:

| Colour | Meaning |
| :---- | :---- |
| **Green** | Angle steering - the EPAS is holding the line, no torque is being commanded |
| **Blue** | Torque steering - the normal openpilot torque loop is doing the work |
| No tint | Lateral not active |

The indicator waits for about 10 frames of genuinely zero torque before calling it "angle", so it will not flicker back and forth every time torque passes through zero mid-corner.

**Hands-on feels immediate.** Putting a hand on the wheel moves you into cooperative torque straight away rather than fighting a servo. Letting go does *not* snap straight back - it waits for the wheel to settle first, which is deliberate and prevents a lurch on release.

**It holds the wheel at a standstill** in Drive, which is expected behaviour on this hardware.

**It will not steer in Reverse, Park or Neutral.** Lateral actuation is hard-gated to Drive. This matters most during a reverse-parking shuffle, where MADS can otherwise re-arm in the gaps between gear changes.

---

## What to watch for on the first drives

This is a change to how the car is steered. Treat the first drives as a shakedown, not a commute.

1. **Lag learning resets once.** Because the control type changed, the system deliberately throws away the steering-lag estimate it learned under the old architecture rather than carrying a wrong number into the new one. Expect the first drive to feel slightly less dialled in while it re-learns.  
2. **Watch the wheel tint** and check it matches what you feel. Green while you are hands-off in a steady curve, blue when you have a hand on.  
3. **Check the handoff in a corner**, not just on a straight. Take a hand on and off mid-curve and confirm it neither lurches nor unwinds.

---

## Known gaps in this specific build

Be aware of these. They are not faults introduced by this change, but they are what this build ships with.

- **No "Take Control" warning in angle mode.** If a corner is tighter than the steering can actually deliver while it is steering on the angle channel, this build gives you **no on-screen prompt**. The stock warning keys off the torque controller, which is idle in angle mode. A fix exists and is queued for a later staging build. Until then, do not rely on the system to tell you it has run out of steering - watch the road.  
- Everything else in the feature queue (configurable minimum speeds, the on-screen wheel-tap angle/torque toggle, the settings for them) is **not** in this build. It is being promoted one commit at a time.

---

## Verification done before release

- Rivian car tests: 18 passed, 0 failed  
- Rivian panda safety tests: 149 passed, 90 skipped, 0 failed  
- Lint on all changed shared files: clean  
- Panda safety model: confirmed unchanged  
- Built and published by CI on the comma 3X runner: success
