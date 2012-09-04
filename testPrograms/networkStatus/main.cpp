/* Copyright (C) 2011 MoSync AB

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

#define MIN(a, b) ((a) < (b) ? (a) : (b))

class MyMoblet : public Moblet {
private:
	bool mPressed;
public:
	MyMoblet() {
		printf("Network status: %i\n", maNetworkStatus());
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			maExit(0);
		if(keyCode == MAK_1)
			printf("Network status: %i\n", maNetworkStatus());
	}

	void customEvent(const MAEvent& e) {
		printf("cE %i\n", e.type);
		if(e.type == EVENT_TYPE_NETWORK) {
			printf("network: %i\n", e.state);
		}
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
