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
