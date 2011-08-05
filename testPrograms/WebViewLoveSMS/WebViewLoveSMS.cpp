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
 * @file WebViewLoveSMS.cpp
 * @author Mikael Kindborg
 *
 * Application for sending Love SMSs.
 */

#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>
#include <IX_WIDGET.h>
#include "MAHeaders.h"
#include "WebViewUtil.h"

using namespace MoSync::UI;

class WebViewLoveSMSApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebView;
	PlatformUtil* mPlatformUtil;

public:
	WebViewLoveSMSApp()
	{
		createUI();
	}

	virtual ~WebViewLoveSMSApp()
	{
		destroyUI();
	}

	void createUI()
	{
		PlatformUtil::checkNativeUISupport();

		if (PlatformUtil::isAndroid())
		{
			mPlatformUtil = new PlatformUtilAndroid();
		}
		else if (PlatformUtil::isIOS())
		{
			mPlatformUtil = new PlatformUtilIOS();
		}

		MAUtil::String html =
			mPlatformUtil->getTextFromDataHandle(MainPage_html);

		// Create screen.
		mScreen = maWidgetCreate(MAW_SCREEN);
		widgetShouldBeValid(mScreen, "Could not create screen");

		// Create web view.
		mWebView = createWebView(html);

		// TODO: Use save phone no. insert via javascript: call.

		// Compose objects.
		maWidgetAddChild(mScreen, mWebView);

		// Show the screen.
		maWidgetScreenShow(mScreen);
	}

	MAWidgetHandle createWebView(const MAUtil::String& html)
	{
		// Create web view
		MAWidgetHandle webView = maWidgetCreate(MAW_WEB_VIEW);
		widgetShouldBeValid(webView, "Could not create web view");

		// Set size of vew view to fill the parent.
		maWidgetSetProperty(webView, "width", "-1");
		maWidgetSetProperty(webView, "height", "-1");

		// Enable zooming.
		maWidgetSetProperty(webView, "enableZoom", "true");

		// Set the HTML the web view displays.
		maWidgetSetProperty(webView, "html", html.c_str());

		WebViewMessage::getMessagesFor(webView);

		return webView;
	}

	void destroyUI()
	{
		maWidgetDestroy(mScreen);
		delete mPlatformUtil;
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
					if (event.key == MAK_MENU)
					{
						// TODO: Show menu in HTML/JS.
					}
					if (event.key == MAK_BACK)
					{
						isRunning = false;
					}
					break;

				case EVENT_TYPE_WIDGET:
					handleWidgetEvent((MAWidgetEventData*) event.data);
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

			if (message.is("SendLoveSMS"))
			{
			}

			if (message.is("SendKissSMS"))
			{
			}

			if (message.is("SetPhoneNo"))
			{
				// Save phone number here.
			}
		}
	}

//	void handleGeoLocationEvent(MALocation* location)
//	{
//		char script[512];
//		sprintf(
//			script,
//			"javascript:GeoLocationUpdated('%f','%f');",
//			location->lat,
//			location->lon);
//		maWidgetSetProperty(mWebView, "url", script);
//	}

	void widgetShouldBeValid(MAWidgetHandle widget, const char* panicMessage)
	{
		if (widget <= 0)
		{
			maPanic(0, panicMessage);
		}
	}
};
// End of class WebViewLoveSMSApp

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	WebViewLoveSMSApp app;
	app.runEventLoop();
	return 0;
}
