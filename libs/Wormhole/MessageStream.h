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
 * @brief Class for parsing a stream of messages from a WebView.
 */

#ifndef WORMHOLE_MESSAGE_STREAM_H_
#define WORMHOLE_MESSAGE_STREAM_H_

#include <ma.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{

/**
 * @brief Class that parses messages from the WebView.
 *
 * The messages are in the form of a message stream sent from a
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
	MessageStream(
		MAWidgetHandle webViewHandle,
		MAHandle dataHandle);

	/**
	 * Constructor.
	 * @deprecated
	 */
	MessageStream(
		NativeUI::WebView* webView,
		MAHandle dataHandle);

	/**
	 * Destructor.
	 */
	virtual ~MessageStream();

	/**
	 * @return true if this is a valid message stream.
	 */
	bool isValid();

	/**
	 * Get the WebView widget handle associated with this message.
	 * @return Handle to WebView widget.
	 */
	MAWidgetHandle getWebViewHandle();

	/**
	 * Get the WebView object associated with this message.
	 * @return WebView object. NULL may be returned if the
	 * MessageStream constructor was not called with a WebView
	 * object. Use getWebViewHandle() instead, or use callJS() if you
	 * wish to evaluate JavScript in the WebViewthat sent this message.
	 * @deprecated
	 */
	NativeUI::WebView* getWebView();

	/**
	 * Evaluate JavaScript the WebView associated with this message.
	 * @param script JavaScript string.
	 */
	void callJS(const MAUtil::String& script);

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
	void initialize(MAHandle dataHandle);

	/**
	 * TODO: Add error handling.
	 */
	int xtoi(char* s, char** newPos);

	/**
	 * TODO: Add error handling.
	 */
	int raise(int base, int e);

	/**
	 * The WebView widget handle of this message.
	 */
	MAWidgetHandle mWebViewHandle;

	/**
	 * The WebView widget handle of this message.
	 * @deprecated
	 */
	NativeUI::WebView* mWebView;

	/**
	 * Variables for internal use only, but made protected
	 * if some future need for subclassing would arise.
	 */
	char* mData;
	int mDataSize;
	char* mStart;
	char* mEnd;
};

} // namespace

#endif

/*! @} */
