/**
 * @file main.cpp
 *
 * Sample application that illustrates how to call into C++
 * from JavaScript.
 */

#include <Wormhole/WebAppMoblet.h>
#include <conprint.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/MessageStreamJSON.h>
#include "NativeUIMessageHandler.h"
#include "ResourceMessageHandler.h"

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
		// Create message handler for NativeUI.
		mNativeUIMessageHandler = new NativeUIMessageHandler(getWebView());
		// Create message handler for Resources.
		mResourceMessageHandler = new ResourceMessageHandler(getWebView());

		// Enable message sending from JavaScript to C++.
		enableWebViewMessages();

		//The webview that processes our Javascript code it hidden
		// users can create other webviews from html
		getWebView()->disableZoom();
		getWebView()->setVisible(false);

		// Remove this line to enable the user to
		// zoom the web page. To disable zoom is one
		// way of making web pages display in a
		// reasonable degault size on devices with
		// different screen sizes.
		getWebView()->enableZoom();

		// The page in the "LocalFiles" folder to
		// show when the application starts.
		showPage("index.html");

		//Send the Device Screen size to JavaScript
		MAExtent scrSize = maGetScrSize();
		int width = EXTENT_X(scrSize);
		int height = EXTENT_Y(scrSize);
		char buf[512];
		sprintf(
				buf,
				"{mosyncScreenWidth=%d, mosyncScreenHeight = %d;}",
				width,
				height);

		lprintfln(buf);
		callJS(buf);
	}

	/**
	 * This method is called when a key is pressed. It closes
	 * the application when the back key (on Android) is pressed.
	 * Forwards the event to JavaScript by Calling the same function
	 * Implement the function to handle events in JavaScript
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		char buffer[256];
		// forward the event to application
		sprintf(buffer,
				"keyPressEvent(%d, %d)",
				keyCode,
				nativeCode);

		callJS(buffer);
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
		Wormhole::MessageProtocol protocol(data);

		if (protocol.isMessageStream())
		{
			 handleMessageStream(webView, data);
		}
		else if (protocol.isMessageStreamJSON())
		{
			 handleMessageStreamJSON(webView, data);
		}
		else
		{
			lprintfln("undefined message protocol");
		}
	}

	void handleMessageStream(WebView* webView, MAHandle data)
	{
		Wormhole::MessageStream stream(webView, data);

		const char* p;

		while (p = stream.getNext())
		{
			if (0 == strcmp(p, "NativeUI"))
			{
				lprintfln("received a NativeUI Message .......");
				//Forward NativeUI messages to the respective message handler
				mNativeUIMessageHandler->handleMessage(stream);
			}
			else if (0 == strcmp(p, "Resource"))
			{
				//Forward Resource messages to the respective message handler
				mResourceMessageHandler->handleMessage(stream);
			}
			else if (0 == strcmp(p, "close"))
			{
				close();
			}
		}
	}

	void handleMessageStreamJSON(WebView* webView, MAHandle data)
	{
		Wormhole::MessageStreamJSON message(webView, data);

		while (message.next())
		{
			handleJSONMessage(message);
		}
	}

	void handleJSONMessage(Wormhole::MessageStreamJSON& message)
	{
		if (message.is("JSONMessage"))
		{
			// No action.
		}
		else if (message.is("JSONMessageEnd"))
		{
			callJS("JSONMessageEnd()");
		}
		else if (message.is("JSONRoundtripMessage"))
		{
			callJS("JSONRoundtripCallback()");
		}
		else
		{
			lprintfln("@@@ C++ Unknown message");
		}
	}
private:
	NativeUIMessageHandler* mNativeUIMessageHandler;
	ResourceMessageHandler* mResourceMessageHandler;

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
