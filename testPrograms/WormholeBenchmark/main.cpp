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
#include "MessageStream.h"

// Namespaces we want to access.
using namespace MAUtil; // Class Moblet
using namespace NativeUI; // WebView widget.
using namespace Wormhole; // Class WebAppMoblet

class MessageProtocol
{
public:
	MessageProtocol(MAHandle dataHandle)
	{
		// Get the protocol.
		maReadData(dataHandle, mProtocol, 0, 3);
	}

	virtual ~MessageProtocol()
	{
	}

	bool isMessageArray()
	{
		if (mProtocol[0] != 'm') { return false; }
		if (mProtocol[1] != 'a') { return false; }
		if (mProtocol[2] != ':') { return false; }
		return true;
	}

	bool isMessageStream()
	{
		if (mProtocol[0] != 'm') { return false; }
		if (mProtocol[1] != 's') { return false; }
		if (mProtocol[2] != ':') { return false; }
		return true;
	}

private:
	char mProtocol[3];
};

void foo()
{
	char* s1 = (char*) malloc(10);
	char* s2 = (char*) malloc(10);
	strcpy(s1, "AAAA");
	strcpy(s2, "AAAA");
	int i1 = *((int*)s1);
	int i2 = *((int*)s2);
	if (i1 == i2)
		lprintfln("@@@@@@@@@@@@@@@@@@@@@@ Yes\n");
	else
		lprintfln("@@@@@@@@@@@@@@@@@@@@@@ No\n");
}

/**
 * The application class.
 */
class MyMoblet : public WebAppMoblet
{
public:
	MyMoblet()
	{
		foo();

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
	void handleWebViewMessage(WebView* webView, MAHandle data)
	{
		MessageProtocol protocol(data);

//		lprintfln("handleWebViewMessage");

		if (protocol.isMessageStream())
		{
			 handleMessageStream(webView, data);
		}
		else if (protocol.isMessageArray())
		{
			 handleMessageArray(webView, data);
		}
		else
		{
			lprintfln("undefined message protocol");
		}
	}

	void handleMessageArray(WebView* webView, MAHandle data)
	{
//		lprintfln("handleMessageArray");

		App::WebViewMessage message(webView, data);

		while (message.next())
		{
			handleJSONMessage(message);

			// Tell the WebView that we have processed the message, so that
			// it can send the next one.
			//callJS("bridge.messagehandler.processedMessage()");
		}
	}

	void handleJSONMessage(App::WebViewMessage& message)
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

	void handleMessageStream(WebView* webView, MAHandle data)
	{
//		lprintfln("handleMessageStream");

		App::MessageStream stream(webView, data);

		char* rawStartString = "RAWS";
		char* rawEndString = "RAWE";
//		int rawStart = *((int*)rawStartString);
//		int rawEnd = *((int*)rawEndString);

		const char* p;

		while (p = stream.getNext())
		{
//			lprintfln("handleMessageStream: @%s@", p);
//
//			int message = *((int*)p);
//
//			lprintfln("@ message: %u rawStart: %u", message, rawStart);
//			lprintfln("@ strcmp: %i", strcmp(rawStartString, p));
//			lprintfln("@ strlen p: %i", strlen(p));
//
//			lprintfln("@@ C %i %i %i %i @@ DATA %i %i %i %i",
//				rawStartString[0], rawStartString[1], rawStartString[2], rawStartString[3],
//				p[0], p[1], p[2], p[3]);

			if (0 == strcmp(p, rawStartString))
			{
				// Get params.
				const char* p1 = stream.getNext();
				const char* p2 = stream.getNext();
//				lprintfln("@@@ p1: %s p2: %s", p1, p2);
			}
			else if (0 == strcmp(p, rawEndString))
			{
				callJS("RawMessageEnd()");
			}
		}
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
