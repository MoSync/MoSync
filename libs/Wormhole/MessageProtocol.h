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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file MessageProtocol.h
 * @author Mikael Kindborg
 *
 * @brief Class for reading the protocol of a message from a WebView.
 */

#ifndef WORMHOLE_MESSAGE_PROTOCOL_H_
#define WORMHOLE_MESSAGE_PROTOCOL_H_

#include <ma.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{

/**
 * @brief Class that checks a Wormhole protocol.
 *
 * This class checks the three character protocol specified
 * as part of a message sent from a WebView as a
 * MAW_EVENT_WEB_VIEW_HOOK_INVOKED event, using one of
 * the send methods in mosync-bridge.js.
 *
 * The prefix has the format "xx:", for example "ma:".
 *
 * Standard protocol prefixes are:
 *
 *   "ma:" MessageStreamJSON (sent with function mosync.bridge.sendJSON
 *
 *   "ms:" MessageStream (sent with function mosync.bridge.send)
 *
 * Note: "ma" is short for "message array", since JSON message
 * objects are sent as an array.
 *
 * You can also use your own prefix and send the message string
 * using function mosync.bridge.sendRaw. The prefix must be two
 * characters plus a colon if you wish to use this class.
 * Use the MessageProtocol::matches method to check for the
 * presence of your own message protocol.
 *
 * TODO: Add copy constructor and assignment operator.
 */
class MessageProtocol
{
public:
	MessageProtocol(MAHandle dataHandle);

	virtual ~MessageProtocol();

	bool matches(const char* protocol);

	bool isMessageStream();

	bool isMessageStreamJSON();

protected:
	char mProtocol[3];
};

} // namespace

#endif

/*! @} */
