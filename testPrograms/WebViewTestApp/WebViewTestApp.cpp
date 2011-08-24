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
 * @file WebViewTestApp.cpp
 * @author Mikael Kindborg
 *
 * Application for testing the WebView widget.
 *
 * TODO: The app contains lots of experimental stuff
 * that could be better structured.
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

class WebViewTestApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebView;
	Platform* mPlatform;
	int mPageLoadCounter;

public:
	WebViewTestApp() : mPageLoadCounter(0)
	{
		// Create platform utility handler.
		mPlatform = Platform::create();

		createFiles();

		createUI();
	}

	virtual ~WebViewTestApp()
	{
		destroyUI();
	}

	void createFiles()
	{
		// Create local HTML files we want to refer to by urls.
		// We read data from resources and write to files.
		// TODO: This can be removed once automatic resource
		// unpacking of file systems is supported.

		mPlatform->writeTextToFile(
			mPlatform->getLocalPath() + "PageOne.html",
			mPlatform->createTextFromHandle(PageOne_html));

		mPlatform->writeTextToFile(
			mPlatform->getLocalPath() + "PageTwo.html",
			mPlatform->createTextFromHandle(PageTwo_html));
	}

	void createUI()
	{
		// Create screen.
		mScreen = maWidgetCreate(MAW_SCREEN);
		widgetShouldBeValid(mScreen, "Could not create screen");

		// Create web view
		mWebView = maWidgetCreate(MAW_WEB_VIEW);
		widgetShouldBeValid(mWebView, "Could not create web view");

		// Set size of the web view to fill the parent.
		maWidgetSetProperty(mWebView, "width", "-1");
		maWidgetSetProperty(mWebView, "height", "-1");

		// Set the initial HTML the web view displays.
		setMainPageHtml();

		// Enable zooming.
		maWidgetSetProperty(mWebView, "enableZoom", "true");

		// We want messages for this web view.
		WebViewMessage::getMessagesFor(mWebView);

		// Add the web view to the screen.
		maWidgetAddChild(mScreen, mWebView);

		// Show the screen.
		maWidgetScreenShow(mScreen);
	}

	void setMainPageHtml()
	{
		MAUtil::String html =
			mPlatform->createTextFromHandle(MainPage_html);
		maWidgetSetProperty(mWebView, "html", html.c_str());
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
						// Decrement page load counter. We use
						// this counter to determine the action
						// of the BACK key (on Android).
						--mPageLoadCounter;

						if (0 == mPageLoadCounter)
						{
							// Exit program.
							isRunning = false;
						}
						if (1 == mPageLoadCounter)
						{
							// Load the original page.
							mPageLoadCounter = 0;
							setMainPageHtml();
						}
						else
						{
							// Decrement one more since the load counter will
							// be incremented when the previous page loads.
							--mPageLoadCounter;

							// Go back one page.
							maWidgetSetProperty(
								mWebView,
								MAW_WEB_VIEW_NAVIGATE,
								"back");
						}
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
		if (MAW_EVENT_WEB_VIEW_CONTENT_LOADING == widgetEvent->eventType)
		{
			if (MAW_CONSTANT_DONE == widgetEvent->status)
			{
				++mPageLoadCounter;
			}
		}

		// Handle messages from the WebView widget.
		if (MAW_EVENT_WEB_VIEW_HOOK_INVOKED == widgetEvent->eventType &&
			MAW_CONSTANT_HARD == widgetEvent->hookType)
		{
			// Get message.
			WebViewMessage message(widgetEvent->urlData);

			lprintfln("@@@ message: %s", message.getMessageString().c_str());

			// Process message.
			if (message.is("BgColor"))
			{
				// This is calls JavaScript.
				setBgColor(message.getParam(0));
			}
			else if (message.is("ProcessData"))
			{
				// We just display the data without any processing to
				// demonstrate that MoSync can receive input and send
				// back a result to the WebView.

				// This is calls JavaScript.
				displayText(MAUtil::String("You wrote: ") + message.getParam(0));
			}
			else if (message.is("ExitApp"))
			{
				maExit(0);
			}
			else if (message.is("CreatePanic"))
			{
				maVibrate(5000);
				maPanic(0, "Panic created!");
			}
			else if (message.is("ShowAlert"))
			{
				maWidgetSetProperty(
					mWebView,
					"url",
					"javascript:alert('Hello World from MoSync C++')");
			}
			else if (message.is("GotoGoogle"))
			{
				maWidgetSetProperty(
					mWebView,
					"url",
					"http://google.com");
			}
			else if (message.is("GotoMoSync"))
			{
				maWidgetSetProperty(
					mWebView,
					"url",
					"http://mosync.com");
			}
		}
	}

	void setBgColor(const MAUtil::String& color)
	{
		char script[256];
		sprintf(
			script,
			"javascript:document.getElementById(\"ColorMessage\")"
			".innerHTML = '%s is a beautiful color.'; "
			"document.bgColor='%s';",
			color.c_str(),
			color.c_str());
		runScript(script);
	}

	void displayText(const MAUtil::String& text)
	{
		char script[256];
		sprintf(
			script,
			"javascript:document.getElementById(\"DataMessage\")"
			".innerHTML = '%s';",
			text.c_str());
		runScript(script);
	}

	void widgetShouldBeValid(MAWidgetHandle widget, const char* panicMessage)
	{
		if (widget <= 0)
		{
			maPanic(0, panicMessage);
		}
	}

	void runScript(const MAUtil::String& script)
	{
		maWidgetSetProperty(mWebView, "url", script.c_str());
	}
};
// End of class WebViewTestApp

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	WebViewTestApp app;
	app.runEventLoop();
	return 0;
}
