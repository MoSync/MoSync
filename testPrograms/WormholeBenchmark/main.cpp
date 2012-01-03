/**
 * @file main.cpp
 *
 * Sample application that illustrates how to call into C++
 * from JavaScript.
 */

// Include Moblet for web applications.
#include <Wormhole/WebAppMoblet.h>

#include <conprint.h>
#include "WebViewMessage.h"

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
		getWebView()->enableZoom();

		// The page in the "LocalFiles" folder to
		// show when the application starts.
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
	void handleWebViewMessage(WebView* webView, MAHandle urlData)
	{
		App::WebViewMessage message(webView, urlData);

		while (message.next())
		{
			if (message.is("ThroughputMessage"))
			{
				//lprintfln("C++ ThroughputMessage");
				// No action.
			}
			else if (message.is("ThroughputEnd"))
			{
				//lprintfln("C++ ThroughputEnd");
				callJS("ThroughputEnd()");
			}
			else if (message.is("DataThroughputMessage"))
			{
				//lprintfln("C++ DataThroughputMessage");
	//			String data = message.getParam("data");
	//			if (data[data.size() - 1] != 'b')
	//			{
	//				maPanic(0, "DataThroughputMessage: Data not ok");
	//			}
			}
			else if (message.is("DataThroughputEnd"))
			{
				//lprintfln("C++ DataThroughputEnd");
				callJS("DataThroughputEnd()");
			}
			else if (message.is("RoundtripMessage"))
			{
				//lprintfln("C++ RoundtripMessage");
				callJS("RoundtripCallback()");
			}
			else if (message.is("UrlDecodingTest"))
			{
	//			int iterations = 100;
	//
	//			int startTime = maGetMilliSecondCount();
	//
	//			for (int i = 0; i < iterations; ++i)
	//			{
	//				App::WebViewMessage msg(webView, urlData);
	//				String data = msg.getParam("data");
	//				if (data[data.size() - 1] != 'b')
	//				{
	//					maPanic(0, "UrlDecodingTest: Data not ok");
	//				}
	//			}
	//
	//			int timeTaken = maGetMilliSecondCount() - startTime;
	//
	//			float decodingsPerSecond = ((float)iterations / (float)timeTaken) * 1000.0;

				float decodingsPerSecond = 0;
				char buf[1024];
				sprintf(buf, "UrlDecodingResult(%f)", decodingsPerSecond);
				callJS(buf);
			}
			else
			{
				lprintfln("@@@ C++ Unknown message");
			}
		}

		// Tell the WebView that we have processed the message, so that
		// it can send the next one.
		//callJS("bridge.messagehandler.processedMessage()");
	}
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
