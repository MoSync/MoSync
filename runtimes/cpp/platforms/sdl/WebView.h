#ifndef WEBVIEW_H
#define WEBVIEW_H

// TODO: Where should these constants be placed to be reachable from MoSync app code?
enum WebViewStatusCode 
{
   WebViewOk = 0,
   WebViewError = 1,
   WebViewNotOpen = 2,
   WebViewInvalidArgument = 3,
   WebViewScriptError = 4
};

int webViewOpen(int left, int top, int width, int height);
int webViewClose();
int webViewSetHTML(const char* html);
int webViewEvaluateScript(const char* script);

#endif

/*  EXAMPLE PROGRAM

#include <ma.h>
#include <IX_WEBVIEW.h>

#define HTML " \
	<html><body> \
	<a href=\"javascript:document.location = 'mosync://Notify/HelloWorld'\">Click Me</a></br> \
    <script>for(var i = 0; i < 100; ++i) { document.write('Hello World ' + i + '</br>'); }</script> \
    </body></html>"

int MAMain ( )
{
	while (1)
	{
		MAEvent e;
		while (maGetEvent( &e ) != 0)
		{
			switch (e.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_KEY_PRESSED:
				case EVENT_TYPE_POINTER_PRESSED:
					maWebViewOpen();
					maWebViewSetHTML(HTML);
					break;

				case EVENT_TYPE_WEBVIEW_SERVICE_REQUEST:
					printf("URL: %s", e.serviceRequest);
					break;
			}
		}
	}

	maWebViewClose();

	return 0;
}
*/