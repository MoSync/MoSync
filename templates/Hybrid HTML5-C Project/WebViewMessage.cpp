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
 * Class for working with messages from a WebView.
 */

#include <ma.h>				// MoSync API
#include <maprofile.h>		// Profile database
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions
#include <MAFS/File.h>		// Library for working with file system bundle
#include <MAUtil/String.h>	// C++ String class
#include <IX_WIDGET.h>		// Widget API
#include <conprint.h>		// Debug printing.
#include "WebViewMessage.h"

namespace NativeUI
{
	/**
	 * Take a string that is "percent encoded" and decode it.
	 * @param url Encoded string.
	 * @return The decoded string.
	 */
	MAUtil::String WebViewMessage::unescape(const MAUtil::String& url)
	{
		// The decoded string.
		MAUtil::String result = "";

		for (int i = 0; i < url.length(); ++i)
		{
			// If the current character is the '%' escape char...
			if ('%' == (char) url[i])
			{
				// Get the char value of the two digit hex value.
				MAUtil::String hex = url.substr(i + 1, 2);
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
				result += url[i];
			}
		}

		return result;
	}

	/**
	 * Constructor.
	 */
	WebViewMessage::WebViewMessage(MAHandle dataHandle)
	{
		if (NULL != dataHandle)
		{
			// Get length of the data, it is not zero terminated.
			int dataSize = maGetDataSize(dataHandle);

			// Allocate buffer for string data.
			char* stringData = (char*) malloc(dataSize + 1);

			// Get the data.
			maReadData(dataHandle, stringData, 0, dataSize);

			// Zero terminate.
			stringData[dataSize] = 0;

			// Point to message data.
			char* p = stringData + strlen("mosync://");

			// Decode "percent encoded" characters
			// and set the message string.
			mMessageString = unescape(p);

			// Destroy string data.
			free(stringData);

			// Destroy the data handle
			maDestroyObject(dataHandle);
		}
	}

	/**
	 * Destructor.
	 */
	WebViewMessage::~WebViewMessage()
	{
		// Nothing needs to be explicitly destroyed.
	}

	/**
	 * Returns the message string.
	 */
	MAUtil::String WebViewMessage::getMessageString()
	{
		return mMessageString.c_str();
	}

	/**
	 * Checks if this message matches the given message name.
	 */
	bool WebViewMessage::is(const MAUtil::String& messageName)
	{
		// Start of messageName should be found at start of message string.
		return 0 == mMessageString.find(messageName);
	}

	/**
	 * Returns the parameter part of a message.
	 */
	MAUtil::String WebViewMessage::getParams()
	{
		// Must be at least three characters in a message
		// that has a data part.
		if (mMessageString.length() < 3)
		{
			return "";
		}

		// Find first slash.
		int index = mMessageString.find("/");
		if (MAUtil::String::npos == index)
		{
			return "";
		}

		// Return the substring after the slash.
		return mMessageString.substr(index + 1);
	}

	/**
	 * Returns a message parameter by index.
	 * Parameters are separated by slashes.
	 */
	MAUtil::String WebViewMessage::getParam(int index)
	{
		// Get params.
		MAUtil::String params = getParams();

		// Find start slash of the param we look for.
		int start = 0;
		for (int i = 0; i < index; ++i)
		{
			start = params.find("/", start);
			if (MAUtil::String::npos == start)
			{
				// Param not found.
				return "";
			}
			// Move to position after slash.
			start = start + 1;
		}

		// Is this the last param?
		int end = params.find("/", start);
		if (MAUtil::String::npos == end)
		{
			// Yes, last param, return rest of the string.
			return params.substr(start);
		}
		else
		{
			// No, not last param, return param part of the string.
			return params.substr(start, end);
		}
	}

} // namespace NativeUI
