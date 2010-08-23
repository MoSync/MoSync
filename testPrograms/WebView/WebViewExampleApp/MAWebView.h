#ifndef MA_WEBVIEW_H
#define MA_WEBVIEW_H

#include <IX_WEBVIEW.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOSYNC_PROTOCOL "mosync://"

/**
 * Allocate a char string.
 */
void* maWebViewAllocateData(int size);

/**
 * Free allocated data.
 */
void maWebViewFreeData(void* data);

/**
 * Return true (non-zero) if the name of the requested service
 * matches the given name. The request has the format:
 * mosync://Service/Data
 */
int maWebViewRequestIs(char* request, char* service);

/**
 * Return the service part of a request that has the form:
 * mosync://Service/Data
 * Returned string must be deallocated with maWebViewFreeData.
 * On error, NULL is returned.
 */
char* maWebViewGetRequestService(char* request);

/**
 * Return the data part of a request that has the form:
 * mosync://Service/Data
 * Returned string must be deallocated with maWebViewFreeData.
 * On error, NULL is returned.
 */
char* maWebViewGetRequestData(const char* request);

// Call JS to go back and forward.
//int maWebViewBack();
//int maWebViewForward();

/**
 * Convert url encoded string to ascii string (or wide char string).
 */
//TODO: char* maWebViewDecodeRequest(char* request);
// --> Or even better, let the syscall maWebViewGetRequest do this!

#ifdef __cplusplus
}
#endif

#endif
