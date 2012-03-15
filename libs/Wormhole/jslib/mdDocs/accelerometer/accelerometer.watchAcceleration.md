accelerometer.watchAcceleration
===============================

At a regular interval, get the acceleration along the x, y, and z axis.

	var watchID = navigator.accelerometer.watchAcceleration(
		accelerometerSuccess,
		accelerometerError,
		[accelerometerOptions]);

Description
-----------

The accelerometer is a motion sensor that detects the change (delta) in movement relative to the current position. The accelerometer can detect 3D movement along the x, y, and z axis.

The `accelerometer.watchAcceleration` gets the device's current acceleration at a regular interval. Each time the `Acceleration` is retrieved, the `accelerometerSuccess` callback function is executed. Currently the implementation of this function only supports the normal rate of the device the app is running on.

The returned watch ID references references the accelerometer watch interval. The watch ID can be used with `accelerometer.clearWatch` to stop watching the accelerometer.


Quick Example
-------------

	function onSuccess(acceleration) {
		alert('Acceleration X: ' + acceleration.x + '\n' +
			'Acceleration Y: ' + acceleration.y + '\n' +
			'Acceleration Z: ' + acceleration.z + '\n' +
			'Timestamp: ' + acceleration.timestamp + '\n');
	};

	function onError() {
		alert('onError!');
	};

	var options = { frequency: 3000 };  // Update every 3 seconds

	var watchID = navigator.accelerometer.watchAcceleration(onSuccess, onError, options);

Full Example
------------

	<!DOCTYPE html>
	<html>
	<head>
		<title>Acceleration Example</title>

		<script type="text/javascript" charset="utf-8" src="js/wormhole.js"></script>
		<script type="text/javascript" charset="utf-8">

		// The watch id references the current `watchAcceleration`
		var watchID = null;

		// Wait for Wormhole to load
		//
		document.addEventListener("deviceready", onDeviceReady, false);

		// Wormhole is ready
		//
		function onDeviceReady() {
			startWatch();
		}

		// Start watching the acceleration
		//
		function startWatch() {

			// Update acceleration every 3 seconds
			var options = { frequency: 3000 };

			watchID = navigator.accelerometer.watchAcceleration(onSuccess, onError, options);
		}

		// Stop watching the acceleration
		//
		function stopWatch() {
			if (watchID) {
				navigator.accelerometer.clearWatch(watchID);
				watchID = null;
			}
		}

		// onSuccess: Get a snapshot of the current acceleration
		//
		function onSuccess(acceleration) {
			var element = document.getElementById('accelerometer');
			element.innerHTML = 'Acceleration X: ' + acceleration.x + '<br />' +
								'Acceleration Y: ' + acceleration.y + '<br />' +
								'Acceleration Z: ' + acceleration.z + '<br />' +
								'Timestamp: ' + acceleration.timestamp + '<br />';
		}

		// onError: Failed to get the acceleration
		//
		function onError() {
			alert('onError!');
		}

		</script>
	</head>
	<body>
		<div id="accelerometer">Waiting for accelerometer...</div>
	</body>
	</html>
