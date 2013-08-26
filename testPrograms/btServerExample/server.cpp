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

static bool sBenchmarkMode = false;

class MyConnectionHandler : public ConnectionListener {
private:
	Connection* mConn;
	char mBuffer[8*1024];
	const int mId;
public:
	MyConnectionHandler(int id) : mConn(NULL), mId(id) {}

	void start(Connection* conn) {
		mConn = conn;

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
			if(sBenchmarkMode) {
				// The client takes care of timing the benchmark.
				// We, the server, need only discard the data.
				conn->recv(mBuffer, sizeof(mBuffer));
				return;
			}
			// Avoid a buffer overrun if result == sizeof(mBuffer).
			// If we've gotten more than 64 bytes,
			// it's probably useless to try to print it.
			if(result < 64) {
				// We received data. Print it.
				mBuffer[result] = 0;
				printf("%i: %s\n", mId, mBuffer);
			}
			// Continue receiving data.
			conn->recv(mBuffer, 63);
		}
	}
};

class MyMoblet : public Moblet, public ServerListener {
private:
	Server mServer;
	int mNextId;
	MyConnectionHandler* mCH;
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

		accept();
	}

	void accept() {
		// Create a handler for the new connection.
		// Give it an id, so we can tell different connections apart.
		// No need to save the pointer; the handler will delete itself
		// when the connection closes.
		mCH = new MyConnectionHandler(mNextId++);
		mServer.accept(mCH);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			close();
		//if(keyCode == MAK_5)
		{
			sBenchmarkMode = !sBenchmarkMode;
			printf("Benchmark mode: %s\n", sBenchmarkMode ? "on" : "off");
		}
	}

	//ServerListener
	void serverAcceptFailed(Server* server, int result) {
		printf("accept failed: %i\n", result);
	}
	void serverAccepted(Server*, Connection* newConnection) {
		mCH->start(newConnection);
		accept();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
