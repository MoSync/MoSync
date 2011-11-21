#include <ma.h>
#include <conprint.h>
#include <MAUtil/String.h>

#define SHOULD_HOLD(condition, errorMessage) \
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

	// If there was an error, return default root path.
	if (size < 0 || size > bufferSize)
	{
		return "/";
	}

	return buffer;
}

/**
 * Utility function that gets a text zero terminated.
 */
int getColumnString(MAHandle cursor, int column, char* buf, int maxSize)
{
	int size = maDBCursorGetColumnText(cursor, column, buf, maxSize);
	if (size >= 0 && size < maxSize)
	{
		buf[size] = 0;
	}
	return size;
}

/**
 * Utility function that gets a text zero terminated.
 */
int getColumnDataAsString(MAHandle cursor, int column, char* buf, int maxSize)
{
	MAHandle data = maCreatePlaceholder();
	int result = maDBCursorGetColumnData(cursor, column, data);
	if (result == MA_DB_OK)
	{
		int size = maGetDataSize(data);
		maReadData(data, buf, 0, size);
		buf[size] = 0;
	}
	return result;
}

/**
 * Test the database syscalls.
 */
void testBasicThings()
{
	int result;

	printf("Test 1 started\n");

	// Open/create the database.
	MAUtil::String path = getLocalPath();
	path += "MikiDB";
	printf("Database path: %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	SHOULD_HOLD(db > 0, "maDBOpen failed");

	// Create a new table, first drop the table if it exists.
	result = maDBExecSQL(db, "DROP TABLE IF EXISTS pet");
	SHOULD_HOLD(MA_DB_OK == result, "DROP TABLE IF EXISTS failed");

	// This drop table should always fail.
	result = maDBExecSQL(db, "DROP TABLE pet");
	SHOULD_HOLD(MA_DB_ERROR == result, "DROP TABLE should have failed");

	// Create table.
	result = maDBExecSQL(db,
		"CREATE TABLE pet (name TEXT(50), age INTEGER, curiosity DOUBLE)");
	SHOULD_HOLD(MA_DB_OK == result, "CREATE TABLE failed");

	// Insert values.
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Kurre', 13, 0.75)");
	SHOULD_HOLD(MA_DB_OK == result, "INSERT 1 failed");
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Vilma', 10, 0.999)");
	SHOULD_HOLD(MA_DB_OK == result, "INSERT 2 failed");
	result = maDBExecSQL(db, "INSERT INTO pet VALUES (NULL, NULL, NULL)");
	SHOULD_HOLD(MA_DB_OK == result, "INSERT 3 failed");

	// Count all rows.
	MAHandle cursorx = maDBExecSQL(db, "SELECT COUNT(*) FROM (SELECT * FROM pet)");
	maDBCursorNext(cursorx);
	int numberOfRows;
	maDBCursorGetColumnInt(cursorx, 0, &numberOfRows);
	printf("Number of rows: %i\n", numberOfRows);
	SHOULD_HOLD(3 == numberOfRows, "Wrong number of rows");
    maDBCursorDestroy(cursorx);

	// Query all rows.
	MAHandle cursor = maDBExecSQL(db, "SELECT * FROM pet");

	// Test and print data in all rows.
	char name[51];
	// Same content as name. Used to test maDBGursorGetColumnData().
	char name2[51];
	int age;
	double curiosity;
	int nameResult;
	int name2Result;
	int ageResult;
	int curiosityResult;
	int row = 0;
	while (MA_DB_OK == maDBCursorNext(cursor))
	{
		// Get and print data.
		nameResult = getColumnString(cursor, 0, name, 50);
		SHOULD_HOLD(MA_DB_ERROR != nameResult, "Error in getColumnString");
		name2Result = getColumnDataAsString(cursor, 0, name2, 50);
		SHOULD_HOLD(MA_DB_ERROR != name2Result, "Error in getColumnString");
		ageResult = maDBCursorGetColumnInt(cursor, 1, &age);
		SHOULD_HOLD(MA_DB_ERROR != ageResult, "Error in getColumnString");
		curiosityResult = maDBCursorGetColumnDouble(cursor, 2, &curiosity);
		SHOULD_HOLD(MA_DB_ERROR != curiosityResult, "Error in getColumnString");

		if (MA_DB_NULL == nameResult) { strcpy(name, "NULL"); }
		if (MA_DB_NULL == name2Result) { strcpy(name2, "NULL"); }
		if (MA_DB_NULL == ageResult) { age = 0; }
		if (MA_DB_NULL == curiosityResult) { curiosity = 0.0; }
		printf("%s %s %d %f\n", name, name2, age, curiosity);

		// Test that data is as expected.
		++row;
		if (1 == row)
		{
			SHOULD_HOLD(0 == strcmp(name, "Kurre"), "Kurre not equal to name");
			SHOULD_HOLD(0 == strcmp(name, name2), "Kurre not equal to name2");
			SHOULD_HOLD(13 == age, "age 13 failed");
			SHOULD_HOLD(curiosity < 0.75001 && curiosity > 0.74999, "curiosity 0.75 failed");
		}
		if (2 == row)
		{
			SHOULD_HOLD(0 == strcmp(name, "Vilma"), "Vilma not equal to name");
			SHOULD_HOLD(0 == strcmp(name, name2), "Vilma not equal to name2");
			SHOULD_HOLD(10 == age, "age 10 failed");
			// Double comparisons are tricky...
			SHOULD_HOLD(0.998 < curiosity && 0.9991 > curiosity,
				"curiosity 0.999 failed");
		}
		if (3 == row)
		{
			SHOULD_HOLD(MA_DB_NULL == nameResult, "MA_DB_NULL != nameResult");
			SHOULD_HOLD(MA_DB_NULL == name2Result, "MA_DB_NULL != name2Result");
			SHOULD_HOLD(MA_DB_NULL == ageResult, "MA_DB_NULL != ageResult");
			SHOULD_HOLD(MA_DB_NULL == curiosityResult, "MA_DB_NULL != curiosityResult");
		}
	}

	result = maDBCursorDestroy(cursor);
	SHOULD_HOLD(MA_DB_OK == result, "maDBCursorDestroy failed");

	result = maDBClose(db);
	SHOULD_HOLD(MA_DB_OK == result, "maDBClose failed");

	printf("Test 1 passed successfully\n");
}

/**
 * Negative tests.
 */
void testThingsThatShouldFail()
{
	int result;

	printf("Test 2 started\n");

	// Open/create the database.
	MAUtil::String path = getLocalPath();
	path += "MikiDB";
	printf("Database path: %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	// Should succeed.
	SHOULD_HOLD(db > 0, "maDBOpen 2 failed");

	// Should always fail.
	result = maDBExecSQL(db, "DROP TABLE foo");
	SHOULD_HOLD(MA_DB_ERROR == result, "Negative 1 failed");

	maSyscallPanicsDisable();
	// Should always fail.
	result = maDBExecSQL(9999, "DROP TABLE foo");
	SHOULD_HOLD(MA_DB_ERROR == result, "Negative 2 failed");
	maSyscallPanicsEnable();

	result = maDBExecSQL(db, "klasj xasdjkas awjlkasd");
	SHOULD_HOLD(MA_DB_ERROR == result, "Negative 3 failed");

	result = maDBClose(db);
	SHOULD_HOLD(MA_DB_OK == result, "maDBClose failed");

	maSyscallPanicsDisable();
	result = maDBCursorDestroy(8888);
	SHOULD_HOLD(MA_DB_ERROR == result, "Negative 4 failed");
	maSyscallPanicsEnable();

	// TODO: Add negative query tests.

	printf("Test 2 passed successfully\n");
}

static void testParameters()
{
	int result;

	printf("Test 3 started\n");

	// Open/create the database.
	MAUtil::String path = getLocalPath();
	path += "MikiDB";
	printf("Database path: %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	SHOULD_HOLD(db > 0, "maDBOpen failed");

	// Create a new table, first drop the table if it exists.
	result = maDBExecSQL(db, "DROP TABLE IF EXISTS para");
	SHOULD_HOLD(MA_DB_OK == result, "DROP TABLE IF EXISTS failed");

	// Create table.
	result = maDBExecSQL(db,
		"CREATE TABLE para (text TEXT(50), int INTEGER, double REAL, i64 INTEGER, blob BLOB, handle BLOB, temp)");
	SHOULD_HOLD(MA_DB_OK == result, "CREATE TABLE failed");

	// Insert value.
	MADBValue params[7];
	printf("sizeof(MA_DB_VALUE): %li\n", sizeof(MADBValue));
	params[0].type = MA_DB_TYPE_TEXT;
	params[0].text.addr = (char*)"some text";
	params[0].text.length = 9;
	params[1].type = MA_DB_TYPE_INT;
	params[1].i = 42;
	params[2].type = MA_DB_TYPE_DOUBLE;
	params[2].d = 3.14;
	params[3].type = MA_DB_TYPE_INT64;
	params[3].d = 1LL << 40;
	params[4].type = MA_DB_TYPE_BLOB;
	params[4].blob.data = (void*)"blob data";
	params[4].blob.size = 8;
	params[5].type = MA_DB_TYPE_DATA;
	params[5].dataHandle = maCreatePlaceholder();
	maCreateData(params[5].dataHandle, 12);
	maWriteData(params[5].dataHandle, "more blob data", 0, 12);
	params[6].type = MA_DB_TYPE_NULL;
	result = maDBExecSQLParams(db, "INSERT INTO para VALUES (?, ?, ?, ?, ?, ?, ?)", params, 7);
	SHOULD_HOLD(MA_DB_OK == result, "INSERT failed");

	// Count all rows.
	MAHandle cursorx = maDBExecSQL(db, "SELECT COUNT(*) FROM (SELECT * FROM para)");
	maDBCursorNext(cursorx);
	int numberOfRows;
	maDBCursorGetColumnInt(cursorx, 0, &numberOfRows);
	printf("Number of rows: %i\n", numberOfRows);
	SHOULD_HOLD(1 == numberOfRows, "Wrong number of rows");
    maDBCursorDestroy(cursorx);

	// Query all rows.
	MAHandle cursor = maDBExecSQL(db, "SELECT * FROM para");

	// Test and print data in all rows.
	result == maDBCursorNext(cursor);
	SHOULD_HOLD(MA_DB_OK == result, "maDBCursorNext failed");
	{
		// Get and print data.
		char buf[50];
		int i;
		//longlong i64;
		double d;

		result = getColumnString(cursor, 0, buf, 50);
		SHOULD_HOLD(result >= 0, "Error in getColumnString");
		SHOULD_HOLD(strcmp(buf, "some text") == 0, "text");

		result = maDBCursorGetColumnInt(cursor, 1, &i);
		SHOULD_HOLD(MA_DB_OK == result, "Error in maDBCursorGetColumnInt");
		SHOULD_HOLD(i == 42, "int");

		result = maDBCursorGetColumnDouble(cursor, 2, &d);
		SHOULD_HOLD(MA_DB_OK == result, "Error in maDBCursorGetColumnDouble");
		SHOULD_HOLD(d > 3.13999 && d < 3.14001, "double");

		// todo
		//result = maDBCursorGetColumnInt64(cursor, 3, &i64);
		//SHOULD_HOLD(MA_DB_OK == result, "Error in maDBCursorGetColumnInt");
		//SHOULD_HOLD(i64 == 1LL << 40, "int64");

		result = getColumnDataAsString(cursor, 4, buf, 50);
		SHOULD_HOLD(MA_DB_OK == result, "Error in getColumnDataAsString");
		SHOULD_HOLD(memcmp(buf, "blob data", 8) == 0, "blob");

		result = getColumnDataAsString(cursor, 5, buf, 50);
		SHOULD_HOLD(MA_DB_OK == result, "Error in getColumnDataAsString");
		SHOULD_HOLD(memcmp(buf, "more blob data", 12) == 0, "data");
	}

	result = maDBCursorNext(cursor);
	SHOULD_HOLD(MA_DB_NO_ROW == result, "MA_DB_NO_ROW failed");

	result = maDBCursorDestroy(cursor);
	SHOULD_HOLD(MA_DB_OK == result, "maDBCursorDestroy failed");

	result = maDBClose(db);
	SHOULD_HOLD(MA_DB_OK == result, "maDBClose failed");

	printf("Test 1 passed successfully\n");
}


/**
 * Test using multiple databases and cursors.
 */
void testMultipleDatabasesAndCursors()
{
	// TODO: Implement.
}

/**
 * Program that does a simple database test.
 */
extern "C" int MAMain()
{
	MAEvent event;

	// printf("Press zero or back to exit\n");
	// printf("Touch screen to run DB test\n");

	maSyscallPanicsEnable();	// just in case it's not enabled by default.

	testParameters();
	testBasicThings();
	testThingsThatShouldFail();
	testMultipleDatabasesAndCursors();

	printf("All tests passed successfully\n");
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
				return 0;
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
