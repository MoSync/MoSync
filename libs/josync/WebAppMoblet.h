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
 * @file WebAppMoblet.h
 * @author Mikael KIndborg
 *
 * \brief High-level moblet that has a WebView and supports
 * communication between a JavaScript and C++.
 */

#ifndef JOSYNC_WEB_APP_MOBLET_H_
#define JOSYNC_WEB_APP_MOBLET_H_

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <NativeUI/Screen.h>
#include <NativeUI/WebView.h>
#include "WebViewMessage.h"
#include "WebViewMessageHandler.h"
#include "FileUtil.h"

namespace josync
{
	// Forward declaration.
	class WebAppMoblet_WebViewListener;

	/**
	 * \brief A web view is a widget used to render web pages.
	 */
	class WebAppMoblet : public MAUtil::Moblet
	{
	public:
		/**
		 * Constructor.
		 */
		WebAppMoblet();

		/**
		 * Destructor.
		 */
		virtual ~WebAppMoblet();

		/**
		 * Get the WebView widget displayed by this moblet.
		 * @return Pointer to the WebView instance.
		 */
		virtual NativeUI::WebView* getWebView();

		/**
		 * Get a file utility object used for accessing the
		 * device's local file system.
		 * @return Pointer to a FileUtil instance.
		 */
		virtual FileUtil* getFileUtil();

		/**
		 * Enable JavaScript to C++ communication.
		 */
		virtual void enableWebViewMessages();

		/**
		 * Disable JavaScript to C++ communication.
		 */
		virtual void disableWebViewMessages();

		/**
		 * Display a page in the WebView of this moblet.
		 * @param url Url of page to open.
		 */
		virtual void showPage(const MAUtil::String& url);

		/**
		 * Run JavaScript code in the WebView.
		 */
		virtual void callJS(const MAUtil::String& script);

		/**
		 * This method passes the event to JavaScript.
		 *
		 * Note that currently you need to call this method
		 * explicitly from your C++ event handling code.
		 *
		 * At present, we just pass on the event type
		 * and the first three int parameters.
		 *
		 * TODO: And more sophisticated event detection.
		 */
		virtual void passEventToJS(MAEvent* event);

		/**
		 * Implement this method in a subclass to handle messages
		 * sent from JavaScript.
		 * @param message Object used to access message content.
		 */
		virtual void handleWebViewMessage(WebViewMessage& message);

		/**
		 * This method handles messages sent from the WebView
		 * via urls. It calls handleWebViewMessage.
		 */
		virtual void processWebViewMessage(
			NativeUI::WebView* webView,
			MAHandle urlData);

		/**
		 * This method is called when a key is pressed. It closes
		 * the application when the back key (on Android) is pressed.
		 * Override to implement your own behaviour.
		 */
		virtual void keyPressEvent(int keyCode, int nativeCode);

	protected:
		/**
		 * Extract HTML/CSS/JS/Media files to the local file system.
		 */
		virtual void extractFileSystem();

		/**
		 * Create the user interface of the application.
		 * This creates a full screen WebView and configures
		 * it to receive messages from JavaScript.
		 */
		virtual void createUI();

	protected:
		/**
		 * The screen widget that is the root of the UI.
		 */
		NativeUI::Screen* mScreen;

		/**
		 * The WebView widget that displays the application UI.
		 */
		NativeUI::WebView* mWebView;

		/**
		 * JavsScript message listener.
		 */
		WebAppMoblet_WebViewListener* mWebViewListener;

		/**
		 * This class implements the standard josync message API.
		 */
		WebViewMessageHandler* mWebViewMessageHandler;

		/**
		 * File utility object.
		 */
		FileUtil* mFileUtil;
	};
} // namespace NativeUI

#endif
