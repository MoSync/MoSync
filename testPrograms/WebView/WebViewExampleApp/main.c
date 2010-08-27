#include <ma.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAWebView.h>

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
		function MoSyncRequest(request) { \
			document.location = 'mosync://' + request; } \
		function ProcessData() { \
			var request = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncRequest(request); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"MoSyncRequest('BgColor/Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"MoSyncRequest('BgColor/Red')\">Red</a> \
		<a href=\"#\" onclick=\"MoSyncRequest('BgColor/Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncRequest('CloseWebView')\">Close WebView</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncRequest('ExitApp')\">Exit Application</a></div> \
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
		function MoSyncRequest(request) { \
			document.location = 'mosync://' + request; } \
		function ProcessData() { \
			var request = 'ProcessData/' + document.getElementById(\"DataField\").value; \
			MoSyncRequest(request); } \
	</script> \
	<div style=\"margin-top:0pt; margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a href=\"#\" onclick=\"MoSyncRequest('BgColor/Yellow')\">Yellow</a> \
		<a href=\"#\" onclick=\"MoSyncRequest('BgColor/Red')\">Red</a> \
		<a href=\"#\" onclick=\"MoSyncRequest('BgColor/Green')\">Green</a> \
	</div> \
	<div id=\"ColorMessage\"></div> \
	<div> \
		<input type='text' id='DataField' /> \
		<input type=\"button\" value=\"Press Me!\" onclick=\"ProcessData()\"/> \
	</div> \
	<div id=\"DataMessage\"></div> \
	<div><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncRequest('CloseWebView')\">Close WebView</a></div> \
	<div><a href=\"#\" onclick=\"MoSyncRequest('ExitApp')\">Exit Application</a></div> \
    </body> \
    </html>"

#define BGCOLOR_SCRIPT " \
	document.getElementById(\"ColorMessage\").innerHTML = '%s is a beautiful color!'; \
	document.bgColor='%s'; \
	"

#define MESSAGE_SCRIPT " \
	document.getElementById(\"DataMessage\").innerHTML = '%s'; \
	"

void SetBgColor(char* color)
{
	char* script = (char*) maWebViewAllocateData(strlen(BGCOLOR_SCRIPT) + strlen(color) + strlen(color) + 1);
	sprintf(script, BGCOLOR_SCRIPT, color, color);
	maWebViewEvaluateScript(script);
	maWebViewFreeData(script);
}

void DisplayData(char* data)
{
	char* script = (char*) maWebViewAllocateData(strlen(MESSAGE_SCRIPT) + strlen(data) + 1);
	sprintf(script, MESSAGE_SCRIPT, data);
	maWebViewEvaluateScript(script);
	maWebViewFreeData(script);
}

#ifndef COMMENT_OUT
int MAMain()
{
	MAEvent event;

	// Paint something into the drawable MoSync view to make it
	// visually identifiable.
	maSetColor(0xFF77FF);
	maFillRect(20, 20, 300, 300);
	maUpdateScreen();

	// Immediately open the WebView.
	maWebViewOpen();

	while (1)
	{
		while (0 != maGetEvent(&event))
		{
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_POINTER_PRESSED:
					// If the WebView has been closed, touching
					// the screen will open it.
					maWebViewOpen();
					break;

				case EVENT_TYPE_KEY_PRESSED:
					if (event.key == MAK_0) { maWebViewClose(); }
					if (event.key == MAK_1) { maWebViewOpen(); }
					if (event.key == MAK_2) { SetBgColor("Gray"); }
					if (event.key == MAK_3) { SetBgColor("White"); }
					if (event.key == MAK_4) { SetBgColor("Pink"); }
					if (event.key == MAK_5) { SetBgColor("Orange"); }
					if (event.key == MAK_6) { SetBgColor("Chocolate"); }
					if (event.key == MAK_7) { SetBgColor("ABCDEF"); }
					if (event.key == MAK_8) { SetBgColor("DeepPink"); }
					if (event.key == MAK_9) { SetBgColor("Gold"); }
					if (event.key == MAK_BACK) { maExit(0); }
					break;

				case EVENT_TYPE_WEBVIEW_OPENED:
					// Must wait to set the HTML until this event to make
					// sure the WebView is fully created before use.
					maWebViewSetHTML(HTML4);
					break;

				case EVENT_TYPE_WEBVIEW_REQUEST:
					// This is where requests from the WebView are handled.
					// I have used a block hete to be able to declare local
					// variables inside a case-statement (would be better to
					// move this to a separate function).
					{
						// Get request.
						int size = maWebViewGetRequestSize(event.key);
						char* request = (char*) maWebViewAllocateData(size);
						int result = maWebViewGetRequest(event.key, request, size);
						if (result < 1) { break; }

						// Process request.
						if (maWebViewRequestIs(request, "BgColor"))
						{
							char* color = maWebViewGetRequestData(request);
							if (color)
							{
								SetBgColor(color); // This is where JS is called.
								maWebViewFreeData(color);
							}
						}
						else if (maWebViewRequestIs(request, "ProcessData"))
						{
							// We just display the data without any processing to
							// demonstrate that MoSync can recieve input and send
							// back a result to the WebView.
							char* data = maWebViewGetRequestData(request);
							if (data)
							{
								DisplayData(data); // This is where JS is called.
								maWebViewFreeData(data);
							}
						}
						else if (maWebViewRequestIs(request, "CloseWebView"))
						{
							maWebViewClose();
						}
						else if (maWebViewRequestIs(request, "ExitApp"))
						{
							maExit(0);
						}
					}
					break;
			}
		}

		// TODO: Cannot call maWait, locks the WebView. Look into this.
		//maWait(0);
	}

	//maWebViewClose();

	return 0;
}
#endif
