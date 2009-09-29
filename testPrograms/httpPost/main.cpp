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
