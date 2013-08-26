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

#include <MAUtil/Moblet.h>
#include <MAUtil/BluetoothConnection.h>
#include <MAUtil/MAUUID.h>
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet, ConnectionListener {
	BluetoothConnection mBC;
	char buf[1024];
public:
	MyMoblet() : mBC(this) {
		printf("Hello World!\n");
		//MABtAddr addr = {{ 0x00, 0x0b, 0x0d, 0x19, 0x1b, 0x6b }};	//Holux
		MABtAddr addr = {{ 0x00, 0x18, 0xc5, 0x3f, 0x74, 0x7e }};	//N73
		int res = mBC.connect(addr, RFCOMM_PROTOCOL_MAUUID);//SerialPort_Service_MAUUID);
		printf("connect: %i\n", res);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			close();
	}

	void connectFinished(Connection* conn, int result) {
		printf("connF: %i\n", result);
		if(result < 0)
			return;
		mBC.recv(buf, sizeof(buf)-1);
	}
	void connRecvFinished(Connection* conn, int result) {
		printf("recv: %i\n", result);
		if(result < 0)
			return;
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
