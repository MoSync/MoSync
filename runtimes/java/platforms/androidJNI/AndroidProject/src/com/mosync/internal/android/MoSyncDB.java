package com.mosync.internal.android;

import java.nio.ByteBuffer;
import java.util.HashMap;

import android.database.Cursor;
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
	private int mDatabaseCounter = 0;
	private HashMap<Integer, MoDatabase> mDatabaseTable =
		new HashMap<Integer, MoDatabase>();

	private int mCursorCounter = 0;
	private HashMap<Integer, MoCursor> mCursorTable =
		new HashMap<Integer, MoCursor>();

	public MoSyncDB()
	{
	}

	/**
	 * Open a database, the database is created if it does not exist.
	 * @param path Full path to database file name.
	 * @return Handle to the database >0 on success, #MA_DB_ERROR on error.
	 */
	public int maDBOpen(String path)
	{
		MoDatabase database = MoDatabase.create(path);
		if (null != database)
		{
			++mDatabaseCounter;
			addDatabase(mDatabaseCounter, database);
			return mDatabaseCounter;
		}
		else
		{
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
		if (hasDatabase(databaseHandle))
		{
			getDatabase(databaseHandle).close();
			removeDatabase(databaseHandle);
			return MA_DB_OK;
		}

		return MA_DB_ERROR;
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
		if (!hasDatabase(databaseHandle)) { return -1; }

		// SELECT statements return values and are handled differently than
		// other statements.
		// See: http://developer.android.com/reference/android/database/sqlite/SQLiteDatabase.html#execSQL(java.lang.String)
		if (sql.toLowerCase().contains("select"))
		{
			MoCursor cursor = getDatabase(databaseHandle).execQuery(sql);
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
			return getDatabase(databaseHandle).execSQL(sql);
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
		if (!hasCursor(cursorHandle)) { return MA_DB_ERROR; }

		removeCursor(cursorHandle);
		return MA_DB_OK;
	}

	/**
	 * Returns the number of rows in the result set pointed
	 * to by the cursor.
	 * @param cursorHandle Handle to the cursor.
	 * @return The number of rows in the result set, MA_DB_ERROR on error.
	 */
	public int maDBCursorGetRowCount(int cursorHandle)
	{
		if (!hasCursor(cursorHandle)) { return MA_DB_ERROR; }

		return getCursor(cursorHandle).getRowCount();
	}

	/**
	 * Move the cursor to the next row in the result set.
	 * @param cursorHandle Handle to the cursor.
	 * @return MA_DB_NO_ROW if there are no more rows in the result set,
	 * MA_DB_OK if successfully moved to next row, MA_DB_ERROR on error.
	 */
	public int maDBCursorNext(int cursorHandle)
	{
		if (!hasCursor(cursorHandle)) { return MA_DB_ERROR; }

		if (getCursor(cursorHandle).next())
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
	 * @param mosync The MoSyncThread instance.
	 * @return MA_DB_OK on success, MA_DB_ERROR on error.
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

		// Get data as a byte array.
		byte[] data = getCursor(cursorHandle).getData(columnIndex);
		if (null == data)
		{
			return MA_DB_ERROR;
		}

		// Put the data in the table of resources. This will make
		// the placeholder reference the data.
		boolean success = mosync.loadBinary(
			placeholder,
			ByteBuffer.wrap(data));
		if (success)
		{
			return MA_DB_OK;
		}
		else
		{
			return MA_DB_ERROR;
		}
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
	 * returns MA_DB_ERROR on other errors.
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

		// Get data as a byte array.
		byte[] data = getCursor(cursorHandle).getData(columnIndex);
		if (null == data)
		{
			return MA_DB_ERROR;
		}

		// Write to MoSync memory. Only copy data if it fits
		// in the destination buffer.
		if (data.length <= bufferSize)
		{
			ByteBuffer buffer = mosync.getMemorySlice(
				bufferAddress,
				data.length);
			buffer.put(data);
		}

		// Return length of the data.
		return data.length;
	}

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as int data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param intValueAddress Adddress to int to receive the value.
	 * @param mosync The MoSyncThread instance.
	 * @return MA_DB_OK on success, MA_DB_ERROR on error.
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

		// Get data as an int value.
		int value = getCursor(cursorHandle).getInt(columnIndex);

		// Write to MoSync memory. Size of an int is 4.
		// TODO: Verify that MoSync uses the same memory layout
		// as putInt(). Check endianness etc. Look at how this
		// is done in the Bluetooth API.
		ByteBuffer buffer = mosync.getMemorySlice(intValueAddress, 4);
		buffer.putInt(value);

		return MA_DB_OK;
	}

	/**
	 * Get the column value at the current row pointed to
	 * by the cursor as float data.
	 * @param cursorHandle Handle to the cursor.
	 * @param columnIndex Index of the column to retrieve value from.
	 * First column has index zero.
	 * @param floatValueAddress Address to float to receive the value.
	 * @param mosync The MoSyncThread instance.
	 * @return #MA_DB_OK on success, #MA_DB_ERROR on error.
	 */
	public int maDBCursorGetColumnFloat(
		int cursorHandle,
		int columnIndex,
		int floatValueAddress,
		MoSyncThread mosync)
	{
		if (!hasCursor(cursorHandle))
		{
			return MA_DB_ERROR;
		}

		// Get data as a float value.
		float value = getCursor(cursorHandle).getFloat(columnIndex);

		// Write to MoSync memory. Size of a float is 4.
		// TODO: Verify that MoSync uses the same memory layout
		// as putFloat(). Check endianness etc. Look at how this
		// is done in the Bluetooth API.
		ByteBuffer buffer = mosync.getMemorySlice(floatValueAddress, 4);
		buffer.putFloat(value);

		return MA_DB_OK;
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
		{
			try
			{
				SQLiteDatabase db = SQLiteDatabase.openDatabase(
					path,
					null,
					SQLiteDatabase.OPEN_READWRITE |
					SQLiteDatabase.CREATE_IF_NECESSARY);
				return new MoDatabase(db);
			}
			catch (SQLiteException ex)
			{
				ex.printStackTrace();
				return null;
			}
		}

		public MoDatabase(SQLiteDatabase db)
		{
			mDB = db;
		}

		public void close()
		{
			if (null != mDB)
			{
				mDB.close();
				mDB = null;
			}
		}

		public MoCursor execQuery(String sql)
		{
			Cursor cursor = mDB.rawQuery(sql, null);
			if (null != cursor)
			{
				return new MoCursor(cursor);
			}
			else
			{
				return null;
			}
		}

		public int execSQL(String sql)
		{
			try
			{
				mDB.execSQL(sql);
				return MA_DB_OK;
			}
			catch (SQLException ex)
			{
				ex.printStackTrace();
				return MA_DB_ERROR;
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

		public int getRowCount()
		{
			return mCursor.getCount();
		}

		public boolean next()
		{
			return mCursor.moveToNext();
		}

		public byte[] getData(int columnIndex)
		{
			return mCursor.getBlob(columnIndex);
		}

		public String getText(int columnIndex)
		{
			return mCursor.getString(columnIndex);
		}

		public int getInt(int columnIndex)
		{
			return mCursor.getInt(columnIndex);
		}

		public float getFloat(int columnIndex)
		{
			return mCursor.getFloat(columnIndex);
		}
	}
}
