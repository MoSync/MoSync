package com.mosync.internal.android;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.util.HashMap;

import android.database.Cursor;
import android.database.CursorIndexOutOfBoundsException;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
import android.database.sqlite.SQLiteStatement;
import android.util.Log;
import static com.mosync.internal.generated.MAAPI_consts.*;

/**
 * Class that handles databases and query results.
 * @author Mikael Kindborg
 */
public class MoSyncDB
{
	public boolean mIsLoggingOn = true;

	private int mDatabaseCounter = 0;
	private HashMap<Integer, MoDatabase> mDatabaseTable =
		new HashMap<Integer, MoDatabase>();

	private int mCursorCounter = 0;
	private HashMap<Integer, MoCursor> mCursorTable =
		new HashMap<Integer, MoCursor>();

	public MoSyncDB()
	{
	}

	public void logStackTrace(Throwable ex)
	{
		if (mIsLoggingOn)
		{
			ex.printStackTrace();
		}
	}

	/**
	 * Open a database, the database is created if it does not exist.
	 * @param path Full path to database file name.
	 * @return Handle to the database >0 on success, #MA_DB_ERROR on error.
	 */
	public int maDBOpen(String path)
	{
		try
		{
			MoDatabase database = MoDatabase.create(path);
			++mDatabaseCounter;
			addDatabase(mDatabaseCounter, database);
			return mDatabaseCounter;
		}
		catch (SQLiteException ex)
		{
			logStackTrace(ex);
			return MA_DB_ERROR;
		}
	}

	/**
	 * Close a database.
	 * @param databaseHandle Handle to the database.
	 * @return MA_DB_OK on success, MA_DB_ERROR on error.
	 */
	public int maDBClose(int databaseHandle)
	{
		if (!hasDatabase(databaseHandle))
		{
			return MA_DB_ERROR;
		}

		try
		{
			getDatabase(databaseHandle).close();
			removeDatabase(databaseHandle);
			return MA_DB_OK;
		}
		catch (SQLiteException ex)
		{
			logStackTrace(ex);
			return MA_DB_ERROR;
		}
	}

	/**
	 * Executes an SQL statement. If the statement returns a
	 * query result, a cursor handle is returned.
	 * @param databaseHandle Handle to the database.
	 * @param sql The SQL statement.
	 * @return MA_DB_ERROR on error, MA_DB_OK on success,
	 * > 0 if there is a cursor to a query result, in this
	 * case the return value is the cursor handle.
	 */
	public int maDBExecSQL(int databaseHandle, String sql)
	{
		return execSQLHelper(databaseHandle, sql, null);
	}

	/**
	 * Executes an SQL statement with parameters.
	 * @param databaseHandle Handle to the database.
	 * @param sql The SQL statement.
	 * @param paramsAddress Address of first MADBValue struct.
	 * @param paramCount Number of MADBValue structs passed.
	 * @return MA_DB_ERROR on error, MA_DB_OK on success,
	 * > 0 if there is a cursor to a query result, in this
	 * case the return value is the cursor handle.
	 */
	public int maDBExecSQLParams(
		int databaseHandle,
		String sql,
		int paramsAddress,
		int paramCount,
		MoSyncThread mosync)
	{

		// Execute query.
		Object[] params = extractExecParams(
			paramsAddress,
			paramCount,
			mosync);
		return execSQLHelper(databaseHandle, sql, params);
	}

