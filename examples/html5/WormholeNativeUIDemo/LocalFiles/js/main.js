function onDeviceReady() {
			var platform = document.getNativeElementById("DeviceOSLabel");
			platform.setProperty("text" , "Platform: " + device.platform);
			var version = document.getNativeElementById("OSVersionLabel");
			version.setProperty("text" , "Version: " + device.version);
			var uuid = document.getNativeElementById("UUIDLabel");
			uuid.setProperty("text" , "UUID: " + device.uuid);
			var deviceName = document.getNativeElementById("DeviceNameLabel");
			deviceName.setProperty("text" , "Device Name: " + device.name);
}
var accelPhonegapWatch = null;
function toggleAccelPhonegap()
{
	if (accelPhonegapWatch !== null) {
		navigator.accelerometer.clearWatch(accelPhonegapWatch);
		updateAccelPhonegap({
			x : " ",
			y : " ",
			z : " "
		});
		accelPhonegapWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		accelPhonegapWatch = navigator.accelerometer.watchAcceleration(
				updateAccelPhonegap, function(ex) {
					alert("accel fail (" + ex.name + ": " + ex.message + ")");
				}, options);
	}
}

function roundNumber(num) {
	if(typeof num == "number")
	{
		var dec = 3;
		var result = Math.round(num * Math.pow(10, dec)) / Math.pow(10, dec);
		return result;
	}
	else
	{
		return num;
	}
}

function updateAccelPhonegap(a) {
	var data = document.getNativeElementById("AccelX");
	data.setProperty("text" , "X: " + a.x);

	var data = document.getNativeElementById("AccelY");
	data.setProperty("text" , "Y: " + a.y);

	var data = document.getNativeElementById("AccelZ");
	data.setProperty("text" , "Z: " + a.z);
}

var geolocationWatch = null;
function toggleGeolocation()
{
	if (geolocationWatch !== null) {
		navigator.geolocation.clearWatch(geolocationWatch);
		updateGeolocation({coords:{
			latitude : " ",
			longitude : " ",
			altitude : " "
		}});
		geolocationWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		geolocationWatch = navigator.geolocation.watchPosition(
				updateGeolocation, function(ex) {
					alert("geolocation fail (" + ex.name + ": " + ex.message + ")");
				}, options);
	}
}

function updateGeolocation(a) {
	var data = document.getNativeElementById("LocLat");
	data.setProperty("text" , "Latitude: " + a.coords.latitude);

	var data = document.getNativeElementById("LocLong");
	data.setProperty("text" , "Longtitude: " + a.coords.longitude);
}

var compassWatch = null;
function toggleCompass()
{
	if (compassWatch !== null) {
		navigator.compass.clearWatch(compassWatch);
		updateCompass({
			magneticHeading : " "
		});
		compassWatch = null;
	} else {
		alert("starting Comapss watch");
		var options = {};
		options.frequency = 1000;
		compassWatch = navigator.compass.watchHeading(
				updateCompass, function(ex) {
					alert("compass fail (" + ex.name + ": " + ex.message + ")");
				}, options);
	}
}

function updateCompass(a) {
	alert("updating compass");
	var data = document.getNativeElementById("CompassLabel");
	data.setProperty("text" , "Compass Heading: " +  a.magneticHeading);
}

var latestRecodedVideo = [];
function captureVideo()
{
	navigator.device.capture.captureVideo(
			function(mediaFiles)
			{
				alert(mediaFiles[0].fullPath);
				latestRecodedVideo = mediaFiles;
			},
			function(error)
			{
				alert("Error " + error.code);
			});
}

var latestCapturedImage = [];

function captureImage()
{
	navigator.device.capture.captureImage(
			function(mediaFiles)
			{
				latestCapturedImage = mediaFiles;
			},
			function(error)
			{
				alert("Error " + error.code);
			});
}