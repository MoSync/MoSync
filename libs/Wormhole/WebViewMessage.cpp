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
 * @file WebViewMessage.cpp
 * @author Mikael Kindborg
 *
 * Class for parsing messages from a WebView.
 */

#include <ma.h>				// MoSync API
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions

#include "WebViewMessage.h"

namespace Wormhole
{
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param str Encoded string.
	 * @return The decoded string.
	 * @deprecated Use method unescape in class Encoder.
	 */
	MAUtil::String WebViewMessage::unescape(const MAUtil::String& str)
	{
		// The decoded string.
		MAUtil::String result = "";

		for (int i = 0; i < str.length(); ++i)
		{
			// If the current character is the '%' escape char...
			if ('%' == (char) str[i])
			{
				// Get the char value of the two digit hex value.
				MAUtil::String hex = str.substr(i + 1, 2);
				long charValue = strtol(
					hex.c_str(),
					NULL,
					16);
				// Append to result.
				result += (char) charValue;

				// Skip over the hex chars.
				i += 2;
			}
			else
			{
				// Not encoded, just copy the character.
				result += str[i];
			}
		}

		return result;
	}

	/**
	 * Take a string and "percent encode" it.
	 * @param str String to be encoded.
	 * @return The encoded string.
	 * @deprecated Use method escape in class Encoder.
	 */
	MAUtil::String WebViewMessage::escape(const MAUtil::String& str)
	{
		// The encoded string.
		MAUtil::String result = "";
		char buf[8];

		for (int i = 0; i < str.length(); ++i)
		{
			char c = str[i];
			if ((48 <= c && c <= 57) ||  // 0-9
				(65 <= c && c <= 90) ||  // a..z
				(97 <= c && c <= 122))   // A..Z
			{
				result.append(&str[i], 1);
			}
			else
			{
				result += "%";
				sprintf(buf, "%02X", str[i]);
				result += buf;
			}
		}

		return result;
	}

	/**
	 * Constructor.
	 */
	WebViewMessage::WebViewMessage(
		NativeUI::WebView* webView,
		MAHandle dataHandle)
	{
		mWebView = webView;
		parse(dataHandle);
	}

	/**
	 * Destructor.
	 */
	WebViewMessage::~WebViewMessage()
	{
		// Nothing needs to be explicitly destroyed.
	}

	/**
	 * Get the WebView widget associated with this message.
	 * @return Pointer to WebView object.
	 */
	NativeUI::WebView* WebViewMessage::getWebView()
	{
		return mWebView;
	}

	/**
	 * Checks if this message matches the given message name.
	 */
	bool WebViewMessage::is(const MAUtil::String& messageName)
	{
		return mMessageName == messageName;
	}

	/**
	 * Returns the string value of a message parameter.
	 * @return The param value as a string.
	 */
	MAUtil::String WebViewMessage::getParam(const MAUtil::String& paramName)
	{
		return mMessageParams[paramName];
	}

	/**
	 * Returns the integer value of a message parameter.
	 * @return The param value as an int.
	 */
	int WebViewMessage::getParamInt(const MAUtil::String& paramName)
	{
		return (int) strtol(
			mMessageParams[paramName].c_str(),
			NULL,
			10);
	}

	/**
	 * Checks if the given parameter name is in the message.
	 */
	bool WebViewMessage::hasParam(const MAUtil::String& paramName)
	{
		return mMessageParams.find(paramName) != mMessageParams.end();
	}

	/**
	 * Parse the message. This finds the message name and
	 * creates a dictionary with the message parameters.
	 */
	void WebViewMessage::parse(MAHandle dataHandle)
	{
		// Set message name to empty string as default.
		mMessageName = "";

		// We must have data.
		if (NULL == dataHandle)
		{
			return;
		}

		// Get length of the data, it is not zero terminated.
		int dataSize = maGetDataSize(dataHandle);

		// Allocate buffer for string data.
		char* stringData = (char*) malloc(dataSize + 1);

		// Get the data.
		maReadData(dataHandle, stringData, 0, dataSize);

		// Zero terminate.
		stringData[dataSize] = 0;

		// Create String object with message.
		MAUtil::String messageString = stringData;

		// Find schema.
		int start = messageString.find("mosync://");
		if (0 != start)
		{
			return;
		}

		// Set start of message name.
		start = 9;

		// Find end of message name.
		int end = messageString.find("?", start);
		if (MAUtil::String::npos == end)
		{
			// No params, set message name to rest of string.
			mMessageName = messageString.substr(start);
			return;
		}

		// Set message name.
		mMessageName = messageString.substr(start, end - start);

		while (1)
		{
			// Find param name.
			start = end + 1;
			end = messageString.find("=", start);
			if (MAUtil::String::npos == end)
			{
				// No param name found, we are done.
				break;
			}

			// Set param name.
			MAUtil::String paramName = messageString.substr(start, end - start);
			MAUtil::String paramValue;

			// Find end of param value.
			start = end + 1;
			end = messageString.find("&", start);
			if (MAUtil::String::npos == end)
			{
				// Last param, set param value to rest of string.
				paramValue = messageString.substr(start);
			}
			else
			{
				paramValue = messageString.substr(start, end - start);
			}

			// Add param to table.
			mMessageParams.insert(unescape(paramName), unescape(paramValue));

			// If no more params we are done.
			if (MAUtil::String::npos == end)
			{
				break;
			}
		}

		// Free string data.
		free(stringData);
	}

} // namespace
