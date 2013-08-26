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

//Test creating a BT connection, then opening an Internet socket. On Symbian, if things work,
//the IAP connection should be created with the latter.

#include <conprint.h>
#include <maassert.h>

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
					Freeze(0);
				}
				return event.conn.result;
			}
		}
		maWait(0);
	}
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	printf("Bluetooth...\n");
	Handle btConn = maConnect("btspp://0018c53f747e:9");
	waitConn(btConn);
	printf("Socket...\n");
	Handle sock = maConnect("socket://130.237.3.118:6666");
	waitConn(sock);

	FREEZE;
}
