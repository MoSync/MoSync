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
	Wormhole::WebAppMoblet* mMoblet;

	MyPhotoListDownloader(Wormhole::WebAppMoblet* moblet)
	{
		mMoblet = moblet;
	}

	virtual ~MyPhotoListDownloader()
	{
	}

	/**
	 * Called when download is complete.
	 * @param text Contains JSON with list of image urls, NULL on error.
	 */
	void onDownloadComplete(char* text)
	{
		lprintfln("onDownloadComplete");
		if (NULL != text)
		{
			// Download success, call JavaScript with the result.
			MAUtil::String js = "app.setPhotoList('";
			js += text;
			js += "')";
			lprintfln("js: %s", js.c_str());
			mMoblet->callJS(js);

			// We need to free downloaded data explicitly, but the
			// downloader itself will be deleted automatically.
			HighLevelTextDownloader::freeData(text);
		}
		else
		{
			// Download error.
			mMoblet->callJS("app.setPhotoList(null)");
		}

		// Delete myself!
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

		// Show the WebView that contains the HTML/CSS UI
		// and the JavaScript code.
		getWebView()->setVisible(true);

		getWebView()->enableZoom();

		// The page in the "LocalFiles" folder to
		// show when the application starts.
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		mMyMessageHandler.init(BEEP_WAV, this);

		// Register a custom function to handle "DownloadPhotoList"
		// messages send from JavaScript.
		mMyMessageHandler.addMessageFun(
			"DownloadPhotoList",
			(MyMessageHandlerFun)&MyMoblet::downloadPhotoList);
	}

	virtual ~MyMoblet()
	{
		// Add cleanup code as needed.
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

	void downloadPhotoList(Wormhole::MessageStream& stream)
	{
		lprintfln("@@@ downloadPhotoList");

		// Get url.
		const char* url = stream.getNext();

		lprintfln("@@@ url: %s", url);

		// Start download.
		(new MyPhotoListDownloader(this))->get(url);
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
		printMessage(data);

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
