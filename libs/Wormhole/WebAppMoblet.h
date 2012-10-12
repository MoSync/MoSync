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

/*! \addtogroup WormHoleGroup
 *  @{
 */

/** @defgroup WormHoleGroup Wormhole Library
 *  @{
 */

/**
 * @file WebAppMoblet.h
 * @author Mikael Kindborg
 *
 * @brief High-level moblet that has a WebView and supports
 * communication between a JavaScript and C++.
 *
 * Class WebAppMoblet is deprecated. For new applications,
 * use class HybridMoblet.
 */

#ifndef WORMHOLE_WEB_APP_MOBLET_H_
#define WORMHOLE_WEB_APP_MOBLET_H_

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <NativeUI/Screen.h>
#include <NativeUI/WebView.h>
#include "WebViewMessage.h"
#include "FileUtil.h"

/**
 * @brief Classes for communication with JavaScript code
 */
namespace Wormhole
{
	// Forward declaration.
	class WebAppMoblet_WebViewListener;

	/**
	 * @deprecated For new applications, use class HybridMoblet.
	 * \brief A Moblet with additional functionality that allows easier interaction with
	 * JavaScript code. It provides a wormhole-enabled WebView, methods to extract and load
	 * your HTML files and JavaScript code, and a structure for communicating with your JS
	 * code.
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
		 * Display the WebView.
		 */
		virtual void showWebView();

		/**
		 * Run JavaScript code in the WebView.
		 */
		virtual void callJS(const MAUtil::String& script);

		/**
		 * This method handles messages sent from the WebView.
		 * Implement this method in a subclass of this class.
		 * @param webView The WebView that sent the message.
		 * @param urlData Data object that holds message content.
		 * Note that the data object will be valid only during
		 * the life-time of the call of this method, then it
		 * will be deallocated.
		 */
		virtual void handleWebViewMessage(
			NativeUI::WebView* webView,
			MAHandle urlData);

		/**
		 * This method is called when a key is pressed. It closes
		 * the application when the back key (on Android) is pressed.
		 * Override to implement your own behaviour.
		 */
		virtual void keyPressEvent(int keyCode, int nativeCode);

		/**
		 * Extract HTML/CSS/JS/Media files to the local file system.
		 */
		virtual void extractFileSystem();

		/**
		 * @return true if the checksum has changed (or if the old
		 * value did not exist, such as on first time load).
		 */
		virtual bool checksumHasChanged();

		/**
		 * Write the current checksum to file.
		 */
		virtual void writeChecksum();

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
		 * File utility object.
		 */
		FileUtil* mFileUtil;

		/**
		 * Has extractFileSystem() been called?
		 */
		bool mFileSystemIsExtracted;
	};
} // namespace

#endif

 /*! @} */
