#include <ma.h>
#include <conprint.h>
#include <MAUtil/String.h>

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
 * Utility function that zero terminates the column text.
 */
void getString(MAHandle cursor, int column, char* buf, int maxSize)
{
   int size = maDBCursorGetColumnText(cursor, column, buf, maxSize);
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
 * Test the database syscalls.
 */
void testDB()
{
	// Open/create the database.
	MAUtil::String path = getLocalPath();
	path += "MikiDB";
	MAHandle db = maDBOpen(path.c_str());
	if (db < 0)
	{
		maPanic(0, "Could not open database.");
	}

	// TODO: Check all return values.

	// Create a new table, drop the table if it exists.
	maDBExecSQL(db, "DROP TABLE person");
	maDBExecSQL(db,
	   "CREATE TABLE person (name TEXT(50), phone TEXT(30), age INTEGER)");

	// Insert values.
	maDBExecSQL(db, "INSERT INTO person VALUES ('Micke', '0707602631', 49)");
	maDBExecSQL(db, "INSERT INTO person VALUES ('Ali', 'xxxxxxxxx', 1)");

	// Query all rows.
	MAHandle cursor = maDBExecSQL(db, "SELECT * FROM person");
	printf("Number of rows %s\n" , maDBCursorGetRowCount(cursor));

	// Print all rows.
	char name[51];
	char phone[31];
	char age[11];
	while (-1 != maDBCursorNext(cursor))
	{
	   getString(cursor, 0, name, 50);
	   getString(cursor, 1, phone, 30);
	   getString(cursor, 2, age, 10);
	   printf("%s %s %s\n", name, phone, age);
	}
	maDBCursorDestroy(cursor);
	maDBClose(db);

	printf("Test passed successfully\n");
}

/**
 * Program that does a simple database test.
 */
extern "C" int MAMain()
{
	MAEvent event;

	printf("Press zero or back to exit\n");
	printf("Touch screen to run DB test\n");

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
			testDB();
		}
	}

	return 0;
}
