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
