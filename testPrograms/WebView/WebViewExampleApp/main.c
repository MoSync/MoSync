#include <ma.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <WidgetMessageUtil.h>

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
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
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
	document.bgColor='Pink'; \
	"

#define MESSAGE_SCRIPT "javascript: \
	document.getElementById(\"DataMessage\").innerHTML = '%s'; \
	"

static void SetBgColor(int webView, char* color)
{
	char* script = (char*) WidgetMessageAllocateString(strlen(BGCOLOR_SCRIPT) + strlen(color) + strlen(color) + 1);
	sprintf(script, BGCOLOR_SCRIPT, color, color);
	maWidgetEvaluateScript(webView, script);
	WidgetMessageFreeString(script);
}

static void DisplayData(int webView, char* data)
{
	char* script = (char*) WidgetMessageAllocateString(strlen(MESSAGE_SCRIPT) + strlen(data) + 1);
	sprintf(script, MESSAGE_SCRIPT, data);
	maWidgetEvaluateScript(webView, script);
	WidgetMessageFreeString(script);
}

static void HandleMessage(MAHandle webView, MAEvent event)
{
	if (webView != event.widgetHandle)
	{
		return;
	}

	// Get message.
	char* message = WidgetMessageGet(event.widgetMessageId);
	lprintfln("message: %s", message);

	if (NULL == message)
	{
		return;
	}

	// Process message.
	if (WidgetMessageNameIs(message, "BgColor"))
	{
		char* color = WidgetMessageGetData(message);
		if (color)
		{
			SetBgColor(webView, color); // This is where JS is called.
			WidgetMessageFreeString(color);
		}
	}
	else if (WidgetMessageNameIs(message, "ProcessData"))
	{
		// We just display the data without any processing to
		// demonstrate that MoSync can recieve input and send
		// back a result to the WebView.
		char* data = WidgetMessageGetData(message);
		if (data)
		{
			DisplayData(webView, data); // This is where JS is called.
			WidgetMessageFreeString(data);
		}
	}
	else if (WidgetMessageNameIs(message, "CloseWebView"))
	{
		maWidgetClose(webView);
	}
	else if (WidgetMessageNameIs(message, "ExitApp"))
	{
		maExit(0);
	}
	WidgetMessageFreeString(message);
}

int MAMain()
{
	MAEvent event;

	// Paint something into the drawable MoSync view to make it
	// visually identifiable.
	//maSetColor(0xFFFFFF);
	//maFillRect(0, 0, 2000, 2000);
	//maUpdateScreen();

	// Immediately open the WebView.
	MAHandle webView = maWidgetCreate(WIDGET_TYPE_WEBVIEW);
	if (WIDGET_ERROR == webView)
	{
		maExit(0);
	}

	lprintfln("Opening WebView");
	int r = maWidgetOpen(webView, WIDGET_ROOT);
	lprintfln("Opening WebView result: %i", r);

	while (1)
	{
		lprintfln("maGetEvent");
		while (0 != maGetEvent(&event))
		{
			lprintfln("switch event.type: %i", event.type);
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_POINTER_PRESSED:
					// If the WebView has been closed, touching
					// the screen will open it.
					maWidgetOpen(webView, WIDGET_ROOT);
					break;

				case EVENT_TYPE_KEY_PRESSED:
					if (event.key == MAK_0) { maWidgetClose(webView); }
					if (event.key == MAK_1) { maWidgetOpen(webView, WIDGET_ROOT); }
					if (event.key == MAK_2) { SetBgColor(webView, "Gray"); }
					if (event.key == MAK_3) { SetBgColor(webView, "White"); }
					if (event.key == MAK_4) { SetBgColor(webView, "Pink"); }
					if (event.key == MAK_5) { SetBgColor(webView, "Orange"); }
					if (event.key == MAK_6) { SetBgColor(webView, "Chocolate"); }
					if (event.key == MAK_7) { SetBgColor(webView, "ABCDEF"); }
					if (event.key == MAK_8) { SetBgColor(webView, "DeepPink"); }
					if (event.key == MAK_9) { SetBgColor(webView, "Gold"); }
					if (event.key == MAK_BACK) { maExit(0); }
					break;

				case EVENT_TYPE_WIDGET_OPENED:
					// Must wait to set the HTML until this event to make
					// sure the WebView is fully created before use.

					lprintfln("EVENT_TYPE_WIDGET_OPENED");
					maWidgetLoadHTML(webView, HTML4);
					break;

				case EVENT_TYPE_WIDGET_MESSAGE:
					// Handle messages from the WebView widget here.
					HandleMessage(webView, event);
					break;
			}
		}

		// TODO: Cannot call maWait, locks the WebView. Look into this.
		//maWait(0);
	}

	//maWidgetClose(webView);
	maWidgetDestroy(webView);

	return 0;
}
