#include <ma.h>
#include <IX_WEBVIEW.h>

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

#define HTML " \
	<html><body> \
	<div id=\"Button\" style=\"cursor: hand;\" onclick=\"document.location = 'mosync://Notify/HelloWorld'\">ClickMe</div> \
	<div id=\"Notify\"></div> \
    </body></html>"

// document.bgColor='black'

#define SCRIPT "\
	document.getElementById(\"Notify\").innerHTML = 'Hello World'; \
	document.bgColor='yellow'; \
	"

int MAMain()
{
	maWebViewOpen();
	maWebViewSetHTML(HTML);

	while (1)
	{
		MAEvent e;
		while (0 != maGetEvent(&e))
		{
			switch (e.type)
			{
				case EVENT_TYPE_CLOSE:
					return 0;

				case EVENT_TYPE_KEY_PRESSED:
					break;

				case EVENT_TYPE_POINTER_PRESSED:
					break;

				case EVENT_TYPE_WEBVIEW_SERVICE_REQUEST:
					printf(e.serviceRequest);
					maWebViewEvaluateScript(SCRIPT);
					break;
			}
		}
	}

	maWebViewClose();

	return 0;
}
