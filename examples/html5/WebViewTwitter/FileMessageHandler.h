/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
