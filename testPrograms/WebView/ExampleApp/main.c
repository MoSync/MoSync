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

// This exact thing can be done in pure HTML/JS, but in this demo the JS call is made via MoSync.
#define HTML " \
	<html><body> \
	<a href=\"http://www.jqtouch.com/preview/demos/main/#home\">jQTouch Demo</a> \
	<script>function MoSyncRequest(request) { document.location = 'mosync://' + request; }</script> \
	<div style=\"margin-bottom:10pt; font-size:150%%;\">Touch a color or press a keypad number key!</div> \
	<div style=\"cursor: hand; font-size:150%%;\" onclick=\"MoSyncRequest('BgColor/Yellow')\">Yellow</div> \
	<div style=\"cursor: hand; font-size:150%%;\" onclick=\"MoSyncRequest('BgColor/Blue')\">Blue</div> \
	<div style=\"cursor: hand; font-size:150%%;\" onclick=\"MoSyncRequest('BgColor/Green')\">Green</div> \
	<div style=\"margin-top:20pt; font-size:150%%;\" id=\"Message\"></div> \
	<div style=\"margin-top:20pt; cursor: hand; font-size:150%%;\" onclick=\"MoSyncRequest('ExitApp')\">Exit Application</div> \
    </body></html>"

// TODO: Fix utf8 issues!!

#define HTML4 " \
	<html><body> \
	<div style=\"margin-bottom:10pt;\">Touch a color or press a keypad number key!</div> \
    </body></html>"

#define SCRIPT " \
	document.getElementById(\"Message\").innerHTML = '%s is a beautiful color!'; \
	document.bgColor='%s'; \
	"

void SetBgColor(char* color)
{
	char* script = CharStringAllocate(strlen(SCRIPT) + strlen(color) + strlen(color) + 1);
	sprintf(script, SCRIPT, color, color);
	maWebViewEvaluateScript(script);
}

#ifndef COMMENT_OUT
int MAMain()
{
	MAEvent e;

	//maWebViewOpen();
	//maWebViewSetHTML("<html><body>MAMain 1</body></html>");

	//maPanic(1, "Hi!!");

	maSetColor(0xFF0000);
	maFillRect(0, 0, 300, 200);
	maUpdateScreen();

	maWebViewOpen();
	//maWebViewSetHTML("<html><body>MAMain1</body></html>");

	while (1)
	{
		maSetColor(0xFF0000);
		maFillRect(0, 0, 300, 200);
		maUpdateScreen();

		//maWait(0);
		while (0 != maGetEvent(&e))
		{
			switch (e.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_POINTER_PRESSED:

					if (e.point.y < 200)
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

				case EVENT_TYPE_WEBVIEW_CREATED:
					maWebViewSetHTML(HTML4);
					break;

				case EVENT_TYPE_WEBVIEW_REQUEST:
					//maWebViewSetHTML("<html><body>&lt;3 &lt;3 &lt;3</body></html>");
					maWebViewEvaluateScript("document.body.innerHTML = '&lt;3 &lt;3 &lt;3 ;-)'");
					break;
			}
		}
	}

	return 0;
}
#endif

#ifdef COMMENT_OUT
int MAMain()
{
	MAEvent e;

	maWebViewOpen();

	while (1)
	{
		while (0 != maGetEvent(&e))
		{
			switch (e.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_KEY_PRESSED:
					if (e.key == MAK_0) { maWebViewClose(); }
					if (e.key == MAK_1) { maWebViewOpen(); maWebViewSetHTML(HTML); }
					if (e.key == MAK_2) { SetBgColor("Gray"); }
					if (e.key == MAK_3) { SetBgColor("White"); }
					if (e.key == MAK_4) { SetBgColor("Pink"); }
					if (e.key == MAK_5) { SetBgColor("Orange"); }
					if (e.key == MAK_6) { SetBgColor("Chocolate"); }
					if (e.key == MAK_7) { SetBgColor("ABCDEF"); }
					if (e.key == MAK_8) { SetBgColor("DeepPink"); }
					if (e.key == MAK_9) { SetBgColor("Gold"); }

					break;

				case EVENT_TYPE_WEBVIEW_CREATED:
					maWebViewSetHTML(HTML);
					break;

				case EVENT_TYPE_WEBVIEW_REQUEST:
					{
						// Print service request.
						printf("Request: %s\n", e.request);
						char* service = WebViewRequestedService(e.request);
						if (service)
						{
							printf("Requested Service: %s\n", service);
							CharStringFree(service);
						}

						// Process request.
						if (WebViewRequestedServiceIs(e.request, "BgColor"))
						{
							char* color = WebViewRequestData(e.request);
							if (color)
							{
								printf("ServiceRequestData: %s\n", color);
								SetBgColor(color); // This is where JS is called.
								CharStringFree(color);
							}

						}
						else if (WebViewRequestedServiceIs(e.request, "ExitApp"))
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
