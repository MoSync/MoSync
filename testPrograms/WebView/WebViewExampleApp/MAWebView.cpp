#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include "MAWebView.h"

#ifdef __cplusplus
extern "C" {
#endif

void* maWebViewAllocateData(int size)
{
	return malloc(sizeof(char) * (size + 1));
}

void maWebViewFreeData(void* data)
{
	free(data);
}

int maWebViewRequestIs(char* request, char* service)
{
	char pattern[256];
	if (strlen(request) < 1) { return 0; }
	// Service should start at beginning of request.
	return (int) (strstr(request, service) == request);
}

char* maWebViewGetRequestService(char* request)
{
	// Must be at least one character in the request.
	if (strlen(request) < 1) { return NULL; }

	// Find start of data part.
	char* p1 = request;
	char* p2 = strstr(p1, "/");
	if (p2)
	{
		// We have a data part, insert temporary zero termination.
		// It goes without saying that this it is bad practice to
		// modify the input string. At least, we reset it below,
		// but this is not at all thread safe.
		*p2 = '\0';
	}

	// Copy data part to buffer.
	char* name = (char*) maWebViewAllocateData(strlen(p1));
	if (!name) { goto exit; }
	strcpy(name, p1);

exit:
	if (p2)
	{
		// Restore slash.
		*p2 = '/';
	}

	return name;
}

char* maWebViewGetRequestData(const char* request)
{
	// Must be at least three characters in a request
	// that has a data part.
	if (strlen(request) < 3) { return NULL; }

	// Find first slash.
	char* p = strstr(request, "/");
	if (!p) { return NULL; }

	// Move to first char after the slash.
	++p;

	// Copy data to buffer.
	char* data = (char*) maWebViewAllocateData(strlen(p));
	if (!data) { return NULL; }
	strcpy(data, p);

	return data;
}

#ifdef __cplusplus
}
#endif
