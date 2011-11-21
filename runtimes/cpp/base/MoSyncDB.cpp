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
#include "Syscall.h"
#include "MoSyncDB.h"
#if FILESYSTEM_CHROOT
#include "helpers/mkdir.h"
#endif

#include <sqlite3.h>
#include "hashmap/hashmap.h"
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
static HashMapNoDelete<sqlite3> gDatabaseTable;
static HashMap<MoDBCursor> gCursorTable;

void MoSyncDBInit(void) {
}
void MoSyncDBClose(void) {
	gCursorTable.close();

	HashMapNoDelete<sqlite3>::TIteratorC itr = gDatabaseTable.begin();
	while(itr.hasMore()) {
		sqlite3_close(itr.next().value);
	}
	gDatabaseTable.close();
}

static sqlite3* MoDBGetDatabase(MAHandle databaseHandle)
{
	// Check if handle exists.
	sqlite3* db = gDatabaseTable.find(databaseHandle);
	if (db)
	{
		return db;
	}
	else
	{
		if(gSyscall->mPanicOnProgrammerError)
			BIG_PHAT_ERROR(ERR_DB_INVALID_HANDLE);
		return NULL;
	}
}

static MAHandle MoDBCreateDatabaseHandle(sqlite3* db)
{
	// Create new table entry.
	++gDatabaseHandle;
	gDatabaseTable.insert(gDatabaseHandle, db);
	return gDatabaseHandle;
}

static MoDBCursor* MoDBGetCursor(MAHandle cursorHandle)
{
	// Check if cursor exists.
	MoDBCursor* c = gCursorTable.find(cursorHandle);
	if (c)
	{
		return c;
	}
	else
	{
		if(gSyscall->mPanicOnProgrammerError)
			BIG_PHAT_ERROR(ERR_DB_INVALID_HANDLE);
		return NULL;
	}
}

static MoDBCursor* MoDBGetCursor(MAHandle cursorHandle, int index, int* result)
{
	MoDBCursor* c = MoDBGetCursor(cursorHandle);
	if(!c)
	{
		*result = MA_DB_ERROR;
		return NULL;
	}
	if(index >= sqlite3_column_count(c->getStatement()))
	{
		if(gSyscall->mPanicOnProgrammerError)
			BIG_PHAT_ERROR(ERR_DB_INVALID_COLUMN_INDEX);
		*result = MA_DB_ERROR;
		return NULL;
	}
	if(sqlite3_column_type(c->getStatement(), index) == SQLITE_NULL)
	{
		*result = MA_DB_NULL;
		return NULL;
	}
	return c;
}

