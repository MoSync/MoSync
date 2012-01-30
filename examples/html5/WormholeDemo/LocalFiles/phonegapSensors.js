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
	document.getElementById('accelPGx').innerHTML = a.x;
	document.getElementById('accelPGy').innerHTML = a.y;
	document.getElementById('accelPGz').innerHTML = a.z;
}

var geolocationWatch = null;
function toggleGeolocation()
{
	if (geolocationWatch !== null) {
		navigator.geolocation.clearWatch(geolocationWatch);
		updateGeolocation({coords:{
			latitude : "&nbsp;",
			longitude : "&nbsp;",
			altitude : "&nbsp;"
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
	document.getElementById('geoLat').innerHTML = a.coords.latitude;
	document.getElementById('geoLong').innerHTML = a.coords.longitude;
	document.getElementById('geoAlt').innerHTML = a.coords.altitude;
}

var compassWatch = null;
function toggleCompass()
{
	if (compassWatch !== null) {
		navigator.geolocation.clearWatch(compassWatch);
		updateCompass({
			magneticHeading : "&nbsp;"
		});
		compassWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		compassWatch = navigator.compass.watchHeading(
				updateCompass, function(ex) {
					alert("compass fail (" + ex.name + ": " + ex.message + ")");
				}, options);
	}
}

function updateCompass(a) {
	document.getElementById('CompassHeading').innerHTML = a.magneticHeading;
}
