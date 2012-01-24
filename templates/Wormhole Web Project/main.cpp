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
#include "MAHeaders.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole;

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet
{
public:
	MyMoblet()
	{
		// Create message handler for PhoneGap.
		mPhoneGapMessageHandler = new PhoneGapMessageHandler(getWebView());

		// Set the beep sound. This is defined in the
		// Resources/Resources.lst file. You can change
		// this by changing the sound file in that folder.
		mPhoneGapMessageHandler->setBeepSound(BEEP_WAV);

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
		mPhoneGapMessageHandler->initializePhoneGap();
	}

	virtual ~MyMoblet()
	{
		if (NULL != mPhoneGapMessageHandler)
		{
			delete mPhoneGapMessageHandler;
			mPhoneGapMessageHandler = NULL;
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
		mPhoneGapMessageHandler->processKeyEvent(keyCode, nativeCode);
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
				// This detects the PhoneGap protocol.
				if (message.is("PhoneGap"))
				{
					mPhoneGapMessageHandler->handlePhoneGapMessage(message);
				}

				// TODO: Add other protocols here as needed.
				// Use the messageName param as the protocol identifier,
				// them make else if statements to branch off message
				// handling to the respective modules.
			}
		}
		else if (protocol.isMessageStream())
		{
			handleMessageStream(webView, data);
		}
		else
		{
			lprintfln("@@@ MOSYNC: Undefined message protocol");
		}
	}

	/**
	 * Handles Stream messages(high performance), it is mainly used for NativeUI
	 *
	 * @param webView a pointer to the web view posting this message
	 * @param data the stream of messages
	 */
	void handleMessageStream(WebView* webView, MAHandle data)
	{
		MessageStream stream(webView, data);

		const char* p;

		while (p = stream.getNext())
		{
			//close the app on a close message
			if (0 == strcmp(p, "close"))
			{
				close();
			}
		}
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
