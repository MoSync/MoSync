The top space is reserved for the ad. Below it, there is a label with the Ad status: bannerLoaded, bannerOnDismiss (the control returned to our app), bannerOnLeaveApplication(when the user has tapped the ad view), or bannerFailedLoad.
When the application is started the ads begin to load, but there can be an initial delay until they are shown.
Android: Note that that the very first time AdMob sees your publisher ID it may take up to two minutes to receive an ad, and this initial two minute lag will recur every time the app goes unused for 24 hours.
Next,  there are the settings:
	- "Enable click on banner" which if set to true enables the ad to be taped.
	- "Is enabled" button which gets the enabled state of the ad: after the buttton is pressed the label next to it will display the state.
	(Note! that this enabled property does not work for the moment on Android, it will work only after we can build the runtime against targetSdkVersion = 13.)
	- "Show banner" option which shows or hides the banner.
	- "Is visible" button gets the visibility of the banner.
	- "Get size" which gets the size of the banner. Note that even when the banner is not visible, the size can still be retrieved.

The back button exits the application on Android.