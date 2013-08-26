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
#include <maxtoa.h>
#include <maassert.h>

//ConnTest
#define CT(a) { int ta = (a); if(ta <= 0) { BIG_PHAT_ERROR_VAL(ta); } }

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
				/*if(event.conn.result <= 0) {
					Freeze(0);
				}*/
				return event.conn.result;
			}
		}
		maWait(0);
	}
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	static const char data[] = "userid=joe&password=guessme";
	int size = sizeof(data) - 1;
	char buffer[64];

	printf("HTTP POST test\n");
	Handle http = maHttpCreate("http://msdev.mine.nu:8080/testing/posttest.php", HTTP_POST);
	CT(http);

	maHttpSetRequestHeader(http, "X-MoSync-test", "terue");

	_itoa(size, buffer, 10);
	maHttpSetRequestHeader(http, "Content-Length", buffer);

	maHttpSetRequestHeader(http, "Content-Type", "application/x-www-form-urlencoded");

	printf("write\n");
	maConnWrite(http, data, size);
	if(waitConn(http) < 0)
		Freeze(0);

	printf("finish\n");
	maHttpFinish(http);
	if(waitConn(http) < 0)
		Freeze(0);

	int res = maHttpGetResponseHeader(http, "Content-Length", buffer, sizeof(buffer));
	if(res <= 0 || res >= (int)sizeof(buffer)) {
		printf("CLerr %i\n", res);
		Freeze(0);
	}
	printf("Content-Length: %s\n", buffer);

	res = 0;
	while(true) {
		maConnRead(http, buffer, sizeof(buffer)-1);
		size = waitConn(http);
		if(size < 0)
			break;
		res += size;
		buffer[size] = 0;
		printf(buffer);
	}
	printf("Bytes read: %i\n", res);
	Freeze(0);
	return 0;
}
