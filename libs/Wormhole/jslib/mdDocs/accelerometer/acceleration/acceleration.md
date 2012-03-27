Acceleration
============

Contains `Accelerometer` data captured at a specific point in time.

Properties
----------

- __x:__ Amount of motion on the x-axis. Range [0, 1] (`Number`)
- __y:__ Amount of motion on the y-axis. Range [0, 1] (`Number`)
- __z:__ Amount of motion on the z-axis. Range [0, 1] (`Number`)
- __timestamp:__ Creation timestamp in milliseconds. (`DOMTimeStamp`)

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

	navigator.accelerometer.getCurrentAcceleration(onSuccess, onError);

Full Example
------------

	<!DOCTYPE html>
	<html>
	<head>
		<title>Acceleration Example</title>

		<script type="text/javascript" charset="utf-8" src="js/wormhole.js"></script>
		<script type="text/javascript" charset="utf-8">

		// Wait for Wormhole to load
		//
		document.addEventListener("deviceready", onDeviceReady, false);

		// Wormhole is ready
		//
		function onDeviceReady() {
			navigator.accelerometer.getCurrentAcceleration(onSuccess, onError);
		}

		// onSuccess: Get a snapshot of the current acceleration
		//
		function onSuccess() {
			alert('Acceleration X: ' + acceleration.x + '\n' +
				'Acceleration Y: ' + acceleration.y + '\n' +
				'Acceleration Z: ' + acceleration.z + '\n' +
				'Timestamp: ' + acceleration.timestamp + '\n');
		}

		// onError: Failed to get the acceleration
		//
		function onError() {
			alert('onError!');
		}

		</script>
	</head>
	<body>
		<h1>Example</h1>
		<p>getCurrentAcceleration</p>
	</body>
	</html>
