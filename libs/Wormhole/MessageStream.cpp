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
 * @file MessageStream.h
 * @author Mikael Kindborg
 *
 * Class for parsing a stream of messages from a WebView.
 */

#include <ma.h>				// MoSync API
#include <maheap.h>			// C memory allocation
#include <mastring.h>		// C string functions
#include <mavsprintf.h>		// C string functions
#include <mastdlib.h>		// C string conversion functions
#include <conprint.h>

#include "MessageStream.h"

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	MessageStream::MessageStream(
		NativeUI::WebView* webView,
		MAHandle dataHandle)
	{
		mWebView = webView;
		initialize(dataHandle);
	}

	/**
	 * Destructor.
	 */
	MessageStream::~MessageStream()
	{
		if (NULL != mData)
		{
			free(mData);
			mData = NULL;
		}
	}

	/**
	 * @return true if this is a valid message stream.
	 */
	bool MessageStream::isValid()
	{
		return NULL != mData;
	}

	/**
	 * Get the WebView widget associated with this message.
	 * @return Pointer to WebView object.
	 */
	NativeUI::WebView* MessageStream::getWebView()
	{
		return mWebView;
	}

	/**
	 * Get a pointer to the next string in the message stream.
	 * @param length Length of the string is returned in this
	 * parameter. Can be set to NULL (default value) in which
	 * case it is not used.
	 * @return Pointer to the string, NULL if there are no more
	 * strings in the message.
	 */
	const char* MessageStream::getNext(int* length)
	{
		char* p;

		// If first call then start at beginning of data,
		// else move forward one character.
		if (NULL == mEnd)
		{
			// Move past the protocol specifier.
			p = mData + 3;
		}
		else
		{
			p = mEnd + 1;
		}

//		lprintfln("value of p: %i", (int)p);

		// Check boundary.
		if (p - mData >= mDataSize)
		{
//			lprintfln("1: p - mData >= mDataSize");
			return NULL;
		}

		// Get string length and updated pointer.
		int len = xtoi(p, &p);

		if (len == 0)
		{
//			lprintfln("len == 0");
			return NULL;
		}

		// Check boundary.
		if (p - mData >= mDataSize)
		{
//			lprintfln("2: p - mData >= mDataSize");
			return NULL;
		}

		// Set length parameter.
		if (NULL != length)
		{
			*length = len;
		}

//		lprintfln("String length: %i", len);

		// Point p to start of string data.
		mStart = p + 1;

		// Point to end of string.
		mEnd = mStart + len;

		// Check boundary.
		if (mEnd - mData >= mDataSize)
		{
//			lprintfln("3: mEnd - mData >= mDataSize");
			return NULL;
		}

		// Zero terminate string.
		*mEnd = 0;

		//lprintfln("@@@ mStart: %s", mStart);

		// Return string start.
		return mStart;
	}

	/**
	 * TODO: Add error handling.
	 */
	int MessageStream::xtoi(char* s, char** newPos)
	{
		int firstChar = 33;
		int lastChar = 126;
		int base = lastChar - firstChar;

		int n = 0;
		int i;
		for (i = 0; s[i] != ' '; ++i)
		{
			// Sanity check.
			if (i > 4)
			{
				return 0;
			}

			n += raise(base, i) * (s[i] - firstChar);
		}

		*newPos = s + i;

		return n;
	}

	/**
	 * TODO: Add error handling.
	 */
	int MessageStream::raise(int base, int e)
	{
		int pow = 1;

		// Sanity check.
		if (e > 4)
		{
			return 0;
		}

		while (e--)
		{
			pow *= base;
		}
		return pow;
	}

	/**
	 * Read data and initialise the stream.
	 */
	void MessageStream::initialize(MAHandle dataHandle)
	{
		mData = NULL;

		// We must have data.
		if (NULL == dataHandle)
		{
			return;
		}

		// Get length of the data, it is not zero terminated.
		int dataSize = maGetDataSize(dataHandle);

		// Allocate buffer for string data.
		char* data = (char*) malloc(dataSize + 1);

		// Get the data.
		maReadData(dataHandle, data, 0, dataSize);

		data[dataSize] = 0;

		// Check that we have the "ms:" prefix.
		if (data[0] != 'm') { return; }
		if (data[1] != 's') { return; }
		if (data[2] != ':') { return; }

		mData = data;
		mDataSize = dataSize;
		mStart = NULL;
		mEnd = NULL;
	}

} // namespace