static MAHandle MoDBCreateCursorHandle(sqlite3_stmt* statement)
{
	// Create new table entry.
	++gCursorHandle;
	gCursorTable.insert(gCursorHandle, new MoDBCursor(statement));
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

static int prepStatement(MAHandle databaseHandle, const char* sql, sqlite3_stmt*& statement)
{
	// Get database object.
	sqlite3* db = MoDBGetDatabase(databaseHandle);
	if (NULL == db)
	{
		//LOGD("MoDBGetDatabase failed\n");
		return MA_DB_ERROR;
	}

	// Prepare the query.
	int result = sqlite3_prepare_v2(
		db,
		sql,
		-1,
		&statement,
		NULL);
	if (SQLITE_OK != result)
	{
		//LOGD("sqlite3_prepare_v2 failed\n");
		return MA_DB_ERROR;
	}
	return MA_DB_OK;
}

static int runStatement(sqlite3_stmt* statement)
{
	// Run the query.
	int result = sqlite3_step(statement);

	// Was there an error?
	if (SQLITE_BUSY == result ||
		SQLITE_ERROR == result ||
		SQLITE_MISUSE == result)
	{
		// The result was an error.
		//LOGD("sqlite3_step failed\n");
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
int maDBExecSQL(MAHandle databaseHandle, const char* sql)
{
	sqlite3_stmt* statement;
	int result = prepStatement(databaseHandle, sql, statement);
	if (result < 0)
		return result;
	return runStatement(statement);
}

extern "C"
MAHandle maDBExecSQLParams(MAHandle databaseHandle, const char* sql,
	const MADBValue* params, int paramCount)
{
	sqlite3_stmt* statement;
	int result = prepStatement(databaseHandle, sql, statement);
	if (result < 0)
		return result;

	// Bind parameters
	for(int i=1; i<=paramCount; i++) {
		const MADBValue& v(params[i-1]);
		switch(v.type) {
		case MA_DB_TYPE_NULL:
			result = sqlite3_bind_null(statement, i);
			break;
		case MA_DB_TYPE_INT:
			result = sqlite3_bind_int(statement, i, v.i);
			break;
		case MA_DB_TYPE_INT64:
			result = sqlite3_bind_int64(statement, i, v.i64);
			break;
		case MA_DB_TYPE_DOUBLE:
			result = sqlite3_bind_double(statement, i, v.d);
			break;
		case MA_DB_TYPE_BLOB:
			result = sqlite3_bind_blob(statement, i,
				gSyscall->GetValidatedMemRange(v.blob.data, v.blob.size),
				v.blob.size, SQLITE_STATIC);
			break;
		case MA_DB_TYPE_DATA:
			{
				Stream* s = gSyscall->resources.get_RT_BINARY(v.dataHandle);
				int length;
				MYASSERT(s->length(length), ERR_DATA_ACCESS_FAILED);
				// if we can access the data directly, do so.
				if(s->ptrc())
					result = sqlite3_bind_blob(statement, i, s->ptrc(), length, SQLITE_STATIC);
				else
				{
					// otherwise, create a temporary copy.
					void* buf = malloc(length);
					MYASSERT(s->read(buf, length), ERR_DATA_ACCESS_FAILED);
					result = sqlite3_bind_blob(statement, i, buf, length, &free);
				}
				// todo: put resource in flux while statement is active.
			}
			break;
		case MA_DB_TYPE_TEXT:
			{
				const char* text;
				if(v.text.length >= 0)
					text = (char*)gSyscall->GetValidatedMemRange(v.text.addr, v.text.length);
				else
					text = gSyscall->GetValidatedStr(v.text.addr);
				result = sqlite3_bind_text(statement, i, text, v.text.length, SQLITE_STATIC);
			}
			break;
		}
		DEBUG_ASSERT(result == SQLITE_OK);
	}

	return runStatement(statement);
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
	int result;
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle, columnIndex, &result);
	if (NULL == cursor)
	{
		return result;
	}

	// Find the bumber of bytes, excluding zero
	// termination character. See this page for details:
	// http://sqlite.org/c3ref/column_blob.html
	int numberOfBytes = sqlite3_column_bytes(
		cursor->getStatement(),
		columnIndex);

	LOGD("numberOfBytes: %d\n", numberOfBytes);

	//Copy the data into the provided placholder
	MemStream *stream = new MemStream(numberOfBytes);
	if(numberOfBytes > 0)
	{
		bool writeResult = stream->write(
			sqlite3_column_blob(cursor->getStatement(), columnIndex),
			numberOfBytes);
		if(!writeResult)
		{
			delete stream;
			return MA_DB_ERROR;
		}
	}
	int writeSize = gSyscall->resources.add_RT_BINARY(placeholder, stream);
	if(writeSize <= 0)
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
	int result;
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle, columnIndex, &result);
	if (NULL == cursor)
	{
		return result;
	}

	// First get pointer to text data.
	const unsigned char* text = sqlite3_column_text(
		cursor->getStatement(),
		columnIndex);
	if (NULL == text)
	{
		return MA_DB_ERROR;
	}

	//LOGD("TEXT: %s\n", text);

	// Next find the bumber of bytes, excluding zero
	// termination character. See this page for details:
	// http://sqlite.org/c3ref/column_blob.html
	int numberOfBytes = sqlite3_column_bytes(
		cursor->getStatement(),
		columnIndex);

	//LOGD("numberOfBytes: %d\n", numberOfBytes);

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
	int result;
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle, columnIndex, &result);
	if (NULL == cursor)
	{
		return result;
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
	int result;
	MoDBCursor* cursor = MoDBGetCursor(cursorHandle, columnIndex, &result);
	if (NULL == cursor)
	{
		return result;
	}

	// Get the int value.
	double v = sqlite3_column_double(
		cursor->getStatement(),
		columnIndex);
	*value = v;
	return MA_DB_OK;
}
