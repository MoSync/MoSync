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
 * @file MessageStream.h
 * @author Mikael Kindborg
 *
 * Class for parsing a stream of messages from a WebView.
 */

#ifndef WORMHOLE_MESSAGE_STREAM_H_
#define WORMHOLE_MESSAGE_STREAM_H_

#include <ma.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{

/**
 * Class that parses messages in the form of a message stream sent from a
 * WebView in a MAW_EVENT_WEB_VIEW_HOOK_INVOKED event.
 *
 * Messages used with this class has the format:
 *
 *   ms:<4-byte opcode><optional string params>
 *
 * TODO: Add copy constructor and assignment operator.
 */
class MessageStream
{
public:
	/**
	 * Constructor.
	 */
	MessageStream(NativeUI::WebView* webView, MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~MessageStream();

	/**
	 * @return true if this is a valid message stream.
	 */
	bool isValid();

	/**
	 * Get the WebView widget associated with this message.
	 * @return Pointer to WebView object.
	 */
	NativeUI::WebView* getWebView();

	/**
	 * Get a pointer to the next string in the message stream,
	 * and optionally get the length of the string.
	 *
	 * @param length Length of the string is returned in this
	 * parameter. Can be set to NULL (default value) in which
	 * case it is not used.
	 *
	 * @return Pointer to the string, NULL if there are no more
	 * strings in the message.
	 */
	const char* getNext(int* length = NULL);

protected:
	/**
	 * Read data and initialise the stream.
	 */
	void MessageStream::initialize(MAHandle dataHandle);

	/**
	 * TODO: Add error handling.
	 */
	int xtoi(char* s, char** newPos);

	/**
	 * TODO: Add error handling.
	 */
	int raise(int base, int e);

protected:
	/**
	 * The WebView of this message.
	 */
	NativeUI::WebView* mWebView;

public:
	char* mData;
	int mDataSize;
	char* mStart;
	char* mEnd;
};

} // namespace

#endif

/*! @} */
