/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <conprint.h>
#include <maassert.h>

#include "MAHeaders.h"

#define BUFSIZE 1024

Handle connect();
int read(Handle conn);
void write(Handle conn, int size);
int waitConn(Handle conn);

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	maCreateData(RES, BUFSIZE);

	Handle conn = connect();
	while(true) {
		int size = read(conn);
		write(conn, size);
	}
	return 0;
}

int read(Handle conn) {
	printf("Read...\n");
	maConnReadToData(conn, RES, 0, BUFSIZE);
	return waitConn(conn);
}

void write(Handle conn, int size) {
	printf("Write...\n");
	maConnWriteFromData(conn, RES, 0, size);
	waitConn(conn);
}

Handle connect() {
	printf("Connect...\n");
	Handle conn = maConnect("socket://lazermasken.fetftp.nu:1234");
	maConnClose(conn);
	waitConn(conn);
	return conn;
}

int waitConn(Handle conn) {
	while(1) {
		EVENT event;
		if(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maConnClose(conn);
				maExit(0);
			} else if(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_1) {
				maConnClose(conn);
			} else if(event.type == EVENT_TYPE_CONN) {
				printf("Result: %i\n", event.conn.result);
				if(event.conn.result <= 0) {
					FREEZE;
				}
				return event.conn.result;
			}
		}
		maWait(0);
	}
}
