#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <helpers/CPP_IX_WEBVIEW.h>

int webViewOpen(int left, int top, int width, int height);
int webViewClose();
int webViewSetHTML(const char* html);
int webViewEvaluateScript(const char* script);

#endif
