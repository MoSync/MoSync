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
 * @file GeoLocation.cpp
 * @author Mikael Kindborg
 *
 * Purpose
 * -------
 *
 * This is a low-level example that shows the basic mechanism for
 * communicating between JavaScript and C++. There are other examples
 * that demonstrate the use of high-level Wormhole classes to reduce
 * the amount of code you need to write in an application.
 *
 * This example is intended for the technically interested developer
 * who wants to learn how the JS to C++ bridge works at the lowest level,
 * without having to analyse all the code in the Wormhole library.
 *
 * Here, a stripped down code example is provided. The Wormhole C++
 * library is not used at all. Also, the mechanism of bundling files in
 * the LocalFiles folder is not used. The HTML and JavaScript is included
 * in one file, GeoLocation.html, which is included in the application as
 * a text resource. Since the communication mechanism is dependent on a
 * callback function on the JavaScript side, in the file mosync-bridge.js,
 * this example includes the relevant parts of mosync-bridge.js in the
 * file GeoLocation.html.
 *
 * Note that this example uses low-level syscalls to create the user
 * interface. You can use the C++ NativeUI library for this, which is
 * shown in other examples.
 *
 * When writing a real-work application, you are strongly recommended
 * to use the high-level mechanisms in the Wormhole library. This is
 * much more convenient and the underlying mechanism may change in the
 * future, which makes using the library a more robust alternative.
 *
 * What the application does
 * -------------------------
 *
 * The application displays the current location, which is fetched
 * via MoSync C++ code (sycalls maLocationStart and maLocationStop,
 * and events of type EVENT_TYPE_LOCATION) and communicated back
 * to the WebView by calling JavaScript from C++.
 */

#include <ma.h>				// MoSync API (base API).
#include <maheap.h>			// C memory allocation functions.
#include <mastring.h>		// C String functions.
#include <mavsprintf.h>		// sprintf etc.
#include <conprint.h>		// lprintfln.
#include <MAUtil/String.h>	// Class String.
#include <IX_WIDGET.h>		// Widget API.
#include "MAHeaders.h"		// Resource identifiers (not a physical file).

/**
 * Class with utility functions.
 */
class Util
{
public:
	/**
	 * Check if NativeUI is supported, panic with a help text
	 * for the user if not.
	 */
	static void checkNativeUISupport()
	{
		int widget = maWidgetCreate(MAW_WEB_VIEW);
		if (widget < 0)
		{
			maPanic(0, "NativeUI is only available on Android, iOS, and Windows Phone 7.");
		}
		else
		{
			maWidgetDestroy(widget);
		}
	}

	/**
	 * Return a String object based on the content of a data handle.
	 */
	static MAUtil::String createTextFromHandle(MAHandle handle)
	{
		// Get size of data.
		int size = maGetDataSize(handle);

		// Allocate space for text plus zero termination character.
		char* tempText = (char*) malloc(size + 1);
		if (NULL == tempText)
		{
			return "";
		}

	    // Read text data from handle.
	    maReadData(handle, tempText, 0, size);

	    // Zero terminate string.
	    tempText[size] = 0;

	    // Create String object.
	    MAUtil::String text = tempText;

	    // Free temporary text.
	    free(tempText);

	    // Return text object.
	    return text;
	}
};
// End of class Util.

/**
 * Class that handles the application life cycle.
 */
class WebViewGeoLocationApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebView;

public:
	WebViewGeoLocationApp()
	{
		// This app only runs on platforms that support NatiuveUI
		// and the WebView widget.
		Util::checkNativeUISupport();

		// Create the user interface. This displays
		// the web page.
		createUI();
	}

	virtual ~WebViewGeoLocationApp()
	{
		destroyUI();
	}

	void createUI()
	{
		// Create screen that holds the WebView.
		mScreen = maWidgetCreate("Screen");

		// Create the WebView.
		mWebView = createWebView();

		// Compose objects.
		maWidgetAddChild(mScreen, mWebView);

		// Show the screen.
		maWidgetScreenShow(mScreen);
	}

	MAWidgetHandle createWebView()
	{
		// Create the WebView
		MAWidgetHandle webView = maWidgetCreate("WebView");

		// Set size of the WebView to fill the parent.
		maWidgetSetProperty(webView, "width", "-1");
		maWidgetSetProperty(webView, "height", "-1");

		// Enable zooming.
		maWidgetSetProperty(webView, "enableZoom", "true");

		// Get the HTML for the page from a resource.
		MAUtil::String html = Util::createTextFromHandle(INDEX_HTML);

		// Set the HTML the WebView displays.
		maWidgetSetProperty(webView, "html", html.c_str());

		return webView;
	}

	void destroyUI()
	{
		maWidgetDestroy(mScreen);
	}

	void runEventLoop()
	{
		MAEvent event;

		bool isRunning = true;
		while (isRunning)
		{
			maWait(0);
			while (maGetEvent(&event) && isRunning)
			{
				switch (event.type)
				{
					case EVENT_TYPE_CLOSE:
						isRunning = false;
						break;

					case EVENT_TYPE_KEY_PRESSED:
						// Exit the app if the back key (on Android) is pressed.
						if (event.key == MAK_BACK)
						{
							isRunning = false;
						}
						break;

					case EVENT_TYPE_WIDGET:
						handleWidgetEvent((MAWidgetEventData*) event.data);
						break;

					case EVENT_TYPE_LOCATION:
						handleGeoLocationEvent((MALocation*) event.data);
						break;
				}
			}
		}
	}

	/**
	 * Here we handle events from the web view. We turn on or off
	 * location tracking depending of the message sent from JavaScript.
	 */
	void handleWidgetEvent(MAWidgetEventData* widgetEvent)
	{
		// Handle messages from the WebView widget.
		if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEvent->eventType)
		{
			// Get message content, this is the url set in JavaScript.
			MAUtil::String message = Util::createTextFromHandle(
				widgetEvent->urlData);

			// Note: It is very important to free the data handle
			// when we are done with using it, since a new data object
			// is allocated for each message. (In the high-level library
			// this is done automatically.)
			maDestroyPlaceholder(widgetEvent->urlData);

			// Check the message string and execute the corresponding syscall.
			if (0 == message.find("StartTrackingGeoLocation"))
			{
				maLocationStart();
			}
			else
			if (0 == message.find("StopTrackingGeoLocation"))
			{
				maLocationStop();
			}
		}
	}

	/**
	 * We have got a location event.
	 * Call a JavaScript function with the updated location data.
	 * Setting the "url" property using the "javascript:" scheme
	 * evaluates the JavaScript code in the WebView.
	 */
	void handleGeoLocationEvent(MALocation* location)
	{
		char script[512];
		sprintf(
			script,
			"javascript:GeoLocationUpdated('%f','%f');",
			location->lat,
			location->lon);
		maWidgetSetProperty(mWebView, "url", script);
	}
};
// End of class WebViewGeoLocationApp.

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	WebViewGeoLocationApp app;
	app.runEventLoop();
	return 0;
}
