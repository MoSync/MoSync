/*
 * dbutil.cpp
 *
 *  Created on: May 15, 2013
 *
 */

#include <maheap.h>

#include "dbutil.h"

using namespace MAUtil;

const String DBUtil::getLocalPath()
{
	int bufferSize = 1024;
	char buffer[bufferSize];

	int size = maGetSystemProperty(
		"mosync.path.local",
		buffer,
		bufferSize);

	// If there was an error, return default root path.
	if (size < 0 || size > bufferSize)
	{
		return "/";
	}

	return buffer;
}


int DBUtil::getColumnString(MAHandle cursor, int column, String& text)
{
	int bufferSize = 1024;
	char buffer[bufferSize];

	// Get the text value of the field.
	int size = maDBCursorGetColumnText(cursor, column, buffer, bufferSize);

	// If it went ok, copy text to result string.
	if (size >= 0 && size < bufferSize)
	{
		// The text buffer is not zero terminated, so we do that now.
		buffer[size] = 0;

		// Copy to result string.
		text = buffer;

		return MA_DB_OK;
	}
	else if (MA_DB_NULL == size)
	{
		return MA_DB_NULL;
	}
	else
	{
		return MA_DB_ERROR;
	}
}


int DBUtil::getColumnDataAsString(MAHandle cursor, int column, String& text)
{
	// Create handle that will hold the data object.
	MAHandle data = maCreatePlaceholder();

	// Get the data of the field.
	int result = maDBCursorGetColumnData(cursor, column, data);

	// If it went ok, copy text to result string.
	if (result == MA_DB_OK)
	{
		// Get the size of the data.
		int size = maGetDataSize(data);

		// Allocate space text buffer, including zero termination char.
		char* buffer = (char*) malloc(size + 1);

		// Copy data to buffer.
		maReadData(data, buffer, 0, size);

		// Zero terminate data.
		buffer[size] = 0;

		// Copy to result string.
		text = buffer;

		// Free buffer.
		free(buffer);
	}

	// Free data object.
	maDestroyObject(data);

	// Return result.
	return result;
}
