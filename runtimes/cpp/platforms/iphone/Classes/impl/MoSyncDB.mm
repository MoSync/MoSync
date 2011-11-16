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
#import "syscall.h"

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

		// Run the query again.
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

/**
 * Open a database, the database is created if it does not exist.
 * @param path Full path to database file name.
 * @return Handle to the database >0 on success, #MA_DB_ERROR on error.
 */
int Syscall::maDBOpen(const char* path)
{
	sqlite3* db;
	int result = sqlite3_open(path, &db);
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

/**
 * Close a database.
 * @param databaseHandle Handle to the database.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int Syscall::maDBClose(MAHandle databaseHandle)
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

/**
 * Executes an SQL statement. If the statement returns a
 * query result, a cursor handle is returned.
 * @param databaseHandle Handle to the database.
 * @param sql The SQL statement.
 * @return #MA_DB_ERROR on error, #MA_DB_OK on success,
 * > 0 if there is a cursor to a query result, in this
 * case the return value is the cursor handle.
 */
int Syscall::maDBExecSQL(MAHandle databaseHandle, const char* sql)
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
}

/**
 * Destroys a cursor. You must call this function
 * when you are done with the cursor to release
 * its resources.
 * @param cursorHandle Handle to the cursor.
 */
int Syscall::maDBCursorDestroy(MAHandle cursorHandle)
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

/**
 * Move the cursor to the next row in the result set.
 * Note that you must call this function before retrieving
 * column data. The initial position of the cursor is
 * before the first row in the result set. If the result
 * set is empty, this function will return a value != MA_DB_OK.
 * @param cursorHandle Handle to the cursor.
 * @return #MA_DB_OK if successfully moved to next row,
 * #MA_DB_NO_ROW if there are no more rows in the result set,
 * #MA_DB_ERROR on error.
 */
int Syscall::maDBCursorNext(MAHandle cursorHandle)
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

/**
 * Get the column value at the current row pointed to
 * by the cursor as a data object. Use this function for
 * blob data or text data.
 * @param cursorHandle Handle to the cursor.
 * @param columnIndex Index of the column to retrieve value from.
 * First column has index zero.
 * @param placeholder Handle created with maCreatePlaceholder.
 * A data object will be created with the column data, and the handle
 * will refer to that data.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int Syscall::maDBCursorGetColumnData(
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

/**
 * Get the column value at the current row pointed to
 * by the cursor as a text data buffer. Use this function for
 * text data.
 * @param cursorHandle Handle to the cursor.
 * @param columnIndex Index of the column to retrieve value from.
 * First column has index zero.
 * @param buffer Pointer to buffer to receive the data.
 * The result is NOT zero terminated.
 * @param bufferSize Max size of the buffer.
 * @return The actual length of the data, if the actual length
 * returned is > bufferSize, data was not copied (buffer too small),
 * returns #MA_DB_ERROR on other errors.
 */
int Syscall::maDBCursorGetColumnText(
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

/**
 * Get the column value at the current row pointed to
 * by the cursor as an int value.
 * @param cursorHandle Handle to the cursor.
 * @param columnIndex Index of the column to retrieve value from.
 * First column has index zero.
 * @param value Pointer to int to receive the value.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int Syscall::maDBCursorGetColumnInt(
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

/**
 * Get the column value at the current row pointed to
 * by the cursor as a double value.
 * @param cursorHandle Handle to the cursor.
 * @param columnIndex Index of the column to retrieve value from.
 * First column has index zero.
 * @param value Pointer to double to receive the value.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int Syscall::maDBCursorGetColumnDouble(
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
