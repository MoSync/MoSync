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
 * @file MessageStreamJSON.h
 * @author Mikael Kindborg
 *
 * Class for parsing JSON messages from a WebView.
 */

#ifndef WORMHOLE_MESSAGE_STREAM_JSON_H_
#define WORMHOLE_MESSAGE_STREAM_JSON_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <NativeUI/WebView.h>
#include <yajl/YAJLDom.h>

namespace Wormhole
{

/**
 * Class that parses JSON messages in the form of urls sent from a
 * WebView as MAW_EVENT_WEB_VIEW_HOOK_INVOKED events.
 *
 * Message data used with this class has the format:
 *
 *   ma:[{"messageName":"message1",...},{"messageName":"message2",...},...]
 *
 * TODO: Add copy constructor and assignment operator.
 */
class MessageStreamJSON
{
public:
	/**
	 * Constructor.
	 */
	MessageStreamJSON(NativeUI::WebView* webView, MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~MessageStreamJSON();

	/**
	 * Get the WebView widget associated with this message.
	 * @return Pointer to WebView object.
	 */
	NativeUI::WebView* getWebView();

	/**
	 * Move to the next message. Initially, the message
	 * pointer is positioned right before the first message.
	 * Do a call to this method to get the first message.
	 *
	 * @return true if moved to next message, false
	 * if there are no more messages.
	 */
	bool next();

	/**
	 * Checks if this message matches the given message name.
	 */
	bool is(const char* paramName);

	/**
	 * Returns the string value of a top-level message parameter.
	 * @return The param value as a string.
	 */
	MAUtil::String getParam(const char* paramName);

	/**
	 * Returns the integer value of a top-level message parameter.
	 * @return The param value as an int.
	 */
	int getParamInt(const char* paramName);

	/**
	 * Checks if the given top-level parameter name is in the message.
	 */
	bool hasParam(const char* paramName);

	/**
	 * Get the node of a top-level parameter in the current message.
	 */
	MAUtil::YAJLDom::Value* getParamNode(const char* paramName);

	/**
	 * @return The JSON root node.
	 */
	MAUtil::YAJLDom::Value* getJSONRoot();

	/**
	 * Parse the message. This finds the message name and
	 * creates a dictionary with the message parameters.
	 */
	void parse(MAHandle dataHandle);

private:
	/**
	 * The WebView of this message.
	 */
	NativeUI::WebView* mWebView;

protected:
	/**
	 * Table for message parameters.
	 */
	MAUtil::YAJLDom::Value* mJSONRoot;

	/**
	 * Index of current message.
	 */
	int mCurrentMessageIndex;
};

} // namespace

#endif

/*! @} */
