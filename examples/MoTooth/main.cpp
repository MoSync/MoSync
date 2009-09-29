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

/** \file main.cpp
* MoTooth is a simple Bluetooth exploration program.
* It can search for devices and RFCOMM services,
* store them in a database,
* connect to a service and display its output.

* On Windows, due to a bug in the Windows Bluetooth stack,
* newly discovered devices may appear without names.
* Re-scanning should make the devices' names appear properly.
* A workaround is planned.
*/

#include "MoTooth.h"
#include <MAUtil/mauuid.h>

Vector<DEVICE> gDevices;
int gnServices;

//clears all events, responds to EVENT_CLOSE
void myClearEvents() {
	MAEvent event;
	while(maGetEvent(&event)) {
		if(event.type == EVENT_TYPE_CLOSE)
			maExit(0);
	}
}

int myGetProcessedEvent(int type, MAEvent* event) {
	int result = maGetEvent(event);
	if(result == 0)
		return 0;
	if(event->type == type)
		return 1;
	if(event->type == EVENT_TYPE_CLOSE ||
		(event->type == EVENT_TYPE_KEY_PRESSED && event->key == MAK_0))
	{
		maExit(0);
	}
	return 0;
}

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	//check store, print number of devices and services stored there.
	printfln("reading store...");
	gDevices.resize(3);	//this avoids a hella-weird crash later, in malloc().
	readStore();
	printfln("done");
	for(;;) {
		menu();
	}
}


void writeStore() {
	//open store
	MAHandle store = maOpenStore(STORENAME, MAS_CREATE_IF_NECESSARY);
	if(store <= 0) {
		printf("writeStore error %i\n", store);
		return;
	}
	writeDatabase();
	maWriteStore(store, RES_STORE);
	maCloseStore(store, false);
}

void readStore() {
	//open store
	gDevices.resize(3);	//does not avoid The Crash.
	MAHandle store = maOpenStore(STORENAME, 0);
	if(store <= 0)
		return;
	maReadStore(store, RES_STORE);
	maCloseStore(store, false);

	//read into database
	if(!readDatabase()) {
		gDevices.clear();
		gnServices = 0;
		printfln("Store corrupted.\n(5)ignore (0)delete\n");
		myClearEvents();
		for(;;) {
			maWait(0);
			MAEvent event;
			MAASSERT(maGetEvent(&event));
			if(event.type == EVENT_TYPE_CLOSE)
				maExit(0);
			if(event.type == EVENT_TYPE_KEY_PRESSED) switch(event.key) {
				case MAK_0:
					deleteStore();
				case MAK_5:
					return;
			}	//switch if
		}	//for
	}	//if
}

void deleteStore() {
	//open store
	MAHandle store = maOpenStore(STORENAME, 0);
	if(store <= 0)
		return;
	maCloseStore(store, true);
}

void menu() {
	//ask user to select one (if any), scan for (more) devices or
	//clear the store (if not already empty).
	printfln("%i known services\n", gnServices);
	if(gnServices) {
		printfln("(F)select (5)scan (0)exit\n");
	} else {
		printf("(5)scan (0)exit\n");
	}
	myClearEvents();

	for(;;) {
		MAEvent event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE)
				maExit(0);
			if(event.type == EVENT_TYPE_KEY_PRESSED) {
				if(event.key == MAK_5) {
					scan();
					return;
				}
				if(event.key == MAK_FIRE && gnServices != 0) {
					run();
					return;
				}
				if(event.key == MAK_0)
					maExit(0);
			}
		}
	}
}

/*int select() {
	//user must press a number of number keys to match the ten-logarithm of the number of available services
	//alt: avsluta med fyrkant :)
	//alt: choose 1-9, press 0 for more choices
	int max = MIN(gnServices, 9);
	int selected;
	printf("Press 1-%i\n", max);
	for(;;) {
		int event;
		while(event = maGetEvent()) {
			switch(event) {
			case EVENT_CLOSE:
				maExit(0);
				break;

			//case MAK_1:
				//selected = 1;
				//break;
#define SELECT(num) case MAK_##num: return num;
#define NUMBERS(m) m(1) m(2) m(3) m(4) m(5) m(6) m(7) m(8) m(9)
			NUMBERS(SELECT);
			}
		}
	}
}*/

