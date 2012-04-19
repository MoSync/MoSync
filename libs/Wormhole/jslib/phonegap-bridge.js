/*
Copyright (C) 2011 MoSync AB

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
 * @file phonegap-bridge.js
 * @author Ali Sarrafi
 *
 * Glue between PhoneGap and the MoSync Wormhole bridge.
 */

mosync.bridge.PhoneGap = {};

// TODO: Is this needed? Remove?
//mosync.bridge.PhoneGap.CallBackTable = {};

/**
 * sends a message through bridge as a PhoneGap message
 *
 * @param callbackId ID of the PhoneGap Callback to be used
 * @param service name of the PhoneGap Service
 * @param action action name for the specified service
 * @param args extra arguments as JSON
 */
mosync.bridge.PhoneGap.send = function(callbackId, service, action, args)
{
	//Generate a MoSync Message from the phonegap command
	var command = {
		"messageName": "PhoneGap",
		"service": service,
		"action": action,
		"args": args,
		"PhoneGapCallBackId": callbackId
	};

    // Call into Mosync C++ through bridge library.
	mosync.bridge.sendJSON(command, null);
};

/**
 * Add support for location services if the platform does not support it by default
 */
if(navigator.geolocation == undefined)
{
	navigator.geolocation = {};

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
	    mosync.bridge.PhoneGap.send(callbackId, "GeoLocation", "watchPosition");
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
	    mosync.bridge.PhoneGap.send(callbackId, "GeoLocation", "getCurrentPosition");
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
	    mosync.bridge.PhoneGap.send(callbackId, "GeoLocation", "clearWatch");
	};

}
