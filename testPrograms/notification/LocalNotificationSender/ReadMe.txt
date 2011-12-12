This app schedules local notifications.
The first field is for content body text.
On Android only there are two more texts to set:
 - content body title and
 - the ticker text.
(Ticker text: the text that flows by in the status bar when the notification first activates.)
Next, you can set for iOS only:
 - the badge number,
  -the alert action (the title of the action button or slider).
 Next the Default Sound can be enabled/disabled.
Below, settings available only on Android:
 - the Vibration settings ( that are available only on Android): By checking the Vibration check box, the default
vibration pattern is applied. If the vibration duration is set, it will be used instead.
 - the Flash LED settings( that are available only on Android): By checking the Flashing check box, the default flashing
pattern is applied to the notification. If the pattern is set(by filling the next 3 edit boxes), it will be used instead.
If the flashing is not supported on the device you will get the message "Not available" inside the edit boxes for this settings.
Note that you need to enable the Notification LED lights from device's Settings.
The Flashing pattern is consisted of:
- a color, in the format 0xRRGGBB, for instance red: 0xFF0000.
- led on = length of time, in seconds, to keep the light on.
- led off = length of time, in seconds, to keep the light off.

The last setting is the fire date: number of seconds for scheduling.

If the input boxes contain invalid values, nothing will happen when clicking
Schedule button.

After a notification is scheduled( by pressing the Schedule btn), the form is initialized with default values.
The back button exits the application on Android.