/*
 * dbtest.cpp
 *
 *  Created on: May 15, 2013
 *      Author:
 */

#include <conprint.h>

#include <MAUtil/String.h>

#include "dbtest.h"
#include "dbutil.h"


using namespace MAUtil;

void DBTest::runTest()
{
	printf("Starting database test\n");

	MAHandle db = openDatabase();
	createTable(db);
	deleteRow(db);
	updateRow(db);
	countRows(db);
	queryRows(db);
}

MAHandle DBTest::openDatabase()
{
	printf("******************************\n");
	printf("Open database\n");

	// Open the database. It will be created if it does not exist.
	// The database will be created in the local directory of the
	// application.
	MAUtil::String path = DBUtil::getLocalPath();
	path += "PetsDatabase";
	printf("Database path:\n   %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	shouldhold(db > 0, "ERROR: maDBOpen failed", "maDBOpen success");

	return db;
}

void DBTest::createTable(MAHandle db)
{
	printf("******************************\n");
	printf("Create table\n");

	// Create a new table, first drop the table if it exists.
	int result = maDBExecSQL(db, "DROP TABLE IF EXISTS pet");
	shouldhold(MA_DB_OK == result, "ERROR: DROP TABLE IF EXISTS failed", "DROP TABLE IF EXISTS success");

	// Create a table called "pet", it will contain info about pets.
	result = maDBExecSQL(db,
		"CREATE TABLE pet (name TEXT(50), age INTEGER, curiosity DOUBLE)");
	shouldhold(MA_DB_OK == result, "ERROR: CREATE TABLE failed", "CREATE TABLE success");

	// Insert rows into the table.
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Charmy', 7, 0.75)");
	shouldhold(MA_DB_OK == result, "ERROR: INSERT failed 1", "INSERT 1 success");
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Sporty', 4, 0.99)");
	shouldhold(MA_DB_OK == result, "ERROR: INSERT failed 2", "INSERT 2 success");
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Rookie', 0, 0.0)");
	shouldhold(MA_DB_OK == result, "ERROR: INSERT failed 3", "INSERT 3 success");
	result = maDBExecSQL(db, "INSERT INTO pet VALUES (NULL, NULL, NULL)");
	shouldhold(MA_DB_OK == result, "ERROR: INSERT failed 4", "INSERT 4 success");
}


void DBTest::updateRow(MAHandle db)
{
	printf("******************************\n");
	printf("Update row\n");

	// Update a row in the table.
	int result = maDBExecSQL(db, "UPDATE pet SET name='Snoopy' WHERE name='Sporty'");
	shouldhold(MA_DB_OK == result, "ERROR: UPDATE failed", "UPDATE success");
}


void DBTest::deleteRow(MAHandle db)
{
	printf("******************************\n");
	printf("Delete row\n");

	// Delete a row in the table.
	int result = maDBExecSQL(db, "DELETE FROM pet WHERE name='Rookie'");
	shouldhold(MA_DB_OK == result, "ERROR: DELETE failed", "DELETE success");
}


void DBTest::countRows(MAHandle db)
{
	printf("******************************\n");
	printf("Count rows\n");

	// Count all rows.
	MAHandle cursor = maDBExecSQL(
		db,
		"SELECT COUNT(*) FROM (SELECT * FROM pet)");
	shouldhold(cursor > 0, "ERROR: SELECT COUNT(*) failed", "SELECT COUNT(*) success");

	// The result should contain one row with one field.
	// We must call maDBCursorNext to advance the cursor to the result row.
	int result = maDBCursorNext(cursor);
	shouldhold(MA_DB_OK == result, "ERROR: maDBCursorNext failed", "maDBCursorNext success");

	// Get the field with the row count,
	int numberOfRows;
	result = maDBCursorGetColumnInt(cursor, 0, &numberOfRows);
	shouldhold(MA_DB_OK == result, "ERROR: maDBCursorGetColumnInt failed", "maDBCursorGetColumnInt success");
	printf("Number of rows: %i\n", numberOfRows);
	shouldhold(3 == numberOfRows, "ERROR: Wrong number of rows", "OK number of rows");

	// Free the cursor.
	maDBCursorDestroy(cursor);
}

void DBTest::queryRows(MAHandle db)
{
	MAHandle cursor;
	printf("*******************************************\nQuery rows\n");

	// Check that query that returns no rows works.
	int result = maDBExecSQL(db, "SELECT * FROM pet WHERE name='NameThatDoesNotExist'");

	//printf("result: %i\n", result);
	shouldhold(MA_DB_OK == result, "ERROR: SELECT did not return MA_DB_OK", "SELECT returned MA_DB_OK");

	// Query all rows.
	cursor = maDBExecSQL(db, "SELECT * FROM pet");
	//printf("result: %i\n", cursor);
	shouldhold(cursor > 0, "ERROR: SELECT * failed", "SELECT * success");

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
		shouldhold(MA_DB_ERROR != nameResult, "ERROR in getColumnString", "ok in getColumnString");
		name2Result = DBUtil::getColumnDataAsString(cursor, 0, name2);
		shouldhold(MA_DB_ERROR != name2Result, "ERROR in getColumnString", "ok in getColumnString");
		ageResult = maDBCursorGetColumnInt(cursor, 1, &age);
		shouldhold(MA_DB_ERROR != ageResult, "ERROR in getColumnString", "ok in getColumnString");
		curiosityResult = maDBCursorGetColumnDouble(cursor, 2, &curiosity);
		shouldhold(MA_DB_ERROR != curiosityResult, "ERROR in getColumnString", "ok in getColumnString");

		if (MA_DB_NULL == nameResult) { name = "NULL"; }
		if (MA_DB_NULL == name2Result) { name2 = "NULL"; }
		if (MA_DB_NULL == ageResult) { age = 0; }
		if (MA_DB_NULL == curiosityResult) { curiosity = 0.0; }
		printf("%s - %s - %d - %f\n", name.c_str(), name2.c_str(), age, curiosity);

		// Test that data is as expected.
		++row;
		if (1 == row)
		{
			shouldhold(name == "Charmy", "ERROR: Charmy not equal to name", "name ok");
			shouldhold(name == name2, "ERROR: Charmy not equal to name2", "name2 ok");
			shouldhold(7 == age, "ERROR: Charmy age failed", "Charmy age ok");
			// Double comparisons are tricky...
			shouldhold(curiosity < 0.751 && curiosity > 0.749,
				"ERROR: Charmy curiosity failed", "Charmmy curiosity ok");
		}
		else if (2 == row)
		{
			shouldhold(name == "Snoopy", "ERROR: Snoopy not equal to name", "name ok");
			shouldhold(name == name2, "ERROR: Snoopy not equal to name2", "name2 ok");
			shouldhold(4 == age, "ERROR: Snoopy age failed", "Snoopy age ok");
			// Double comparisons are tricky...
			shouldhold(0.989 < curiosity && 0.991 > curiosity, "ERROR: curiosity failed", "");
		}
		else if (3 == row)
		{
			shouldhold(MA_DB_NULL == nameResult, "ERROR: MA_DB_NULL != nameResult", "name null ok");
			shouldhold(MA_DB_NULL == name2Result, "ERROR: MA_DB_NULL != name2Result", "name2 null ok");
			shouldhold(MA_DB_NULL == ageResult, "ERROR: MA_DB_NULL != ageResult", "null age ok");
			shouldhold(MA_DB_NULL == curiosityResult, "ERROR: MA_DB_NULL != curiosityResult", "null curiosity ok");
		}
	}

	result = maDBCursorDestroy(cursor);
	shouldhold(MA_DB_OK == result, "ERROR: maDBCursorDestroy failed", "maDBCursorDestroy ok");

	result = maDBClose(db);
	shouldhold(MA_DB_OK == result, "ERROR: maDBClose failed", "maDBClose ok");
}

