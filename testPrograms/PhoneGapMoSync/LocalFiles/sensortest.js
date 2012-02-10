var deviceInfo = function() {
	document.getElementById("platform").innerHTML = device.platform;
	document.getElementById("version").innerHTML = device.version;
	document.getElementById("uuid").innerHTML = device.uuid;
	document.getElementById("name").innerHTML = device.name;
	document.getElementById("width").innerHTML = screen.width;
	document.getElementById("height").innerHTML = screen.height;
	document.getElementById("colorDepth").innerHTML = screen.colorDepth;
};

function roundNumber(num) {
	var dec = 3;
	var result = Math.round(num * Math.pow(10, dec)) / Math.pow(10, dec);
	return result;
}

var accelerometer = new SensorConnection('Accelerometer');
accelerometer.addEventListener("onsensordata", function(event)
		{
			updateAcceleration(event.data);
		});

function updateAcceleration(a) {
	document.getElementById('accel-x').innerHTML = roundNumber(a.x);
	document.getElementById('accel-y').innerHTML = roundNumber(a.y);
	document.getElementById('accel-z').innerHTML = roundNumber(a.z);
}

var toggleAccel = function() {
	if (accelerometer.status == "watching") {
		accelerometer.endWatch();
		updateAcceleration({
			x : "",
			y : "",
			z : ""
		});
	} else {
		accelerometer.startWatch({interval:1000});
	}
};

var magnetometer = new SensorConnection('MagneticField');
magnetometer.addEventListener("onsensordata", function(event)
		{
			updateMagneticField(event.data);
		});

function updateMagneticField(a) {
	document.getElementById('magnetic-x').innerHTML = roundNumber(a.x);
	document.getElementById('magnetic-y').innerHTML = roundNumber(a.y);
	document.getElementById('magnetic-z').innerHTML = roundNumber(a.z);
}

var toggleMagnetic = function() {
	if (magnetometer.status == "watching") {
		magnetometer.endWatch();
		updateMagneticField({
			x : "",
			y : "",
			z : ""
		});
	} else {
		magnetometer.startWatch({interval:1000});
	}
};

var orientationSensor = new SensorConnection('Orientation');

orientationSensor.addEventListener("onsensordata", function(event)
		{
			updateOrientation(event.data);
		});

function updateOrientation(a) {
	document.getElementById('orientation-x').innerHTML = roundNumber(a.x);
	document.getElementById('orientation-y').innerHTML = roundNumber(a.y);
	document.getElementById('orientation-z').innerHTML = roundNumber(a.z);
}

var toggleOrientation = function() {
	if (orientationSensor.status == "watching") {
		orientationSensor.endWatch();
		updateOrientation({
			x : "",
			y : "",
			z : ""
		});
	} else {
		orientationSensor.startWatch({interval:1000});
	}
};

var proximity = new SensorConnection('Proximity');
proximity.addEventListener("onsensordata", function(event)
		{
			updateProximity(event.data);
		});

function updateProximity(a) {
	document.getElementById('proximity-x').innerHTML = roundNumber(a.x);
	document.getElementById('proximity-y').innerHTML = roundNumber(a.y);
	document.getElementById('proximity-z').innerHTML = roundNumber(a.z);
}

var toggleProximity = function() {
	if (proximity.status == "watching") {
		proximity.endWatch();
		updateProximity({
			x : "",
			y : "",
			z : ""
		});
	} else {
		proximity.startWatch({interval:1000});
	}
};

var gyroscope = new SensorConnection('Gyroscope');
gyroscope.addEventListener("onsensordata", function(event)
		{
			updateGyroscope(event.data);
		});

function updateGyroscope(a) {
	document.getElementById('gyroscope-x').innerHTML = roundNumber(a.x);
	document.getElementById('gyroscope-y').innerHTML = roundNumber(a.y);
	document.getElementById('gyroscope-z').innerHTML = roundNumber(a.z);
}

var toggleGyroscope = function() {
	if (gyroscope.status == "watching") {
		gyroscope.endWatch();
		updateGyroscope({
			x : "",
			y : "",
			z : ""
		});
	} else {
		gyroscope.startWatch({interval:1000});
	}
};

function fail(msg) {
	alert(msg);
}

function check_network() {
	var networkState = navigator.network.connection.type;

	var states = {};
	states[Connection.UNKNOWN]  = 'Unknown connection';
	states[Connection.ETHERNET] = 'Ethernet connection';
	states[Connection.WIFI]	 = 'WiFi connection';
	states[Connection.CELL_2G]  = 'Cell 2G connection';
	states[Connection.CELL_3G]  = 'Cell 3G connection';
	states[Connection.CELL_4G]  = 'Cell 4G connection';
	states[Connection.NONE]	 = 'No network connection';

	confirm('Connection type:\n ' + states[networkState]);
}

function find_sensors() {
	var results = navigator.findSensors();
	results.onsuccess = function()
	{
		sensorList="";
		for(var i = 0; i < results.result.length; i++)
		{
			sensorList += results.result[i].type + "<br/>";
			//alert("name:" + results.result[i].name + " type:" + results.result[i].type);
		}
		alert(sensorList);
		document.getElementByID("sensorList").innerHTML = sensorList;
	};
}

function captureTest() {
	Capture.prototype.captureImage(function(mediaFiles){alert(mediaFiles[0].fullPath + "  " + mediaFiles[0].name);}, function(){}, {duration:10});
}

function init() {
	// the next line makes it impossible to see Contacts on the HTC Evo since it
	// doesn't have a scroll button
	// document.addEventListener("touchmove", preventBehavior, false);
	document.addEventListener("deviceready", deviceInfo, true);
}
