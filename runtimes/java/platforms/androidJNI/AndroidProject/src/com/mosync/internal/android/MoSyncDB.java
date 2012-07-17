package com.mosync.internal.android;

import java.nio.ByteBuffer;
import java.util.HashMap;

import android.database.Cursor;
import android.database.CursorIndexOutOfBoundsException;
import android.database.SQLException;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteException;
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
		if (!hasDatabase(databaseHandle))
		{
			return MA_DB_ERROR;
		}

		MoDatabase database = getDatabase(databaseHandle);

		try
		{
			// SELECT statements return values and are handled differently than
			// other statements.
			// See: http://developer.android.com/reference/android/database/sqlite/SQLiteDatabase.html#execSQL(java.lang.String)
			if (sql.toLowerCase().contains("select"))
			{
				MoCursor cursor = database.execQuery(sql);
				if (null != cursor)
				{
					++mCursorCounter;
					addCursor(mCursorCounter, cursor);
					return mCursorCounter;
				}
				else
				{
					return MA_DB_ERROR;
				}
			}
			else
			{
				database.execSQL(sql);
				return MA_DB_OK;
			}
		}
		catch (SQLiteException ex)
		{
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

		public MoCursor execQuery(String sql)
			throws SQLException
		{
			Cursor cursor = mDB.rawQuery(sql, null);
			// TODO: Check null return value?
			return new MoCursor(cursor);
		}

		public void execSQL(String sql)
			throws SQLException
		{
			mDB.execSQL(sql);
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
