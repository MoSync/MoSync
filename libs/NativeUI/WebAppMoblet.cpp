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
 * @file WebAppMoblet.cpp
 * @author Mikael KIndborg
 *
 * \brief High-level moblet that has a WebView and supports
 * communication between a JavaScript and C++.
 */

#include "WebAppMoblet.h"
#include "WebViewListener.h"

namespace NativeUI
{
	/**
	 * Class that listens for WebView events.
	 * Here we receive messages from JavaScript.
	 */
	class WebAppMobletWebViewListener : public WebViewListener
	{
	public:
		/**
		 * Constructor that saves the pointer to the moblet.
		 */
		WebAppMobletWebViewListener(WebAppMoblet* moblet)
		{
			mMoblet = moblet;
		}

		/**
		 * This method is called when a "mosync://" url is
		 * invoked in the WebView.
		 *
		 * Note: The urlData is deallocated automatically
		 * by the WebView after this call finishes.
		 */
		virtual void webViewHookInvoked(
			WebView* webView,
			int hookType,
			MAHandle urlData)
		{
			// Create message object
			WebViewMessage message(urlData);

			// Let the moblet handle the message.
			mMoblet->handleWebViewMessage(webView, message);

			// Note: urlData is deallocated automatically by
			// the framework, we should not deallocate it here.
		}

		/**
		 * TODO: Remove when made non-abstract.
		 */
		virtual void webViewContentLoading(
			WebView* webView,
			const int webViewState)
		{
		}

	private:
		/**
		 * Pointer to the moblet.
		 */
		WebAppMoblet* mMoblet;
	};
	// End of class WebAppMobletWebViewListener

	/**
	 * Constructor.
	 */
	WebAppMoblet::WebAppMoblet()
	{
		extractFileSystem();
		createUI();
	}

	/**
	 * Destructor.
	 */
	WebAppMoblet::~WebAppMoblet()
	{
		// Deleting the root widget will also delete child widgets.
		delete mScreen;

		// Deleting the WebView listener.
		delete mWebViewListener;

		// Delete the file utility object.
		delete mFileUtil;
	}

	/**
	 * Get the WebView widget displayed by this moblet.
	 * @return Pointer to a WebView instance.
	 */
	WebView* WebAppMoblet::getWebView()
	{
		return mWebView;
	}

	/**
	 * Get a file utility object used for accessing the
	 * device's local file system.
	 * @return Pointer to a FileUtil instance.
	 */
	FileUtil* WebAppMoblet::getFileUtil()
	{
		return mFileUtil;
	}

	/**
	 * Display a page in the WebView of this moblet.
	 * @param url Url of page to open.
	 */
	void WebAppMoblet::showPage(const MAUtil::String& url)
	{
		mWebView->openURL(url);
	}

	/**
	 * Run JavaScript code in the WebView.
	 */
	void WebAppMoblet::callJS(const MAUtil::String& script)
	{
		mWebView->callJS(script);
	}

	/**
	 * This method is called when a key is pressed. It closes
	 * the application when the back key (on Android) is pressed.
	 * Override to implement your own behaviour.
	 */
	void WebAppMoblet::keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}

	/**
	 * Extract HTML/CSS/JS/Media files to the local file system.
	 */
	void WebAppMoblet::extractFileSystem()
	{
		// Create file utility object.
		mFileUtil = FileUtil::create();

		// Extract bundled files to the local file system.
		mFileUtil->extractLocalFiles();
	}

	/**
	 * Create the user interface of the application.
	 * This creates a full screen WebView and configures
	 * it to receive messages from JavaScript.
	 */
	void WebAppMoblet::createUI()
	{
		// Create and configure the WebView.
		mWebView = new WebView();
		mWebViewListener = new WebAppMobletWebViewListener(this);
		mWebView->addWebViewListener(mWebViewListener);
		mWebView->enableWebViewMessages();
		mWebView->fillSpaceHorizontally();
		mWebView->fillSpaceVertically();

		// Create and show the screen that holds the WebView.
		mScreen = new Screen();
		mScreen->setMainWidget(mWebView);
		mScreen->show();
	}

} // namespace NativeUI
