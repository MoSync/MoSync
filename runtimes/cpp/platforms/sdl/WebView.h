#ifndef WEBVIEW_H
#define WEBVIEW_H

enum WebViewStatusCode 
{
   WebViewOk = 0,
   WebViewNotOpen = 1,
   WebViewInvalidArgument = 2,
   WebViewScriptError = 3
};

int webViewOpen();
int webViewClose();
int webViewSetHTML(int a);
int webViewEvaluateScript(int a);

#endif
