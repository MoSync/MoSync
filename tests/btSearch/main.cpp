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

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

#define CONFIG_H
#include <helpers/helpers.h>
using namespace MoSyncError;

#include <bluetooth/discovery.h>

using namespace Bluetooth;
#include <MAUtil/mauuid.h>

static int str2ba(const char* str, btaddr_t* bda);

#ifdef _DEBUG
#define DEBUG_GETCH printf("press a key\n"); _getch()
#else	//release
#define DEBUG_GETCH
#endif

int dummy = __COUNTER__;

void myCallback();

Handle gSearchHandle;
bool gSearching;

const char* FileNameFromPath(const char* path) {
	const char* ptr = strrchr(path, '\\');
	return ptr ? (ptr + 1) : path;
}

int main(int argc, char** argv) {
	if(argc != 2) {
		const char* exename = FileNameFromPath(argv[0]);
		printf(
			"argc: %i\n"
			"\n"
			"Usage: %s <address>\n"
			"\n"
			"Performs an RFCOMM service search on a Bluetooth device.\n"
			"\n"
			"Example: %s 010203040506\n",
			argc, exename, exename);
		DEBUG_GETCH;
		return __COUNTER__;
	}

	btaddr_t address;
	TEST_NZ(str2ba(argv[1], &address));

	MABtInit();

	//MAUUID uuid = { 0x10203040, 0x50607080, 0x90A0B0C0, 0xD0E0F010 };

	maBtStartServiceDiscovery(&address, &L2CAP_PROTOCOL_MAUUID, myCallback);
	gSearching = true;
	while(gSearching) {
		printf("still searching. press a key.\n");
		_getch();
	}
	int nServ = maBtDiscoveryState() > 0 ? (maBtDiscoveryState() - 1) : maBtDiscoveryState();
	printf("%i services found.\n", nServ);
	DEBUG_GETCH;
	return 0;
}

void myCallback() {
	printf("myCallback %i\n", maBtDiscoveryState());
	gSearching = maBtDiscoveryState() == 0;
	MABtServiceSize ss;
	MABtService s;
	int gnssRes = maBtGetNextServiceSize(&ss);
	if(gnssRes == 0)
		return;
	DEBUG_ASRTINT(gnssRes, 1);
	s.name = new char[ss.nameBufSize];
	s.nameBufSize = ss.nameBufSize;
	s.uuids = new MAUUID[ss.nUuids];
	DEBUG_ASRTINT(maBtGetNewService(&s), 1);
	printf("name: \"%s\"\n", s.name);
	printf("port: %i\n", s.port);
	printf("%i UUIDs:\n", ss.nUuids);
	for(int i=0; i<ss.nUuids; i++) {
		printf("%08X-%08X-%08X-%08X\n",
			s.uuids[i].i[0], s.uuids[i].i[1], s.uuids[i].i[2], s.uuids[i].i[3]);
	}
	printf("Service done.\n");
}

int str2ba(const char* str, btaddr_t *bta) {
	int ai[6];	//address ints
	if(6 != sscanf_s(str, "%2x%2x%2x%2x%2x%2x", &ai[0], &ai[1], &ai[2], &ai[3], &ai[4], &ai[5])) {
		return CONNERR_URL;
	}
	for(int i=0; i<BTADDR_LEN; i++) {
		bta->a[i] = (byte)ai[i];
	}
	return 0;
}

void MoSyncErrorExit(int code) {
	DEBUG_GETCH;
	exit(code);
}
