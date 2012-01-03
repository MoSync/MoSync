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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file WebViewMessage.h
 * @author Mikael Kindborg
 *
 * Class for parsing messages from a WebView.
 */

#ifndef WEB_VIEW_MESSAGE_H_
#define WEB_VIEW_MESSAGE_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <NativeUI/WebView.h>
#include <yajl/YAJLDom.h>

namespace App
{

/**
 * Class that parses messages in the form of urls sent from a
 * WebView as MAW_EVENT_WEB_VIEW_HOOK_INVOKED events.
 *
 * Messages used with this class has the format:
 *
 *   ma:[{"messageName":"message1",...},{"messageName":"message2",...},...]
 *
 * TODO: Add copy constructor and assignment operator.
 */
class WebViewMessage
{
public:
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param str Encoded string.
	 * @return The decoded string.
	 */
	static MAUtil::String unescape(const MAUtil::String& str);

	/**
	 * Take a string and "percent encode" it.
	 * @param str String to be encoded.
	 * @return The encoded string.
	 */
	static MAUtil::String escape(const MAUtil::String& str);

	/**
	 * Constructor.
	 */
	WebViewMessage(NativeUI::WebView* webView, MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~WebViewMessage();

	/**
	 * Get the WebView widget associated with this message.
	 * @return Pointer to WebView object.
	 */
	virtual NativeUI::WebView* getWebView();

	/**
	 * Move to the next message. Initially, the message
	 * pointer is positioned right before the first message.
	 * Do a call to this method to get the first message.
	 *
	 * @return true if moved to next message, false
	 * if there are no more messages.
	 */
	virtual bool next();

	/**
	 * Checks if this message matches the given message name.
	 */
	virtual bool is(const char* paramName);

	/**
	 * Returns the string value of a message parameter.
	 * @return The param value as a string.
	 */
	virtual MAUtil::String getParam(const char* paramName);

	/**
	 * Returns the integer value of a message parameter.
	 * @return The param value as an int.
	 */
	virtual int getParamInt(const char* paramName);

	/**
	 * Checks if the given parameter name is in the message.
	 */
	virtual bool hasParam(const char* paramName);

	/**
	 * Get the node of a parameter in the current message.
	 */
	virtual MAUtil::YAJLDom::Value* getParamNode(const char* paramName);

protected:
	/**
	 * Parse the message. This finds the message name and
	 * creates a dictionary with the message parameters.
	 */
	virtual void parse(MAHandle dataHandle);

	/**
	 * The WebView of this message.
	 */
	NativeUI::WebView* mWebView;

public:
	/**
	 * The name of the current message (the "command name").
	 */
	MAUtil::String mMessageName;

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
