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
 * @file WebViewMessage.h
 * @author Mikael Kindborg
 *
 * @brief Class for parsing messages from a WebView.
 *
 * @deprecated Use class MessageStream or MessageStreamJSON
 * instead of this class. This class cannot be used with the
 * the new version of the Wormhole JavaScrip library. It is
 * included only for backwards compatibility.
 */

#ifndef WORMHOLE_WEB_VIEW_MESSAGE_H_
#define WORMHOLE_WEB_VIEW_MESSAGE_H_

#include <ma.h>
#include <MAUtil/String.h>
#include <MAUtil/HashMap.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{

/**
 * @brief Class that parses messages from the WebView.
 *
 * The messages are in the form of urls sent from a
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
	 * @param str Encoded string.
	 * @return The decoded string.
	 * @deprecated Use method unescape in class Encoder.
	 */
	static MAUtil::String unescape(const MAUtil::String& str);

	/**
	 * Take a string and "percent encode" it.
	 * @param str String to be encoded.
	 * @return The encoded string.
	 * @deprecated Use method escape in class Encoder.
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
	 * Checks if this message matches the given message name.
	 */
	virtual bool is(const MAUtil::String& messageName);

	/**
	 * Returns the string value of a message parameter.
	 * @return The param value as a string.
	 */
	virtual MAUtil::String getParam(const MAUtil::String& paramName);

	/**
	 * Returns the integer value of a message parameter.
	 * @return The param value as an int.
	 */
	virtual int getParamInt(const MAUtil::String& paramName);

	/**
	 * Checks if the given parameter name is in the message.
	 */
	virtual bool hasParam(const MAUtil::String& paramName);

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

	/**
	 * The message name is the "command name".
	 */
	MAUtil::String mMessageName;

	/**
	 * Table for message parameters.
	 */
	MAUtil::HashMap<MAUtil::String, MAUtil::String> mMessageParams;
};

} // namespace

#endif

/*! @} */
