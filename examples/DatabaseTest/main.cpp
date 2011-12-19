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
 * @file main.cpp
 * @author Mikael Kindborg
 *
 * This is a test program that illustrates how to use the SQL API.
 *
 * The program exits with a panic message if a test fails.
 *
 * Touch the screen to start running the test. A database file will
 * be created on the device, in the local file system of the application.
 *
 * If the test is successful the text "Database test passed successfully"
 * is displayed at the end of the output.
 *
 * If you run in MoRE and output text strings are truncated, select
 * a device profile with a bigger screen, then run again.
 */

#include <MAUtil/Moblet.h>
#include <MAUtil/String.h>
#include <maheap.h>
#include <conprint.h>

using namespace MAUtil;

// Macro used to test for a condition. Shows a panic
// if the condition is false.
#define SHOULD_HOLD(condition, errorMessage) \
	if (!(condition)) { maPanic(0, errorMessage); }

/**
 * Utility class that simplifies using the Database API.
 */
class DBUtil
{
public:
	/**
	 * Get the path to the local file system.
	 * @param path Reference to string that will
	 * receive the path. The path ends with a slash.
	 * @return true on success, false on error.
	 */
	static bool getLocalPath(String& path)
	{
		int bufferSize = 1024;
		char buffer[bufferSize];

		int size = maGetSystemProperty(
			"mosync.path.local",
			buffer,
			bufferSize);

		// Check if there was an error.
		if (size < 0 || size > bufferSize)
		{
			// Error.
			return false;
		}
		else
		{
			// Success
			path = buffer;
			return true;
		}
	}

	/**
	 * Utility function that gets a text field as a string, using the
	 * maDBCursorGetColumnText function.
	 * @param cursor Handle to a cursor object.
	 * @param column Zero-based index to the column to retrieve.
	 * @param path Reference to string that will receive the field data.
	 * @return MA_DB_OK on success, MA_DB_NULL if the field value is NULL,
	 * or MA_DB_ERROR on error.
	 */
	static int getColumnString(MAHandle cursor, int column, String& text)
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

	/**
	 * Utility function that gets a text field as a string, using the
	 * maDBCursorGetColumnData function.
	 * @param cursor Handle to a cursor object.
	 * @param column Zero-based index to the column to retrieve.
	 * @param path Reference to string that will receive the field data.
	 * @return MA_DB_OK on success, MA_DB_NULL if the field value is NULL,
	 * or MA_DB_ERROR on error.
	 */
	static int getColumnDataAsString(MAHandle cursor, int column, String& text)
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
};

/**
 * Class that contains the database tests.
 */
class DBTest
{
public:
	void runTest()
	{
		printf("Starting database test\n");

		MAHandle db = openDatabase();
		createTable(db);
		countRows(db);
		queryRows(db);

		printf("Database test passed successfully\n");
	}

	MAHandle openDatabase()
	{
		printf("Open database\n");

		// Open the database. It will be created if it does not exist.
		// The database will be created in the local directory of the
		// application.
		MAUtil::String path;
		bool success = DBUtil::getLocalPath(path);
		SHOULD_HOLD(success, "getLocalPath failed");
		path += "PetsDatabase";
		printf("Database path:\n  %s\n", path.c_str());
		MAHandle db = maDBOpen(path.c_str());
		SHOULD_HOLD(db > 0, "maDBOpen failed");

		printf("Open database success\n");

		return db;
	}

	void createTable(MAHandle db)
	{
		int result;

		printf("Create table\n");

		// Create a new table, first drop the table if it exists.
		result = maDBExecSQL(db, "DROP TABLE IF EXISTS pet");
		SHOULD_HOLD(MA_DB_OK == result, "DROP TABLE IF EXISTS failed");

		// Create a table called "pet", it will contain info about pets.
		result = maDBExecSQL(db,
			"CREATE TABLE pet (name TEXT(50), age INTEGER, curiosity DOUBLE)");
		SHOULD_HOLD(MA_DB_OK == result, "CREATE TABLE failed");

		// Insert rows into the table.
		result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Charmy', 7, 0.75)");
		SHOULD_HOLD(MA_DB_OK == result, "INSERT failed 1");
		result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Sporty', 4, 0.99)");
		SHOULD_HOLD(MA_DB_OK == result, "INSERT failed 2");
		result = maDBExecSQL(db, "INSERT INTO pet VALUES (NULL, NULL, NULL)");
		SHOULD_HOLD(MA_DB_OK == result, "INSERT failed 3");

		printf("Create table success\n");
	}

