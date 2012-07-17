#include <stdio.h>
#include <string.h>
#include "sqlite3.h"
#include "MoSyncDB.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
  int i;
  for(i=0; i<argc; i++){
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

#if(0)
int sqlite3_prepare_v2(
  sqlite3 *db,            /* Database handle */
  const char *zSql,       /* SQL statement, UTF-8 encoded */
  int nByte,              /* Maximum length of zSql in bytes. */
  sqlite3_stmt **ppStmt,  /* OUT: Statement handle */
  const char **pzTail     /* OUT: Pointer to unused portion of zSql */
);

int sqlite3_step(sqlite3_stmt*);

SQLITE_ROW
SQLITE_DONE
#endif

int testdb(){
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("c:\\cprog\\testdb.db", &db);
  if( rc ){
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return(1);
  }
  rc = sqlite3_exec(db, "CREATE TABLE pets (name TEXT(50), age INTEGER)", callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  rc = sqlite3_exec(db, "INSERT INTO pet VALUES ('Kurre', 13)", callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  rc = sqlite3_exec(db, "INSERT INTO pet VALUES ('Vilma', 10)", callback, 0, &zErrMsg);
  if( rc!=SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
  return 0;
}

int testdb2()
{
	int db = maDBOpen("c:\\cprog\\testdb.db");
	if (db < 1)
	{
		printf("maDBOpen failed\n");
	}

	maDBExecSQL(db, "DROP TABLE pet");

	int result = maDBExecSQL(db,
		"CREATE TABLE pet (name TEXT(50), age INTEGER, playfulness DOUBLE)");
	if (0 != result)
	{
		printf("CREATE TABLE failed\n");
	}

	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Kurre', 13, 0.65)");
	if (0 != result)
	{
		printf("INSERT 1 failed\n");
	}
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Vilma', 10, 0.999)");
	if (0 != result)
	{
		printf("INSERT 2 failed\n");
	}

	// Test to get the number of rows.
	MAHandle cursor1 = maDBExecSQL(db, "SELECT COUNT(*) FROM (SELECT * FROM pet)");
	if (cursor1 < 1)
	{
		printf("SELECT COUNT failed\n");
	}
	maDBCursorNext(cursor1);
	int numberOfRows;
	maDBCursorGetColumnInt(cursor1, 0, &numberOfRows);
	printf("Number of rows: %d\n", numberOfRows);
	maDBCursorDestroy(cursor1);

	MAHandle cursor = maDBExecSQL(db, "SELECT * FROM pet");
	if (cursor < 1)
	{
		printf("SELECT failed\n");
	}

	// Print all rows.
	char name[51];
	int age;
	double playfulness;
	while (0 == maDBCursorNext(cursor))
	{
		int length = maDBCursorGetColumnText(cursor, 0, name, 50);
		printf("length: %d\n", length);
		if (length <= 50)
		{
			name[length] = 0;
		}
		else
		{
			strcpy(name, "ERROR: name buffer too small");
		}
		maDBCursorGetColumnInt(cursor, 1, &age);
		maDBCursorGetColumnDouble(cursor, 2, &playfulness);
		printf("%s %d %f\n", name, age, playfulness);
	}

	// Free the cursor.
	maDBCursorDestroy(cursor);

	// Close the database.
	maDBClose(db);
}

int main()
{
	return testdb2();
}
