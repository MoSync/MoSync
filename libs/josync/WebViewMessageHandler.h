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
 * @file WebViewMessageHandler.h
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


#ifndef JOSYNC_WEB_VIEW_MESSAGE_HANDLER_H_
#define JOSYNC_WEB_VIEW_MESSAGE_HANDLER_H_

#include "WebViewMessage.h"

namespace josync
{

class WebViewMessageHandler
{
public:
	/**
	 * Constructor.
	 */
	WebViewMessageHandler::WebViewMessageHandler();

	/**
	 * Destructor.
	 */
	virtual WebViewMessageHandler::~WebViewMessageHandler();

	/**
	 * Implementation of standard API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	bool WebViewMessageHandler::handleMessage(WebViewMessage& message);

protected:

	void handleFileGetLocalPath(WebViewMessage& message);

	void handleFileRead(WebViewMessage& message);

	void handleFileWrite(WebViewMessage& message);

	void handleLog(WebViewMessage& message);
};

} // namespace NativeUI

#endif // NATIVEUI_WEB_VIEW_MESSAGE_H_
