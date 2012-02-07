//Initialization of W3C Accelerometer sensor
var accelerometer = new SensorConnection("Accelerometer");

accelerometer.addEventListener("onsensordata", updateAccelW3C);

/**
 * Callback that updates the W3C Accelerometer fields
 * @param sensorData the latest sampled vector from the accelerometer sensor
 */
function updateAccelW3C(sensorData){
	document.getElementById('accelW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('accelW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('accelW3Cz').innerHTML = sensorData.data.z;
}

/**
 * This function changes the state of the W3C accelerometer
 * sensor when it's called
 */
function toggleAccelW3C()
{
	if(accelerometer.status == "open")
	{
		accelerometer.startWatch();
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

//Initialization of W3C Magnetic field sensor
var magneticField = new SensorConnection("MagneticField");

magneticField.addEventListener("onsensordata", updateMagDataW3C);

/**
 * Callback that updates the W3C Magnetic fields
 * @param sensorData the latest sampled vector from the magnetic field sensor
 */
function updateMagDataW3C(sensorData){
	document.getElementById('MagFieldW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('MagFieldW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('MagFieldW3Cz').innerHTML = sensorData.data.z;
}

/**
 * This function changes the state of the W3C magnetic field
 * sensor when it's called
 */
function toggleMagFieldW3C()
{
	if(magneticField.status == "open")
	{
		magneticField.startWatch();
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

//Initialization of W3C Orientation sensor
var orientationSensor = new SensorConnection("Orientation");

orientationSensor.addEventListener("onsensordata", updateOrientDataW3C);

/**
 * Callback that updates the W3C Orientation fields
 * @param sensorData the current orientation of the device
 */
function updateOrientDataW3C(sensorData){
	document.getElementById('OrientationW3C').innerHTML = sensorData.data.x;
}

/**
 * This function changes the state of the W3C orientation
 * sensor when it's called
 */
function toggleOrientationW3C()
{
	if(orientationSensor.status == "open")
	{
		orientationSensor.startWatch();
	}
	else
	{
		orientationSensor.endWatch();
		updateOrientDataW3C({data:{
			x: "&nbsp;",
			y: "&nbsp;",
			z: "&nbsp;"
		}});
	}
}

//Initialization of W3C Gyroscope
var gyroscope = new SensorConnection("Gyroscope");

gyroscope.addEventListener("onsensordata", updateGyroDataW3C);

/**
 * Callback that updates the W3C Gyroscope fields
 * @param sensorData the latest sampled vector from the gyroscope
 */
function updateGyroDataW3C(sensorData){
	document.getElementById('GyroscopeW3Cx').innerHTML = sensorData.data.x;
	document.getElementById('GyroscopeW3Cy').innerHTML = sensorData.data.y;
	document.getElementById('GyroscopeW3Cz').innerHTML = sensorData.data.z;
}

/**
 * This function changes the state of the W3C gyroscope when it's called
 */
function toggleGyroscopeW3C()
{
	if(gyroscope.status == "open")
	{
		gyroscope.startWatch();
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

//Initialization of W3C Proximity sensor
var proximity = new SensorConnection("Proximity");

proximity.addEventListener("onsensordata", updateProxDataW3C);

/**
 * Callback that updates the W3C Proximity field
 * @param sensorData the binary reading of the proximity sensor
 */
function updateProxDataW3C(sensorData){
	document.getElementById('ProximityW3C').innerHTML = sensorData.data.x;
}

/**
 * This function changes the state of the W3C proximity sensor
 * when it's called
 */
function toggleProximityW3C()
{
	if(proximity.status == "open")
	{
		proximity.startWatch();
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