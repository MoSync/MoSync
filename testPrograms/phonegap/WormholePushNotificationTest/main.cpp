/**
 * @file main.cpp
 *
 * Sample application that illustrates how to call into C++
 * from JavaScript.
 */

// Include Moblet for web applications.
#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/Libs/JSONMessage.h>
#include <Wormhole/Libs/PhoneGap/PhoneGapMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/NativeUIMessageHandler.h>
#include <Wormhole/Libs/JSNativeUI/ResourceMessageHandler.h>

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Class WebAppMoblet

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet
{
public:
	MyMoblet()
	{
		// Enable message sending from JavaScript to C++.
		enableWebViewMessages();

		// Remove this line to enable the user to
		// zoom the web page. To disable zoom is one
		// way of making web pages display in a
		// reasonable degault size on devices with
		// different screen sizes.
		getWebView()->disableZoom();

		// The page in the "LocalFiles" folder to
		// show when the application starts.
		showPage("index.html");

		mPhoneGapMessageHandler = new PhoneGapMessageHandler(getWebView());
	}

	virtual ~MyMoblet()
	{
		delete mPhoneGapMessageHandler;
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

		lprintfln("@@@ MOSYNC Message: %s\n", stringData);

		free(stringData);
	}

	/**
	 * This method handles messages sent from the WebView.
	 * @param webView The WebView that sent the message.
	 * @param urlData Data object that holds message content.
	 * Note that the data object will be valid only during
	 * the life-time of the call of this method, then it
	 * will be deallocated.
	 */
	void handleWebViewMessage(WebView* webView, MAHandle urlData)
	{
		printf("handleWebViewMessage");
		this->printMessage(urlData);

		MessageProtocol protocol(urlData);
		if (protocol.isMessageStreamJSON())
		{
			// Create the message object. This parses the message data.
			// The message object contains one or more messages.
			JSONMessage message(webView, urlData);

			// Loop through messages.
			while (message.next())
			{
				// This detects the PhoneGap protocol.
				if (message.is("PhoneGap"))
				{
					mPhoneGapMessageHandler->handlePhoneGapMessage(message);
				}
			}
		}
		// Tell the WebView that we have processed the message, so that
		// it can send the next one.
		callJS("bridge.messagehandler.processedMessage()");
	}

private:
	PhoneGapMessageHandler* mPhoneGapMessageHandler;
};

/**
 * Main function that is called when the program starts.
 * Here an instance of the MyMoblet class is created and
 * the program enters the main event loop.
 */
extern "C" int MAMain()
{
	Moblet::run(new MyMoblet());
	return 0;
}