	Object[] extractExecParams(
		int paramsAddress,
		int paramCount,
		MoSyncThread mosync)
	{
		Object[] params = new Object[paramCount];

		// sizeof(MADBValue) is 12 bytes, 8 byte data, 4 byte type.
		int sizeofMADBValue = 8 + 4;

		// Get parameter array.
		ByteBuffer buffer = mosync.getMemorySlice(
			paramsAddress,
			paramCount * sizeofMADBValue
			).order(null);

		// Set parameter values.
		// http://stackoverflow.com/questions/9558657/binding-ints-longs-and-blobs-byte-to-android-sql
		for (int i = 0; i < paramCount; ++i)
		{
			// Get param type.
			buffer.position((i * sizeofMADBValue) + 8);
			int type = buffer.getInt();

			// Move to start of data in current MADBValue struct.
			buffer.position(i * sizeofMADBValue);

			if (MA_DB_TYPE_INT == type)
			{
				int value = buffer.getInt();
				params[i] = new Integer(value);
				//Log.i("@@@@@", "MA_DB_TYPE_INT params[i]: " + params[i]);
			}
			else
			if (MA_DB_TYPE_INT64 == type)
			{
				long value = buffer.getLong();
				params[i] = new Long(value);
				//Log.i("@@@@@", "MA_DB_TYPE_INT64 params[i]: " + params[i]);
			}
			else
			if (MA_DB_TYPE_DOUBLE == type)
			{
				double value = buffer.getDouble();
				params[i] = new Double(value);
				//Log.i("@@@@@", "MA_DB_TYPE_DOUBLE params[i]: " + params[i]);
			}
			else
			if (MA_DB_TYPE_NULL == type)
			{
				params[i] = null;
			}
			else
			if (MA_DB_TYPE_DATA == type)
			{
				// Read data from handle.
				int handle = buffer.getInt();
				ByteBuffer buf = mosync.getBinaryResource(handle);
				byte[] bytes = new byte[buf.capacity()];
				buf.get(bytes, 0, buf.capacity());
				params[i] = ByteBuffer.wrap(bytes);
				//Log.i("@@@@@", "MA_DB_TYPE_DATA params[i]: " + new String(bytes));
			}
			else
			if (MA_DB_TYPE_TEXT == type)
			{
				// Read data from struct MADBText.
				int dataAddress = buffer.getInt();
				int dataSize = buffer.getInt();
				ByteBuffer dataBuffer = mosync.getMemorySlice(
					dataAddress,
					dataSize
					);
				byte[] bytes = new byte[dataSize];
				dataBuffer.get(bytes, 0, dataSize);
				params[i] = new String(bytes);
				//Log.i("@@@@@", "MA_DB_TYPE_TEXT params[i]: " + params[i]);
			}
			else
			if (MA_DB_TYPE_BLOB == type)
			{
				// Read data from struct MADBBlob.
				int dataAddress = buffer.getInt();
				int dataSize = buffer.getInt();
				ByteBuffer dataBuffer = mosync.getMemorySlice(
					dataAddress,
					dataSize
					);
				byte[] bytes = new byte[dataSize];
				dataBuffer.get(bytes, 0, dataSize);
				params[i] = ByteBuffer.wrap(bytes);
				//Log.i("@@@@@", "MA_DB_TYPE_BLOB params[i]: " + new String(bytes));
			}
			else
			{
				// TODO: ERROR handling?
				Log.e("@@@@@@@", "maDBExecSQLParams - unknown data type: " + type);
			}
		}

		return params;
	}

	/**
	 * Helper function for making SQL queries, used both for
	 * parameterized and non-parameterized queries.
	 * @param databaseHandle
	 * @param sql
	 * @param params
	 * @return Success or error code.
	 */
	protected int execSQLHelper(
		int databaseHandle,
		String sql,
		Object[] params)
	{
		if (!hasDatabase(databaseHandle))
		{
			return MA_DB_ERROR;
		}

		MoDatabase database = getDatabase(databaseHandle);

		try
		{
			//Log.i("@@@@@", "execSQLHelper query: " + sql);
			MoCursor cursor = database.execQuery(sql, params);
			if (null != cursor)
			{
				++mCursorCounter;
				addCursor(mCursorCounter, cursor);
				return mCursorCounter;
			}
			else
			{
				return MA_DB_OK;
			}
		}
		catch (SQLiteException ex)
		{
			Log.e("@@@@@", "execSQLHelper exception: " + ex);
			ex.printStackTrace();

			logStackTrace(ex);
			return MA_DB_ERROR;
		}
	}

