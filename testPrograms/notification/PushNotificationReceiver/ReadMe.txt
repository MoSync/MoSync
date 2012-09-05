Enable Android GCM service following these steps: http://developer.android.com/guide/google/gcm/gs.html , and write down
the Google API project ID of the registered account.

Android server setup steps can be found here: http://developer.android.com/guide/google/gcm/demo.html#webserver-setup

Android requirements:
 - Complete Config.h file, by filling the GCM_PROJECT_ID field: that is the Google API project ID you obtained in the service enabling steps above.
 - The device must be running Android 2.2 or higher and have the Market application installed.
 - an existing connection for Google services. This requires users to set up their Google account on their mobile devices.
 - to use it on the emulator you need to emulate a device with Google API 8 or higher. You also have to register a Google user
on the virtual device under Settings > Accounts Sync.

The application consists of a Tab Screen with 2 Screens:
 1. The received Notification screen.
Here you will see all the info related to the latest received push notification:
	- on IOS the badge number
	- on both platforms the message text.

2. The Settings screen. Here you can connect to the server side and this will result in your registration_id being sent to the server.
On iOS the registration_id is sent to the server via a TCP connection, while on Android it's sent using an HTTP connection ( This is
because the GCM server-demo web server supplied by Google is waiting for the registration via an HTTP POST).
Follow this link for setting up the Web server for Android: http://developer.android.com/guide/google/gcm/demo.html#webserver-setup
<<NOTE: if you're using Apache Tomcat follow the exact same steps written in the documentation above, but at step 5-deploy, after copying the
generated war file to tomcat/webapps folder, run startup.bat from tomcat/bin . >>

Remember to press Connect after you got the registration result - you will be updated in the 2nd screen when this occurs.
 Also, remember that on Android the registration is done only once per app and so you don't need to connect to the server more that once.
- For both platforms you can set here: the IP address and the port of the server.
- On Android: set the ticker text and the title for the displayed notifications. They are set by default to the following: "ticker text" and "content title".
 (Ticker text: the text that flows by in the status bar when the notification first activates.) Also set the SHOW_ONLY_IF_NOT_RUNNING setting. If it's checked then the
 notification will be displayed only if the application is not running. If it's unchecked it means that the notifications will be displayed even when the application
 is running.

When the app is launched it will automatically try to register for push notifications. There is a platform specific difference here:
 - On iOS the registration process is done every time the app is launched.
  -On Android the registration is done only the first time the app is launched. Because it is recommended that the application
 registers with Google GCM only the first time it’s launched, and then stores the registration ID for later use. Due to this,
 the server side should wait for the REGISTRATION_ID only once. If you wish to register again you should go to Settings->Apps ->PushNotificationReceiver and ClearData or Uninstall.
