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

// This program acts as a Bluetooth server.
// It sets up an RFCOMM service with a particular UUID and listens for connections.
// For each connection accepted, it prints any data received.

#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <MAUtil/Server.h>
#include <ma.h>
#include <maassert.h>
#include <conprint.h>

#include "common.h"

using namespace MAUtil;

class MyConnectionHandler : public ConnectionListener {
private:
	Connection* mConn;
	char mBuffer[64];
	const int mId;
public:
	MyConnectionHandler(Connection* conn, int id) : mConn(conn), mId(id) {
		// Print the remote address.
		MAConnAddr addr;
		int res = mConn->getAddr(&addr);
		MAASSERT(res > 0);
		byte* a = addr.bt.addr.a;
		printf("%i @ %02X%02X%02X%02X%02X%02X\n", mId,
			a[0], a[1], a[2], a[3], a[4], a[5]);

		// Start receiving data.
		mConn->setListener(this);
		mConn->recv(mBuffer, 4);
	}

	virtual ~MyConnectionHandler() {
		delete mConn;
	}

	//ConnectionListener
	void connRecvFinished(Connection* conn, int result) {
		if(result < 0) {	// error
			printf("recv %i: %i\n", mId, result);
			delete this;	// should be safe
		} else {
			// We received data. Print it.
			mBuffer[result] = 0;
			printf("%i: %s\n", mId, mBuffer);
			// Continue receiving data.
			conn->recv(mBuffer, 63);
		}
	}
};

class MyMoblet : public Moblet, public ServerListener {
private:
	Server mServer;
	int mNextId;
public:
	MyMoblet() : mServer(this), mNextId(1) {
		// Start the server.
		printf("Hello World!\n");
		int res = mServer.start("btspp://localhost:"SERVER_UUID_STRING";name=btServer");
		printf("startRes: %i\n", res);
		if(res < 0)
			return;

		// Print the server's address.
		MAConnAddr addr;
		res = mServer.getAddr(&addr);
		MAASSERT(res > 0);
		MAASSERT(addr.family == CONN_FAMILY_BT);
		byte* a = addr.bt.addr.a;
		printf("@ %i %02X%02X%02X%02X%02X%02X\n", addr.bt.port,
			a[0], a[1], a[2], a[3], a[4], a[5]);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}

	//ServerListener
	void serverAcceptFailed(Server* server, int result) {
		printf("accept failed: %i\n", result);
	}
	void serverAccepted(Server* server, Connection* newConnection) {
		// Create a handler for the new connection.
		// Give it an id, so we can tell different connections apart.
		// No need to save the pointer; the handler will delete itself
		// when the connection closes.
		new MyConnectionHandler(newConnection, mNextId++);
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
