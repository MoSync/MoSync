/*
Copyright (C) 2012 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/


/**
 * Returns an object that manages device sensor enumeration
 * @param type (optional) The type of sensor to look for. Null for every sensor
 /code
	var sensorReq = navigator.findSensors();

	sensorReq.addEventListener('onsuccess', function() {
		for(var count = 0; count < this.result.length; count++) {
			window.console.log("Discovered: " + this.result[count].name);
		}
	});
/endcode
 */
navigator.findSensors = function(type)
{
	return new SensorRequest(type);
};

/**
The SensorRequest object handles the device sensor enumeration
_Constructor_
@param type (optional) The sensor type. If specified, only that kind of sensor will be queried.
It can be one of the following:
	+"Accelerometer"
	+"MagneticField"
	+"Orientation"
	+"Gyroscope"
	+"Proximity"

@field result 	A Sensor array with the sensors that were discovered in the system.
				It should be read only after the 'onsuccess' event has fired.
@field readyState A string describing the state of the request. Can be either "processing" or "done"
 */
function SensorRequest(type)
{
	var self = this;

	this.result = [];
	this.readyState = "processing";
	if(type == undefined)
	{
		type = "";
	}
	this.type = type;
	var callbackId = "SensorManager" + PhoneGap.callbackId++;
	PhoneGap.callbacks[callbackId] = {
		success: function(sensorList)
		{
			self.result = sensorList.result;
			self.readyState = "done";
			for(var i = 0; i < self.events.onsuccess.length; i++)
			{
				self.events.onsuccess[i](self.result);
			}
		}
	};

	/**
		@event onsuccess Called when enumeration has finished
		@param result A Sensor array with the sensors that were discovered in the system.
	*/
	this.events = {
			"onsuccess": []
	};
	/**
		Registers a callback to an event
		@param event The name of the event
		@param listener The callback function
	*/
	this.addEventListener = function(event, listener)
	{
		if(self.events[event] != undefined)
		{
			self.events[event].push(listener);
		}
	};

	/**
		Unregisters a callback from an event
		@param event The name of the event
		@param listener The callback function
	*/
	this.removeEventListener = function(event, listener)
	{
		if(self.events[event] != undefined)
		{
			for(var i = 0; i < self.events[event].length; i++)
			{
				if(self.events[event] == listener)
				{
					self.events[event].splice(i,1);
					return;
				}
			}
		}
	};

	mosync.bridge.PhoneGap.send(
		callbackId,
		"SensorManager",
		"findSensors",
		{"type":"" + type});
}

/**
 * This object represents a connection to a sensor
 _Constructor_
 * @param options (Object or string) The sensor to connect to
 *
 * @event onerror Called when there is an error
 * @event onstatuschange Called when the status of the connection has changed
/code
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
/endcode
 */
function SensorConnection(options)
{
	var self = this;

	if(typeof options == "string")
	{
		this.type = options;
	}
	else if(typeof options.name == "string")
	{
		this.type = options.name;
	}
	else
	{
		this.type = options.type;
	}

	/**
		Starts the periodic sampling of the sensor
	*/
	this.startWatch = function()
	{
		if(self.status != "open")
		{
			var exception = new DOMException();
			exception.code = DOMException.INVALID_STATE_ERR;
			throw exception;
			return;
		}
		this.setStatus("watching");
		var callbackId = "SensorManager" + PhoneGap.callbackId++;
		PhoneGap.callbacks[callbackId] = {
				success:self.sensorEvent,
				fail:self.sensorError
		};
		mosync.bridge.PhoneGap.send(
			callbackId,
			"SensorManager",
			"startSensor",
			{"type":"" + self.type, "interval":0});
	};

	/**
		Stops the sampling process
	*/
	this.endWatch = function()
	{
		if(self.status != "watching")
		{
			var exception = new DOMException();
			exception.code = DOMException.INVALID_STATE_ERR;
			throw exception;
			return;
		}
		this.setStatus("open");
		mosync.bridge.PhoneGap.send(
			null,
			"SensorManager",
			"stopSensor",
			{"type":"" + self.type});
	};

	/**
		Initiates a single sampling of the sensor data
	*/
	this.read = function()
	{
		if(self.status != "open")
		{
			var exception = new DOMException();
			exception.code = DOMException.INVALID_STATE_ERR;
			throw exception;
			return;
		}

		var callbackId = "SensorManager" + PhoneGap.callbackId++;

		PhoneGap.callbacks[callbackId] = {
			success:self.sensorEvent
		};

		mosync.bridge.PhoneGap.send(
			callbackId,
			"SensorManager",
			"startSensor",
			{"type":"" + self.type, "interval":-1});
	};
	/**
	 onsensordata Called when there is new data from the sensor
	 @param sensorData Struct containing sampling information from the sensor
	 @param sensorData.data A (x,y,z) vector with the sensor reading
	 @param sensorData.timestamp Time of sampling
	 @param sensorData.reason "read" or "watch"
	 @endparam
	 onerror Called when there is an error
	 @param sensorError Struct containing the error information
	 @param sensorError.message A string describing the error
	 @param sensorError.code The error code
	 @endparam
	 @event onstatuschange Called when the status of the sensor changes
	 @param status The new status
	 @endparam
	*/
	this.events = {
		"onsensordata": [],
		"onerror": [],
		"onstatuschange":[]
	};

	/**
		Registers a callback to an event
		@param event The name of the event
		@param listener The callback function
	*/
	this.addEventListener = function(event, listener, captureMethod)
	{
		if(self.events[event] != undefined)
		{
			self.events[event].push(listener);
		}
	};

	/**
		Unregisters a callback from an event
		@param event The name of the event
		@param listener The callback function
	*/
	this.removeEventListener = function(event, listener)
	{
		if(self.events[event] != undefined)
		{
			for(var i = 0; i < self.events[event].length; i++)
			{
				if(self.events[event] == listener)
				{
					self.events[event].splice(i,1);
					return;
				}
			}
		}
	};

	this.sensorEvent = function(sensorData)
	{
		var event = {
			data: {
				x: sensorData.x,
				y: sensorData.y,
				z: sensorData.z
			},
			accuracy: "high",
			timestamp: sensorData.timestamp,
			reason: sensorData.reason
		};

		for(var i = 0; i < self.events.onsensordata.length; i++)
		{
			self.events.onsensordata[i](event);
		}
	};

	this.sensorError = function(errorData)
	{
		this.setStatus("error");
		var sensorError = {
			code: errorData.code,
			message: errorData.message
		};
		self.error = sensorError;
		for(var i = 0; i < self.events.onerror.length; i++)
		{
			self.events.onerror[i](sensorError);
		}
	};

	this.setStatus = function(status)
	{
		if(status != self.status)
		{
			self.status = status;
			for(var i = 0; i < self.events.onstatuschange.length; i++)
			{
				self.events.onstatuschange[i](status);
			}
		}
	};

	this.setStatus("open");
}
