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
 * @file MoSyncDB.h
 * @author Mikael Kindborg
 *
 * @brief Implemention of the MoSync Database API.
 */

#ifndef MOSYNCDB_H
#define MOSYNCDB_H

// MoSync constants and types.
// TODO: Remove these in the iOS implementation.
#define MA_DB_OK 0;
#define MA_DB_ERROR -2;
#define MA_DB_NO_ROW -3;
#define MAHandle int

/**
 * Open a database, the database is created if it does not exist.
 * @param path Full path to database file name.
 * @return Handle to the database >0 on success, #MA_DB_ERROR on error.
 */
MAHandle maDBOpen(const char* path);

/**
 * Close a database.
 * @param databaseHandle Handle to the database.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int maDBClose(MAHandle databaseHandle);

/**
 * Executes an SQL statement. If the statement returns a
 * query result, a cursor handle is returned.
 * @param databaseHandle Handle to the database.
 * @param sql The SQL statement.
 * @return #MA_DB_ERROR on error, #MA_DB_OK on success,
 * > 0 if there is a cursor to a query result, in this
 * case the return value is the cursor handle.
 */
MAHandle maDBExecSQL(MAHandle databaseHandle, const char* sql);

/**
 * Destroys a cursor. You must call this function
 * when you are done with the cursor to release
 * its resources.
 * @param cursorHandle Handle to the cursor.
 */
int maDBCursorDestroy(MAHandle cursorHandle);

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
int maDBCursorNext(MAHandle cursorHandle);

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
int maDBCursorGetColumnData(
	MAHandle cursorHandle,
	int columnIndex,
	MAHandle placeholder);

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
int maDBCursorGetColumnText(
	MAHandle cursorHandle,
	int columnIndex,
	void* buffer,
	int bufferSize);

/**
 * Get the column value at the current row pointed to
 * by the cursor as an int value.
 * @param cursorHandle Handle to the cursor.
 * @param columnIndex Index of the column to retrieve value from.
 * First column has index zero.
 * @param value Pointer to int to receive the value.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int maDBCursorGetColumnInt(
	MAHandle cursorHandle,
	int columnIndex,
	int* value);

/**
 * Get the column value at the current row pointed to
 * by the cursor as a double value.
 * @param cursorHandle Handle to the cursor.
 * @param columnIndex Index of the column to retrieve value from.
 * First column has index zero.
 * @param value Pointer to double to receive the value.
 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
 */
int maDBCursorGetColumnDouble(
	MAHandle cursorHandle,
	int columnIndex,
	double* value);

#endif
