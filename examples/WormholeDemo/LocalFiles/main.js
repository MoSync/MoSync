document.addEventListener("deviceready", deviceInfo, true);
var deviceInfo = function() {
				document.getElementById("platform_li").innerHTML = "Platform: " + device.platform;
				document.getElementById("plaformversion_li").innerHTML = "Version: " + device.version;
				document.getElementById("deviceName_li").innerHTML = "Device: " + device.name;
};

var accelerometer = new SensorConnection("Accelerometer");

accelerometer.addEventListener("onsensordata", updateAccelW3C);

function updateAccelW3C(sensorData){
	document.getElementById('accelW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('accelW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('accelW3Cz').innerHTML = sensorData.data.z;
}

function toggleAccelW3C()
{
	if(accelerometer.status == "open")
	{
		accelerometer.startWatch({interval:1000});
	}
	else
	{
		accelerometer.endWatch();
		updateAccelW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var magneticField = new SensorConnection("MagneticField");

magneticField.addEventListener("onsensordata", updateMagDataW3C);

function updateMagDataW3C(sensorData){
	document.getElementById('MagFieldW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('MagFieldW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('MagFieldW3Cz').innerHTML = sensorData.data.z;
}

function toggleMagFieldW3C()
{
	if(magneticField.status == "open")
	{
		magneticField.startWatch({interval:1000});
	}
	else
	{
		magneticField.endWatch();
		updateMagDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var orientation = new SensorConnection("Orientation");

orientation.addEventListener("onsensordata", updateOrientDataW3C);

function updateOrientDataW3C(sensorData){
	document.getElementById('OrientationW3C').innerHTML = sensorData.data.x;
}

function toggleOrientationW3C()
{
	alert(typeof orientation);
	if(orientation.status == "open")
	{
		alert("1");
		orientation.startWatch({interval:1000});
		alert("2");
	}
	else
	{
		orientation.endWatch();
		updateOrientDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var gyroscope = new SensorConnection("Gyroscope");

gyroscope.addEventListener("onsensordata", updateGyroDataW3C);

function updateGyroDataW3C(sensorData){
	document.getElementById('GyroscopeW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('GyroscopeW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('GyroscopeW3Cz').innerHTML = sensorData.data.z;
}

function toggleGyroscopeW3C()
{
	alert(typeof gyroscope);
	if(gyroscope.status == "open")
	{
		gyroscope.startWatch({interval:1000});
	}
	else
	{
		gyroscope.endWatch();
		updateGyroDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

var proximity = new SensorConnection("Proximity");

proximity.addEventListener("onsensordata", updateProxDataW3C);

function updateProxDataW3C(sensorData){
	document.getElementById('ProximityW3C').innerHTML = sensorData.data.x;
}

function toggleProximityW3C()
{
	alert(typeof proximity);
	if(proximity.status == "open")
	{
		proximity.startWatch({interval:1000});
	}
	else
	{
		proximity.endWatch();
		updateProxDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}