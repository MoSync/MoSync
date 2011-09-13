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
 * @file WebGeoLocation.cpp
 * @author Mikael Kindborg
 *
 * Application for testing communication between JavaScript in a
 * WebView and MoSync C++ code.
 *
 * The application displays the current location, which is fetched
 * via MoSync C++ code (sycalls maLocationStart and maLocationStop,
 * and events of type EVENT_TYPE_LOCATION) and communicated back
 * to the WebView by calling JavaScript.
 */

#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>
#include <IX_WIDGET.h>
#include "MAHeaders.h"
#include "WebViewUtil.h"

using namespace MoSync;

/**
 * Class that handles the application life cycle.
 */
class WebViewGeoLocationApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebView;
	Platform* mPlatform;

public:
	WebViewGeoLocationApp()
	{
		// Create an file utility object.
		mPlatform = Platform::create();

		// Create the user interface.
		createUI();
	}

	virtual ~WebViewGeoLocationApp()
	{
		destroyUI();
	}

	void createUI()
	{
		// Create screen that holds the WebView.
		mScreen = maWidgetCreate(MAW_SCREEN);
		widgetShouldBeValid(mScreen, "Could not create screen");

		// Create web view.
		mWebView = createWebView();

		// Compose objects.
		maWidgetAddChild(mScreen, mWebView);

		// Show the screen.
		maWidgetScreenShow(mScreen);
	}

	MAWidgetHandle createWebView()
	{
		// Create web view
		MAWidgetHandle webView = maWidgetCreate(MAW_WEB_VIEW);
		widgetShouldBeValid(webView, "Could not create web view");

		// Set size of the WebView to fill the parent.
		maWidgetSetProperty(webView, "width", "-1");
		maWidgetSetProperty(webView, "height", "-1");

		// Disable zooming. This should make the page display
		// in a readable zoom state.
		maWidgetSetProperty(webView, "enableZoom", "false");

		// Get the HTML for the page from a resource.
		MAUtil::String html =
			mPlatform->createTextFromHandle(GeoLocationPage_html);

		// Set the HTML the web view displays.
		maWidgetSetProperty(webView, "html", html.c_str());

		// Register a handler for JavaScript messages.
		WebViewMessage::getMessagesFor(webView);

		return webView;
	}

	void destroyUI()
	{
		maWidgetDestroy(mScreen);
		delete mPlatform;
	}

	void runEventLoop()
	{
		MAEvent event;

		bool isRunning = true;
		while (isRunning)
		{
			maWait(0);
			maGetEvent(&event);
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					isRunning = false;
					break;

				case EVENT_TYPE_KEY_PRESSED:
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

	void handleWidgetEvent(MAWidgetEventData* widgetEvent)
	{
		// Handle messages from the WebView widget.
		if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEvent->eventType &&
			MAW_CONSTANT_HARD == widgetEvent->hookType)
		{
			// Get message.
			WebViewMessage message(widgetEvent->urlData);

			if (message.is("StartTrackingGeoLocation"))
			{
				maLocationStart();
			}

			if (message.is("StopTrackingGeoLocation"))
			{
				maLocationStop();
			}
		}
	}

	/**
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

	/**
	 * Just a simple test that the widget was created.
	 */
	void widgetShouldBeValid(MAWidgetHandle widget, const char* panicMessage)
	{
		if (widget <= 0)
		{
			maPanic(0, panicMessage);
		}
	}
};
// End of class WebViewGeoLocationApp

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	WebViewGeoLocationApp app;
	app.runEventLoop();
	return 0;
}
