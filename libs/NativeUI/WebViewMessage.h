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
 * @file WebViewMessage.h
 * @author Mikael Kindborg
 *
 * Class for parsing messages from a WebView.
 */

#ifndef NATIVEUI_WEB_VIEW_MESSAGE_H_
#define NATIVEUI_WEB_VIEW_MESSAGE_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <IX_WIDGET.h>

namespace NativeUI
{

/**
 * Class that parses messages in the form of urls sent from a
 * WebView as MAW_EVENT_WEB_VIEW_HOOK_INVOKED events.
 *
 * Message (urls) used with this class has the format:
 *
 *   mosync://MessageName?Param1=Value1&Param2=Value2&...
 *
 * Example JavaScript call (with no parameters):
 *
 *   document.location = "mosync://GetGeoLocation";
 *
 * TODO: Add copy constructor and assignment operator.
 */
class WebViewMessage
{
public:
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param url Encoded string.
	 * @return The decoded string.
	 */
	static MAUtil::String unescape(const MAUtil::String& url);

	/**
	 * Constructor.
	 */
	WebViewMessage(MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~WebViewMessage();

	/**
	 * Checks if this message matches the given message name.
	 */
	bool is(const MAUtil::String& messageName);

	/**
	 * Returns a message parameter by param name.
	 */
	MAUtil::String getParam(const MAUtil::String& paramName);

protected:
	/**
	 * Parse the message. This finds the message name and
	 * creates a dictionary with the message parameters.
	 */
	void parse(MAHandle dataHandle);

private:
	/**
	 * The message name is the "command name".
	 */
	MAUtil::String mMessageName;

	/**
	 * Table for message parameters.
	 */
	MAUtil::HashMap<MAUtil::String, MAUtil::String> mMessageParams;
};

} // namespace NativeUI

#endif // NATIVEUI_WEB_VIEW_MESSAGE_H_
