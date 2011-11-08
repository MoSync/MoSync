#include <ma.h>
#include <conprint.h>
#include <MAUtil/String.h>

#define MUST_HOLD(condition, errorMessage) \
	if (!(condition)) { maPanic(0, errorMessage); }

/**
 * Get the path to the local file system.
 * @return Path that ends with a slash.
 */
MAUtil::String getLocalPath()
{
	int bufferSize = 1024;
	char buffer[bufferSize];

	int size = maGetSystemProperty(
		"mosync.path.local",
		buffer,
		bufferSize);

	// If there was an error, return empty string.
	if (size < 0 || size > bufferSize)
	{
		return "";
	}

	return buffer;
}

/**
 * Utility function that gets a text zero terminated.
 */
void getColumnString(MAHandle cursor, int column, char* buf, int maxSize)
{
	int size = maDBCursorGetColumnText(cursor, column, buf, maxSize);
	MUST_HOLD(MA_DB_ERROR != size, "getColumnString failed");
	if (size < maxSize)
	{
		buf[size] = 0;
	}
	else
	{
		buf[maxSize] = 0;
	}
}

/**
 * Utility function that gets an int value.
 */
int getColumnInt(MAHandle cursor, int column)
{
	int value;
	int success = maDBCursorGetColumnInt(cursor, column, &value);
	MUST_HOLD(MA_DB_OK == success, "getColumnInt failed");
	return value;
}

/**
 * Utility function that gets a float value.
 */
double getColumnDouble(MAHandle cursor, int column)
{
	double value;
	int success = maDBCursorGetColumnDouble(cursor, column, &value);
	MUST_HOLD(MA_DB_OK == success, "getColumnDouble failed");
	return value;
}

/**
 * Test the database syscalls.
 */
void test1()
{
	int result;

	printf("Test 1 started\n");

	// Open/create the database.
	MAUtil::String path = getLocalPath();
	path += "MikiDB";
	printf("Database path: %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	MUST_HOLD(db > 0, "maDBOpen failed");

	// TODO: Check all return values.

	// Create a new table, first drop the table if it exists.
	maDBExecSQL(db, "DROP TABLE pet");

	// This drop table should always fail.
	result = maDBExecSQL(db, "DROP TABLE pet");
	MUST_HOLD(MA_DB_ERROR == result, "DROP TABLE should have failed");

	// Create table.
	result = maDBExecSQL(db,
		"CREATE TABLE pet (name TEXT(50), age INTEGER, curiosity DOUBLE)");
	MUST_HOLD(MA_DB_OK == result, "CREATE TABLE failed");

	// Insert values.
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Kurre', 13, 0.75)");
	MUST_HOLD(MA_DB_OK == result, "INSERT 1 failed");
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Vilma', 10, 0.999)");
	MUST_HOLD(MA_DB_OK == result, "INSERT 2 failed");

	// Count all rows.
	MAHandle cursorx = maDBExecSQL(db, "SELECT COUNT(*) FROM (SELECT * FROM pet)");
	maDBCursorNext(cursorx);
	int numberOfRows = getColumnInt(cursorx, 0);
	printf("Number of rows: %i\n", numberOfRows);
	MUST_HOLD(2 == numberOfRows, "Wrong number of rows");
    maDBCursorDestroy(cursorx);

	// Query all rows.
	MAHandle cursor = maDBExecSQL(db, "SELECT * FROM pet");

	// Print all rows.
	char name[51];
	int age;
	double curiosity;
	int row = 0;
	while (MA_DB_OK == maDBCursorNext(cursor))
	{
		// Get and print data.
		getColumnString(cursor, 0, name, 50);
		age = getColumnInt(cursor, 1);
		curiosity = getColumnDouble(cursor, 2);
		printf("%s %d %f\n", name, age, curiosity);

		// Test that data is as expected.
		++row;
		if (1 == row)
		{
			MUST_HOLD(0 == strcmp(name, "Kurre"), "name Kurre failed");
			MUST_HOLD(13 == age, "age 13 failed");
			MUST_HOLD(0.75 == curiosity, "curiosity 0.75 failed");
		}
		if (2 == row)
		{
			MUST_HOLD(0 == strcmp(name, "Vilma"), "name Vilma failed");
			MUST_HOLD(10 == age, "age 10 failed");
			// Float comparisons are tricky...
			MUST_HOLD(0.998 < curiosity && 0.9991 > curiosity,
				"curiosity 0.999 failed");
		}
	}

	result = maDBCursorDestroy(cursor);
	MUST_HOLD(MA_DB_OK == result, "maDBCursorDestroy failed");

	result = maDBClose(db);
	MUST_HOLD(MA_DB_OK == result, "maDBClose failed");

	printf("Test 1 passed successfully\n");
}


/**
 * Negative tests.
 */
void test2()
{
	int result;

	printf("Test 2 started\n");

	// Open/create the database.
	MAUtil::String path = getLocalPath();
	path += "MikiDB";
	printf("Database path: %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	// Should succeed.
	MUST_HOLD(db > 0, "maDBOpen 2 failed");

	// Should always fail.
	result = maDBExecSQL(db, "DROP TABLE foo");
	MUST_HOLD(MA_DB_ERROR == result, "Negative 1 failed");

	// Should always fail.
	result = maDBExecSQL(9999, "DROP TABLE foo");
	MUST_HOLD(MA_DB_ERROR == result, "Negative 2 failed");

	result = maDBExecSQL(db, "klasj xasdjkas awjlkasd");
	MUST_HOLD(MA_DB_ERROR == result, "Negative 3 failed");

	result = maDBClose(db);
	MUST_HOLD(MA_DB_OK == result, "maDBClose failed");

	result = maDBCursorDestroy(8888);
	MUST_HOLD(MA_DB_ERROR == result, "Negative 4 failed");

	// TODO: Add negative query tests.

	printf("Test 2 passed successfully\n");
}

/**
 * Program that does a simple database test.
 */
extern "C" int MAMain()
{
	MAEvent event;

	// printf("Press zero or back to exit\n");
	// printf("Touch screen to run DB test\n");

	test1();
	test2();
	printf("Press zero or back to exit\n");

	while (TRUE)
	{
		maWait(100);
		maGetEvent(&event);

		if (EVENT_TYPE_CLOSE == event.type)
		{
			break;
		}
		else if (EVENT_TYPE_KEY_PRESSED == event.type)
		{
			if (MAK_BACK == event.key || MAK_0 == event.key)
			{
				break;
			}
		}
		else if (EVENT_TYPE_POINTER_PRESSED == event.type)
		{
//			test1();
//			test2();
		}
	}

	return 0;
}
