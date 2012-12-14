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
 * @author Ali Sarrafi, Mikael Kindborg
 *
 * Application that reads Twitter flows.
 *
 * The entire UI and most of the application logic is implemented
 * in JavaScript.
 *
 * This program illustrates how to send messages between JavaScript
 * and C++. The app uses a simple JavaScript File API, which is
 * implemented in C++.
 */

#include <conprint.h>
#include <Wormhole/HybridMoblet.h>
#include "FileMessageHandler.h"   // Custom File API.
#include "MAHeaders.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Wormhole library.

/**
 * The application class.
 */
class TwitterMoblet : public HybridMoblet
{
public:
	TwitterMoblet()
	{
		// Show the start page. This will also perform initialization if needed.
		showPage("index.html");

		// The beep sound is defined in file "Resources/Resources.lst".
		setBeepSound(BEEP_WAV);

		// Turn off zoom.
		getWebView()->disableZoom();

		// Register functions to handle custom messages sent from JavaScript.
		addMessageFun(
			"File",
			(FunTable::MessageHandlerFun)&TwitterMoblet::handleFileMessage);
	}

	virtual ~TwitterMoblet()
	{
		// Add cleanup code as needed.
	}

	void handleFileMessage(Wormhole::MessageStream& message)
	{
		mMessageHandler.handleMessage(message);
	}

private:
	/**
	 * Object that handles messages from JavaScript.
	 */
	FileMessageHandler mMessageHandler;
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	(new TwitterMoblet())->enterEventLoop();
	return 0;
}
