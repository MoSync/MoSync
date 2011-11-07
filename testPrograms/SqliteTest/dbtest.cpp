#include <stdio.h>
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

	int result = maDBExecSQL(db,
		"CREATE TABLE pet (name TEXT(50), age INTEGER)");
	if (0 != result)
	{
		printf("CREATE TABLE failed\n");
	}

	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Kurre', 13)");
	if (0 != result)
	{
		printf("INSERT 1 failed\n");
	}
	result = maDBExecSQL(db, "INSERT INTO pet VALUES ('Vilma', 10)");
	if (0 != result)
	{
		printf("INSERT 2 failed\n");
	}

	MAHandle cursor = maDBExecSQL(db, "SELECT * FROM pet");
	if (cursor < 1)
	{
		printf("SELECT failed\n");
	}

	// Print all rows.
	int age;
	while (0 == maDBCursorNext(cursor))
	{
		maDBCursorGetColumnInt(cursor, 1, &age);
		printf("age: %d\n", age);
	}

	maDBClose(db);
}

int main()
{
	return testdb2();
}
