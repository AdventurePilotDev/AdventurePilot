# Curve Speed Control

Curve Speed Control slows the vehicle down ahead of a curve, holds a steady, comfortable speed through the bend, and then powers smoothly back out - keeping the cornering forces within what the steering can actually deliver. It is aimed at the kind of tight, sighted curves you get on canyon and mountain roads.

This is a **separate feature** from Smart Cruise Control - Vision (SCC-V) and Smart Cruise Control - Map (SCC-M). It has its own toggle and can be run on its own or alongside them. See [How it differs from SCC-V / SCC-M](#how-it-differs-from-scc-v--scc-m) below.

## What it does

Curve Speed Control has two parts that work together:

1. **A look-ahead speed profile (feedforward).** It reads the curve of the road from the vision driving model out to the model's horizon, works out the fastest comfortable speed for the sharpest part of each curve, and then plans the braking *backwards* from there. The result is that the car starts easing off **early** and arrives at the curve already at the right speed, rather than braking hard at the last moment.  
     
2. **A lateral-load backstop (governor).** While you are actually in the curve, it watches how hard the steering is really working. If the curve turns out tighter than predicted, or you entered a little hot, it trims speed further to keep the steering load within its real limit - and it will not add throttle while the steering is near that limit ("don't accelerate into a hard corner").

Part 1 does the smooth, planned-ahead work; part 2 is the safety net that catches anything the plan under-read.

## Turning it on

1. Open the **Cruise** menu.  
2. Turn on **Curve Speed Control**.

Both parts are controlled by this single toggle. No map data or GPS is required - it works purely from the forward camera / driving model.

## Requirements

* The car's speed must be managed by sunnypilot (sunnypilot Longitudinal Control). Curve Speed Control only acts while longitudinal control is engaged.  
* It does not operate below **20 km/h (about 12 mph)**.  
* It never drives faster than your set cruise speed - it only ever reduces speed for curves.  
* Pressing the accelerator overrides it; it resumes when you lift off.

This feature is currently tuned and validated for the Rivian R1T / R1S (it adapts to whichever steering mode - torque or angle - is active).

## How it behaves on the road

While cruise is engaged and you are above 20 km/h:

1. **Approaching a curve (Slowing)** - the car eases off early and settles to a comfortable entry speed before the bend.  
2. **In the curve (Curve)** - it holds a steady speed chosen to keep cornering comfortable; sharper curves are held slower.  
3. **If the curve is tighter than expected (Governing)** - the backstop trims a little more speed and holds off the throttle until the steering unloads.  
4. **Leaving the curve** - it smoothly returns to your set cruise speed.

The on-road display shows a Curve Speed Control indicator that lights up (flashes) while the feature is slowing for, or working through, a curve.

## How it differs from SCC-V / SCC-M

All three slow you down for turns, but they are not the same feature:

|  | Curve Speed Control | SCC-V (Vision) | SCC-M (Map) |
| :---- | :---- | :---- | :---- |
| **Source** | Vision driving model | Vision driving model | Map data + GPS |
| **Method** | Full speed profile planned over the road ahead, braking worked backwards from the sharpest point, plus a reactive steering-load backstop | Reduces the curve ahead to a single number and reacts with a state machine | Looks up curve speeds from map geometry ahead of your position |
| **Reacts to what the steering is actually doing?** | Yes - trims speed and holds throttle if the steering is near its limit | No | No |
| **Needs GPS / maps?** | No | No | Yes |
| **Best for** | Tight, sighted curves (canyon / mountain roads) | Curves visibly ahead | Curves still out of sight (over a crest, beyond trees) |

You can enable Curve Speed Control together with SCC-V and/or SCC-M. When more than one is on, the system always uses whichever one asks for the **lower** speed at that moment, so you get the most cautious result.

## Things to keep in mind

* Alphalong (developer menu) must be enabled for this to be available  
* This assists you - it does not replace attention. Keep your hands close to the wheel and be ready to take over, especially on unfamiliar or very sharp curves.  
* Because it plans from what the camera can see, very sudden curves hidden right up to the last moment give it less room to slow early; the steering-load backstop is what covers those cases.  
* Below 20 km/h it is inactive by design.  
* Tuning (how hard it is willing to load the steering in a curve) is currently set for the Rivian platform.

