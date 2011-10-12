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
 * @file MessageHandler.cpp
 * @author Mikael Kindborg
 *
 * Implementation of system calls made from JavaScript.
 */

#include <Wormhole/FileUtil.h>
#include "MessageHandler.h"

using namespace Wormhole;

/**
 * Constructor.
 */
MessageHandler::MessageHandler()
{
}

/**
 * Destructor.
 */
MessageHandler::~MessageHandler()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Implementation of standard API exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
bool MessageHandler::handleMessage(WebViewMessage& message)
{
	if (message.is("bridge.file.getLocalPath"))
	{
		handleFileGetLocalPath(message);
	}
	else if (message.is("bridge.file.read"))
	{
		handleFileRead(message);
	}
	else if (message.is("bridge.file.write"))
	{
		handleFileWrite(message);
	}
	else if (message.is("bridge.log"))
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
void MessageHandler::handleFileGetLocalPath(
	WebViewMessage& message)
{
	FileUtil fileUtil;
	MAUtil::String path = fileUtil.getLocalPath();
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
void MessageHandler::handleFileRead(WebViewMessage& message)
{
	FileUtil fileUtil;
	MAUtil::String inText;
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
void MessageHandler::handleFileWrite(WebViewMessage& message)
{
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
void MessageHandler::handleLog(WebViewMessage& message)
{
	MAUtil::String s = message.getParam("message");
	maWriteLog(s.c_str(), s.length());
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * using the bridge.messagehandler.send function.
 * @param result A string that contains the data to be returned
 * to the JavaScript callback function.
 * @return true on success, false on error.
 */
bool MessageHandler::replyString(
	WebViewMessage& message,
	const MAUtil::String& result)
{
	// Message must have an callbackId parameter.
	if (!message.hasParam("callbackId"))
	{
		return false;
	}

	// Get the callbackID parameter.
	MAUtil::String callbackId = message.getParam("callbackId");

	// Call JavaScript reply handler.
	MAUtil::String script = "bridge.messagehandler.reply(";
	script += callbackId + ", " + "'" + result + "')";
	message.getWebView()->callJS(script);

	return true;
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. The callbackId is supplied automatically when
 * using the bridge.messagehandler.send function.
 * @param result A boolean to be returned
 * to the JavaScript callback function.
 * @return true on success, false on error.
 */
bool MessageHandler::replyBoolean(WebViewMessage& message, bool result)
{
	// Message must have an callbackId parameter.
	if (!message.hasParam("callbackId"))
	{
		return false;
	}

	// Get the callbackID parameter.
	MAUtil::String callbackId = message.getParam("callbackId");

	// Call JavaScript reply handler.
	MAUtil::String script = "bridge.messagehandler.reply(";
	if (result)
	{
		script += callbackId + ", true)";
	}
	else
	{
		script += callbackId + ", false)";
	}
	message.getWebView()->callJS(script);

	return true;
}

/**
 * Calls a JavaScript callback function using the "callbackId"
 * parameter. Returns null to the callback function.
 * The callbackId is supplied automatically when
 * using the bridge.messagehandler.send function.
 * @return true on success, false on error.
 */
bool MessageHandler::replyNull(WebViewMessage& message)
{
	// Message must have an callbackId parameter.
	if (!message.hasParam("callbackId"))
	{
		return false;
	}

	// Get the callbackID parameter.
	MAUtil::String callbackId = message.getParam("callbackId");

	// Call JavaScript reply handler.
	MAUtil::String script = "bridge.messagehandler.reply(";
	script += callbackId + ", null)";
	message.getWebView()->callJS(script);

	return true;
}
