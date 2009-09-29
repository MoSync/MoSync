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

#include <MAUtil/Moblet.h>
#include <conprint.h>
#include <MAUtil/BluetoothDiscovery.h>
#include <MAUtil/MAUUID.h>

using namespace MAUtil;

const char* btaddr2string(const btaddr_t& a) {
	static char buffer[16];
	sprintf(buffer, "%02x%02x%02x%02x%02x%02x", a.a[0], a.a[1], a.a[2], a.a[3], a.a[4], a.a[5]);
	return buffer;
}

//first iteration
class BTDMoblet1 : public Moblet, public BluetoothListener {
public:
	BTDMoblet1() {
		int result;

		ndev = 0;
		printf("Hello World!\n");
		setBluetoothListener(this);
		result = maBtStartDeviceDiscovery();
		printf("SDD %i\n", result);
	}

	void bluetoothEvent(int state) {
		int res;
		MABtDevice d;
		char namebuf[256];
		d.name = namebuf;
		d.nameBufSize = sizeof(namebuf);

		printf("bluetoothEvent(%i)\n", state);

		do {
			strcpy(namebuf, "Hello Debugger!");
			res = maBtGetNewDevice(&d);
			if(res) {
				printf("d%i: %i %s\n", ndev++, res, d.name);
				if(res < 0) {
					break;
				}
				printf("%s\n", btaddr2string(d.address));
			} else if(res != 0) {
				printf("res %i\n", res);
			}
		} while(res > 0);

		if(state != 0)
			removeBluetoothListener();
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	int ndev;
};


//second iteration
class BTDMoblet2 : public Moblet, public BluetoothDeviceDiscoveryListener,
	public BluetoothServiceDiscoveryListener {
public:
	BTDMoblet2() {
		int result;

		mNDev = 0;
		printf("Hello World!\n");

#if 0
		result = mBD.startDeviceDiscovery(this);
		printf("startDD %i\n", result);
#else
		btaddr_t address = { { 0x00, 0x11, 0x9f, 0xc0, 0x99, 0x12 } };	//Tony 6630
		result = mBD.startServiceDiscovery(address, L2CAP_PROTOCOL_MAUUID, this);
		printf("startSD %i\n", result);
#endif
	}

	//BluetoothServiceDiscoveryListener
	void btNewService(const BtService& serv) {
		printf("s: %i %i %s\n", serv.port, serv.uuids.size(), serv.name.c_str());
		for(int i=0; i<serv.uuids.size(); i++) {
			const int* u = serv.uuids[i].i;
			printf("%08X-%08X-%08X-%08X\n", u[0], u[1], u[2], u[3]);
		}
	}
	void btServiceDiscoveryFinished(int state) {
		printf("finished(%i)\n", state);
	}

	//BluetoothDeviceDiscoveryListener
	void btNewDevice(const BtDevice& dev) {
		printf("d%i: %i %s\n", mNDev++, dev.name.length(), dev.name.c_str());
		printf("%s\n", btaddr2string(dev.address));
	}
	void btDeviceDiscoveryFinished(int state) {
		printf("finished(%i)\n", state);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	int mNDev;
	BluetoothDiscoverer mBD;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	//Moblet::run(new BTDMoblet1());
	Moblet::run(new BTDMoblet2());
}
