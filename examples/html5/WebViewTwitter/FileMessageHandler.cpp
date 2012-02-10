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
 * @file FileMessageHandler.cpp
 * @author Mikael Kindborg
 *
 * Implementation of system calls made from JavaScript.
 */

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
bool FileMessageHandler::handleMessage(MessageStreamJSON& message)
{
	if (message.is("mosync.file.getLocalPath"))
	{
		handleFileGetLocalPath(message);
	}
	else if (message.is("mosync.file.read"))
	{
		handleFileRead(message);
	}
	else if (message.is("mosync.file.write"))
	{
		handleFileWrite(message);
	}
	else if (message.is("mosync.log"))
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
	MessageStreamJSON& message)
{
	FileUtil fileUtil;
	String path = fileUtil.getLocalPath();
	if (0 == path.length())
	{
		replyNull(message);
	}
	else
	{
		replyString(message, path);
	}
}

/**
 * Handle the file read message.
 */
void FileMessageHandler::handleFileRead(MessageStreamJSON& message)
{
	FileUtil fileUtil;
	String inText;
	bool success = fileUtil.readTextFromFile(
		message.getParam("filePath"),
		inText);
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
void FileMessageHandler::handleFileWrite(MessageStreamJSON& message)
{
	maWriteLog("@@@ 1", 4);
	FileUtil fileUtil;
	bool success = fileUtil.writeTextToFile(
		message.getParam("filePath"),
		message.getParam("data"));
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
void FileMessageHandler::handleLog(MessageStreamJSON& message)
{
	String s = message.getParam("message");
	maWriteLog(s.c_str(), s.size());
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * using the mosync.bridge.sendJSON function.
 * @param result A string that contains the data to be returned
 * to the JavaScript callback function.
 * @return true on success, false on error.
 */
bool FileMessageHandler::replyString(
	MessageStreamJSON& message,
	const String& result)
{
	// Message must have an callbackId parameter.
	if (hasNoCallbackId(message))
	{
		return false;
	}

	// Get the callbackID parameter.
	String callbackId = getCallBackId(message);

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId + ", " + "'" + result + "')";

	maWriteLog("P1", 2);
	maWriteLog(script.c_str(), script.size());

	message.getWebView()->callJS(script);

	return true;
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * using the mosync.bridge.sendJSON function.
 * @param result A boolean to be returned
 * to the JavaScript callback function.
 * @return true on success, false on error.
 */
bool FileMessageHandler::replyBoolean(MessageStreamJSON& message, bool result)
{
	// Message must have an callbackId parameter.
	if (hasNoCallbackId(message))
	{
		return false;
	}

	// Get the callbackID parameter.
	String callbackId = getCallBackId(message);

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	if (result)
	{
		script += callbackId + ", true)";
	}
	else
	{
		script += callbackId + ", false)";
	}

	maWriteLog("P2", 2);
	maWriteLog(script.c_str(), script.size());

	message.getWebView()->callJS(script);

	return true;
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. Returns null to the callback function.
 * The callbackId is supplied automatically when
 * using the mosync.bridge.sendJSON function.
 * @return true on success, false on error.
 */
bool FileMessageHandler::replyNull(MessageStreamJSON& message)
{
	// Message must have an callbackId parameter.
	if (hasNoCallbackId(message))
	{
		return false;
	}

	// Get the callbackID parameter.
	String callbackId = getCallBackId(message);

	// Call JavaScript reply handler.
	String script = "mosync.bridge.reply(";
	script += callbackId + ", null)";

	maWriteLog("P3", 2);
	maWriteLog(script.c_str(), script.size());

	message.getWebView()->callJS(script);

	return true;
}

/**
 * Utility method to check if there is a callbackId.
 */
bool FileMessageHandler::hasNoCallbackId(MessageStreamJSON& message)
{
	return !message.hasParam("callbackId");
}

/**
 * Utility method for getting the message callbackId as a string.
 */
String FileMessageHandler::getCallBackId(MessageStreamJSON& message)
{
	int id = message.getParamInt("callbackId");
	char buf[32];
	sprintf(buf, "%i", id);
	return buf;
}
