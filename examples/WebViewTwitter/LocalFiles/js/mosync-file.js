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
 * @file mosync-file.js
 * @author Mikael Kindborg
 *
 * Very simple library for file access. Extends the
 * mosync object with new functions.
 */

/**
 * The file submodule.
 */
mosync.file = {};

/**
 * Get the path to the local storage area on the device.
 * The path is returned asynchronously to the callback function.
 */
mosync.file.getLocalPath = function(callbackFun)
{
	mosync.bridge.sendJSON(
		{ "messageName": "mosync.file.getLocalPath" },
		callbackFun);
};

/**
 * Read the contents of a text file asynchronously.
 */
mosync.file.read = function(filePath, callbackFun)
{
	mosync.bridge.sendJSON(
		{ "messageName": "mosync.file.read",
		  "filePath": filePath },
		callbackFun);
};

/**
 * Write the contents of a text file asynchronously.
 */
mosync.file.write = function(filePath, data, callbackFun)
{
	mosync.bridge.sendJSON(
		{ "messageName": "mosync.file.write",
		  "filePath": filePath,
		  "data": data },
		callbackFun);
};

/**
 * Add function for logging to the top-level
 * of the bridge object.
 */
mosync.log = function(message)
{
	mosync.bridge.sendJSON(
		{ "messageName": "bridge.log",
		  "message": message },
		null);
};
