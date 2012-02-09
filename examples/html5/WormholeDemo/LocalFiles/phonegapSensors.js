// Sensors implemented by PhoneGap.

// ************ Accelerometer ************

var accelPhonegapWatch = null;

function toggleAccelPhonegap()
{
	if (accelPhonegapWatch !== null) {
		navigator.accelerometer.clearWatch(accelPhonegapWatch);
		updateAccelPhonegap({
			x : "&nbsp;",
			y : "&nbsp;",
			z : "&nbsp;"
		});
		accelPhonegapWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		accelPhonegapWatch = navigator.accelerometer.watchAcceleration(
			updateAccelPhonegap,
			errorAccelPhonegap,
			options);
	}
}
/*
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
*/

function updateAccelPhonegap(accel) {
	document.getElementById('accelPGx').innerHTML = accel.x;
	document.getElementById('accelPGy').innerHTML = accel.y;
	document.getElementById('accelPGz').innerHTML = accel.z;
}

function errorAccelPhonegap(error) {
	document.getElementById('accelPGx').innerHTML = "n/a";
	document.getElementById('accelPGy').innerHTML = "n/a";
	document.getElementById('accelPGz').innerHTML = "n/a";
}

// ************ GeoLocation ************

var geolocationWatch = null;

function toggleGeolocation()
{
	if (geolocationWatch !== null) {
		navigator.geolocation.clearWatch(geolocationWatch);
		updateGeolocation({ coords: {
			latitude : "&nbsp;",
			longitude : "&nbsp;",
			altitude : "&nbsp;"
		}});
		geolocationWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		geolocationWatch = navigator.geolocation.watchPosition(
			updateGeolocation,
			errorGeolocation,
			options);
	}
}

function updateGeolocation(loc) {
	document.getElementById('geoLat').innerHTML = loc.coords.latitude;
	document.getElementById('geoLong').innerHTML = loc.coords.longitude;
	document.getElementById('geoAlt').innerHTML = loc.coords.altitude;
}

function errorGeolocation(loc) {
	document.getElementById('geoLat').innerHTML = "n/a";
	document.getElementById('geoLong').innerHTML = "n/a";
	document.getElementById('geoAlt').innerHTML = "n/a";
}

// ************ Compass ************

var compassWatch = null;

function toggleCompass()
{
	if (compassWatch !== null) {
		navigator.compass.clearWatch(compassWatch);
		updateCompass({
			magneticHeading : "&nbsp;"
		});
		compassWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		compassWatch = navigator.compass.watchHeading(
			updateCompass,
			errorCompass,
			options);
	}
}

function updateCompass(result) {
	document.getElementById('CompassHeading').innerHTML =
		result.magneticHeading;
}

function errorCompass(error) {
	document.getElementById('CompassHeading').innerHTML = "n/a";
}
