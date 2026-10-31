# OPRivView

**Applies to:** comma 3X and comma four, on any car. Built with the Rivian in mind, including the EV Play box that puts Android on a Rivian's center screen.

OPRivView is an Android app that shows your comma device's driving screen live on a phone, tablet or car head unit. You see the road camera with the overlay on top: the planned path, lane lines, lead car, speeds, alerts, the state of steering and cruise control, and the [Lane Centering](lane-centering.md) readout.

**It only watches.** Nothing in the app changes how the car drives or any setting on the comma.

**Where it has landed:** the device side is on the `dev` and `dev-a` development trunks. It is not in the `stg` or `rel` prebuilts yet. It is off by default.

---

## Where the app lives

AdventurePilot supplies the comma side. The app itself is a separate project, built and released from [Geomglot/opview](https://github.com/Geomglot/opview), and it is not an AdventurePilot release.

- **Download:** the newest `OPRivView-<version>.apk` from [Geomglot/opview releases](https://github.com/Geomglot/opview/releases). Releases are marked "Pre-release"; that is normal.
- **Full guide to the app:** the [OPRivView user guide](https://github.com/Geomglot/opview/blob/rivian-support/docs/USER_GUIDE.md) covers installing on a phone or an EV Play, the three layouts, every setting and troubleshooting.

If the app proves successful, the APK work may eventually move into AdventurePilot. For now, go to the Geomglot repository for the app and report app problems there.

---

## What you need

- A comma 3X or comma four on `dev` or `dev-a`.
- An Android device on Android 7.0 or later: a phone, a tablet, a head unit or an EV Play Max. There is no iPhone version yet, and it does not run through Android Auto or Apple CarPlay.
- One Wi-Fi network for both. The comma and the Android device must be on the same one.

---

## Setting it up

### 1. Turn on streaming on the comma

**On a comma 3X:**

1. Open **Settings > Developer**.
2. Turn on **Show Advanced Controls**, if it is not already on.
3. Turn on **opview Screen Streaming**.

**On a comma four:** open **developer** in settings and turn on **opview streaming**.

It takes effect the next time the car is turned on.

> **After updating, reboot the device before looking for this.** It is a new setting, and new settings are only registered on a full restart. Until then the switch is not there.

### 2. Install the app

Follow the [install steps in the user guide](https://github.com/Geomglot/opview/blob/rivian-support/docs/USER_GUIDE.md#installing). After the first install the app offers its own updates.

> **EV Play warning: never set a password on the EV Play while installing OPRivView.** If anything on the EV Play asks you to set a password, PIN or screen lock, do not do it. Back out instead. A password makes the EV Play unreachable, and getting it back takes a tricky recovery process. If it happens, contact EV Play support at alex@evplay.net for recovery instructions.

### 3. Put both on one network

Any of these works:

- The Rivian's own hotspot, if it is turned on.
- The comma's hotspot: turn on tethering under **Settings > Network > Advanced**.
- A phone's hotspot.
- Home Wi-Fi, for trying it out in the driveway.

### 4. Start the car

Open OPRivView. It finds the comma by itself. The picture appears once the comma is driving, which means the car is on. If it cannot find the comma, enter the comma's IP address in the app's settings. The address is shown on the comma under **Settings > Network > Advanced**.

---

## Watching in a web browser: OP WebView

There is also a web version, [OP WebView](https://inertiaimpact.github.io/OPWebView/), made by inertiaimpact. It shows the
same kind of overlay in a browser, with no app to install, so it also works on a laptop.
It can be installed to the home screen from the browser.

1. Turn on **opview Screen Streaming** on the comma, as in step 1 above, and put both on one network, as in step 3.
2. Start the car.
3. Open [https://inertiaimpact.github.io/OPWebView/](https://inertiaimpact.github.io/OPWebView/) in Chrome, and allow
   local network access when the browser asks. It looks for the comma by itself; if it does not find it, use
   **+ Add IP address** and enter the address from **Settings > Network > Advanced** on the comma.

Current Chrome is what it is built for. Other browsers may refuse to connect from a web page to a device on your own
network. The page itself comes from the internet, but the video and driving data go straight from the comma to your
browser, not through any website. **Demo** shows it working without a car.

The comma lets only that one website connect from a browser. Other websites cannot reach it. OP WebView is a
separate project, not part of AdventurePilot: report problems with it on [its own repository](https://github.com/inertiaimpact/OPWebView).

---

## What to be aware of

- **Anyone on the same network can watch while this is on.** There is no login. The road camera and driving data are visible to any device on that network that asks for them, so use a network you trust, such as your own hotspot, and leave the setting off otherwise.
- **A browser on that network can watch too, but only through OP WebView.** Other websites are refused by the comma.
- **Nothing can be sent to the car through it.** While the setting is on, the comma ignores anything a viewer tries to send, from the app or from a browser. One side effect: remote joystick control through comma Connect does not work while this is on.
- **With the setting off, nothing changes.** The streaming service is not started for the app, and the comma is not open to the network.
- **A problem in the stream cannot disengage the car.** If the video stream or its service crashes, you lose the picture and the car carries on. This protection is on for everyone on these branches, whether or not this setting is on.
- **It is a display, not a mirror you should rely on.** The comma's own screen and alerts remain the ones that count. Keep your eyes on the road.

---

## If something is not working

- **There is no opview Screen Streaming switch.** Turn on **Show Advanced Controls** first. If it is still missing, check you are on `dev` or `dev-a` under **Settings > Software**, then reboot.
- **The app is stuck searching for the comma.** Check both are on the same network, that the setting is on, and that the car has been turned on since you turned it on. Then try entering the comma's IP address.
- **Connected but no picture.** The picture only appears while the car is on. Wait a few seconds after starting it.
- **No lane centering readout.** Turn on **Lane Centering** on the comma. The readout only shows while openpilot is steering.
- **Remote joystick stopped working.** Turn this setting off.
- **OP WebView cannot connect.** Use current Chrome and allow local network access when asked. If you said no, allow it
  again in the site's settings in Chrome. Check you are on `dev` or `dev-a` from 31 October 2026 or later: earlier builds
  refuse browser connections.

For anything else about the app, see the [troubleshooting table in the user guide](https://github.com/Geomglot/opview/blob/rivian-support/docs/USER_GUIDE.md#troubleshooting).

---

## Credits

- [eFiniLan/opview](https://github.com/eFiniLan/opview), the original app.
- [Gerrylum](https://github.com/gerrylum/opview), for the Enhanced and Detailed layouts, the settings menu, in-app updates, the OPRivView name and icon, and much of the comma-side streaming work.
- [Geomglot](https://github.com/Geomglot/opview), for sunnypilot support, the lane centering readout and the releases.
- [inertiaimpact](https://github.com/inertiaimpact/OPWebView), for the OP WebView web client.
- [comma.ai openpilot](https://github.com/commaai/openpilot), whose driving screen it reproduces.

## Related pages

- [Lane Centering](lane-centering.md) - what the readout in the app is showing.
