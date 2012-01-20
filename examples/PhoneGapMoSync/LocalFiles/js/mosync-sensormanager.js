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
 * @file mosync-sensormanager.js
 * @author Iraklis
 *
 * Implementation of W3C sensor API.
 */

/**
 * Returns an object that manages device sensor enumeration
 * @param type (optional) The type of sensor to look for. Null for every sensor
 */
navigator.findSensors = function(type)
{
	return new SensorRequest(type);
};

/**
 * This object handles sensor enumeration
 * @param type (optional) The type of sensor to look for. Null for every sensor
 * @event onsuccess Called when enumeration is finished, with a list of the device sensors
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

	this.events = {
			"onsuccess": [],
	};
	this.addEventListener = function(event, listener, captureMethod)
	{
		if(self.events[event] != undefined)
		{
			self.events[event].push(listener);
		}
	};

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
 * @param options (Object or string) The sensor to connect to
 * @event onsensordata Called when there is new data from the sensor
 * @event onerror Called when there is an error
 * @event onstatuschange Called when the status of the connection has changed
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

	this.startWatch = function(watchOptions)
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
			{"type":"" + self.type, "interval":"" + watchOptions.interval});
	};

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

	this.events = {
		"onsensordata": [],
		"onerror": [],
		"onstatuschange":[]
	};

	this.addEventListener = function(event, listener, captureMethod)
	{
		if(self.events[event] != undefined)
		{
			self.events[event].push(listener);
		}
	};

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