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
 * This file contains the support code at the C++ level for an HTML5/JS
 * application that can access device services from JavaScript.
 *
 * You don't need to change anything in this code file unless you
 * wish to add support for functions not available out-of-the box
 * in wormhole.js.
 *
 * When reading the code below, it is good to know that there are
 * two message formats: JSON and string streams. String streams are
 * generally faster. See comments in the code below for further details.
 * PhoneGap uses JSON messages, NativeUI uses string streams.
 */

#include <Wormhole/WebAppMoblet.h>
//#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
//#include <Wormhole/Libs/JSONMessage.h>
//#include <Wormhole/Encoder.h>
#include "MyMessageHandler.h"
#include "MAHeaders.h"

// Namespaces we want to access.
//using namespace MAUtil; // Class Moblet
//using namespace NativeUI; // WebView widget.
//using namespace Wormhole; // Wormhole library.

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

		// The page in the "LocalFiles" folder to
		// show when the application starts.
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		mMyMessageHandler.init(BEEP_WAV, this);

		MyMessageHandlerFun fun = (MyMessageHandlerFun)&MyMoblet::foo;
		mMyMessageHandler.addMessageFun("DownloadLatestPhotoURLs", fun);
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

	void foo(const Wormhole::MessageStream& stream)
	{
		lprintfln("@@@ foo!!!!");
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
