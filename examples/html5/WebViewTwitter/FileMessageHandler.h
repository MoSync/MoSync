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
 * @file FileMessageHandler.h
 * @author Mikael Kindborg
 *
 * Implementation of file calls made from JavaScript.
 */

#ifndef FILE_MESSAGE_HANDLER_H_
#define FILE_MESSAGE_HANDLER_H_

#include <Wormhole/MessageStream.h>

/**
 * Class that implements JavaScript calls.
 *
 * The JavaScript side is in file mosync-file.js.
 */
class FileMessageHandler
{
public:
	/**
	 * Constructor.
	 */
	FileMessageHandler();

	/**
	 * Destructor.
	 */
	virtual ~FileMessageHandler();

	/**
	 * Implementation of standard API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	bool handleMessage(Wormhole::MessageStream& message);

protected:
	/**
	 * Handle the getLocalPath message.
	 */
	void handleFileGetLocalPath(Wormhole::MessageStream& message);

	/**
	 * Handle the file read message.
	 */
	void handleFileRead(Wormhole::MessageStream& message);

	/**
	 * Handle the file write message.
	 */
	void handleFileWrite(Wormhole::MessageStream& message);

	/**
	 * Handle the log message.
	 */
	void handleLog(Wormhole::MessageStream& message);

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. The callbackId is supplied automatically when
	 * calling mosync.bridge.send wuth a callback function.
	 * @param message The message stream from which to get the
	 * callback id.
	 * @param result A string that contains the data to be returned
	 * to the JavaScript callback function.
	 */
	void replyString(
		Wormhole::MessageStream& message,
		const MAUtil::String& result);

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. The callbackId is supplied automatically when
	 * calling mosync.bridge.send wuth a callback function.
	 * @param message The message stream from which to get the
	 * callback id.
	 * @param result A boolean to be returned
	 * to the JavaScript callback function.
	 */
	void replyBoolean(Wormhole::MessageStream& message, bool result);

	/**
	 * Calls a JavaScript callback function using the "callbackId"
	 * parameter. The callbackId is supplied automatically when
	 * calling mosync.bridge.send wuth a callback function.
	 * Returns null to the callback function.
	 * @param message The message stream from which to get the
	 * callback id.
	 */
	void replyNull(Wormhole::MessageStream& message);
};

#endif
