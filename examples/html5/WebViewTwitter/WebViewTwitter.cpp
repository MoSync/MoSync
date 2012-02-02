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
 * @file WebViewTwitter.cpp
 * @author Ali Sarrafi
 *
 * Application that reads Twitter flows.
 *
 * This program illustrates how to use the jQTouch JavAScript
 * library in a MoSync WebView application. The program also
 * shows how to communicate from JavaScript to C++.
 *
 * The entire UI and most of the application logic is implemented
 * in JavaScript.
 */

#include <Wormhole/WebAppMoblet.h>	     // Moblet for web applications.
#include <Wormhole/MessageStreamJSON.h>	 // Messages from JavaScript.
#include "FileMessageHandler.h"          // Handles file messages.

using namespace MAUtil;
using namespace NativeUI;
using namespace Wormhole;

/**
 * The application class.
 */
class TwitterMoblet : public WebAppMoblet
{
private:
	/**
	 * Object that handles messages from JavaScript.
	 */
	FileMessageHandler mMessageHandler;

public:
	TwitterMoblet()
	{
		enableWebViewMessages();
		getWebView()->disableZoom();
		showPage("index.html");
	}

	/**
	 * This method handles messages sent from the WebView.
	 * @param webView The WebView that sent the message.
	 * @param urlData Data object that holds message content.
	 * Note that the data object will be valid only during
	 * the life-time of the call of this method, then it
	 * will be deallocated.
	 */
	void handleWebViewMessage(WebView* webView, MAHandle data)
	{
		printMessage(data);

		// Create message object. This parses the message.
		MessageStreamJSON message(webView, data);

		// Get next message.
		while (message.next())
		{
			// Let the message handler handle the message.
			mMessageHandler.handleMessage(message);
		}
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
};
// End of class TwitterMoblet

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	Moblet::run(new TwitterMoblet());
	return 0;
}
