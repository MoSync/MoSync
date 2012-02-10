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
 * @file main.cpp
 *
 * Sample application that illustrates how to call into C++
 * from JavaScript.
 */

#include <Wormhole/WebAppMoblet.h>
#include <Wormhole/MessageProtocol.h>
#include <Wormhole/MessageStream.h>
#include <Wormhole/MessageStreamJSON.h>
#include <conprint.h>

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
		// Extract files in LocalFiles folder to the device.
		extractFileSystem();

		// Enable message sending from JavaScript to C++.
		enableWebViewMessages();

		// Enable zooming.
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
		MessageStream stream(webView, data);

		const char* p;

		while (p = stream.getNext())
		{
			if (0 == strcmp(p, "StringMessage"))
			{
				// Get params.
				const char* p1 = stream.getNext();
				const char* p2 = stream.getNext();
//				lprintfln("@@@ p1: %s p2: %s", p1, p2);
			}
			else if (0 == strcmp(p, "StringMessageEnd"))
			{
				callJS("StringMessageEnd()");
			}
			else if (0 == strcmp(p, "StringRoundtripMessage"))
			{
				callJS("StringRoundtripCallback()");
			}
			else if (0 == strcmp(p, "UTF8Test"))
			{
				const char* p1 = stream.getNext();

				String result = "UTF8TestResult('";
				for (int i = 0; i < strlen(p1); ++i)
				{
					char buf[16];
					sprintf(buf, "%u", (unsigned char)(p1[i]));
					result += buf;
					result += " ";
				}
				result += "')";

				maWriteLog(result.c_str(), result.size());

				callJS(result);
			}
		}
	}

	void handleMessageStreamJSON(WebView* webView, MAHandle data)
	{
		MessageStreamJSON message(webView, data);

		while (message.next())
		{
			handleJSONMessage(message);
		}
	}

	void handleJSONMessage(MessageStreamJSON& message)
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
