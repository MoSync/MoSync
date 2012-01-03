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
#include <conprint.h>

#include "WebViewMessage.h"

using namespace MAUtil;

namespace App
{
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param str Encoded string.
	 * @return The decoded string.
	 */
	String WebViewMessage::unescape(const String& str)
	{
		// The decoded string.
		String result = "";

		for (int i = 0; i < str.length(); ++i)
		{
			// If the current character is the '%' escape char...
			if ('%' == (char) str[i])
			{
				// Get the char value of the two digit hex value.
				String hex = str.substr(i + 1, 2);
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
	 */
	String WebViewMessage::escape(const String& str)
	{
		// The encoded string.
		String result = "";
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
	 * Constructor. Here we parse the message.
	 */
	WebViewMessage::WebViewMessage(
		NativeUI::WebView* webView,
		MAHandle dataHandle)
	{
		mWebView = webView;
		mCurrentMessageIndex = -1;
		parse(dataHandle);
	}

	/**
	 * Destructor. Here we delete the JSON tree.
	 */
	WebViewMessage::~WebViewMessage()
	{
		// The root must not be NULL or Value::NUL.
		if (NULL != mJSONRoot && YAJLDom::Value::NUL != mJSONRoot->getType())
		{
			// Delete the JSON tree.
			YAJLDom::deleteValue(mJSONRoot);
			mJSONRoot = NULL;
		}
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
	 * Move to the next message. Initially, the message
	 * pointer is positioned right before the first message.
	 * Do a call to this method to get the first message.
	 *
	 * @return true if moved to next message, false
	 * if there are no more messages.
	 */
	bool WebViewMessage::next()
	{
		if (NULL != mJSONRoot && YAJLDom::Value::ARRAY == mJSONRoot->getType())
		{
			++mCurrentMessageIndex;
			return mCurrentMessageIndex < mJSONRoot->getNumChildValues();
		}
		return false;
	}

	/**
	 * Checks if this message matches the given message name.
	 */
	bool WebViewMessage::is(const char* paramName)
	{
		YAJLDom::Value* value = getParamNode("messageName");
		if (NULL != value && YAJLDom::Value::STRING == value->getType());
		{
//			YAJLDom::StringValue* stringValue = (YAJLDom::StringValue*) value;
//			return 0 == strncmp(
//				paramName,
//				stringValue->getCharPointer(),
//				stringValue->getLength());
			return value->toString() == paramName;
		}
		return false;
	}

	/**
	 * Returns the string value of a message parameter.
	 * @return The param value as a string.
	 */
	String WebViewMessage::getParam(const char* paramName)
	{
		YAJLDom::Value* value = getParamNode(paramName);
		if (NULL != value && YAJLDom::Value::STRING == value->getType());
		{
			return value->toString();
		}
		return "";
	}

	/**
	 * Returns the integer value of a message parameter.
	 * @return The param value as an int.
	 */
	int WebViewMessage::getParamInt(const char* paramName)
	{
		YAJLDom::Value* value = getParamNode(paramName);
		if (NULL != value && YAJLDom::Value::NUMBER == value->getType());
		{
			return value->toInt();
		}
		return 0;
	}

	/**
	 * Checks if the given parameter name is in the message.
	 */
	bool WebViewMessage::hasParam(const char* paramName)
	{
		YAJLDom::Value* value = getParamNode(paramName);
		return (NULL != value && YAJLDom::Value::NUL != value->getType());
	}

	/**
	 * Get the node of a parameter in the current message.
	 */
	YAJLDom::Value* WebViewMessage::getParamNode(const char* paramName)
	{
		if (NULL != mJSONRoot && YAJLDom::Value::ARRAY == mJSONRoot->getType())
		{
			YAJLDom::Value* message = mJSONRoot->getValueByIndex(mCurrentMessageIndex);
			if (YAJLDom::Value::MAP == message->getType())
			{
				return message->getValueForKey(paramName);
			}
		}
		return NULL;
	}

	/**
	 * Parse the message. This finds the message name and
	 * creates a dictionary with the message parameters.
	 */
	void WebViewMessage::parse(MAHandle dataHandle)
	{
		//lprintfln("@@@ WebViewMessage::parse %i", dataHandle);

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

		//maWriteLog(stringData, dataSize);

		// Check that we have the "ma:" prefix,
		// followed by the JSON array.
		if (stringData[0] != 'm') { return; }
		if (stringData[1] != 'a') { return; }
		if (stringData[2] != ':') { return; }
		if (stringData[3] != '[') { return; }

		// Pointer to the start of the JSOn array at the
		// opening '[' character.
		char* jsonData = stringData + 3;

		mJSONRoot = YAJLDom::parse(
			(const unsigned char*)jsonData,
			dataSize - 3);

		free(stringData);
	}

} // namespace