	/**
	 * Destroys a cursor. You must call this function
	 * when you are done with the cursor to release
	 * its resources.
	 * @param cursorHandle Handle to the cursor.
	 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
	 */
	public int maDBCursorDestroy(int cursorHandle)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		removeCursor(cursorHandle);
		return MA_DB_OK;
	}

	/**
	 * Move the cursor to the next row in the result set.
	 * Note that you must call this function before retrieving
	 * column data. The initial position of the cursor is
	 * before the first row in the result set. If the result
	 * set is empty, this function will return a value != MA_DB_OK.
	 * @param cursorHandle Handle to the cursor.
	 * @return MA_DB_OK if successfully moved to next row,
	 * MA_DB_NO_ROW if there are no more rows in the result set,
	 * MA_DB_ERROR on error.
	 */
	public int maDBCursorNext(int cursorHandle)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		try
		{
			if (getCursor(cursorHandle).next())
			{
				return MA_DB_OK;
			}
			else
			{
				return MA_DB_NO_ROW;
			}
		}
		catch (SQLiteException ex)
		{
			logStackTrace(ex);
			return MA_DB_ERROR;
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
	 * @param mosync The MoSyncThread instance.
	 * @return #MA_DB_OK on success, #MA_DB_NULL if the column value
	 * is NULL, #MA_#MA_DB_ERROR on error.
	 */
	public int maDBCursorGetColumnData(
		int cursorHandle,
		int columnIndex,
		int placeholder,
		MoSyncThread mosync)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		try
		{
			// Check for NULL value.
			if (getCursor(cursorHandle).isNull(columnIndex))
			{
				return MA_DB_NULL;
			}

			// Get data as a byte array.
			byte[] data = getCursor(cursorHandle).getData(columnIndex);
			if (null == data)
			{
				return MA_DB_NULL;
			}

			// This calls maCreateData and copies data to the data object.
			int result = mosync.createDataObject(placeholder, data);
			if ((result > 0) && (result == placeholder))
			{
				return MA_DB_OK;
			}
		}
		catch (CursorIndexOutOfBoundsException ex1)
		{
			logStackTrace(ex1);
		}
		catch (SQLException ex2)
		{
			logStackTrace(ex2);
		}

		return MA_DB_ERROR;
	}

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor into a data buffer. Use this function
	 * for text data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param bufferAddress Address to buffer to receive the data.
	 * The result is NOT zero terminated.
	 * @param bufferSize Max size of the buffer.
	 * @param mosync The MoSyncThread instance.
	 * @return The actual length of the data, if the actual length
	 * returned is > bufferSize, data was not copied (buffer too small),
	 * returns #MA_DB_ERROR on other errors, returns #MA_DB_NULL if the
	 * column value is NULL.
	 */
	public int maDBCursorGetColumnText(
		int cursorHandle,
		int columnIndex,
		int bufferAddress,
		int bufferSize,
		MoSyncThread mosync)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		try
		{
			// Check for NULL value.
			if (getCursor(cursorHandle).isNull(columnIndex))
			{
				return MA_DB_NULL;
			}

			// Get data as a byte array.
			String text = getCursor(cursorHandle).getText(columnIndex);
			if (null == text)
			{
				return MA_DB_NULL;
			}

			// Write to MoSync memory. Only copy data if it fits
			// in the destination buffer.
			byte[] data = text.getBytes();
			if (data.length <= bufferSize)
			{
				// TODO: Replace with high-level call to write text.
				ByteBuffer buffer = mosync.getMemorySlice(
					bufferAddress,
					data.length);
				buffer.put(data);
			}

			// Return length of the data.
			return data.length;
		}
		catch (CursorIndexOutOfBoundsException ex1)
		{
			logStackTrace(ex1);
		}
		catch (SQLException ex2)
		{
			logStackTrace(ex2);
		}

		return MA_DB_ERROR;
	}

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as int data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param intValueAddress Address to int to receive the value.
	 * @param mosync The MoSyncThread instance.
	 * @return #MA_DB_OK on success, #MA_DB_NULL if the column value
	 * is NULL, #MA_#MA_DB_ERROR on error.
	 */
	public int maDBCursorGetColumnInt(
		int cursorHandle,
		int columnIndex,
		int intValueAddress,
		MoSyncThread mosync)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		try
		{
			// Check for NULL value.
			if (getCursor(cursorHandle).isNull(columnIndex))
			{
				return MA_DB_NULL;
			}

			// Get data as an int value.
			int value = getCursor(cursorHandle).getInt(columnIndex);

			// Write to MoSync memory. Size of an int is 4.
			// TODO: Replace with high-level call to write int.
			ByteBuffer buffer = mosync.getMemorySlice(intValueAddress, 4);
			buffer.putInt(value);

			return MA_DB_OK;
		}
		catch (CursorIndexOutOfBoundsException ex1)
		{
			logStackTrace(ex1);
		}
		catch (SQLException ex2)
		{
			logStackTrace(ex2);
		}

		return MA_DB_ERROR;
	}

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as double data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param doubleValueAddress Address to double to receive the value.
	 * @param mosync The MoSyncThread instance.
	 * @return #MA_DB_OK on success, #MA_DB_NULL if the column value
	 * is NULL, #MA_#MA_DB_ERROR on error.
	 */
	public int maDBCursorGetColumnDouble(
		int cursorHandle,
		int columnIndex,
		int doubleValueAddress,
		MoSyncThread mosync)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		try
		{
			// Check for NULL value.
			if (getCursor(cursorHandle).isNull(columnIndex))
			{
				return MA_DB_NULL;
			}

			// Get data as a float value.
			double value = getCursor(cursorHandle).getDouble(columnIndex);

			// Write to MoSync memory. Size of a double is 8.
			// TODO: Replace with high-level call to write double.
			ByteBuffer buffer = mosync.getMemorySlice(doubleValueAddress, 8);
			buffer.putLong(Double.doubleToLongBits(value));

			return MA_DB_OK;
		}
		catch (CursorIndexOutOfBoundsException ex1)
		{
			logStackTrace(ex1);
		}
		catch (SQLException ex2)
		{
			logStackTrace(ex2);
		}

		return MA_DB_ERROR;
	}

	private boolean hasDatabase(int databaseHandle)
	{
		return mDatabaseTable.containsKey(databaseHandle);
	}

	private MoDatabase getDatabase(int databaseHandle)
	{
		return mDatabaseTable.get(databaseHandle);
	}

	private void addDatabase(int databaseHandle, MoDatabase database)
	{
		mDatabaseTable.put(databaseHandle, database);
	}

	private void removeDatabase(int databaseHandle)
	{
		mDatabaseTable.remove(databaseHandle);
	}

	private boolean hasCursor(int cursorHandle)
	{
		return mCursorTable.containsKey(cursorHandle);
	}

	private MoCursor getCursor(int cursorHandle)
	{
		return mCursorTable.get(cursorHandle);
	}

	private void addCursor(int cursorHandle, MoCursor cursor)
	{
		mCursorTable.put(cursorHandle, cursor);
	}

	private void removeCursor(int cursorHandle)
	{
		mCursorTable.remove(cursorHandle);
	}

	/**
	 * Internal class that represents a database.
	 */
	static class MoDatabase
	{
		private SQLiteDatabase mDB;

		public static MoDatabase create(String path)
			throws SQLException
		{
			SQLiteDatabase db = SQLiteDatabase.openDatabase(
				path,
				null,
				SQLiteDatabase.OPEN_READWRITE |
				SQLiteDatabase.CREATE_IF_NECESSARY);
			// TODO: Check null return value?
			return new MoDatabase(db);
		}

		public MoDatabase(SQLiteDatabase db)
		{
			mDB = db;
		}

		public void close()
			throws SQLException
		{
			if (null != mDB)
			{
				mDB.close();
				mDB = null;
			}
		}

		public MoCursor execQuery(String sql, Object[] params)
			throws SQLException
		{
			Cursor cursor = null;

			String trimmedSQL = sql.trim().toUpperCase();

			// SELECT queries need to be called using rawQuery.
			// NOTE: To use BLOB fields in a WHERE clause is not supported.
			if (trimmedSQL.startsWith("SELECT"))
			{
				// For rawQuery we need to have the params as Strings.
				String[] stringParams = null;
				if (null != params)
				{
					stringParams = new String[params.length];
					for (int i = 0; i < params.length; ++i)
					{
						if (null == params[i])
						{
							stringParams[i] = null;
						}
						else if (params[i] instanceof Integer)
						{
							stringParams[i] = params[i].toString();
						}
						else if (params[i] instanceof Long)
						{
							stringParams[i] = params[i].toString();
						}
						else if (params[i] instanceof Double)
						{
							stringParams[i] = params[i].toString();
						}
						else if (params[i] instanceof String)
						{
							stringParams[i] = (String)params[i];
						}
						else if (params[i] instanceof ByteBuffer)
						{
							// TODO: ERROR
							Log.e("@@@@@@",
								"maDBExecSQLParams: BLOB fields are "+
								"not supported in WHERE clauses");
						}
						else
						{
							// TODO: ERROR
							Log.e("@@@@@@",
								"maDBExecSQLParams: Unsupported field type: " +
								params[i].getClass().getName());
						}
					}
				}
				cursor = mDB.rawQuery(sql, stringParams);
			}
			else if (
				trimmedSQL.startsWith("INSERT") ||
				trimmedSQL.startsWith("UPDATE") ||
				trimmedSQL.startsWith("DELETE"))
			{
				// INSERT/UPDATE/DELETE queries need to be called
				// using an SQLiteStatement.
				SQLiteStatement query = mDB.compileStatement(sql);
				if (null != params)
				{
					for (int i = 0; i < params.length; ++i)
					{
						if (null == params[i])
						{
							query.bindNull(i + 1);
						}
						else if (params[i] instanceof Integer)
						{
							query.bindLong(i + 1, ((Integer)params[i]).longValue());
						}
						else if (params[i] instanceof Long)
						{
							query.bindLong(i + 1, ((Long)params[i]).longValue());
						}
						else if (params[i] instanceof Double)
						{
							query.bindDouble(i + 1, ((Double)params[i]).doubleValue());
						}
						else if (params[i] instanceof String)
						{
							query.bindString(i + 1, (String)params[i]);
						}
						else if (params[i] instanceof ByteBuffer)
						{
							ByteBuffer buf = (ByteBuffer) params[i];
							if (buf.hasArray())
							{
								query.bindBlob(i + 1, buf.array());
							}
							else
							{
								query.bindString(i + 1, "ByteBuffer ERROR in column " + i);
							}
						}
						else
						{
							query.bindString(i + 1, "ERROR in column " + i
								+ " " + params[i].getClass().getName());
						}
					}
				}
				query.executeInsert();
				query.releaseReference();
				query.close();
			}
			else
			{
				// Queries other than SELECT/INSERT/UPDATE/DELETE
				// are evaluated here.
				// This is an alternative: mDB.execSQL(sql);
				SQLiteStatement query = mDB.compileStatement(sql);
				query.execute();
				query.releaseReference();
				query.close();
			}

			// If we got a cursor, we return it wrapped in a MoSync
			// cursor object.
			if (null == cursor)
			{
				return null;
			}
			else
			{
				return new MoCursor(cursor);
			}
		}
	}

	/**
	 * Internal class that represents a cursor in a query result.
	 */
	static class MoCursor
	{
		private Cursor mCursor;

		public MoCursor(Cursor cursor)
		{
			mCursor = cursor;
		}

		public boolean next()
			throws SQLException
		{
			return mCursor.moveToNext();
		}

		public boolean isNull(int columnIndex)
			throws SQLException
		{
			return mCursor.isNull(columnIndex);
		}

		public byte[] getData(int columnIndex)
			throws SQLException
		{
			return mCursor.getBlob(columnIndex);
		}

		public String getText(int columnIndex)
			throws SQLException
		{
			return mCursor.getString(columnIndex);
		}

		public int getInt(int columnIndex)
			throws SQLException
		{
			return mCursor.getInt(columnIndex);
		}

		public double getDouble(int columnIndex)
			throws SQLException
		{
			return mCursor.getDouble(columnIndex);
		}
	}
}
