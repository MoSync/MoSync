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

// This program will find and connect to a Bluetooth service
// with a specific UUID on a hardcoded address.
// It will then send character input to that service.

#include <MAUtil/Moblet.h>
#include <MAUtil/BluetoothConnection.h>
#include <ma.h>
#include <maassert.h>
#include <conprint.h>

#include "common.h"

using namespace MAUtil;

#if 1
#error This program requires a bluetooth address to a local, active server.
#error Below is an example address. Change it, then change the above line to "#if 0".
#error Alternatively, you can use another example program: MoTooth.
#endif

static const MABtAddr sAddress = { { 0x00,0x18,0xC5,0x3F,0x74,0x7E } };

static const MAUUID sUuid = SERVER_MAUUID_DECL;

static bool sBenchmarkMode = false;

class MyMoblet : public Moblet, ConnectionListener, TimerListener {
private:
	BluetoothConnection mConn;
	char buffer1[512], buffer2[512];
	char* currentBuffer;
	int pos;
	bool writing, online;
	int mBenchWriteCount, mBenchTime, mBenchLastWriteCount;
public:
	MyMoblet() : mConn(this), pos(0), writing(false), online(false) {
		printf("Hello World!\n");
#if 1
		// Connect to the service, via UUID.
		int res = mConn.connect(sAddress, sUuid);
#else
		// Connect directly to a known port.
		char buf[64];
		const byte* a = sAddress.a;
		sprintf(buf, "btspp://%02x%02x%02x%02x%02x%02x:%i",
			a[0], a[1], a[2], a[3], a[4], a[5], 2);
		int res = mConn.Connection::connect(buf);
#endif
		printf("connect res %i\n", res);
		currentBuffer = buffer1;
	}

	void benchWrite() {
		// this is technically a buffer read overrun,
		// but since we don't care what we send, it doesn't matter.
		mConn.write(buffer1, 8*1024);
	}

	void runTimerEvent() {
		// another second has passed since the start of the benchmark.
		mBenchTime++;
		// let's print the total average speed.
		printf("%i kBps (current: %i)\n", (mBenchWriteCount * 8) / mBenchTime, (mBenchWriteCount - mBenchLastWriteCount) * 8);
		mBenchLastWriteCount = mBenchWriteCount;
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("kPE %i\n", keyCode);
		if(keyCode == MAK_5 || keyCode == MAK_MENU) {
			sBenchmarkMode = !sBenchmarkMode;
			printf("Benchmark mode: %s\n", sBenchmarkMode ? "on" : "off");
			if(sBenchmarkMode) {
				if(!writing) {
					benchWrite();
					writing = true;
				}
				mBenchWriteCount = 0;
				mBenchTime = 0;
				mBenchLastWriteCount = 0;
				addTimer(this, 1000, 0);
				return;
			} else {
				removeTimer(this);
			}
		}
		// If it's a printable key and we're connected...
		if(keyCode >= MAK_SPACE && keyCode <= MAK_Z && online) {
			// Store the digit.
			currentBuffer[pos++] = keyCode;
			// If we're not still writing...
			if(!writing) {
				// Send data to server.
				printf("write %i\n", pos);
				mConn.write(currentBuffer, pos);
				writing = true;
				pos = 0;
				// Swap buffers, to avoid overwriting data that's still being sent.
				currentBuffer = (currentBuffer == buffer1) ? buffer2 : buffer1;
			}
		}
	}

	void connectFinished(Connection* conn, int result) {
		printf("cF %i\n", result);
		online = result > 0;
	}
	void connWriteFinished(Connection* conn, int result) {
		online = result > 0;
		if(sBenchmarkMode) {
			mBenchWriteCount++;
			benchWrite();
			return;
		}
		printf("cWF %i\n", result);
		writing = false;
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
