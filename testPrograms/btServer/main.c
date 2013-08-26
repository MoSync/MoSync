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
#include <IX_CONNSERVER.H>
#include <conprint.h>
#include <maassert.h>

int waitConn() {
	while(TRUE) {
		maWait(0);

		EVENT event;
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CONN) {
				return event.conn.result;
			}
		}
	}
}


int MAMain() {
	Handle hServ;
	Handle hConn;
	int result;
	const char* msg = "Howdy, pardner.";
	char buffer[64];
	ConnAddr addr;
	byte* a;

	printf("Hello World!\n");

	hServ = maConnect("btspp://localhost:102030405060708090A0B0C0D0E0F010;name=btServer");
	printf("hServ: %i\n", hServ);

	result = maGetConnAddr(hServ, &addr);
	printf("gRes: %i\n", result);
	a = addr.bt.addr.a;
	printf("f %i, p %i, a %02X%02X%02X%02X%02X%02X\n", addr.family, addr.bt.port,
		a[0], a[1], a[2], a[3], a[4], a[5]);

	result = maAccept(hServ);
	printf("aRes: %i\n", result);
	hConn = waitConn();
	printf("hConn: %i\n", hConn);

	result = maGetConnAddr(hConn, &addr);
	printf("gRes: %i\n", result);
	a = addr.bt.addr.a;
	printf("f %i, p %i, a %02X%02X%02X%02X%02X%02X\n", addr.family, addr.bt.port,
		a[0], a[1], a[2], a[3], a[4], a[5]);

	maConnWrite(hConn, msg, strlen(msg));
	result = waitConn();
	printf("wRes: %i\n", result);

	while(result > 0) {
		maConnRead(hConn, buffer, sizeof(buffer) - 1);
		result = waitConn();
		printf("rRes: %i\n", result);
		if(result > 0) {
			buffer[result] = 0;
			PrintConsole(buffer);
		}
	}
	FREEZE;
}
