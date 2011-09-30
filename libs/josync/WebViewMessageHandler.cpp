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
 * @file WebViewMessageHandler.cpp
 * @author Mikael Kindborg
 *
 * Class that implements standard josync JavScript API for MoSync.
 *
 * The JavaScript side of the API is in file josync.js.
 *
 * TODO: This is work in progress, more APIs will be added.
 *
 * You can add your own API as needed by subclassing this
 * class and set the WebAppMoblet to use it with method
 * setWebViewMessageHandler.
 */

#include "FileUtil.h"
#include "WebViewMessageHandler.h"

namespace josync
{
	/**
	 * Constructor.
	 */
	WebViewMessageHandler::WebViewMessageHandler()
	{
	}

	/**
	 * Destructor.
	 */
	WebViewMessageHandler::~WebViewMessageHandler()
	{
		// Nothing needs to be explicitly destroyed.
	}

	/**
	 * Implementation of standard API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	bool WebViewMessageHandler::handleMessage(WebViewMessage& message)
	{
		if (message.is("josync.file.getLocalPath"))
		{
			handleFileGetLocalPath(message);
		}
		else if (message.is("josync.file.read"))
		{
			handleFileRead(message);
		}
		else if (message.is("josync.file.write"))
		{
			handleFileWrite(message);
		}
		else if (message.is("josync.log"))
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

	void WebViewMessageHandler::handleFileGetLocalPath(
		WebViewMessage& message)
	{
		FileUtil fileUtil;
		MAUtil::String path = fileUtil.getLocalPath();
		if (0 == path.length())
		{
			message.replyNull();
		}
		else
		{
			message.replyString(path);
		}
	}

	void WebViewMessageHandler::handleFileRead(WebViewMessage& message)
	{
		FileUtil fileUtil;
		MAUtil::String inText;
		bool success = fileUtil.readTextFromFile(
			message.getParam("filePath"),
			inText);
		if (success)
		{
			message.replyString(inText);
		}
		else
		{
			message.replyNull();
		}
	}

	void WebViewMessageHandler::handleFileWrite(WebViewMessage& message)
	{
		FileUtil fileUtil;
		bool success = fileUtil.writeTextToFile(
			message.getParam("filePath"),
			message.getParam("data"));
		if (success)
		{
			message.replyBoolean(true);
		}
		else
		{
			message.replyString(false);
		}
	}

	void WebViewMessageHandler::handleLog(WebViewMessage& message)
	{
		MAUtil::String s = message.getParam("message");
		maWriteLog(s.c_str(), s.length());
	}
} // namespace
