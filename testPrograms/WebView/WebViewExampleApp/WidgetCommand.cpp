#include <ma.h>
#include <maheap.h>
#include <mastring.h>
#include <mavsprintf.h>
#include "WidgetCommand.h"

#ifdef __cplusplus
extern "C" {
#endif

char* WidgetCommandAllocateString(int size)
{
	return (char*) malloc(size);
}

void WidgetCommandFreeString(char* data)
{
	free(data);
}

char* WidgetCommandGet(int commandId)
{
	int size = maWidgetGetCommandSize(commandId);
	char* command = (char*) WidgetCommandAllocateString(size);
	if (NULL == command) 
	{ 
		return NULL;
	}
	
	int result = maWidgetGetCommand(commandId, command, size);
	if (result != size) 
	{ 
		WidgetCommandFreeString(command);
		return NULL;
	}
	
	return command;
}

int WidgetCommandNameIs(char* command, char* commandName)
{
	if ((NULL == command) ||
	    (NULL == commandName) ||
	    (strlen(command) < 1) ||
	    (strlen(commandName) < 1))
	{
		return 0; 
	}
	
	// Start of commandName should be found at start of command string.
	return (int) (strstr(command, commandName) == command);
}

char* WidgetCommandGetName(char* command)
{
	// Must be at least one character in the command.
	if ((NULL == command) || (strlen(command) < 1)) { return NULL; }

	// Find start of data part.
	char* p1 = command;
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
	char* name = (char*) WidgetCommandAllocateString(strlen(p1) + 1);
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

char* WidgetCommandGetData(const char* command)
{
	// Must be at least three characters in a request
	// that has a data part.
	if ((NULL != command) || (strlen(command) < 3)) { return NULL; }

	// Find first slash.
	char* p = strstr(command, "/");
	if (!p) { return NULL; }

	// Move to first char after the slash.
	++p;

	// Copy data to buffer.
	char* data = (char*) WidgetCommandAllocateString(strlen(p) + 1);
	if (!data) { return NULL; }
	strcpy(data, p);

	return data;
}

#ifdef __cplusplus
}
#endif
