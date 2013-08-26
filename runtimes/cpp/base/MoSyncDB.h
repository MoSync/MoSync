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

/**
 * @file MoSyncDB.h
 * @author Mikael Kindborg
 *
 * @brief Implemention of the MoSync Database API.
 */

#ifndef MOSYNCDB_H
#define MOSYNCDB_H

#include <helpers/cpp_defs.h>

#ifdef __cplusplus
extern "C" {
#endif

MAHandle maDBOpen(const char* path);
int maDBClose(MAHandle databaseHandle);
MAHandle maDBExecSQL(MAHandle databaseHandle, const char* sql);
MAHandle maDBExecSQLParams(MAHandle databaseHandle, const char* sql,
	const MADBValue* params, int paramCount);
int maDBCursorDestroy(MAHandle cursorHandle);
int maDBCursorNext(MAHandle cursorHandle);
int maDBCursorGetColumnData(
	MAHandle cursorHandle,
	int columnIndex,
	MAHandle placeholder);
int maDBCursorGetColumnText(
	MAHandle cursorHandle,
	int columnIndex,
	void* buffer,
	int bufferSize);
int maDBCursorGetColumnInt(
	MAHandle cursorHandle,
	int columnIndex,
	int* value);
int maDBCursorGetColumnDouble(
	MAHandle cursorHandle,
	int columnIndex,
	double* value);

void MoSyncDBInit(void);
void MoSyncDBClose(void);

#ifdef __cplusplus
}
#endif

#endif
