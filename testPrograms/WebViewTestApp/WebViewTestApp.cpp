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
 * @file Main.cpp
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

// Test that JavaScript runs in the browser.
#define HTML1 " \
	<html><body> \
    <script>for(var i = 0; i < 100; ++i) { document.write('Hello World ' + i + '</br>'); }</script> \
    </body></html>"

// Call MoSync via JavsScript from a clickable link.
#define HTML2 " \
	<html><body> \
	<a href=\"javascript:document.location = 'mosync://Notify/HelloWorld'\">Click Me</a></br> \
    </body></html>"

// Another way to specify the click action.
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
	<div><a href=\"#\" onclick=\"MoSyncMessage('CloseWebView')\">Close WebView</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
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
	<div><a href=\"#\" onclick=\"MoSyncMessage('CloseWebView')\">Close WebView</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"

// Test performance of JS background color change, if better than via MoSync roundtrip.
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
	<div><a href=\"#\" onclick=\"MoSyncMessage('CloseWebView')\">Close WebView</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncMessage('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"

#define BGCOLOR_SCRIPT "javascript: \
	document.getElementById(\"ColorMessage\").innerHTML = '%s is a beautiful color.'; \
	document.bgColor='%s'; \
	"

#define MESSAGE_SCRIPT "javascript: \
	document.getElementById(\"DataMessage\").innerHTML = '%s'; \
	"

/**
 * Class that reads and parses custom messages from a web view.
 */
class WebViewMessage
{
private:
	MAUtil::String mMessageString;

public:
	WebViewMessage(MAHandle dataHandle)
	{
		if (NULL != dataHandle)
		{
			// Get length of the data, it is not zero terminated.
			int dataSize = maGetDataSize(dataHandle);

			// Allocate buffer for string data.
			char* stringData = (char*) malloc(dataSize + 1);

			// Get the data.
			maReadData(dataHandle, stringData, 0, dataSize);

			// Zero terminate.
			stringData[dataSize] = 0;

			// Set string data.
			mMessageString = stringData;

			// Destroy string data.
			free(stringData);

			// Destroy the data handle
			maDestroyObject(dataHandle);
		}
	}

	virtual ~WebViewMessage()
	{
	}

	const char* getString()
	{
		return mMessageString.c_str();
	}

	int is(const MAUtil::String& messageName)
	{
		// Start of messageName should be found at start of message string.
			return 0 == mMessageString.find(messageName);
	}

	MAUtil::String getData()
	{
		// Must be at least three characters in a message
		// that has a data part.
		if (mMessageString.length() < 3)
		{
			return "";
		}

		// Find first slash.
		int index = mMessageString.find("/");
		if (MAUtil::String::npos == index)
		{
			return "";
		}

		// Return the substring after the slash.
		return mMessageString.substr(index + 1);
	}
};

class WebViewTestApp
{
private:
	MAWidgetHandle mScreen;
	MAWidgetHandle mWebView;

public:
	void createUI()
	{
		// Create screen.
		mScreen = maWidgetCreate(MAW_SCREEN);
		widgetShouldBeValid(mScreen, "Could not create screen");

		// Create web view
		mWebView = maWidgetCreate(MAW_WEB_VIEW);
		widgetShouldBeValid(mWebView, "Could not create web view");

		// Set size of vew view to fill the parent.
		maWidgetSetProperty(mWebView, "width", "-1");
		maWidgetSetProperty(mWebView, "height", "-1");

		// Set the HTML the web view displays.
		maWidgetSetProperty(mWebView, "html", HTML4);

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

		// Paint something into the drawable MoSync view to make it
		// visually identifiable.
		//maSetColor(0xFFFFFF);
		//maFillRect(0, 0, 2000, 2000);
		//maUpdateScreen();

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

				case EVENT_TYPE_POINTER_PRESSED:
					// If the WebView has been closed, touching
					// the screen will open it.
					//maWidgetOpen(webView, WIDGET_ROOT);
					break;

				case EVENT_TYPE_KEY_PRESSED:
					// TODO: Implement as a button bar.
//					if (event.key == MAK_2) { SetBgColor(webView, "Gray"); }
//					if (event.key == MAK_3) { SetBgColor(webView, "White"); }
//					if (event.key == MAK_4) { SetBgColor(webView, "Pink"); }
//					if (event.key == MAK_5) { SetBgColor(webView, "Orange"); }
//					if (event.key == MAK_6) { SetBgColor(webView, "Chocolate"); }
//					if (event.key == MAK_7) { SetBgColor(webView, "ABCDEF"); }
//					if (event.key == MAK_8) { SetBgColor(webView, "DeepPink"); }
					if (event.key == MAK_MENU) { /* TODO: Show menu in HTML/JS */ }
					if (event.key == MAK_BACK) { isRunning = false; }
					break;

				case EVENT_TYPE_WIDGET:
					handleWidgetEvent((MAWidgetEventData*) event.data);
					break;
			}
		}
	}

	void handleWidgetEvent(MAWidgetEventData* widgetEvent)
	{
		// Handle custom messages from the WebView widget.
		if (mWebView == widgetEvent->widgetHandle &&
			MAW_EVENT_CUSTOM_MESSAGE == widgetEvent->eventType)
		{
			// Get message.
			WebViewMessage message(widgetEvent->messageDataHandle);

			lprintfln("@@@ message: %s", message.getString());

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
				displayText(message.getData());
			}
			else if (message.is("CloseWebView"))
			{
				//maWidgetClose(mWebView);
			}
			else if (message.is("ExitApp"))
			{
				maExit(0);
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
	app.createUI();
	app.runEventLoop();
	app.destroyUI();
	return 0;
}

