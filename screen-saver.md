# Screen Saver

When the car is parked and the screen has not been touched for a while, AdventurePilot shows a screen saver instead of just turning the screen off. It can show the word **AdventurePilot**, the word **sunnypilot**, up to fourteen characters you type yourself, or a picture you send from your phone or a laptop. Whatever it shows drifts around and changes color every time it touches an edge, which is what stops a still image or a line of text wearing a permanent mark into the screen.

## Turning it on

Everything here lives under **Settings > Display**.

- **Screen Saver** - the master switch. Turn it off and the screen just goes black when idle, the ordinary way.
- **Screen Saver Duration** - how long the screen saver runs before the screen finally turns off. One to ten minutes.
- **Screen Saver Text** - which of the four things it shows: **AdventurePilot**, **sunnypilot**, **Custom**, or **Logo**. Switching to AdventurePilot or sunnypilot is a single tap here, nothing else to set up for either.

Two more rows appear only once they matter: **Custom Text** under Custom, and **Add a Logo** / **Screen Saver Logo** under Logo.

## Setting your own words

1. Park the car and switch it off - the screen saver and everything to do with it are parked-only.
2. Open **Settings > Display** and set **Screen Saver Text** to **Custom**.
3. Tap **EDIT** on the **Custom Text** row that appears, and type up to fourteen characters.

Leave it blank and the screen saver falls back to **AdventurePilot** rather than showing nothing.

