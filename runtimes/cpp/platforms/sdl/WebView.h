#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <helpers/CPP_IX_WEBVIEW.h>

int maWebViewOpen(int left, int top, int width, int height);
int maWebViewClose();
int maWebViewSetHTML(const char* html);
int maWebViewLoadURL(const char* url);
int maWebViewEvaluateScript(const char* script);
int maWebViewGetRequestSize(int requestID);
int maWebViewGetRequest(int requestID, char* buf, int bufSize);

#endif
