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
