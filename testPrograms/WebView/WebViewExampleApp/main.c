#include <ma.h>
#include <mastring.h>
#include <mavsprintf.h>
#include <MAWebView.h>

#define HTML1 " \
	<html><body> \
    <script>for(var i = 0; i < 100; ++i) { document.write('Hello World ' + i + '</br>'); }</script> \
    </body></html>"

#define HTML2 " \
	<html><body> \
	<a href=\"javascript:document.location = 'mosync://Notify/HelloWorld'\">Click Me</a></br> \
    </body></html>"

#define HTML3 " \
	<html><body> \
	<a href=\"#\" onclick=\"document.location = 'mosync://Notify/HelloWorld'\">Click Me</a></br> \
    </body></html>"

#define HTML4 " \
	<html><body> \
	<script> \
		function MoSyncRequest(request) { document.location = 'mosync://' + request; } \
		function ProcessData() { var request = 'ProcessData/' + document.getElementById(\"DataField\").value; MoSyncRequest(request); } \
	</script> \
	<div style=\"margin-bottom:10pt; font-size:150%%;\">Touch a color or press a keypad number key!</div> \
	<div> \
		<a style=\"cursor: hand; font-size:150%%;\" href=\"#\" onclick=\"MoSyncRequest('BgColor/Yellow')\">Yellow</a> \
		<a style=\"cursor: hand; font-size:150%%;\" href=\"#\" onclick=\"MoSyncRequest('BgColor/Red')\">Red</a> \
		<a style=\"cursor: hand; font-size:150%%;\" href=\"#\" onclick=\"MoSyncRequest('BgColor/Green')\">Green</a> \
	</div> \
	<div style=\"margin-top:20pt; font-size:150%%;\" id=\"ColorMessage\"></div> \
	<div style=\"margin-top:20pt;\"> \
		<input style=\"font-size:150%%;\" type='text' id='DataField' /> \
		<input style=\"font-size:150%%;\" type=\"button\" value=\"ClickeMe!\" onclick=\"ProcessData()\"/> \
	<div> \
	<div style=\"margin-top:20pt; font-size:150%%;\" id=\"DataMessage\"></div> \
	<div style=\"margin-top:20pt; font-size:150%%;\"><a href=\"http://www.jqtouch.com/preview/demos/main/#home\">Open jQTouch Demo</a></div> \
	<div style=\"margin-top:20pt; font-size:150%%;\"<a cursor: hand; font-size:150%%;\" href=\"#\" onclick=\"MoSyncRequest('ExitApp')\">Exit Application</a></div> \
    </body></html>"

// TODO: Fix utf8 issues!!

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

#ifdef COMMENT_OUT
int MAMain()
{
	MAEvent event;

	//maWebViewOpen();
	//maWebViewSetHTML("<html><body>MAMain 1</body></html>");

	//maPanic(1, "Hi!!");

	maSetColor(0xFF0000);
	maFillRect(0, 0, 300, 200);
	maUpdateScreen();

	//maWebViewOpen();
	//maWebViewSetHTML("<html><body>MAMain1</body></html>");

	while (1)
	{
		maSetColor(0xFF0000);
		maFillRect(0, 0, 300, 200);
		maUpdateScreen();

		//maWait(0);
		while (0 != maGetEvent(&event))
		{
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_POINTER_PRESSED:

					if (event.point.y < 200)
					{
						maExit(0);
					}
					else
					{
						maSetColor(0x888888);
						maFillRect(0, 200, 300, 100);
						maUpdateScreen();

						//maWebViewOpen();
						//maWebViewSetHTML("<html><body>MAMain2</body></html>");

						//maPanic(1, "Hellooo!!");

						maSetColor(0xFFFF00);
						maFillRect(0, 300, 300, 100);
						maUpdateScreen();
					}

					break;

				case EVENT_TYPE_WEBVIEW_OPENED:
					//maWebViewSetHTML(HTML3);
					break;

				case EVENT_TYPE_WEBVIEW_REQUEST:
				{
					//maWebViewSetHTML("<html><body>&lt;3 &lt;3 &lt;3</body></html>");
					int size = maWebViewGetRequestSize(event.key);
					char* request = (char*) maWebViewAllocateData(size);
					int result = maWebViewGetRequest(event.key, request, size);
					if (result < 1) { break; }
					maWebViewSetHTML(request);
					//maWebViewEvaluateScript("document.body.innerHTML = '&lt;3 &lt;3 &lt;3 ;-)'");
					break;
				}
			}
		}
	}

	return 0;
}
#endif

#ifndef COMMENT_OUT
int MAMain()
{
	MAEvent event;

	maWebViewOpen();

	while (1)
	{
		while (0 != maGetEvent(&event))
		{
			switch (event.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

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
					maWebViewSetHTML(HTML4);
					break;

				case EVENT_TYPE_WEBVIEW_REQUEST:
					{
						// Get request.
						int size = maWebViewGetRequestSize(event.key);
						char* request = (char*) maWebViewAllocateData(size);
						int result = maWebViewGetRequest(event.key, request, size);
						if (result < 1) { break; }

						// Print request.
						printf("Request: %s\n", request);

						// Process request.
						char* service = maWebViewGetRequestService(request);
						if (service)
						{
							printf("Requested Service: %s\n", service);
							maWebViewFreeData(service);
						}

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
						else if (maWebViewRequestIs(request, "ExitApp"))
						{
							maExit(0);
						}
					}
					break;
			}
		}

		//maWait(0);
	}

	maWebViewClose();

	return 0;
}
#endif
