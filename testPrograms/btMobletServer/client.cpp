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

//This program will find and connect to a bluetooth service on a hardcoded
//address. It will then send character input to that service.

#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <MAUtil/BluetoothDiscovery.h>
#include <ma.h>
#include <maassert.h>
#include <conprint.h>
//#include <MAUtil/mauuid.h>

#include "common.h"

using namespace MAUtil;

//static const MABtAddr sAddress = { { 0x00,0x11,0x67,0x9c,0xd9,0x3c } }; //MS-FREDRIK, sweex dongle
//static const MABtAddr sAddress = { { 0x00,0x25,0xE7,0x13,0x12,0xB5 } }; //W995
static const MABtAddr sAddress = { { 0x00,0x19,0x0e,0x04,0x17,0x04 } }; //MS-FREDRIK, Belkin dongle
static const MAUUID sUuid = SERVER_MAUUID_DECL;

class MyMoblet : public Moblet, BluetoothServiceDiscoveryListener, ConnectionListener {
private:
	Connection mConn;
	BluetoothDiscoverer mDisc;
	char buffer[1024];
	int pos;
	bool writing, online;
public:
	MyMoblet() : mConn(this), pos(0), online(false) {
		printf("Hello World!\n");
#if 1
		int res = mDisc.startServiceDiscovery(sAddress, sUuid, this);
		printf("startDiscRes: %i\n", res);
		if(res < 0)
			return;
#else
		char buf[64];
		const byte* a = sAddress.a;
		sprintf(buf, "btspp://%02x%02x%02x%02x%02x%02x:%i",
			a[0], a[1], a[2], a[3], a[4], a[5], 2);
		int res = mConn.connect(buf);
		printf("connect res %i\n", res);
#endif
	}

	void keyPressEvent(int keyCode) {
		if(keyCode >= MAK_0 && keyCode <= MAK_9 && online) {
			buffer[pos++] = '0' + (keyCode - MAK_0);
			if(!writing) {
				mConn.write(buffer, pos);
				writing = true;
				pos = 0;
			}
		}
	}

	//BluetoothServiceDiscoveryListener
	void btNewService(const BtService& serv) {
		printf("found service, port %i\n", serv.port);
		printf("name: %s\n", serv.name.c_str());
		for(int j=0; j<serv.uuids.size(); j++) {
			const int* u = serv.uuids[j].i;
			printf("%08X-%08X-%08X-%08X\n", u[0], u[1], u[2], u[3]);
		}
		const byte* a = sAddress.a;
		char buf[64];
		sprintf(buf, "btspp://%02x%02x%02x%02x%02x%02x:%i",
			a[0], a[1], a[2], a[3], a[4], a[5], serv.port);
		int res = mConn.connect(buf);
		printf("connect res %i\n", res);
	}
	void btServiceDiscoveryFinished(int state) {
		printf("endDisc: %i\n", state);
	}

	void connectFinished(Connection* conn, int result) {
		printf("cF %i\n", result);
		online = result > 0;
	}
	void connWriteFinished(Connection* conn, int result) {
		printf("cWF %i\n", result);
		online = result > 0;
		writing = false;
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
