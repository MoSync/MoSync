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
 * @file MessageHandler.h
 * @author Mikael Kindborg
 *
 * Implementation of system calls made from JavaScript.
 */

#ifndef WEB_VIEW_MESSAGE_HANDLER_H_
#define WEB_VIEW_MESSAGE_HANDLER_H_

#include <Wormhole/WebViewMessage.h>

/**
 * Class that implements JavaScript calls.
 *
 * The JavaScript side is in file extendedbridge.js.
 */
class MessageHandler
{
public:
	/**
	 * Constructor.
	 */
	MessageHandler();

	/**
	 * Destructor.
	 */
	virtual ~MessageHandler();

	/**
	 * Implementation of standard API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	bool handleMessage(Wormhole::WebViewMessage& message);

protected:
	/**
	 * Handle the getLocalPath message.
	 */
	void handleFileGetLocalPath(Wormhole::WebViewMessage& message);

	/**
	 * Handle the file read message.
	 */
	void handleFileRead(Wormhole::WebViewMessage& message);

	/**
	 * Handle the file write message.
	 */
	void handleFileWrite(Wormhole::WebViewMessage& message);

	/**
	 * Handle the log message.
	 */
	void handleLog(Wormhole::WebViewMessage& message);

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. The callbackId is supplied automatically when
	 * using the bridge.messagehandler.send function.
	 * @param result A string that contains the data to be returned
	 * to the JavaScript callback function.
	 * @return true on success, false on error.
	 */
	bool replyString(
		Wormhole::WebViewMessage& message,
		const MAUtil::String& result);

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. The callbackId is supplied automatically when
	 * using the bridge.messagehandler.send function.
	 * @param result A boolean to be returned
	 * to the JavaScript callback function.
	 * @return true on success, false on error.
	 */
	bool replyBoolean(Wormhole::WebViewMessage& message, bool result);

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. Returns null to the callback function.
	 * The callbackId is supplied automatically when
	 * using the bridge.messagehandler.send function.
	 * @return true on success, false on error.
	 */
	bool replyNull(Wormhole::WebViewMessage& message);

};

#endif
