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
		puts(buffer);
		maWriteLog(buffer, result);
	}
}
