bridge.PhoneGap = {};

bridge.PhoneGap.CallBackTable = {};

/**
 * A replica of the PhoneGap.exec function that translates phongap
 * commands into Mosync commands and then calls send to send the message
 *
 * @param callbackId ID of the PhoneGap Callback to be used
 * @param service name of the PhoneGap Service
 * @param action action name for the specified service
 * @param args extra arguments
 */
bridge.PhoneGap.exec = function(callbackId, service, action, args)
{
	// We need to use JSON format for this option because of its structure.
	bridge.PhoneGap.send(
		callbackId,
		service,
		action,
		JSON.stringify(args));
};

/**
 * sends a message through bridge as a PhoneGap message
 *
 * @param callbackId ID of the PhoneGap Callback to be used
 * @param service name of the PhoneGap Service
 * @param action action name for the specified service
 * @param argsJSON extra arguments as an array - always JSON
 */
bridge.PhoneGap.send = function(callbackId, service, action, argsJSON)
{
	//Generate a MoSync Message from the phonegap command
	var command = {
		"messageName": "PhoneGap",
		"service": service,
		"action": action,
		"args": argsJSON,
		"PhoneGapCallBackId": callbackId,
	};

    // Call into Mosync C++ through bridge library.
	bridge.messagehandler.send(command, null);
};

/**
 * Starts watching the phone position and listening to the GPS events
 * Overrides the original geolocation api
 * to use MoSync Location API.
 *
 * @param success the callback function for returning the success result
 * @param fail callback function for failure
 */
navigator.geolocation.watchPosition = function(success, fail)
{
	var callbackId = "GeoLocation" + PhoneGap.callbackId++;
    if (typeof success == "function" || typeof fail == "function")
	{
        PhoneGap.callbacks[callbackId] = {success:success, fail:fail};
    }
    bridge.PhoneGap.send(callbackId, "GeoLocation", "watchPosition");
};

/**
 * Gets the current GPS position once
 * Overrides the original geolocation api
 * to use MoSync Location API.
 *
 * @param success the callback function for returning the success result
 * @param fail callback function for failure
 */
navigator.geolocation.getCurrentPosition = function(success, fail)
{
	var callbackId = "GeoLocation" + PhoneGap.callbackId++;
    if (typeof success == "function" || typeof fail == "function")
	{
        PhoneGap.callbacks[callbackId] = {success:success, fail:fail};
    }
    bridge.PhoneGap.send(callbackId, "GeoLocation", "getCurrentPosition");
};

/**
 * Stops watching the phone position and listening to the GPS events
 * Overrides the original geolocation api
 * to use MoSync Location API.
 *
 * @param success the callback function for returning the success result
 * @param fail callback function for failure
 */
navigator.geolocation.clearWatch = function(success, fail)
{
	var callbackId = "GeoLocation" + PhoneGap.callbackId++;
    if (typeof success == "function" || typeof fail == "function")
	{
        PhoneGap.callbacks[callbackId] = {success:success, fail:fail};
    }
    bridge.PhoneGap.send(callbackId, "GeoLocation", "clearWatch");
};