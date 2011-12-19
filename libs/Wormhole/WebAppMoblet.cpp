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
 * @author Mikael Kindborg
 *
 * @brief High-level moblet that has a WebView and supports
 * communication between a JavaScript and C++.
 */

#include <mastdlib.h>
#include <mavsprintf.h>
#include <NativeUI/WebViewListener.h>
#include "WebAppMoblet.h"

namespace Wormhole
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

			mMoblet->handleWebViewMessage(webView, urlData);
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
		// I don't care about using initialisers!
		mFileSystemIsExtracted = false;
		mWebView = NULL;

		// Create file utility object.
		mFileUtil = new FileUtil();
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
	 * Creates the WebView if it does not exist.
	 * @return Pointer to a WebView instance.
	 */
	NativeUI::WebView* WebAppMoblet::getWebView()
	{
		// Create the WebView if it does not exist.
		if (NULL == mWebView)
		{
			// Create and configure the WebView.
			mWebView = new NativeUI::WebView();
			mWebView->fillSpaceHorizontally();
			mWebView->fillSpaceVertically();

			// Create and show the screen that holds the WebView.
			mScreen = new NativeUI::Screen();
			mScreen->setMainWidget(mWebView);
		}

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
			getWebView()->addWebViewListener(mWebViewListener);
			getWebView()->enableWebViewMessages();
		}
	}

	/**
	 * Disable JavaScript to C++ communication.
	 */
	void WebAppMoblet::disableWebViewMessages()
	{
		if (NULL != mWebViewListener)
		{
			getWebView()->removeWebViewListener(mWebViewListener);
			getWebView()->disableWebViewMessages();
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
		// Since extractFileSystem() is moved out of the constructor
		// into application code, make sure it has been called before
		// displaying the page. This makes the code updates backwards
		// compatible with old application code.
		if (!mFileSystemIsExtracted)
		{
			extractFileSystem();
		}

		// Make sure the WebView is displayed.
		// It should do no harm to call this method multiple times.
		showWebView();

		// Open the page.
		getWebView()->openURL(url);
	}

	/**
	 * Display the WebView.
	 */
	void WebAppMoblet::showWebView()
	{
		// Make sure the WebView is created.
		getWebView();

		// Show the screen that contains the WebView.
		mScreen->show();
	}

	/**
	 * Run JavaScript code in the WebView.
	 */
	void WebAppMoblet::callJS(const MAUtil::String& script)
	{
		getWebView()->callJS(script);
	}

	// /**
	 // * This method is called to show a screen while unpacking the
	 // * file bundle. You can override this method to customize
	 // * the screen displayed. You can display anything you wish
	 // * in the WebView widget. The default implementation just
	 // * displays a plain message.
	 // */
	// void WebAppMoblet::displayUnpackScreen()
	// {
		// getWebView()->setHtml(
			// "<!DOCTYPE html>"
			// "<html><head>"
			// "<style type=\"text/css\">"
			// "#Message {"
				// "margin: 0.3em 0.3em;"
				// "font-size: 3.0em;"
				// "font-weight: bold;"
				// "color: white;"
				// "text-align: center;"
				// "background-color: black;"
				// "font-family: sans-serif;"
			// "}"
			// "</head><body>"
			// "<div id=\"Message\">Installing application files...</div>"
			// "</body></html>"
			// );
	// }

	/**
	 * This method handles messages sent from the WebView.
	 * Implement this method in a subclass of this class.
	 * @param webView The WebView that sent the message.
	 * @param urlData Data object that holds message content.
	 * Note that the data object will be valid only during
	 * the life-time of the call of this method, then it
	 * will be deallocated.
	 */
	void WebAppMoblet::handleWebViewMessage(
		NativeUI::WebView* webView,
		MAHandle urlData)
	{
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
		// This function has been called.
		mFileSystemIsExtracted = true;

		// Display splash screen if this is the first time launch
		// or if the checksum has changed.
		if (checksumHasChanged())
		{
			// Extract bundled files to the local file system.
			mFileUtil->extractLocalFiles();
		}
	}

	/**
	 * @return true if the checksum has changed (or if the old
	 * value did not exist, such as on first time load).
	 */
	bool WebAppMoblet::checksumHasChanged()
	{
		// Assume checksum has changed (or does not exist).
		bool hasChanged = true;

		// Read existing checksum value and check it.
		MAUtil::String filePath = getFileUtil()->getLocalPath();
		filePath += "MoSyncFileBundleChecksum";
		int checksum = getFileUtil()->getFileSystemChecksum(1);

		MAUtil::String data;
		if (getFileUtil()->readTextFromFile(filePath, data))
		{
			int existingChecksum = (int)strtol(data.c_str(), NULL, 10);
			hasChanged = checksum != existingChecksum;
		}

		// Save checksum value if it has changed.
		if (hasChanged && checksum != 0)
		{
			char checksumBuf[16];
			sprintf(checksumBuf, "%d", checksum);
			getFileUtil()->writeTextToFile(filePath, checksumBuf);
		}

		return hasChanged;
	}

} // namespace
