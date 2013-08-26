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
