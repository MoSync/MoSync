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

#include "MAHeaders.h"
#include <conprint.h>
#include <ma.h>
#include <maassert.h>
#include <MAUtil/Vector.h>

void testConnectOverload(const char* url, bool acceptSuccess) {
	int connects = 0, events = 0;
	int result = 0;
	int conn;
	bool hasConn = false;
	MAUtil::Vector<Handle> conns;
	do {
		EVENT event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CONN) {
				printf("Op %i conn %i result %i\n", event.conn.opType, event.conn.handle, event.conn.result);
				MAASSERT(event.conn.opType == CONNOP_CONNECT);
				conn = event.conn.handle;
				if(acceptSuccess) {
					if(event.conn.result < 0) {
						result = event.conn.result;
					}
				} else {
					result = event.conn.result;
				}
				MAASSERT(event.conn.result != 0);
				hasConn = true;
				events++;
				printf("Event %i\n", events);
				break;
			}
		}
		if(result == 0) {
			conn = maConnect(url);
			conns.add(conn);
			if(conn < 0) {
				printf("maConnect error %i\n", conn);
				result = conn;
				hasConn = false;
			} else {
				connects++;
				printf("Connect %i\n", connects);
			}
		} else if(events != connects)
			maWait(0);
	} while(events != connects);// && connects < 3);
	if(hasConn) {
		printf("Result %i on handle %i after %i connects\n", result, conn, connects);
	} else {
		printf("Result %i after %i connects\n", result, connects);
	}
	printf("Closing %i handles\n", conns.size());
	for(int i=0; i<conns.size(); i++) {
		maConnClose(conns[i]);
	}
	printf("Done.\n");
}

void testReadOverload(const char* url) {
	Handle conn = maConnect(url);
	if(conn < 0) {
		printf("maConnect error %i\n", conn);
		return;
	}

	int result = 0;
	char buffer[1024];
	while(result >= 0) {
		EVENT event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CONN) {
				printf("Op %i result %i\n", event.conn.opType, event.conn.result);
				MAASSERT(event.conn.handle == conn);
				result = event.conn.result;
				MAASSERT(result != 0);
			}
		}
		if(result == 0) {
			maWait(0);
		} else {
			maConnRead(conn, buffer, sizeof(buffer));
		}
	}
	printf("Done.\n");
}


#define DO_TEST_1 0
#define DO_TEST_2 1
#define DO_TEST_3 1
#define DO_TEST_4 0

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

#if DO_TEST_1
	//Connect overload to stealthed port
	//should return CONNMAX error after CONN_MAX tries.
	printf("Test 1\n");
	testConnectOverload("socket://lazermasken.fetftp.nu:6666", false);
#endif

#if DO_TEST_2
	//Connect overload to open port
	printf("Test 2\n");
	testConnectOverload("socket://130.237.3.118:6666", true);
#endif

#if DO_TEST_3
	//Multiple reads from one connection
	printf("Test 3\n");
	//should cause a fatal error.
	testReadOverload("socket://130.237.3.118:6666");
#endif

#if DO_TEST_4
	//Multiple writes to one connection
	printf("Test 4\n");
#endif

	//reads/writes to multiple connections (stability test)

	//Access a non-existant (or already closed) connection

	//HTTP state tests:
	//*Read from unfinished
	//*SetRequestHeader in WRITING, FINISHING or FINISHED states
	//*GetResponseHeader from unfinished
	//*Write to GET, FINISHING or FINISHED

	printf("And we're done.\n");
	FREEZE;
}
