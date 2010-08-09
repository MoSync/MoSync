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
