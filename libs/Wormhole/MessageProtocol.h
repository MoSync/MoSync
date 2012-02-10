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
 * Class for reading the protocol of a message from a WebView.
 */

#ifndef WORMHOLE_MESSAGE_PROTOCOL_H_
#define WORMHOLE_MESSAGE_PROTOCOL_H_

#include <ma.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{

/**
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

private:
	char mProtocol[3];
};

} // namespace

#endif

/*! @} */
