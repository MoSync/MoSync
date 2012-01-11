
navigator.findSensors = function(type)
{
	return new SensorRequest(type)

    return sensorRequest;
};

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
    PhoneGap.callbacks[callbackId] = {success:	function(sensorList)
        											{
        												self.result = sensorList.result;
        												self.readyState = "done";
        												for(var i = 0; i < self.events.onsuccess.length; i++)
        												{
        													self.events.onsuccess[i](event);
        												}
        											},
        									fail:	function()
        											{
        												self.readyState = "done";
        												for(var i = 0; i < self.events.onerror.length; i++)
        												{
        													self.events.onerror[i](event);
        												}
        											}
        								};

    bridge.PhoneGap.send(callbackId, "SensorManager", "findSensors","{\"type\":\"" + type + "\"}");
	this.events = {
			"onsuccess": [],
			"onerror": [],
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
}

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

	this.startWatch = 	function(watchOptions)
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
							bridge.PhoneGap.send(callbackId,
												"SensorManager",
												"startSensor",
												"{\"type\":\"" + self.type + "\", \"interval\":" + watchOptions.interval + "}");
						};

	this.endWatch = 	function()
						{
							if(self.status != "watching")
							{
								var exception = new DOMException();
								exception.code = DOMException.INVALID_STATE_ERR;
								throw exception;
								return;
							}
							this.setStatus("open");
							bridge.PhoneGap.send(null, "SensorManager", "stopSensor","{\"type\":\"" + self.type + "\"}");
						};

	this.read = 		function()
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
							bridge.PhoneGap.send(callbackId,
									"SensorManager",
									"startSensor",
									"{\"type\":\"" + self.type + "\", \"interval\":-1}");
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
						reason:sensorData.reason
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