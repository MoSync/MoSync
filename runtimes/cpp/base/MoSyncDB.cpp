/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
 * @file MoSyncDB.cpp
 * @author Mikael Kindborg
 *
 * @brief Implemention of the MoSync Database API.
 */

#include "config_platform.h"
#include "syscall.h"
#include "MoSyncDB.h"
#if FILESYSTEM_CHROOT
#include "helpers/mkdir.h"
#endif

#include <sqlite3.h>
#include <map>
#include <stdio.h>
#include <string.h>

using namespace Base;
/**
 * Class that represents a cursor for a query result.
 */
class MoDBCursor
{
private:
	/**
	 * Object that holds the query and the query result.
	 */
	sqlite3_stmt* mStatement;

	/**
	 * Just used internally to know if we are
	 * at the first result.
	 */
	int mCursorPosition;

public:
	MoDBCursor(sqlite3_stmt* statement) :
		mStatement(statement),
		mCursorPosition(0)
	{
	}

	virtual ~MoDBCursor()
	{
		sqlite3_finalize(mStatement);
	}

	sqlite3_stmt* getStatement()
	{
		return mStatement;
	}

	bool next()
	{
		if (0 == mCursorPosition)
		{
			// We have a cursor that has a result.
			// No need to get next result yet.
			// Advance cursor position. We are now at
			// the first result.
			++mCursorPosition;
			return true;
		}

		// Run the next stop of the query.
		int result = sqlite3_step(mStatement);

		// Is there a query result available?
		if (SQLITE_ROW == result)
		{
			++mCursorPosition;
			return true;
		}

		// No more results.
		return false;
	}
};

// Handle counters.
static int gDatabaseHandle = 0;
static int gCursorHandle = 0;

// Object tables.
static std::map <MAHandle, sqlite3*> gDatabaseTable;
static std::map <MAHandle, MoDBCursor*> gCursorTable;

static int MoDBDatabaseExists(MAHandle databaseHandle)
{
	return gDatabaseTable.find(databaseHandle) != gDatabaseTable.end();
}

static sqlite3* MoDBGetDatabase(MAHandle databaseHandle)
{
	// Check if handle exists.
	if (MoDBDatabaseExists(databaseHandle))
	{
		return gDatabaseTable[databaseHandle];
	}
	else
	{
		return NULL;
	}
}

static MAHandle MoDBCreateDatabaseHandle(sqlite3* db)
{
	// Create new table entry.
	++gDatabaseHandle;
	gDatabaseTable[gDatabaseHandle] = db;
	return gDatabaseHandle;
}

static int MoDBCursorExists(MAHandle cursorHandle)
{
	return gCursorTable.find(cursorHandle) != gCursorTable.end();
}

static MoDBCursor* MoDBGetCursor(MAHandle cursorHandle)
{
	// Check if cursor exists.
	if (MoDBCursorExists(cursorHandle))
	{
		return gCursorTable[cursorHandle];
	}
	else
	{
		return NULL;
	}
}

static MAHandle MoDBCreateCursorHandle(sqlite3_stmt* statement)
{
	// Create new table entry.
	++gCursorHandle;
	gCursorTable[gCursorHandle] = new MoDBCursor(statement);
	return gCursorHandle;
}

extern "C"
int maDBOpen(const char* path)
{
	sqlite3* db;

	const char* fn;
#if FILESYSTEM_CHROOT
	std::string name = std::string(FILESYSTEM_DIR) + path;
	fn = name.c_str();
	MYASSERT(path[0] == '/', ERR_FILE_PATH_INVALID);
	_mkdir(FILESYSTEM_DIR);
#else
	fn = path;
#endif

	int result = sqlite3_open(fn, &db);
	if (result)
	{
		sqlite3_close(db);
		return MA_DB_ERROR;
	}
	else
	{
		return MoDBCreateDatabaseHandle(db);
	}
}

extern "C"
int maDBClose(MAHandle databaseHandle)
{
	sqlite3* db = MoDBGetDatabase(databaseHandle);
	if (NULL == db)
	{
		return MA_DB_ERROR;
	}
	sqlite3_close(db);
	gDatabaseTable.erase(databaseHandle);
	return MA_DB_OK;
}

