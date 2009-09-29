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

using namespace MAUtil;

#define BUFSIZE 1024
#define MIN(a, b) ((a) < (b) ? (a) : (b))

//first iteration
class ConnMoblet : public Moblet, public ConnectionListener {
public:
	ConnMoblet() {
		printf("Hello World!\n");

		Handle conn = maConnect("socket://130.237.3.118:6666");
		setConnectionListener(conn, this);
	}

	void connEvent(const CONN_EVENT_DATA& data) {
		printf("cE %i %i %i\n", data.handle, data.opType, data.result);
		if(data.opType == CONNOP_CONNECT) {
			if(data.result > 0) {
				maConnRead(data.handle, buffer, BUFSIZE);
			}
		} else if(data.opType == CONNOP_READ) {
			if(data.result > 0) {
				buffer[MIN(80, data.result)] = 0;	//hack
				printf("%s", buffer);
				maConnRead(data.handle, buffer, BUFSIZE);
			}
		} else {
			printf("Unhandled!\n");
		}
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	char buffer[BUFSIZE];
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new ConnMoblet());
}
