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

#define LOGGING_ENABLED
#define CONFIG_H

#include <helpers/helpers.h>
#include <helpers/log.h>
#include <bluetooth/server.h>
#include <bluetooth/btinit.h>

#ifdef _DEBUG
#include <conio.h>
#include <stdio.h>
#define DEBUG_GETCH printf("press a key\n"); _getch()
#else	//release
#define DEBUG_GETCH
#endif

void MoSyncErrorExit(int code) {
	DEBUG_GETCH;
	exit(code);
}

int main2() {
	BtSppServer server;
	MAUUID uuid = {{ 1,2,3,4 }};
	BtSppConnection* conn;
	int port;
	MAConnAddr addr;

	Bluetooth::MABtInit();

	TEST_LEZ(port = server.open(uuid, "btCon"));
	LOG("Server opened.\n");
	TEST_LEZ(server.getAddr(addr));
	byte* a = addr.bt.addr.a;
	LOG("Connection URL: btspp://%02x%02x%02x%02x%02x%02x:%i\n",
		a[0], a[1], a[2], a[3], a[4], a[5], port);
	while(true) {
		TEST_LEZ(server.accept(conn));
		LOG("Connection accepted.\n");
		MAConnAddr remAddr;
		TEST_LEZ(conn->getAddr(remAddr));
		byte* b = remAddr.bt.addr.a;
		LOG("Remote address: %02x%02x%02x%02x%02x%02x\n", b[0], b[1], b[2], b[3], b[4], b[5]);
		while(true) {
			char buffer[1024];
			int res = conn->read(buffer, sizeof(buffer));
			if(res <= 0) {
				LOG("read error %i\n", res);
				delete conn;
				LOG("Connection deleted.\n");
				break;
			}
			LOGBIN(buffer, res);
		}
	}
}

int main() {
	int res = main2();
	DEBUG_GETCH;
	return res;
}