Not available on a comma 4 - see [comma 4](#comma-4) below.

## Sending a picture

**On your phone?** Scan a code shown on the device. Nothing to set up, and the device checks the picture while you are still holding the phone.

**On a laptop?** Use the file browser instead, further down. Drag the file straight across, no need to get it onto your phone first, and it is the easier way to manage several pictures at once.

### From a phone

1. Park the car and switch it off.
2. Open **Settings > Display** and set **Screen Saver Text** to **Logo**. Two new rows appear underneath.
3. Tap **UPLOAD** on the **Add a Logo** row. The screen shows a square code and a web address.
4. Point your phone's camera at the code. Your phone offers to open a page - tap it, choose a picture, and tap Upload. Your phone must be on the **same Wi-Fi network** as the device, and the page will tell you straight away if the picture cannot be used, and why.
5. Tap **SELECT** on the **Screen Saver Logo** row and pick your picture. If it was your first, it is chosen for you already.

Scanning the code matters more than it looks: typed by hand, phones and computers now assume a secure connection the device cannot answer, and the page fails with something unhelpful like *This site can't be reached*. The code carries the full address, so that cannot happen.

### From a laptop

If the picture is already on a computer, this is the better route: drag it straight across rather than getting it onto your phone first. It is also the easiest way to load several pictures at once, rename them, or delete ones you are finished with, since you get an ordinary folder view.

> The file browser has **no password**. While it runs, anyone else on the same Wi-Fi can browse your driving footage and logs, and can change files on the device, including AdventurePilot's own program files. Fine on your home network; not on a hotel, campsite or workplace one. Switch it on, do what you need, switch it off.

1. Park the car and switch it off.
2. **Settings > Developer**, switch on **Show Advanced Controls**, then **copyparty Service**.
3. Read the **IP Address** under **Settings > Network > Advanced**.
4. On a computer on the same network, open `http://THAT-ADDRESS:8080` and put your picture in the **branding** folder.
5. Switch **copyparty Service** off again.

Type the `http://` yourself. Browsers now assume `https://` when you leave it off, the device does not speak it, and the page fails with *This site can't be reached* or *ERR_ADDRESS_UNREACHABLE*. That looks like a network fault, which sends people hunting in the wrong place - this is the main reason to use the code instead.

Nothing checks a picture sent this way. The device only inspects it when it tries to draw it, and if anything is wrong it quietly shows text instead, with no error to find. If a picture never appears, send the same file through the phone page instead - that one explains itself.

## What makes a good picture

The screen saver recolors your picture as it moves, picking a new color each time it touches an edge, the same as it does with text. A white shape on a transparent background works beautifully.

This is deliberate: constantly changing the color is what stops a still image wearing a permanent mark into the screen. It has a consequence worth knowing - recoloring works by filtering, so a picture made of one strong color disappears completely whenever it is tinted a color it does not contain. A red logo is invisible for roughly a third of every cycle. The upload page warns you when it spots this; the fix is always the same, send a white version.

| Property | Requirement | Why |
| :---- | :---- | :---- |
| File type | PNG, JPEG, WebP or GIF | PNG is best, because it can have a transparent background. Everything is converted to PNG on arrival |
| File size | 5 MB | A logo is far smaller than this |
| Largest side | 4096 pixels | Anything bigger is refused |
| Total size | 8 megapixels | A 4000 by 3000 photo is refused even though each side is under the limit |
| Good size | 1440 by 720 or less | It is shown at about a third of the screen anyway |
| Background | Transparent | The screen saver draws on black, so a white or colored rectangle around your logo will show |
| Color | White, or several colors | A picture made of one strong color vanishes when it is tinted the opposite one |
| Animation | Not supported | An animated GIF works, but only its first frame is shown |

## comma 4

A comma 4 has no Display settings screen, so it works a little differently and more simply. Go to **Settings > screen saver**, scan the code, and pick a picture - that is the whole thing, sending a picture also switches the screen saver to show it.

It keeps **one picture**, because there is no room to browse a list. Whatever you send replaces what was there, and the page says so before you send it. A **remove picture** button switches back to text.

Typing your own text is not offered on a comma 4. Locally it can only switch between showing a picture and showing text; it shows whichever preset text was already set, AdventurePilot by default.

comma 4 support is newer than the rest of this and has had less use. If something looks wrong, say so.

## What it will not do

- **Your picture is recolored.** Brand colors will not survive - see above for why this is worth accepting.
- **Only partly changeable from Sunnylink.** Sunnylink can turn the screen saver on or off, set its duration, and switch which of the four modes it uses, but it cannot send a picture or type custom text. Both appear there read-only, so you can see what is set without being able to change it remotely.
- **Only while parked.** The upload page only runs while that screen is open, and the screen saver itself only ever appears offroad.
- **One picture at a time.** You can store several and switch between them, but it does not cycle through them.
- **Only one picture on a comma 4.** Sending a new one replaces the old.
- **Lost on a full reinstall.** Pictures survive reboots, software updates and branch switches. They do not survive reinstalling the device software from scratch, and are not part of a settings backup.

## If something is not working

- **There is no Custom or Logo option under Screen Saver Text.** The update has not finished installing. Check **Settings > Software**, let any pending update complete, and restart the device - this needs a full restart, not just new files.
- **The screen saver never seems to start.** Check **Screen Saver** is switched on. It only ever appears while the car is parked and idle, and touching the screen counts as not idle.
- **It looks like the screen just goes black, not the screen saver.** **Screen Saver Duration** may be set low. That number is how long the screen saver itself keeps running before the screen finally turns off, so a one-minute setting can look, on a second glance, as though it skipped straight to black.
- **I set it to Custom but it still shows AdventurePilot.** Your custom text is empty. A blank field always falls back to AdventurePilot rather than showing nothing. Tap EDIT and type something.
- **My phone will not open the page after scanning.** Your phone is probably on a different network from the device, or on mobile data - check it is on the same Wi-Fi. Many home networks keep guest Wi-Fi separate from the main one, so a phone there cannot reach the device even though the name looks familiar.
- **The device says it is not on a network.** Connect it to Wi-Fi under **Settings > Network**. With nothing to join, switch on **Tethering** under **Settings > Network > Advanced** - the device makes its own network, called `weedle-` and four characters.
- **The upload page refused my picture.** It will have told you why on the phone. The usual reasons are that the picture is larger than 4096 pixels on a side, has more than 8 megapixels in total, is not really an image despite its name, or is entirely transparent.
- **I chose a logo but it still shows AdventurePilot.** The device could not use the picture, so it fell back to text. Unlikely if it arrived through the phone page, since that checks first; more likely if you dragged it across from a laptop, since nothing checks it that way. Send the same file through the phone page to find out why.
- **The logo is there but almost invisible, or keeps vanishing.** A dark picture is dim against the black background. A picture made of one strong color disappears entirely whenever it is tinted the opposite color, so it flashes in and out. Send a white version on a transparent background - most logos come with one meant for dark backgrounds.
- **The colors are wrong.** They are supposed to be. Whatever is showing takes a new color every time it hits the edge, text and pictures alike, and will never show its own colors.
- **How do I remove a picture?** On a comma 4, use the **remove picture** button on the screen saver screen. On other devices, switch on the file browser (above), open the **branding** folder and delete the file there. If the deleted picture was the chosen one, the screen saver goes back to text.

## Where things are kept

Which mode you chose and any custom text you typed are stored the same way as every other setting on the device, so there is nothing extra to know there. Pictures are different, because they are too big for that: they live at `/data/media/0/branding`, alongside your driving models and offline maps, deliberately outside the AdventurePilot program folder, which is replaced every time the software updates or you switch branch. Neither an update nor a branch switch will wipe your logo or your custom text - only reinstalling the device software from scratch will.
