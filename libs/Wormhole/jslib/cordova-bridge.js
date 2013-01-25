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

mosync.bridge.PhoneGap = {};

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
