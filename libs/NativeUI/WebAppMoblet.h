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

#ifndef NATIVEUI_WEB_APP_MOBLET_H_
#define NATIVEUI_WEB_APP_MOBLET_H_

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include "Screen.h"
#include "WebView.h"
#include "WebViewMessage.h"
#include "FileUtil.h"

namespace NativeUI
{
    // Forward declaration.
    class WebAppMobletWebViewListener;

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
		WebView* getWebView();

		/**
		 * Get a file utility object used for accessing the
		 * device's local file system.
		 * @return Pointer to a FileUtil instance.
		 */
		FileUtil* getFileUtil();

		/**
		 * Display a page in the WebView of this moblet.
		 * @param url Url of page to open.
		 */
		void showPage(const MAUtil::String& url);

		/**
		 * Run JavaScript code in the WebView.
		 */
		void callJS(const MAUtil::String& script);

		/**
		 * This method is called when a key is pressed. It closes
		 * the application when the back key (on Android) is pressed.
		 * Override to implement your own behaviour.
		 */
		virtual void keyPressEvent(int keyCode, int nativeCode);

		/**
		 * Implement this method in a subclass to handle messages
		 * sent from JavaScript.
		 * @param webView The WebView that sent the message.
		 * @param message Object used to access message content.
		 */
		virtual void handleWebViewMessage(
			WebView* webView,
			WebViewMessage& message) = 0;

	protected:
		/**
		 * Extract HTML/CSS/JS/Media files to the local file system.
		 */
		void extractFileSystem();

		/**
		 * Create the user interface of the application.
		 * This creates a full screen WebView and configures
		 * it to receive messages from JavaScript.
		 */
		void createUI();

	private:
		/**
		 * The screen widget that is the root of the UI.
		 */
		Screen* mScreen;

		/**
		 * The WebView widget that displays the application UI.
		 */
		WebView* mWebView;

		/**
		 * JavsScript message listener.
		 */
		WebAppMobletWebViewListener* mWebViewListener;

		/**
		 * File utility object.
		 */
		FileUtil* mFileUtil;
	};
} // namespace NativeUI

#endif // NATIVEUI_WEB_APP_MOBLET_H_
