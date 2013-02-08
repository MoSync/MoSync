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
 * @file cordova-bridge.js
 * @author Eric Svensson
 *
 * Glue between Cordova and the MoSync Wormhole bridge.
 */

mosync.bridge.cordova = {};

/**
 * We override the JSON.parse function because it does not work properly on all phones.
 */
JSON.parse = function(JSONDocument)
{
	try{
		var parsedObject = eval('(' + JSONDocument + ')');
	}catch (e) { console.error('JSON parsing failed: ' + e); }
	return parsedObject;
};

/**
 * sends a message through bridge as a Cordova message
 *
 * @param callbackId ID of the Cordova Callback to be used
 * @param service name of the Cordova Service
 * @param action action name for the specified service
 * @param args extra arguments as JSON
 */
mosync.bridge.cordova.send = function(callbackId, service, action, args)
{
	//Generate a MoSync Message from the Cordova command
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

mosync.bridge.PhoneGap = mosync.bridge.cordova;

var PhoneGap = window.cordova;