void run() {
	//select
	int deviceIndex, serviceIndex;
	if(!doSelect(deviceIndex, serviceIndex))
		return;

	//connect
	MAHandle conn;
	int result = 0;
	char url[32];
	const DEVICE& d(gDevices[deviceIndex]);
	const SERVICE& s(d.services[serviceIndex]);

	sprintf(url, "btspp://%s:%i", btaddr2string(d.address), s.port);
	conn = maConnect(url);

	//on failure, print error message
	if(conn < 0) {
		printf("Error %i\n", conn);
		return;
	}

	printf("Connecting...\n");
	while(result == 0) {
		MAEvent event;
		maWait(0);
		while(maGetEvent(&event)) {
			if(event.type == EVENT_TYPE_CLOSE ||
				(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
			{
				maExit(0);
			} else if(event.type == EVENT_TYPE_CONN) {
				MAASSERT(event.conn.handle == conn);
				MAASSERT(event.conn.opType == CONNOP_CONNECT);
				result = event.conn.result;
				MAASSERT(result != 0);
			}
		}
	}

	//on failure, print error message
	if(result < 0) {
		printf("Error %i\n", result);
		maConnClose(conn);
		return;
	}

	//on success, start dumping data flow to console
	printf("Connected. Press 0 to disconnect.\n");
	for(;;) {
		char buffer[1024];
		bool close = false;

		result = 0;
		maConnRead(conn, buffer, sizeof(buffer) - 1);
		while(result == 0) {
			MAEvent event;
			maWait(0);
			while(maGetEvent(&event)) {
				if(event.type == EVENT_TYPE_CLOSE) {
					maExit(0);
				} else if(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0) {
					printf("Closing connection...\n");
					maConnClose(conn);
					close = true;
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

#define SCAN_DEVICES 1

void scan() {
	int nserv=0;
	int state;

#if SCAN_DEVICES
	int ndev=0;
	gDevices.clear();
	gnServices = 0;

	printfln("DevDisc...");
#endif
	int startTime = maGetMilliSecondCount();
#if SCAN_DEVICES
	maBtStartDeviceDiscovery(1);
	//printfln("Started");

	state = 0;
	do {
		MABtDevice d;
		char namebuf[256];
		int res;
		MAEvent event;

		maWait(0);
		while(myGetProcessedEvent(EVENT_TYPE_BT, &event)) {
			state = event.state;
		}

		d.name = namebuf;
		d.nameBufSize = sizeof(namebuf);

		do {
			strcpy(namebuf, "Hello Debugger!");
			res = maBtGetNewDevice(&d);
			if(res) {
				if(res < 0) {
					printfln("Error %i\n", res);
					break;
				}
				printfln("d%i: %i %s", ndev++, res, d.name);
				printfln("%s", btaddr2string(d.address));
				addDevice(d);
			}
		} while(res > 0);
	} while(state == 0);

	printfln("Done %i, %i ms", state, maGetMilliSecondCount() - startTime);
#endif	//SCAN_DEVICES
	printfln("Scanning %i devices...\n", gDevices.size());

	for(int i=0; i<gDevices.size(); i++) {
		const DEVICE& d = gDevices[i];
		printfln("ServDisc %s", btaddr2string(d.address));
		int servStartTime = maGetMilliSecondCount();
		maBtStartServiceDiscovery(&d.address, &RFCOMM_PROTOCOL_MAUUID);
		//printfln("Started");

		state = 0;
		do {
			MABtService s;
			char namebuf[256];
			static const int MAX_UUIDS = 32;
			MAUUID uuidBuf[MAX_UUIDS];
			int res;
			MAEvent event;

			s.name = namebuf;
			s.nameBufSize = sizeof(namebuf);
			s.uuids = uuidBuf;

			maWait(0);
			while(myGetProcessedEvent(EVENT_TYPE_BT, &event)) {
				state = event.state;
			}
			do {
				strcpy(namebuf, "Hello Sebugger!");
				MABtServiceSize ss;
				res = maBtGetNextServiceSize(&ss);
				if(res) {
					MAASSERT(ss.nUuids <= MAX_UUIDS);
					res = maBtGetNewService(&s);
					if(res) {
						if(res < 0) {
							printfln("Error %i\n", res);
							break;
						}
						printfln("s%i: %i %i %s", nserv++, res, s.port, s.name);
						for(int j=0; j<ss.nUuids; j++) {
							int* u = s.uuids[j].i;
							printfln("%08X-%08X-%08X-%08X", u[0], u[1], u[2], u[3]);
						}
						addService(i, s);
					}
				}
			} while(res > 0);
		} while(state == 0);
		printfln("Done %i, %i ms", state, maGetMilliSecondCount() - servStartTime);
	}
	printfln("Done, total %i ms", maGetMilliSecondCount() - startTime);
	writeStore();
}

const char* btaddr2string(const MABtAddr& a) {
	static char buffer[16];
	sprintf(buffer, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
	return buffer;
}
