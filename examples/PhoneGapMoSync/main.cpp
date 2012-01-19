/**
 * @file main.cpp
 *
 * Sample application that illustrates how to call into C++
 * from JavaScript.
 */

// Include Moblet for web applications.
#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include "Libs/JSONMessageHandler.h"

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
		// Create message handler for PhoneGap.
		mJSONMessageHandler = new JSONMessageHandler(getWebView());

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

		// Initialize PhoneGap.
		mJSONMessageHandler->initializePhoneGap();
	}

	virtual ~MyMoblet()
	{
		if (NULL != mJSONMessageHandler)
		{
			delete mJSONMessageHandler;
			mJSONMessageHandler = NULL;
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

		lprintfln("@@@ MOSYNC Message: %s\n", stringData);

		free(stringData);
	}

	/**
	 * This method is called when a key is pressed.
	 * Forwards the event to PhoneGapMessageHandler.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Forward to PhoneGap MessageHandler.
		mJSONMessageHandler->processKeyEvent(keyCode, nativeCode);
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
		// For debugging.
		printMessage(data);

		// Check the message protocol.
		MessageProtocol protocol(data);
		if (protocol.isMessageArrayJSON())
		{
			// Create the message object. This parses the message data.
			// The message object contains one or more messages.
			JSONMessage message(webView, data);

			// Loop through messages.
			while (message.next())
			{
				lprintfln("@@@ MOSYNC: Got next message");
				// This detects the PhoneGap protocol.
				if (message.is("PhoneGap"))
				{
					lprintfln("@@@ MOSYNC: Message is phonegap");
					mJSONMessageHandler->handlePhoneGapMessage(message);
				}

				// TODO: Add other protocols here as needed.
				// Use the messageName param as the protocol identifier,
				// them make else if statements to branch off message
				// handling to the respective modules.
			}
		}
		else if (protocol.isMessageStream())
		{
			 // TODO: Add message handler if string streams are used.
		}
		else
		{
			lprintfln("@@@ MOSYNC: Undefined message protocol");
		}
	}

private:
	JSONMessageHandler* mJSONMessageHandler;
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
