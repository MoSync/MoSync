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
