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
 */

#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAUtil/String.h>
#include <IX_WIDGET.h>

#include "WebViewUtil.h"

using namespace MoSync::UI;

// Test that JavaScript runs in the browser.
#define HTML1 " \
	<html><body> \
    <script>for(var i = 0; i < 100; ++i) { document.write('Hello World ' + i + '</br>'); }</script> \
    </body></html>"

// HTML2 has been removed.

// Call MoSync via JavsScript from a clickable link.
#define HTML3 " \
	<html><body> \
	<a href=\"#\" onclick=\"document.location = 'mosync://Notify/HelloWorld'\">Click Me</a></br> \
    </body></html>"

// Test many features in one example.
#define HTML4 " \
	<html> \
	<head> \
    <style type=\"text/css\"> \
		div { font-size:1.8em; margin-top:20pt; } \
		a { cursor: hand; } \
		input { font-size:1.2em; } \
	</style> \
	</head> \
	<body> \
	<script> \
		function MoSyncMessage(message) { \
			document.location = 'mosync://' + message; } \
		function ProcessData() { \
			var message = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncMessage(message); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Red')\">Red</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('CreatePanic')\">Create Panic</a></div> \
    </body> \
    </html>"

// Test that percent signs work (was an issue on Android).
#define HTML5 " \
	<html> \
	<head> \
    <style type=\"text/css\"> \
		div { font-size:150%; margin-top:20pt; } \
		a { cursor: hand; } \
		input { font-size:120%; } \
	</style> \
	</head> \
	<body> \
	<script> \
		function MoSyncMessage(message) { \
			document.location = 'mosync://' + message; } \
		function ProcessData() { \
			var message = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncMessage(message); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Red')\">Red</a> \
		<a href=\"#\" onclick=\"MoSyncMessage('BgColor/Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"

// Test subjective performance of JS background color change,
// if better than via MoSync roundtrip.
#define HTML6 " \
	<html> \
	<head> \
    <style type=\"text/css\"> \
		div { font-size:1.8em; margin-top:20pt; } \
		a { cursor: hand; } \
		input { font-size:1.2em; } \
	</style> \
	</head> \
	<body> \
	<script> \
		function SetBgColor(color) { \
			document.getElementById('ColorMessage').innerHTML = color + ' is a beautiful color!'; \
			document.bgColor = color; } \
		function MoSyncMessage(message) { \
			document.location = 'mosync://' + message; } \
		function ProcessData() { \
			var message = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncMessage(message); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"SetBgColor('Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"SetBgColor('Red')\">Red</a> \
		<a href=\"#\" onclick=\"SetBgColor('Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"

// Script to set the background color of the web page.
#define BGCOLOR_SCRIPT "javascript: \
	document.getElementById(\"ColorMessage\").innerHTML = '%s is a beautiful color.'; \
	document.bgColor='%s';"

// Script to display a text on the web page.
#define MESSAGE_SCRIPT "javascript: \
	document.getElementById(\"DataMessage\").innerHTML = '%s';"

class WebViewTestApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebView;

public:
	WebViewTestApp()
	{
		createUI();
	}

	virtual ~WebViewTestApp()
	{
		destroyUI();
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

		// Set the HTML the web view displays.
		maWidgetSetProperty(mWebView, "html", HTML4);

		// Enable zooming.
		maWidgetSetProperty(mWebView, "enableZoom", "true");

		// We want messages for this web view.
		WebViewMessage::getMessagesFor(mWebView);

		// Add the web view to the screen.
		maWidgetAddChild(mScreen, mWebView);

		// Show the screen.
		maWidgetScreenShow(mScreen);
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

			lprintfln("@@@ message: %s", message.getMessageString().c_str());

			// Process message.
			if (message.is("BgColor"))
			{
				// This is calls JavaScript.
				setBgColor(message.getData());
			}
			else if (message.is("ProcessData"))
			{
				// We just display the data without any processing to
				// demonstrate that MoSync can receive input and send
				// back a result to the WebView.

				// This is calls JavaScript.
				displayText(MAUtil::String("You wrote: ") + message.getData());
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
		}
	}

	void setBgColor(const MAUtil::String& color)
	{
		char script[256];
		sprintf(script, BGCOLOR_SCRIPT, color.c_str(), color.c_str());
		runScript(script);
	}

	void displayText(const MAUtil::String& text)
	{
		char script[256];
		sprintf(script, MESSAGE_SCRIPT, text.c_str());
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

