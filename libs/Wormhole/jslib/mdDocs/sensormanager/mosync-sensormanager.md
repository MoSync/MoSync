Sensor Manager
==============

> A W3C compliant sensor API

This is an implementation of the W3C specifications for device sensor management, as found in [W3C Sensor API Specification](http://dev.w3.org/2009/dap/system-info/Sensors.html).

Functions
----------
 * navigator.findSensors

Objects
---------
 * SensorRequest

Methods
---------
 * SensorRequest.addEventListener(event, listener)
 * SensorRequest.removeEventListener(event, listener)

 * SensorConnection.startWatch()
 * SensorConnection.endWatch()
 * SensorConnection.read()
 * SensorRequest.addEventListener(event, listener)
 * SensorRequest.removeEventListener(event, listener)

Data Structs
-------------
 * sensorData
 * sensorError


navigator.findSensors
=====================
Returns an object that manages device sensor enumeration

Parameters
----------

 * __type__ (optional) The type of sensor to look for. Null for every sensor.


Usage
-----
 
    		var sensorReq = navigator.findSensors();
    		sensorReq.addEventListener('onsuccess', function() {
    			for(var count = 0; count < this.result.length; count++) {
    				window.console.log("Discovered: " + this.result[count].name);
	    		}
		    });
		
SensorRequest
==============
The SensorRequest object handles the device sensor enumeration

SensorRequest(type)
-------------------
* __type__ (optional) The sensor type. If specified, only that kind of sensor will be queried.
It can be one of the following: 
    <!-- We need to break the link to the other Accelerometer by adding an HTML tag in the middle --> 
	* "Accele<strong/>rometer"  
	* "MagneticField"  
	* "Orientation"  
	* "Gyroscope"  
	* "Proximity"  

Fields
----------
 * __result__ 	A Sensor array with the sensors that were discovered in the system.
				It should be read only after the 'onsuccess' event has fired.
 * __readyState__ A string describing the state of the request. Can be either "processing" or "done"
 
Events
---------
 * __onsuccess__ Called when sensor enumeration has finished, with parameter "result"
 * __result__ A Sensor array with the sensors that were discovered in the system.

SensorRequest.addEventListener(event, listener)
-----------------------------------------------
Registers a callback to an event
	
Parameters
--------------
 * __event__ The name of the event
 * __listener__ The callback function

SensorRequest.removeEventListener(event, listener)
-----------------------------------------------
Unregisters a callback from an event
	
Parameters
--------------
 * __event__ The name of the event
 * __listener__ The callback function

SensorConnection
=================
This object represents a connection to a sensor

Constructor
------------
 * __options__ (Object or string) The sensor to connect to

Events
-------
 * __onerror__ Called when there is an error
 * __onstatuschange__ Called when the status of the connection has changed

Usage
-------

		//Initialization of W3C Accelerometer sensor
		var accelerometer = new SensorConnection("Accelerometer");

		accelerometer.addEventListener("onsensordata", updateAccelerometer);

		function updateAccelerometer(sensorData){
			window.console.log("X:" + sensorData.data.x);
			window.console.log("Y:" + sensorData.data.y);
			window.console.log("Z:" + sensorData.data.z);
		}

		function toggleAccelerometer()
		{
			if(accelerometer.status == "open")
			{
				accelerometer.startWatch({interval:1000});
			}
			else
			{
				accelerometer.endWatch();
			}
		}

SensorConnection.startWatch()
==================
Starts the periodic sampling of the sensor

SensorConnection.endWatch()
==========================
Stops the sampling process

SensorConnection.read()
==========================
Initiates a single sampling of the sensor data

Events
------
* __onsensordata__ Called when there is new data from the sensor, with a sensorData parameter.

* __onerror__ Called when there is an error with a sensorError parameter. 

* __onstatuschange__ Called when the status of the sensor changes with a string describing the new status.


SensorRequest.addEventListener(event, listener)
=============================
Registers a callback to an event
	
Parameters
--------------
* __event__ The name of the event
* __listener__ The callback function

SensorRequest.removeEventListener(event, listener)
=============================
Unregisters a callback from an event
	
Parameters
--------------
* __event__ The name of the event
* __listener__ The callback function


sensorData
==========
Struct containing sampling information from the sensor. It is passed to the call back function of onsensordata event.

Members
-------
* __data__ A (x,y,z) vector with the sensor reading. 
* __timestamp__ Time of sampling
* __reason__ "read" or "watch"

sensorError
===========
Struct containing the error information. It is passed to the call back function of onerror event.

Members
-------
* __sensorError.message__ A string describing the error
* __sensorError.code__ The error code
