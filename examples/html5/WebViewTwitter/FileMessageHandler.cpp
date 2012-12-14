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
 * @file FileMessageHandler.cpp
 * @author Mikael Kindborg
 *
 * Implementation of system calls made from JavaScript.
 */

#include <conprint.h>
#include <Wormhole/FileUtil.h>
#include "FileMessageHandler.h"

using namespace MAUtil;
using namespace Wormhole;

/**
 * Constructor.
 */
FileMessageHandler::FileMessageHandler()
{
}

/**
 * Destructor.
 */
FileMessageHandler::~FileMessageHandler()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Implementation of standard API exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
bool FileMessageHandler::handleMessage(Wormhole::MessageStream& message)
{
	const char* operation = message.getNext();

	if (0 == strcmp(operation, "mosync.file.getLocalPath"))
	{
		handleFileGetLocalPath(message);
	}
	else if (0 == strcmp(operation, "mosync.file.read"))
	{
		handleFileRead(message);
	}
	else if (0 == strcmp(operation, "mosync.file.write"))
	{
		handleFileWrite(message);
	}
	else if (0 == strcmp(operation, "mosync.log"))
	{
		handleLog(message);
	}
	else
	{
		// Message was not handled.
		return false;
	}

	// Message was handled.
	return true;
}

/**
 * Handle the getLocalPath message.
 */
void FileMessageHandler::handleFileGetLocalPath(
	Wormhole::MessageStream& message)
{
	FileUtil fileUtil;
	String path = fileUtil.getLocalPath();
	replyString(message, path);
}

/**
 * Handle the file read message.
 */
void FileMessageHandler::handleFileRead(Wormhole::MessageStream& message)
{
	FileUtil fileUtil;
	const char* filePath = message.getNext();
	String inText;
	bool success = fileUtil.readTextFromFile(filePath, inText);
	if (success)
	{
		replyString(message, inText);
	}
	else
	{
		replyNull(message);
	}
}

/**
 * Handle the file write message.
 */
void FileMessageHandler::handleFileWrite(Wormhole::MessageStream& message)
{
	FileUtil fileUtil;
	const char* filePath = message.getNext();
	const char* data = message.getNext();
	bool success = fileUtil.writeTextToFile(filePath, data);
	if (success)
	{
		replyBoolean(message, true);
	}
	else
	{
		replyString(message, false);
	}
}

/**
 * Handle the log message.
 */
void FileMessageHandler::handleLog(Wormhole::MessageStream& message)
{
	const char* s = message.getNext();
	maWriteLog(s, strlen(s));
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * calling mosync.bridge.send wuth a callback function.
 * @param message The message stream from which to get the
 * callback id.
 * @param result A string that contains the data to be returned
 * to the JavaScript callback function.
 */
void FileMessageHandler::replyString(
	Wormhole::MessageStream& message,
	const String& result)
{
	// Get the callbackID parameter.
	const char* callbackId = message.getNext();

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId;
	script += ", '" + result + "')";

	message.callJS(script);
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * calling mosync.bridge.send wuth a callback function.
 * @param message The message stream from which to get the
 * callback id.
 * @param result A boolean to be returned
 * to the JavaScript callback function.
 */
void FileMessageHandler::replyBoolean(Wormhole::MessageStream& message, bool result)
{
	// Get the callbackID parameter.
	const char* callbackId = message.getNext();

	// Set truth value.
	String truth = result ? "true" : "false";

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId;
	script += "," + truth + ")";

	message.callJS(script);
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * calling mosync.bridge.send wuth a callback function.
 * Returns null to the callback function.
 * @param message The message stream from which to get the
 * callback id.
 */
void FileMessageHandler::replyNull(Wormhole::MessageStream& message)
{
	// Get the callbackID parameter.
	const char* callbackId = message.getNext();

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId;
	script += ", null)";

	message.callJS(script);
}
