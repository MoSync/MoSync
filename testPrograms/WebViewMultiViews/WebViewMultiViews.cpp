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
 * @file WebViewMultiViews.cpp
 * @author Mikael Kindborg
 *
 * Application for testing multiple WebView widgets.
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

class WebViewMultiViewsApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebViews[4];
	MAWidgetHandle mButtonAllWhite;
	MAWidgetHandle mButtonAllBlack;
	PlatformUtil* mPlatformUtil;

public:
	WebViewMultiViewsApp()
	{
		createUI();
	}

	virtual ~WebViewMultiViewsApp()
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

		// Commented out this since file loading is not yet
		// implemented on iOS.

//		// Write HTML resource files to the local file system
//		// so they can be accessed by the web view.
//		// TODO: This can be removed once automatic resource
//		// unpacking of file systems is supported.
//		mPlatformUtil->writeTextToLocalFile(
//			"ColorPage.html",
//			mPlatformUtil->getTextFromDataHandle(ColorPage_html));

		// Create screen.
		mScreen = maWidgetCreate(MAW_SCREEN);
		widgetShouldBeValid(mScreen, "Could not create screen");

		// Create layouts.
		MAWidgetHandle verticalLayout = createLayout(MAW_VERTICAL_LAYOUT);
		MAWidgetHandle horizontalLayoutOne = createLayout(MAW_HORIZONTAL_LAYOUT);
		MAWidgetHandle horizontalLayoutTwo = createLayout(MAW_HORIZONTAL_LAYOUT);
		MAWidgetHandle horizontalLayoutThree = createLayout(MAW_HORIZONTAL_LAYOUT);

		// Shrink wrap last layout, which will be used for buttons.
		maWidgetSetProperty(horizontalLayoutThree, "height", "-2");

		// Create web views.
		mWebViews[0] = createWebView();
		mWebViews[1] = createWebView();
		mWebViews[2] = createWebView();
		mWebViews[3] = createWebView();

		mButtonAllWhite = createButton("All White");
		mButtonAllBlack = createButton("All Black");

		// Compose objects.
		maWidgetAddChild(verticalLayout, horizontalLayoutOne);
		maWidgetAddChild(verticalLayout, horizontalLayoutTwo);
		maWidgetAddChild(verticalLayout, horizontalLayoutThree);
		maWidgetAddChild(horizontalLayoutOne, mWebViews[0]);
		maWidgetAddChild(horizontalLayoutOne, mWebViews[1]);
		maWidgetAddChild(horizontalLayoutTwo, mWebViews[2]);
		maWidgetAddChild(horizontalLayoutTwo, mWebViews[3]);
		maWidgetAddChild(horizontalLayoutThree, mButtonAllWhite);
		maWidgetAddChild(horizontalLayoutThree, mButtonAllBlack);
		maWidgetAddChild(mScreen, verticalLayout);

		// Show the screen.
		maWidgetScreenShow(mScreen);
	}

	MAWidgetHandle createWebView()
	{
		// Create web view.
		MAWidgetHandle webView = maWidgetCreate(MAW_WEB_VIEW);
		widgetShouldBeValid(webView, "Could not create web view");

		// Set size of the web view to fill the parent.
		maWidgetSetProperty(webView, "width", "-1");
		maWidgetSetProperty(webView, "height", "-1");

		// Commented out this since file loading is not yet
		// implemented on iOS.

//		// Set the URL the web view displays.
//		maWidgetSetProperty(webView, "url", "ColorPage.html");

		// Instead using this code to load the HTML.

		// Set the HTML the web view displays.
		MAUtil::String html =
			mPlatformUtil->getTextFromDataHandle(ColorPage_html);
		maWidgetSetProperty(webView, "html", html.c_str());

		WebViewMessage::getMessagesFor(webView);

		return webView;
	}

	MAWidgetHandle createButton(const MAUtil::String& text)
	{
		// Create button.
		MAWidgetHandle button = maWidgetCreate(MAW_BUTTON);
		widgetShouldBeValid(button, "Could not create button");

		// Set button text.
		maWidgetSetProperty(button, "text", text.c_str());

		// Set size of the button to fill the parent horizontally
		// and to wrap content vertically.
		maWidgetSetProperty(button, "width", "-1");
		maWidgetSetProperty(button, "height", "-2");

		return button;
	}

	MAWidgetHandle createLayout(const char* type)
	{
		// Create web view
		MAWidgetHandle layout = maWidgetCreate(type);
		widgetShouldBeValid(layout, "Could not create layout");

		// Set size of the layout to fill the parent.
		maWidgetSetProperty(layout, "width", "-1");
		maWidgetSetProperty(layout, "height", "-1");

		return layout;
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

	/**
	 * Change background colors of web views when we get the
	 * OnBackgorundClicked message, and also handle button clicks.
	 */
	void handleWidgetEvent(MAWidgetEventData* widgetEvent)
	{
		// Get handle to the widget.
		MAWidgetHandle widget = widgetEvent->widgetHandle;

		// Handle custom messages from the WebView widgets.
		if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEvent->eventType &&
			MAW_CONSTANT_HARD == widgetEvent->hookType)
		{
			// Get message.
			WebViewMessage message(widgetEvent->urlData);

			// This is the only message we handle.
			if ( ! message.is("OnBackgroundClick") )
			{
				return;
			}

			maVibrate(500);

			if (widget == mWebViews[0])
			{
				setWebViewBackgroundColor(mWebViews[0], "red");
			}
			if (widget == mWebViews[1])
			{
				setWebViewBackgroundColor(mWebViews[0], "blue");
				setWebViewBackgroundColor(mWebViews[1], "green");
			}
			if (widget == mWebViews[2])
			{
				setWebViewBackgroundColor(mWebViews[0], "yellow");
				setWebViewBackgroundColor(mWebViews[1], "orange");
				setWebViewBackgroundColor(mWebViews[2], "pink");
			}
			if (widget == mWebViews[3])
			{
				setWebViewBackgroundColor(mWebViews[0], "black");
				setWebViewBackgroundColor(mWebViews[1], "white");
				setWebViewBackgroundColor(mWebViews[2], "white");
				setWebViewBackgroundColor(mWebViews[3], "black");
			}
		}

		if (MAW_EVENT_CLICKED == widgetEvent->eventType)
		{
			if (widget == mButtonAllWhite)
			{
				setWebViewBackgroundColor(mWebViews[0], "white");
				setWebViewBackgroundColor(mWebViews[1], "white");
				setWebViewBackgroundColor(mWebViews[2], "white");
				setWebViewBackgroundColor(mWebViews[3], "white");
			}
			if (widget == mButtonAllBlack)
			{
				setWebViewBackgroundColor(mWebViews[0], "black");
				setWebViewBackgroundColor(mWebViews[1], "black");
				setWebViewBackgroundColor(mWebViews[2], "black");
				setWebViewBackgroundColor(mWebViews[3], "black");
			}
		}
	}

	void setWebViewBackgroundColor(
		MAWidgetHandle webView,
		const MAUtil::String& color)
	{
		char script[256];
		sprintf(
			script,
			"javascript:document.bgColor='%s';",
			color.c_str());
		maWidgetSetProperty(webView, "url", script);
	}

	void widgetShouldBeValid(MAWidgetHandle widget, const char* panicMessage)
	{
		if (widget <= 0)
		{
			maPanic(0, panicMessage);
		}
	}
};
// End of class WebViewMultiViews

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	WebViewMultiViewsApp app;
	app.runEventLoop();
	return 0;
}
