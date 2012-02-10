notification.beep
=================

The device will play a beep sound.

	navigator.notification.beep(times);

- __times:__ The number of times to repeat the beep (`Number`)

Quick Example
-------------

	// Beep twice!
	navigator.notification.beep(2);

Full Example
------------

	<!DOCTYPE html>
	<html>
	<head>
		<title>Notification Example</title>

		<script type="text/javascript" charset="utf-8" src="js/wormhole.js"></script>
		<script type="text/javascript" charset="utf-8">

		// Wait for Wormhole to load
		//
		document.addEventListener("deviceready", onDeviceReady, false);

		// Wormhole is ready
		//
		function onDeviceReady() {
			// Empty
		}

		// Show a custom alert
		//
		function showAlert() {
			navigator.notification.alert(
				'You are the winner!',  // message
				'Game Over',            // title
				'Done'                  // buttonName
			);
		}

		// Beep three times
		//
		function playBeep() {
			navigator.notification.beep(3);
		}

		// Vibrate for 2 seconds
		//
		function vibrate() {
			navigator.notification.vibrate(2000);
		}

		</script>
	</head>
	<body>
		<p><a href="#" onclick="showAlert(); return false;">Show Alert</a></p>
		<p><a href="#" onclick="playBeep(); return false;">Play Beep</a></p>
		<p><a href="#" onclick="vibrate(); return false;">Vibrate</a></p>
	</body>
	</html>

Notes
-----

- Wormhole implements beep by playing an audio file via the MoSync API.
- The developer must provide a file with the desired beep tone.
- This file must be located in the application's Resources folder and must be named `beep.wav` (can be changed by editing the resource file).
- Template applications and example applications have this set up.

