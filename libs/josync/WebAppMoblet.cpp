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

#include <conprint.h>
#include <NativeUI/WebViewListener.h>
#include "WebAppMoblet.h"

namespace josync
{
	/**
	 * Class that listens for WebView events.
	 * Here we receive messages from JavaScript.
	 */
	class WebAppMoblet_WebViewListener : public NativeUI::WebViewListener
	{
	public:
		/**
		 * Constructor that saves the pointer to the moblet.
		 */
		WebAppMoblet_WebViewListener(WebAppMoblet* moblet)
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
			NativeUI::WebView* webView,
			int hookType,
			MAHandle urlData)
		{
			// Note: urlData is deallocated automatically by
			// the framework, we should not deallocate it here.

			mMoblet->processWebViewMessage(webView, urlData);
		}

	private:
		/**
		 * Pointer to the moblet.
		 */
		WebAppMoblet* mMoblet;
	};
	// End of class WebAppMoblet_WebViewListener

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

		// Delete the WebView listener.
		if (NULL != mWebViewListener)
		{
			delete mWebViewListener;
			mWebViewListener = NULL;
		}

		// Delete the file utility object.
		delete mFileUtil;
	}

	/**
	 * Get the WebView widget displayed by this moblet.
	 * @return Pointer to a WebView instance.
	 */
	NativeUI::WebView* WebAppMoblet::getWebView()
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
	 * Enable JavaScript to C++ communication.
	 */
	void WebAppMoblet::enableWebViewMessages()
	{
		if (NULL == mWebViewListener)
		{
			mWebViewListener = new WebAppMoblet_WebViewListener(this);
			mWebView->addWebViewListener(mWebViewListener);
			mWebView->enableWebViewMessages();
		}
	}

	/**
	 * Disable JavaScript to C++ communication.
	 */
	void WebAppMoblet::disableWebViewMessages()
	{
		if (NULL != mWebViewListener)
		{
			mWebView->removeWebViewListener(mWebViewListener);
			mWebView->disableWebViewMessages();
			delete mWebViewListener;
			mWebViewListener = NULL;
		}
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
	void WebAppMoblet::passEventToJS(MAEvent* event)
	{
		char script[512];
		sprintf(
			script,
			"josync.messagehandler.processEvent(%d, %d, %d, %d)",
			event->type,
			*(((int*)event) + 1),
			*(((int*)event) + 2),
			*(((int*)event) + 3));
		callJS(script);
	}

	/**
	 * Implement this method in a subclass to handle messages
	 * sent from JavaScript.
	 * @param webView The WebView that sent the message.
	 * @param message Object used to access message content.
	 */
	void WebAppMoblet::handleWebViewMessage(WebViewMessage& message)
	{
	}

	/**
	 * This method handles messages sent from the WebView
	 * via urls. It calls handleWebViewMessage.
	 */
	void WebAppMoblet::processWebViewMessage(
		NativeUI::WebView* webView,
		MAHandle urlData)
	{
		// Tell the WebView that we have the message, so that
		// it can send the next one.
		callJS("josync.messagehandler.processedMessage()");

		// Create message object
		WebViewMessage message(webView, urlData);

		// Handle josync messages.
		bool handled = mWebViewMessageHandler->handleMessage(message);

		// If not a josync message, let a subclass
		// handle the message.
		if (!handled)
		{
			handleWebViewMessage(message);
		}
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
		mFileUtil = new FileUtil();

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
		mWebView = new NativeUI::WebView();
		mWebView->fillSpaceHorizontally();
		mWebView->fillSpaceVertically();

		// Create and show the screen that holds the WebView.
		mScreen = new NativeUI::Screen();
		mScreen->setMainWidget(mWebView);
		mScreen->show();
	}

} // namespace