	void countRows(MAHandle db)
	{
		int result;

		printf("Count rows\n");

		// Count all rows.
		MAHandle cursor = maDBExecSQL(
			db,
			"SELECT COUNT(*) FROM (SELECT * FROM pet)");
		SHOULD_HOLD(cursor > 0, "SELECT COUNT(*) failed");

		// The result should contain one row with one field.
		// We must call maDBCursorNext to advance the cursor to the result row.
		result = maDBCursorNext(cursor);
		SHOULD_HOLD(MA_DB_OK == result, "maDBCursorNext failed");

		// Get the field with the row count,
		int numberOfRows;
		result = maDBCursorGetColumnInt(cursor, 0, &numberOfRows);
		SHOULD_HOLD(MA_DB_OK == result, "maDBCursorGetColumnInt failed");
		printf("Number of rows: %i\n", numberOfRows);
		SHOULD_HOLD(3 == numberOfRows, "Wrong number of rows");

		// Free the cursor.
	    maDBCursorDestroy(cursor);

		printf("Count rows success\n");
	}

	void queryRows(MAHandle db)
	{
		printf("Query rows\n");

		// Query all rows.
		MAHandle cursor = maDBExecSQL(db, "SELECT * FROM pet");
		SHOULD_HOLD(cursor > 0, "SELECT * failed");

		// Here follows variables that hold field values,
		// name2 should have same content as name and is
		// used to test maDBGursorGetColumnData().
		String name;
		String name2;
		int age;
		double curiosity;

		// Return values.
		int nameResult;
		int name2Result;
		int ageResult;
		int curiosityResult;

		// Row counter.
		int row = 0;

		// Loop over the rows in the result.
		while (MA_DB_OK == maDBCursorNext(cursor))
		{
			// Get and print data.
			nameResult = DBUtil::getColumnString(cursor, 0, name);
			SHOULD_HOLD(MA_DB_ERROR != nameResult, "Error in getColumnString");
			name2Result = DBUtil::getColumnDataAsString(cursor, 0, name2);
			SHOULD_HOLD(MA_DB_ERROR != name2Result, "Error in getColumnString");
			ageResult = maDBCursorGetColumnInt(cursor, 1, &age);
			SHOULD_HOLD(MA_DB_ERROR != ageResult, "Error in getColumnString");
			curiosityResult = maDBCursorGetColumnDouble(cursor, 2, &curiosity);
			SHOULD_HOLD(MA_DB_ERROR != curiosityResult, "Error in getColumnString");

			if (MA_DB_NULL == nameResult) { name = "NULL"; }
			if (MA_DB_NULL == name2Result) { name2 = "NULL"; }
			if (MA_DB_NULL == ageResult) { age = 0; }
			if (MA_DB_NULL == curiosityResult) { curiosity = 0.0; }
			printf("%s %s %d %f\n", name.c_str(), name2.c_str(), age, curiosity);

			// Test that data is as expected.
			++row;
			if (1 == row)
			{
				SHOULD_HOLD(name == "Charmy", "Charmy not equal to name");
				SHOULD_HOLD(name == name2, "Charmy not equal to name2");
				SHOULD_HOLD(7 == age, "Charmy age failed");
				// Double comparisons are tricky...
				SHOULD_HOLD(curiosity < 0.751 && curiosity > 0.749,
					"Charmy curiosity failed");
			}
			else if (2 == row)
			{
				SHOULD_HOLD(name == "Sporty", "Sporty not equal to name");
				SHOULD_HOLD(name == name2, "Sporty not equal to name2");
				SHOULD_HOLD(4 == age, "Sporty age failed");
				// Double comparisons are tricky...
				SHOULD_HOLD(0.989 < curiosity && 0.991 > curiosity,
					"curiosity failed");
			}
			else if (3 == row)
			{
				SHOULD_HOLD(MA_DB_NULL == nameResult, "MA_DB_NULL != nameResult");
				SHOULD_HOLD(MA_DB_NULL == name2Result, "MA_DB_NULL != name2Result");
				SHOULD_HOLD(MA_DB_NULL == ageResult, "MA_DB_NULL != ageResult");
				SHOULD_HOLD(MA_DB_NULL == curiosityResult, "MA_DB_NULL != curiosityResult");
			}
		}

		int result = maDBCursorDestroy(cursor);
		SHOULD_HOLD(MA_DB_OK == result, "maDBCursorDestroy failed");

		result = maDBClose(db);
		SHOULD_HOLD(MA_DB_OK == result, "maDBClose failed");

		printf("Query rows success\n");
	}
};

/**
 * Moblet that runs the database tests.
 */
class DBTestMoblet : public Moblet
{
public:
	/**
	 * Initialize the application in the constructor.
	 */
	DBTestMoblet()
	{
		printf("Touch screen to run database test\n");
		printf("Press zero or back (on Android) to exit\n");
	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Closes the application.
			close();
		}
	}

	/**
	 * Called when the screen is touched.
	 */
	void pointerPressEvent(MAPoint2d point)
	{
		DBTest test;
		test.runTest();
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new DBTestMoblet());
	return 0;
}
