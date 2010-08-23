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
	if (strlen(request) < strlen(MOSYNC_PROTOCOL) + 1) { return 0; }
	sprintf(pattern, "%s%s", MOSYNC_PROTOCOL, service);
	return (int) (strstr(request, pattern) == request);
}

char* maWebViewGetRequestService(char* request)
{
	if (strlen(request) < strlen(MOSYNC_PROTOCOL) + 1) { return NULL; }

	char* p1 = strstr(request, MOSYNC_PROTOCOL);
	if (!p1) { return NULL; }

	// Move to start of request name.
	p1 += strlen(MOSYNC_PROTOCOL);

	// Find start of data part.
	char* p2 = strstr(p1, "/");
	if (p2)
	{
		// We have a data part, insert temporary zero termination.
		*p2 = '\0';
	}

	char* name = (char*) maWebViewAllocateData(strlen(p1));
	if (!name) { goto exit; }
	strcpy(name, p1);

exit:
	if (p2)
	{
		// Restore slash
		*p2 = '/';
	}

	return name;
}

char* maWebViewGetRequestData(const char* request)
{
	if (strlen(request) < strlen(MOSYNC_PROTOCOL) + 3) { return NULL; }

	char* p = strstr(request, MOSYNC_PROTOCOL);
	if (!p) { return NULL; }

	p += strlen(MOSYNC_PROTOCOL);
	p = strstr(p, "/");
	if (!p) { return NULL; }

	++p; // Move to first char after the slash
	char* data = (char*) maWebViewAllocateData(strlen(p));
	if (!data) { return NULL; }

	strcpy(data, p);

	return data;
}

#ifdef __cplusplus
}
#endif
