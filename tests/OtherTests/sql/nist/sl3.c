/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <sqlite3.h>
#include <stdio.h>

struct SqlSelectResultsStruct {
	sqlite3_stmt* s;
};

struct SqlSelectResultsStruct sSSRS;

#ifndef NULL
#define NULL 0
#endif

#define CREATE(table) sqlExec("CREATE TABLE IF NOT EXISTS " table "")

#define CHECK(res, name) if(res != SQLITE_OK) {\
	printf("%s: %s (%i)\n", name, sqlite3_errmsg(sHandle), res); }

static sqlite3* sHandle = NULL;

static void init(void) {
	if(sHandle)
		return;

	// todo: error handling
	sqlite3_open("build/db.bin", &sHandle);
	sqlite3_extended_result_codes(sHandle, 1);
	CREATE("'HU.ECCO' (C1 CHAR(2))");
	CREATE("STAFF (EMPNUM CHAR(3) NOT NULL UNIQUE,"
		"EMPNAME  CHAR(20),"
		"GRADE    DECIMAL(4),"
		"CITY     CHAR(15))");
	CREATE("PROJ   (PNUM     CHAR(3) NOT NULL UNIQUE,"
		"PNAME    CHAR(20),"
		"PTYPE    CHAR(6),"
		"BUDGET   DECIMAL(9),"
		"CITY     CHAR(15))");
	CREATE("WORKS (EMPNUM   CHAR(3) NOT NULL,"
		"PNUM     CHAR(3) NOT NULL,"
		"HOURS    DECIMAL(5),"
		"UNIQUE(EMPNUM,PNUM))");
}

void sqlExec(const char* sqlString) {
	char* err;
	int res;
	init();
	// todo: abort on error
	res = sqlite3_exec(sHandle, sqlString, NULL, NULL, &err);
	if(res != SQLITE_OK) {
		printf("sqlite3_exec: %s\n", err);
	}
}

// make sure only one row is returned.
SqlSelectResults sqlSelect(const char* sqlString) {
	int res;

	init();

	// todo: abort on error
	res = sqlite3_prepare_v2(sHandle, sqlString, -1, &sSSRS.s, NULL);
	CHECK(res, "sqlite3_prepare_v2");

	res = sqlite3_step(sSSRS.s);
	if(res == SQLITE_OK) {
		printf("sqlite3_step returned SQLITE_OK!\n");
	}
	if(res != SQLITE_ROW) {
		CHECK(res, "sqlite3_step");
	}
	return &sSSRS;
}

long sqlGetLong(SqlSelectResults ssr, int index) {
	return sqlite3_column_int(ssr->s, index);
}

void sqlFinalize(SqlSelectResults ssr) {
	int res = sqlite3_step(ssr->s);
	if(res == SQLITE_OK) {
		printf("sqlite3_step returned SQLITE_OK!\n");
	}
	if(res == SQLITE_ROW) {
		printf("sqlite3_step returned multiple rows!\n");
	}
	if(res != SQLITE_DONE) {
		CHECK(res, "sqlite3_step");
	}

	res = sqlite3_finalize(ssr->s);
	CHECK(res, "sqlite3_finalize");
}
