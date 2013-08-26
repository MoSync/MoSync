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
