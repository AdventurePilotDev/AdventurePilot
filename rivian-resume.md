# Rivian Resume Instructions

Although Rivian does not provide a native cruise control resume function unlike other brands, if you have Comma with the xnor longitudinal harness (xnor.shop) you can have most of the functions of resume that you might have been used to elsewhere.

# Setting up:

You should disable the native Rivian feature that enables the "Set to speed limit on divided highways" feature by holding the right hand stalk down for 0.5 seconds. Note that you should disable this in any case even if not using the resume feature because its use with longitudinal cruise control on comma will usually result in a significant disconnect between the comma and Rivian cruise set speed display. This disconnect is harmless but could cause confusion for the driver.

To use resume you must enable longitudinal (on the developer menu)

In the ap-dev-mr branch an additional toggle (Rivian: Enable Resume) has been added in the Device's Cruise menu and it is also available in the Sunnylink Vehicle menu. It is available only once the the comma has fingerprinted a Rivian and is OffRoad. It defaults to off (Disabled) - the Resume feature is effective only when this toggle is enabled

![Cruise settings menu with the "Rivian: Enable Resume" toggle](images/rivian-resume-cruise-menu.png)

# Sunnylink

![Sunnylink Vehicle page showing "Rivian: Enable Resume"](images/rivian-resume-sunnylink.png)

# How it works:

Holding the right hand stalk down full for at least 0.5 seconds will set the cruise set speed to the same as the most recent set speed the last time cruise was turned off. If cruise has never been enabled it will just set the speed to the vehicle's current speed.  It will of course, only work whenever it is possible to invoke cruise on stock Rivian - i.e. at speeds greater than 20mph/32kph or, under some circumstances, when stopped behind another vehicle (such as at a traffic light or a stop sign) just as for stock Rivian.

# Things to note:

Inevitably when resume is invoked the Rivian's set speed display will be out of sync with the comma's actual set speed since it will display the speed at which the vehicle was moving when resume was invoked. This is similar to the disconnect that happens when using the speed control buttons on the steering wheel. only a bit more dramatic. If the vehicle was moving slower than the resume set speed the driver can take advantage of the feature whereby a tap down of the rh stalk will update the Rivian's cruise set speed display to equal the vehicle's speed at the time. Unfortunately if the vehicle was moving faster than the resume set speed this "trick" will not work owing to the inherent limitation in the Rivian system.