extern "C"
int maDBExecSQL(MAHandle databaseHandle, const char* sql)
{
	// Get database object.
	sqlite3* db = MoDBGetDatabase(databaseHandle);
	if (NULL == db)
	{
		//printf("MoDBGetDatabase failed\n");
		return MA_DB_ERROR;
	}

	// Prepare the query.
	sqlite3_stmt* statement;
	int result = sqlite3_prepare_v2(
		db,
		sql,
		-1,
		&statement,
		NULL);
	if (SQLITE_OK != result)
	{
		//printf("sqlite3_prepare_v2 failed\n");
		return MA_DB_ERROR;
	}

	// Run the query.
	result = sqlite3_step(statement);

	// Was there an error?
	if (SQLITE_BUSY == result ||
		SQLITE_ERROR == result ||
		SQLITE_MISUSE == result)
	{
		// The result was an error.
		//printf("sqlite3_step failed\n");
		sqlite3_finalize(statement);
		return MA_DB_ERROR;
	}

	// Was the query completed?
	if (SQLITE_DONE == result)
	{
		sqlite3_finalize(statement);
		return MA_DB_OK;
	}

	// Is there a query result available?
	if (SQLITE_ROW == result)
	{
		// Return the handle to a cursor object
		// that can be used for further processing
		// of the result.
		return MoDBCreateCursorHandle(statement);
	}
	DEBIG_PHAT_ERROR;
}

extern "C"
int maDBCursorDestroy(MAHandle cursorHandle)
{
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle);
	if (NULL == cursor)
	{
		return MA_DB_ERROR;
	}
	gCursorTable.erase(cursorHandle);
	delete cursor;
	return MA_DB_OK;
}

extern "C"
int maDBCursorNext(MAHandle cursorHandle)
{
	// Get the cursor object.
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle);
	if (NULL == cursor)
	{
		return MA_DB_ERROR;
	}

	// Advance the cursor.
	if (cursor->next())
	{
		return MA_DB_OK;
	}
	else
	{
		return MA_DB_NO_ROW;
	}
}

extern "C"
int maDBCursorGetColumnData(
	MAHandle cursorHandle,
	int columnIndex,
	MAHandle placeholder)
{
	// Get the cursor object.
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle);
	if (NULL == cursor)
	{
		return MA_DB_ERROR;
	}

	// First get pointer to text data.
	const unsigned char* text = sqlite3_column_text(
		cursor->getStatement(),
		columnIndex);
	if (NULL == text)
	{
		return MA_DB_ERROR;
	}

	printf("TEXT: %s\n", text);

	// Next find the bumber of bytes, excluding zero
	// termination character. See this page for details:
	// http://sqlite.org/c3ref/column_blob.html
	int numberOfBytes = sqlite3_column_bytes(
		cursor->getStatement(),
		columnIndex);

	printf("numberOfBytes: %d\n", numberOfBytes);

	//Copy the data into the provided placholder
	MemStream *stream = new MemStream(numberOfBytes);
	bool writeResult = stream->write(text, numberOfBytes);
	if(!writeResult)
	{
		return MA_DB_ERROR;
	}
	int writeSize = gSyscall->resources.add_RT_BINARY(placeholder, stream);
	if(writeSize <=0)
	{
		return MA_DB_ERROR;
	}
	return MA_DB_OK;

}

extern "C"
int maDBCursorGetColumnText(
	MAHandle cursorHandle,
	int columnIndex,
	void* buffer,
	int bufferSize)
{
	// Get the cursor object.
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle);
	if (NULL == cursor)
	{
		return MA_DB_ERROR;
	}

	// First get pointer to text data.
	const unsigned char* text = sqlite3_column_text(
		cursor->getStatement(),
		columnIndex);
	if (NULL == text)
	{
		return MA_DB_ERROR;
	}

	//printf("TEXT: %s\n", text);

	// Next find the bumber of bytes, excluding zero
	// termination character. See this page for details:
	// http://sqlite.org/c3ref/column_blob.html
	int numberOfBytes = sqlite3_column_bytes(
		cursor->getStatement(),
		columnIndex);

	//printf("numberOfBytes: %d\n", numberOfBytes);

	// Is the buffer big enough?
	if (bufferSize < numberOfBytes)
	{
		// No, return number of bytes.
		return numberOfBytes;
	}

	// Copy text data to buffer.
	memcpy(buffer, text, numberOfBytes);

	return numberOfBytes;
}

extern "C"
int maDBCursorGetColumnInt(
	MAHandle cursorHandle,
	int columnIndex,
	int* value)
{
	// Get the cursor object.
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle);
	if (NULL == cursor)
	{
		return MA_DB_ERROR;
	}

	// Get the int value.
	int v = sqlite3_column_int(
		cursor->getStatement(),
		columnIndex);
	*value = v;
	return MA_DB_OK;
}

extern "C"
int maDBCursorGetColumnDouble(
	MAHandle cursorHandle,
	int columnIndex,
	double* value)
{
	// Get the cursor object.
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle);
	if (NULL == cursor)
	{
		return MA_DB_ERROR;
	}

	// Get the int value.
	double v = sqlite3_column_double(
		cursor->getStatement(),
		columnIndex);
	*value = v;
	return MA_DB_OK;
}
