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

/**
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * The PhotoGallery application lets you take pictures using
 * the device camera and upload them to a server. It also lets
 * you view the most recent pictures uploaded to the server.
 *
 * The main purpose is to demonstrate file upload, but the
 * program illustrates several other programming techniques.
 *
 * This application demonstrates the following functionality:
 *
 * - Take pictures using the JavaScript Capture API
 * - Upload of files using the FileTransfer.upload() JavaScript API
 * - Saving uploaded files on a web server using a PHP script.
 * - Downloading data from a server, using a custom function added
 *   to JavaScript.
 * - A simplified way of adding custom JavaScript message handling
 *   functions in C++, implemented by the MyMessageHandler class
 *   (this may be added to the MoSync Wormhole library in an
 *   upcoming release of MoSync).
 * - How to send messages between WebViews (via C++).
 *
 * The folder "Server" contains the server side code. Note that this
 * code is not part of the actual mobile app, but is intended to run
 * on a web server.
 */

#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/HighLevelTextDownloader.h>
#include "MyMessageHandler.h"
#include "MAHeaders.h"

/**
 * Helper class for downloading a list of photos from a server.
 * This downloader will delete itself upon completion.
 */
class MyPhotoListDownloader : public Wormhole::HighLevelTextDownloader
{
public:
	MAWidgetHandle mGalleryWebViewHandle;

	MyPhotoListDownloader(MAWidgetHandle galleryWebViewHandle) :
		mGalleryWebViewHandle(galleryWebViewHandle)
	{
	}

	virtual ~MyPhotoListDownloader()
	{
	}

	/**
	 * Called when download is complete.
	 * @param text Contains JSON with list of image urls, NULL on error.
	 */
	virtual void onDownloadComplete(char* text)
	{
		// URL array.
		MAUtil::String result;

		if (NULL != text)
		{
			// Download success, set result string (text contains a JSON array).
			result = "'";
			result += text;
			result += "'";

			// We need to free downloaded data.
			HighLevelTextDownloader::freeData(text);
		}
		else
		{
			// Download error.
			result = "null";
		}

		// Call JavaScript with the result.
		MAUtil::String url = "javascript:gallery.setPhotoList(";
		url += result;
		url += ")";

		maWidgetSetProperty(
			mGalleryWebViewHandle,
			MAW_WEB_VIEW_URL,
			url.c_str());

		// Delete me!
		delete this;
	}
};

/**
 * The application class.
 */
class MyMoblet : public Wormhole::WebAppMoblet
{
public:
	MyMoblet() :
		mMyMessageHandler(getWebView())
	{
		// Extract files in LocalFiles folder to the device.
		extractFileSystem();

		// Enable message sending from JavaScript to C++.
		enableWebViewMessages();

		// Hide the main WebView (we will use NativeUI to display
		// widgets and use this hidden WebView to handle application
		// logic written in JavaScript).
		getWebView()->setVisible(false);

		// Load page in the "LocalFiles" folder into the hidden WebView.
		//getWebView()->openURL("index.html");
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		mMyMessageHandler.init(BEEP_WAV, this);

		// Register functions to handle custom messages sent from JavaScript.
		mMyMessageHandler.addMessageFun(
			"DownloadPhotoList",
			(MyMessageHandlerFun)&MyMoblet::downloadPhotoList);
		mMyMessageHandler.addMessageFun(
			"CallJS",
			(MyMessageHandlerFun)&MyMoblet::callJSInWebView);
	}

	virtual ~MyMoblet()
	{
	}

	/**
	 * Here we handle HOOK_INVOKED events for WebViews in the app.
	 * This code enables WebViews to send messages to each other.
	 */
	void customEvent(const MAEvent& event)
	{
		if (EVENT_TYPE_WIDGET == event.type)
		{
			MAWidgetEventData* widgetEventData = (MAWidgetEventData*)event.data;
			MAWidgetHandle widgetHandle = widgetEventData->widgetHandle;

			// If target object is the main WebView, then we just return
			// because this is handled by the NativeUI library event processing.
			if (getWebView()->getWidgetHandle() == widgetHandle)
			{
				return;
			}

			// Process HOOK_INVOKED messages. This makes CallJS messages work.
			if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEventData->eventType)
			{
				//int hookType = widgetEventData->hookType;
				MAHandle data = widgetEventData->urlData;

				// Works with NULL as first param as long as the the only
				// thing we do is passing messages to other WebViews.
				// This is done by custom function callJSInWebView,
				// which is invoked via handleWebViewMessage.
				handleWebViewMessage(NULL, data);
				//handleWebViewMessage(getWebView(), data); // Alternative.

				// Free data.
				maDestroyPlaceholder(data);
			}
		}
	}

	/**
	 * This method is called when a key is pressed.
	 * Forwards the event to PhoneGapMessageHandler.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Forward to MyMessageHandler.
		mMyMessageHandler.keyPressEvent(keyCode, nativeCode);
	}

	/**
	 * Called from JavaScript to download a list of photos.
	 */
	void downloadPhotoList(Wormhole::MessageStream& stream)
	{
		// Get the Gallery WebView widget handle.
		int webViewHandle = MAUtil::stringToInteger(stream.getNext());

		// Get download url.
		const char* url = stream.getNext();

		// Initiate download.
		(new MyPhotoListDownloader(webViewHandle))->get(url);
	}

	/**
	 * Called from JavaScript to evaluate a JS script in a WebView.
	 */
	void callJSInWebView(Wormhole::MessageStream& stream)
	{
		// Get the native MoSync widget handle for the WebView
		// this call should be forwarded to.
		int webViewHandle = MAUtil::stringToInteger(stream.getNext());
		if (0 == webViewHandle)
		{
			// When the handle is zero, we should use the main
			// WebView (hidden in this app).
			webViewHandle = getWebView()->getWidgetHandle();
		}

		// Evaluate the JavaScript code in the WebView.
		callJS(webViewHandle, stream.getNext());
	}

	/**
	 * Evaluate JavaScript code in the given WebView.
	 */
	void callJS(MAWidgetHandle webViewHandle, const MAUtil::String& script)
	{
		// Call the JavaScript code on the WebView.
		MAUtil::String url = "javascript:" + script;
		maWidgetSetProperty(
			webViewHandle,
			MAW_WEB_VIEW_URL,
			url.c_str());
	}

	/**
	 * This method handles messages sent from the WebView.
	 *
	 * Note that the data object will be valid only during
	 * the life-time of the call of this method, then it
	 * will be deallocated.
	 *
	 * @param webView The WebView that sent the message.
	 * @param urlData Data object that holds message content.
	 */
	void handleWebViewMessage(NativeUI::WebView* webView, MAHandle data)
	{
		// Uncomment to print message data for debugging.
		// You need to build the project in debug mode for
		// the log output to be displayed.
		//printMessage(data);

		mMyMessageHandler.handleWebViewMessage(webView, data, this);
	}

	/**
	 * For debugging.
	 */
	void printMessage(MAHandle dataHandle)
	{
		// Get length of the data, it is not zero terminated.
		int dataSize = maGetDataSize(dataHandle);

		// Allocate buffer for string data.
		char* stringData = (char*) malloc(dataSize + 1);

		// Get the data.
		maReadData(dataHandle, stringData, 0, dataSize);

		// Zero terminate.
		stringData[dataSize] = 0;

		// Print unparsed message data.
		maWriteLog("@@@ MOSYNC Message:", 19);
		maWriteLog(stringData, dataSize);

		free(stringData);
	}

private:
	MyMessageHandler mMyMessageHandler;
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	MAUtil::Moblet::run(new MyMoblet());

	return 0;
}
