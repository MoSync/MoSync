#ifndef MA_WEBVIEW_H
#define MA_WEBVIEW_H

#include <IX_WEBVIEW.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MOSYNC_PROTOCOL "mosync://"

/**
 * Free a char string.
 */
void CharStringFree(char* p);

/**
 * Allocate a char string.
 */
char* CharStringAllocate(int size);


/**
 * Return true (non-zero) if the service name of the request
 * matches the given name.
 */
int ServiceRequestNameIs(char* request, char* name);

/**
 * Return the service name part of a request that has the form:
 * mosync://ServiceName/Data
 * Returned string must be deallocated with CharStringFree().
 * On error, NULL is returned.
 */
char* ServiceRequestName(char* request);

/**
 * Return the data part of a request that has the form:
 * mosync://ServiceName/Data
 * Returned string must be deallocated with CharStringFree().
 * On error, NULL is returned.
 */
char* ServiceRequestData(const char* request);

#ifdef __cplusplus
}
#endif

#endif
