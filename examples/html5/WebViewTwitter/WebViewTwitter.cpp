/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
