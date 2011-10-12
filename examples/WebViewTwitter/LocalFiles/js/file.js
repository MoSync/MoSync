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
 * @file file.js
 * @author Mikael Kindborg
 *
 * This file extends the bridge library wil functions
 * for file access.
 */

/**
 * The file submodule.
 */
bridge.file = {};

/**
 * Get the path to the local storage area on the device.
 * The path is returned asynchronously to the callback function.
 */
bridge.file.getLocalPath = function(callbackFun)
{
	bridge.messagehandler.send(
		{ "messageName": "bridge.file.getLocalPath" },
		callbackFun);
};

/**
 * Read the contents of a text file asynchronously.
 */
bridge.file.read = function(filePath, callbackFun)
{
	bridge.messagehandler.send(
		{ "messageName": "bridge.file.read",
		  "filePath": filePath },
		callbackFun);
};

/**
 * Write the contents of a text file asynchronously.
 */
bridge.file.write = function(filePath, data, callbackFun)
{
	bridge.messagehandler.send(
		{ "messageName": "bridge.file.write",
		  "filePath": filePath,
		  "data": data },
		callbackFun);
};

/**
 * Add function for logging to the top-level
 * of the bridge object.
 */
bridge.log = function(message)
{
	bridge.messagehandler.send(
		{ "messageName": "bridge.log",
		  "message": message },
		null);
};
