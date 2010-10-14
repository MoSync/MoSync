#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include "WidgetMessageUtil.h"

#ifdef __cplusplus
extern "C" {
#endif

char* WidgetMessageAllocateString(int size)
{
	return (char*) malloc(size);
}

void WidgetMessageFreeString(char* data)
{
	free(data);
}

char* WidgetMessageGet(int messageId)
{
	int size = maWidgetGetMessageSize(messageId);
	char* message = (char*) WidgetMessageAllocateString(size);
	if (NULL == message)
	{ 
		return NULL;
	}
	
	int result = maWidgetGetMessage(messageId, message, size);
	if (result != size) 
	{ 
		WidgetMessageFreeString(message);
		return NULL;
	}
	
	return message;
}

int WidgetMessageNameIs(char* message, char* messageName)
{
	if ((NULL == message) ||
	    (NULL == messageName) ||
	    (strlen(message) < 1) ||
	    (strlen(messageName) < 1))
	{
		return 0; 
	}
	
	// Start of messageName should be found at start of message string.
	return (int) (strstr(message, messageName) == message);
}

char* WidgetMessageGetName(char* message)
{
	// Must be at least one character in the message.
	if ((NULL == message) || (strlen(message) < 1)) { return NULL; }

	// Find start of data part.
	char* p1 = message;
	char* p2 = strstr(p1, "/");
	if (p2)
	{
		// We have a data part, insert temporary zero termination.
		// It goes without saying that this it is bad practice to
		// modify the input string. At least, we reset it below,
		// but this is not thread safe.
		*p2 = '\0';
	}

	// Copy data part to buffer.
	char* name = (char*) WidgetMessageAllocateString(strlen(p1) + 1);
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

char* WidgetMessageGetData(const char* message)
{
	// Must be at least three characters in a request
	// that has a data part.
	if ((NULL == message) || (strlen(message) < 3)) { return NULL; }

	// Find first slash.
	char* p = strstr(message, "/");
	if (!p) { return NULL; }

	// Move to first char after the slash.
	++p;

	// Copy data to buffer.
	char* data = (char*) WidgetMessageAllocateString(strlen(p) + 1);
	if (!data) { return NULL; }
	strcpy(data, p);

	return data;
}

#ifdef __cplusplus
}
#endif
