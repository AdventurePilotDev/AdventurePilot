# Jetlink: Run the Big Driving Model on a Computer or Phone

**Applies to:** any car AdventurePilot supports, on a **comma 3X or comma 4**.

**Where it has landed:** only on two experimental branches, from 17 November 2026:

| Branch | For |
| :---- | :---- |
| `devj` | Torque steering (the same as `dev`, plus Jetlink) |
| `devj-a` | Angle steering with the angle harness (the same as `dev-a`, plus Jetlink) |

Jetlink is **not** on `dev`, `dev-a`, `stg` / `stg-a` or `rel` / `rel-a`.

> **Experimental, and not yet road tested on AdventurePilot.** These branches have only been checked on the bench so far. Jetlink itself is already used on other openpilot forks, and when the AdventurePilot version was tried on a comma with a Mac, the big model ran at full speed for several sessions of a few minutes each. Only use it if you are comfortable being an early tester, and be ready to take over at any time.

Jetlink is built by zoompilot ([zoompilot/jetlink](https://github.com/zoompilot/jetlink)). Nimijneb added it to AdventurePilot, with help from Rawsignal, and Gerrylum's version for his own fork supplied the Sunnylink setting.

---

## What it does

openpilot can drive with one of two kinds of driving model: a **small model**, which the comma runs itself, and a **big model**, which is too big for the comma and needs extra hardware. comma's answer to that is its own add-on board. Jetlink is another way: it runs the big model on a computer or phone that you plug into the comma's USB-C port.

- The comma always keeps the small model running, ready to take over.
- When the big model is ready, you choose when it takes over.
- If the link drops or falls behind, the comma warns you and carries on driving on the small model.

Nothing changes unless you turn Jetlink on. With it off, `devj` drives exactly like `dev`, and `devj-a` exactly like `dev-a`.

---

## What you need

- **A comma 3X or comma 4.**
- **A computer or phone to run the big model**, one of:

  | Device | Jetlink setting |
  | :---- | :---- |
  | NVIDIA Jetson Orin Nano Super | USB |
  | Apple silicon Mac | USB |
  | Linux PC with an NVIDIA GPU | USB |
  | Android phone with USB 3 (experimental) | USB |
  | iPhone or iPad with USB-C (experimental, through TestFlight) | iOS |

- **A USB 3 data cable.** A charge-only cable will not work, and a USB 2 cable is unreliable.
- **Separate power for the comma and for the computer.** Keep powering the comma from the car as you do now. Do not try to power it through the USB-C port Jetlink uses.
- **No chestnut board fitted.** The USB-C port cannot serve both. With a chestnut fitted, the Jetlink setting does not appear.

### Setting up the computer or phone

Each device has its own setup guide in the Jetlink project:

- [Jetson](https://github.com/zoompilot/jetlink/blob/main/docs/jetson.md)
- [Mac](https://github.com/zoompilot/jetlink/blob/main/docs/macos-app.md)
- [Linux PC](https://github.com/zoompilot/jetlink/blob/main/docs/linux-pc.md)
- [iPhone or iPad](https://github.com/zoompilot/jetlink/blob/main/docs/iphone-app.md)
- [Android](https://github.com/zoompilot/jetlink/blob/main/docs/android-app.md)

Do the setup with the car parked and both devices online. The cables guide is also worth reading: [cables and power](https://github.com/zoompilot/jetlink/blob/main/docs/transport.md).

---

## Setting it up on the comma

1. **Install the branch.** Change the comma's software branch to `devj` (torque steering) or `devj-a` (angle steering). The first start on the new branch builds the software, which takes several minutes.
2. **Reboot the comma once more** after it finishes, so that the new Jetlink setting appears.
3. **Turn Jetlink on**, with the car parked:
   - **On the device:** **Settings > Models > Jetlink**. Tap to step through **Off**, **USB** and **iOS**.
   - **On Sunnylink:** the **Device** page, **Jetlink (Experimental)**.

   Choose **USB** for a Jetson, Mac, Linux PC or Android phone, and **iOS** for an iPhone or iPad. The setting can only be changed while the car is OffRoad.
4. **Plug in the computer or phone** with the USB 3 cable and start Jetlink on it.
5. **Pick the big model.** With Jetlink on, the **Big Model** list under **Settings > Models** is Jetlink's own list of models. Start with the default. The computer downloads the model and prepares it, which takes a few minutes the first time.

Under the Jetlink row the Models screen shows what is going on: what is on the USB-C port, and progress while the model downloads and prepares.

---

## Driving with it

The small model always drives first. When the big model is ready, the comma tells you:

| Alert | What it means | What to do |
| :---- | :---- | :---- |
| **Jetlink Ready** - *Disengage fully to switch* | The big model is ready to take over. | Turn openpilot **fully** off: cruise **and** lane keeping (MADS). You do not need to stop the car. |
| **Big Model Loading** | The switch is in progress, about a second. Engaging is blocked until it finishes. | Wait. |
| **Jetlink Active** | The big model is now driving. | Engage again as normal. |
| **Jetlink Lost** - *Small model driving* | The link dropped or fell behind. openpilot carries on with the small model. | **Be ready to take over**, especially in the first few seconds. Jetlink reconnects by itself, and Jetlink Ready appears again when you can switch back. |

**Why "fully" off:** with MADS, lane keeping can stay on after cruise is off, and openpilot still counts that as driving. The big model only takes over when nothing is engaged, so turn off lane keeping too.

**If something is wrong with Jetlink itself**, an alert on the home screen says **Jetlink unavailable** and gives the reason. Driving is not affected: the comma uses the small model as usual.

---

## Things to note

- **The small model is the safety net, not a guarantee.** When the link is lost, the car keeps steering on the small model, which may behave differently from the big one. Stay alert.
- **ADB is switched off while Jetlink is using the USB-C port.** If you use ADB over USB for development, it turns off on its own while Jetlink holds the port.
- **Parking.** When the comma powers down after a drive, it asks an always-on Jetson to power down as well, waiting up to about 30 seconds. A Jetson set up as **Always on** then wakes again when the car starts.
- **Each frame has 50 ms.** The computer must finish the big model inside that time, every frame. A hot computer, a laptop going to sleep, or a poor cable can make it miss, which shows up as Jetlink Lost. Keep laptops powered and awake, and keep the iPhone app on screen.
- **Turning it off:** set **Jetlink** back to **Off** under **Settings > Models** (car parked), or switch back to `dev` / `dev-a`.

### Help

- Jetlink's own [troubleshooting guide](https://github.com/zoompilot/jetlink/blob/main/docs/troubleshooting.md) and [daily use guide](https://github.com/zoompilot/jetlink/blob/main/docs/using-jetlink.md) apply here too. Where they mention zoompilot's or StarPilot's branches, read `devj` or `devj-a`.
- When reporting a problem, include your computer or phone, the comma model, the driving model, the exact alert, and when it happened.
