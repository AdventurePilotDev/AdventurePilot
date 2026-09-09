# Rivian enhanced MADS Instructions

Up to now the MADS feature for Rivian has been restricted to having Steering Mode on Brake Pedal (under Customize MADS in the Steering Menu) set to Disengage. This update allows for the other two options, Remain Active and Pause. This document describes how MADS works in each of the three configurations.

This should NOT require the longitudinal upgrade harness.

For safety the design provides a single action for drivers to disengage MADS regardless of the brake setting

Although stalk full up does disengage MADS, users should not get used to using this since it could inadvertently put the vehicle into reverse if moving very slowly or stopped. 

# Disengage

Tapping (or holding) the brake pedal disengages ACC and MADS.

To engage MADS use the stalk tap up action (i.e. not past the detent) from either ACC or disengaged. 

Subsequent taps up will toggle MADS off and on. This (toggle) feature might not have been available in previous versions of Rivian MADS.

Stalk Full up action or putting the vehicle in Park or Reverse[^1] will disengage ACC and MADS altogether. **IT IS NOT RECOMMENDED TO GET USED TO USING STALK FULL UP TO DISENGAGE BECAUSE OF THE DANGER OF PUTTING THE CAR IN REVERSE ACCIDENTALLY IF YOU DO THIS WHILE STOPPED.**

This is the default setting upon first install. Note that if you have previously navigated the brake setting and changed it then a fresh install will not overwrite your previously selected choice. Be aware that right after installation the Settings screen or Sunnylink might still show "Remain Active" (because this is the Sunnypilot default which is overridden for Rivians) even though the vehicle will already correctly use the "Disengage" behavior. This is just a display catching up, not a sign anything is wrong. The screen will show the correct value once you restart the device (or the app).

# Remain Active

Tapping (or holding) the brake pedal disengages ACC if currently active but keeps MADS active. This is the primary method to engage MADS.

To disengage MADS use the stalk tap up action (i.e. not past the detent) from either ACC or MADS. 

Subsequent taps up will toggle MADS on and off. 

Stalk Full up action or putting the vehicle in Park or Reverse[^2] will disengage ACC and MADS altogether. **IT IS NOT RECOMMENDED TO GET USED TO USING STALK FULL UP TO DISENGAGE BECAUSE OF THE DANGER OF PUTTING THE CAR IN REVERSE ACCIDENTALLY IF YOU DO THIS WHILE STOPPED.**

# Pause

Pause is essentially the same as Remain Active except that holding the brake pedal will temporarily disengage (i.e. Pause) MADS. Releasing the brake pedal will then re-engage MADS.

Stalk actions are as for Remain Active

[^1]:  Reverse disengagement is still being rolled out to AdventurePilot branches - it is currently in dev, stg and stg-a

[^2]:  Reverse disengagement is still being rolled out to AdventurePilot branches - it is currently in dev, stg and stg-a  
