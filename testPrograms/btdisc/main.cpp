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
#include <mautil/mauuid.h>
#include <maassert.h>
#include <conprint.h>
#define printfln printf

void scan();

#define SCAN_DEVICES

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;
	scan();
	Freeze(0);
	return 0;
}

const char* btaddr2string(const btaddr_t& a) {
	static char buffer[16];
	sprintf(buffer, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
	return buffer;
}

void scan() {
	int nserv=0;
	EVENT event;
#ifdef SCAN_DEVICES
scan_devices:
	int ndev=0;
	MABtDevice d;
	char namebuf[256];
	int res;

	printfln("DevDisc...");
	int startTime = maGetMilliSecondCount();
	maBtStartDeviceDiscovery(1);
	//printfln("Started");

	while(true) {
		d.name = namebuf;
		d.nameBufSize = sizeof(namebuf);

		do {
			strcpy(namebuf, "Hello Debugger!");
			res = maBtGetNewDevice(&d);
			if(res) {
				printfln("d%i: %i %s", ndev++, res, d.name);
				if(res < 0) {
					break;
				}
				printfln("%s", btaddr2string(d.address));
			} else {
				printfln("res %i\n", res);
			}
		} while(res > 0);

		maWait(0);
		maGetEvent(&event);	//this oughta work, cause maWait(0) guarantees that there's at least one event.
		if(event.type == EVENT_TYPE_BT && event.state != 0)
			break;
		if(event.type == EVENT_TYPE_CLOSE ||
			(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
		{
			maExit(0);
		}
	}
	printfln("Done %i, %i ms", event.state, maGetMilliSecondCount() - startTime);

	printfln("Press 1 or 3");
	while(true) {
		maWait(0);
		maGetEvent(&event);
		if(event.type == EVENT_TYPE_CLOSE ||
			(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
		{
			maExit(0);
		}
		if(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_1) {
			goto scan_devices;
		}
		if(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_3) {
			break;
		}
	}


	const btaddr_t& address = d.address;
#else
	//const btaddr_t address = { { 0x00, 0x0f, 0xde, 0xa1, 0x4b, 0x70 } };	//K700i Jacob
	//const btaddr_t address = { { 0x00, 0x0b, 0x0d, 0x14, 0x99, 0x0f } };	//Holux
	//const btaddr_t address = { { 0x00, 0x11, 0x9f, 0xcb, 0x74, 0x10 } };	//6630 v1.0
	//const btaddr_t address = { { 0x00, 0x18, 0xc5, 0x3f, 0x74, 0x7e } };	//N73
	//const btaddr_t address = { { 0x00, 0x80, 0x98, 0x44, 0x74, 0xc8 } };	//MS-FREDRIK
	const btaddr_t address = { { 0x00, 0x11, 0x67, 0x9c, 0xd9, 0x3c } };	//MS-FREDRIK, new dongle
#endif	//SCAN_DEVICES

	printfln("ServDisc %s", btaddr2string(address));
	int servStartTime = maGetMilliSecondCount();
	maBtStartServiceDiscovery(&address, &L2CAP_PROTOCOL_MAUUID);

	while(true) {
		MABtService s;
		char sNameBuf[256];
		static const int MAX_UUIDS = 32;
		MAUUID uuidBuf[MAX_UUIDS];
		int res;
		s.name = sNameBuf;
		s.nameBufSize = sizeof(sNameBuf);
		s.uuids = uuidBuf;

		//printfln("state: %i\n", maBtDiscoveryState2());
		do {
			strcpy(sNameBuf, "Hello Sebugger!");
			MABtServiceSize ss;
			res = maBtGetNextServiceSize(&ss);
			if(res) {
				MAASSERT(ss.nUuids <= MAX_UUIDS);
				if(ss.nameBufSize < 0) {
					printfln("%i\n", ss.nameBufSize);
				}
				res = maBtGetNewService(&s);
				if(res) {
					if(ss.nameBufSize < 0) {
						printfln("s%i: %i %i No name", nserv++, res, s.port);
					} else {
						printfln("s%i: %i %i %i \"%s\"", nserv++, res, s.port, ss.nameBufSize, s.name);
					}
					/*for(int j=0; j<ss.nUuids; j++) {
						int* u = s.uuids[j].i;
						printfln("%08X-%08X-%08X-%08X", u[0], u[1], u[2], u[3]);
					}*/
					if(res < 0) {
						//printfln("res1 %i\n", res);
						break;
					}
				} else {
					//printfln("res2 %i\n", res);
				}
			} else {
				//printfln("res3 %i\n", res);
			}
		} while(res > 0);

		maWait(0);
		maGetEvent(&event);
		if(event.type == EVENT_TYPE_BT) {
			printfln("event %i", event.state);
			if(event.state != 0)
				break;
		}
		if(event.type == EVENT_TYPE_CLOSE ||
			(event.type == EVENT_TYPE_KEY_PRESSED && event.key == MAK_0))
		{
			maExit(0);
		}
	}
	printfln("Done %i, %i ms", event.state, maGetMilliSecondCount() - servStartTime);
#ifdef SCAN_DEVICES
	printfln("Done, total %i ms", maGetMilliSecondCount() - startTime);
#endif	//SCAN_DEVICES
}