void DBTest::testParameters()
{
	printf("******************************\n");
	printf("Param test started\n");

	// Open the database (will create it if it does not exist).
	MAUtil::String path = DBUtil::getLocalPath();
	path += "MikiDB";
	printf("Database path: %s\n", path.c_str());
	MAHandle db = maDBOpen(path.c_str());
	shouldhold(db > 0, "ERROR: maDBOpen failed", "maDBOpen ok");

	// Create a new table, first drop the table if it exists.
	int result = maDBExecSQL(db, "DROP TABLE IF EXISTS paramtest");
	shouldhold(MA_DB_OK == result, "ERROR: DROP TABLE IF EXISTS failed", "DROP TABLE IF EXISTS ok");

	// Create table.
	result = maDBExecSQL(db,
		"CREATE TABLE paramtest (textValue TEXT(50), "
		"intValue INTEGER, doubleValue DOUBLE, "
		"blobValue BLOB, dataValue BLOB, nullValue BLOB)");
	shouldhold(MA_DB_OK == result, "ERROR: CREATE TABLE failed", "CREATE TABLE ok");

	// Insert values into the table using a parameter array
	// of type MADBValue.
	MADBValue params[6];

	params[0].type = MA_DB_TYPE_TEXT;
	params[0].text.addr = (char*)"some text";
	params[0].text.length = 9;

	params[1].type = MA_DB_TYPE_INT;
	params[1].i = 42;

	params[2].type = MA_DB_TYPE_DOUBLE;
	params[2].d = 3.14;

	params[3].type = MA_DB_TYPE_BLOB;
	params[3].blob.data = (void*)"blob data";
	params[3].blob.size = 9;

	params[4].type = MA_DB_TYPE_DATA;
	params[4].dataHandle = maCreatePlaceholder();
	maCreateData(params[4].dataHandle, 14);
	maWriteData(params[4].dataHandle, "more blob data", 0, 14);

	params[5].type = MA_DB_TYPE_NULL;

	result = maDBExecSQLParams(
		db,
		"INSERT INTO paramtest VALUES (?, ?, ?, ?, ?, ?)",
		params,
		6);
	shouldhold(MA_DB_OK == result, "ERROR: INSERT failed", "INSERT ok");

	// Count rows, there should be one row.
	MAHandle cursor = maDBExecSQL(
		db,
		"SELECT COUNT(*) FROM (SELECT * FROM paramtest)");
	shouldhold(0 < cursor, "ERROR: SELECT 1 did not return cursor", "SELECT 1 ok");
	maDBCursorNext(cursor);
	int numberOfRows;
	maDBCursorGetColumnInt(cursor, 0, &numberOfRows);
	maDBCursorDestroy(cursor);
	shouldhold(1 == numberOfRows, "ERROR: Wrong number of rows", "number of rows ok");

	// Check that query that returns no rows works.
	result = maDBExecSQLParams(
		db,
		"SELECT * FROM paramtest WHERE "
		"nullValue IS NOT NULL",
		params,
		0
		);
	shouldhold(result == MA_DB_OK, "ERROR: SELECT 2 did not return MA_DB_OK", "SELECT 2 ok");

	// Query all rows and check that column data is correct.
	// Note: To use blob fields in a WHERE clause is not supported.
	cursor = maDBExecSQLParams(
		db,
		"SELECT * FROM paramtest WHERE "
		"textValue=? AND "
		"intValue=? AND "
		"doubleValue=? AND "
		"nullValue IS NULL",
		params,
		3
		);
	shouldhold(0 < cursor, "ERROR: SELECT 3 did not return cursor", "select 3 ok");
	result = maDBCursorNext(cursor);
	shouldhold(MA_DB_OK == result, "ERROR: maDBCursorNext failed", "maDBCursorNext ok");

	String text;
	result = DBUtil::getColumnString(cursor, 0, text);
	shouldhold(result >= 0, "ERROR: Could not get value of column 1", "");
	shouldhold(strcmp(text.c_str(), "some text") == 0, "ERROR: text", "");

	int i;
	result = maDBCursorGetColumnInt(cursor, 1, &i);
	shouldhold(MA_DB_OK == result, "ERROR: Could not get value of column 2","");
	shouldhold(i == 42, "ERROR: int", "");

	double d;
	result = maDBCursorGetColumnDouble(cursor, 2, &d);
	shouldhold(MA_DB_OK == result, "ERROR: Could not get value of column 3", "");
	shouldhold(d > 3.13999 && d < 3.14001, "ERROR: double", "");

	result = DBUtil::getColumnDataAsString(cursor, 3, text);
	shouldhold(MA_DB_OK == result, "ERROR: Could not get value of column 4", "");
	shouldhold(memcmp(text.c_str(), "ERROR: blob data", 9) == 0, "blob", "");

	result = DBUtil::getColumnDataAsString(cursor, 4, text);
	shouldhold(MA_DB_OK == result, "ERROR: Could not get value of column 5", "");
	shouldhold(memcmp(text.c_str(), "ERROR: more blob data", 14) == 0, "data", "");

	result = DBUtil::getColumnDataAsString(cursor, 5, text);
	shouldhold(MA_DB_NULL == result, "ERROR: Column 6 is not NULL", "");

	// There should be no more rows.
	result = maDBCursorNext(cursor);
	shouldhold(MA_DB_NO_ROW == result, "ERROR: MA_DB_NO_ROW failed", "");

	result = maDBCursorDestroy(cursor);
	shouldhold(MA_DB_OK == result, "ERROR: maDBCursorDestroy failed", "");

	result = maDBClose(db);
	shouldhold(MA_DB_OK == result, "ERROR: maDBClose failed", "");
}

void DBTest::shouldhold(bool condition, MAUtil::String errorMessage, MAUtil::String succesMessage)
{
	if (!condition)
	{
		printf(errorMessage.c_str());
		printf("\n");
	}
	//else printf(succesMessage.c_str());
}
