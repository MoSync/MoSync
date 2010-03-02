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

#include <ma.h>
#include <conprint.h>
#include <maassert.h>

void run();

int MAMain() {
	MAHandle conn;
	int result = 0;

	InitConsole();
	gConsoleLogging = 1;
	
	printf("Hello Gyro!\n");
	//conn = maConnect("btspp://000195061869:1");	//GyroDRM
	//conn = maConnect("btspp://000b0d14990f:1");	//Holux 1
	conn = maConnect("btspp://000b0d191b6b:1");	//Holux 2
	//conn = maConnect("btspp://0080984474c8:17");	//MS-FREDRIK

	if(conn < 0) {
		printf("Error %i\n", conn);
		Freeze(0);
	}

	printf("Connecting handle %i...\n", conn);
	while(result == 0) {
		MAEvent event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED &&
				(event.key == MAK_0 || event.key == MAK_KP0)))
			{
				maExit(0);
			}
			if(event.type == EVENT_TYPE_CONN) {
				MAASSERT(event.conn.handle == conn);
				MAASSERT(event.conn.opType == CONNOP_CONNECT);
				result = event.conn.result;
			}
		}
	}
	printf("Result: %i\n", result);
	if(result > 0) {
		run(conn);
		printf("Done.\n");
	}

	Freeze(0);
	return 0;
}

void run(MAHandle conn) {
	printf("Connected. Press 0 to disconnect.\n");
	for(;;) {
		char buffer[1024];
		BOOL close = FALSE;

		int result = 0;
		maConnRead(conn, buffer, sizeof(buffer) - 1);
		while(result == 0) {
			MAEvent event;
			maWait(0);
			while(maGetEvent(&event)) {
				if(event.type == EVENT_TYPE_CLOSE) {
					maExit(0);
				} else if(event.type == EVENT_TYPE_KEY_PRESSED &&
					(event.key == MAK_0 || event.key == MAK_KP0))
				{
					printf("Closing connection...\n");
					maConnClose(conn);
					close = TRUE;
					return;
				} else if(event.type == EVENT_TYPE_CONN) {
					MAASSERT(event.conn.handle == conn);
					MAASSERT(event.conn.opType == CONNOP_READ);
					result = event.conn.result;
					MAASSERT(result != 0);
				}
			}
		}
		if(result < 0) {
			printf("Error %i\n", result);
			if(!close) {
				maConnClose(conn);
				printf("Connection closed.\n");
			}
			return;
		}
		buffer[result] = 0;
		PrintConsole(buffer);
		maWriteLog(buffer, result);
	}
}
