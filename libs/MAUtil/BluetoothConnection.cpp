/* Copyright (C) 2010 MoSync AB

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

#include "BluetoothConnection.h"
#include <maassert.h>
#include <mavsprintf.h>

using namespace MAUtil;

// mPort: <0 when closed, ==0 when discovering, >0 when connect-ing/-ed.

BluetoothConnection::BluetoothConnection(ConnectionListener* listener)
: Connection(listener), mPort(-1) {}

BluetoothConnection::~BluetoothConnection() {
	this->close();
}

int BluetoothConnection::connect(const MABtAddr& address, const MAUUID& uuid) {
	ASSERT_MSG(mPort < 0, "Connection already active");
	mAddr = address;
	mPort = 0;
	int res = mBD.startServiceDiscovery(address, uuid, this);
	if(res < 0) {
		// No service discovery. Either we're on Android,
		// or this device doesn't have bluetooth at all.
		// Attempt to use the Android maConnect() extension.
		char buf[64];
		char* ptr = buf;
		const byte* a = address.a;
		ptr += sprintf(ptr, "btspp://%02x%02x%02x%02x%02x%02x:",
			a[0], a[1], a[2], a[3], a[4], a[5]);
		for(int i=0; i<4; i++) {
			ptr += sprintf(ptr, "%08x", uuid.i[i]);
		}
		return Connection::connect(buf);
	}
	return res;
}

void BluetoothConnection::close() {
	mPort = -1;
	mBD.cancel();
	Connection::close();
}

void BluetoothConnection::btNewService(const BtService& serv) {
	if(mPort > 0)
		return;
	MAASSERT(serv.port > 0);
	mPort = serv.port;

	char buf[64];
	const byte* a = mAddr.a;
	sprintf(buf, "btspp://%02x%02x%02x%02x%02x%02x:%i",
		a[0], a[1], a[2], a[3], a[4], a[5], serv.port);
	int res = Connection::connect(buf);
	if(res < 0)
		mListener->connectFinished(this, res);
}

void BluetoothConnection::btServiceDiscoveryFinished(int state) {
	if(mPort <= 0) {
		if(state >= 0)
			state = CONNERR_NOTFOUND;
		mListener->connectFinished(this, state);
	}
}
