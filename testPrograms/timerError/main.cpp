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
#include <conprint.h>
using namespace MAUtil;

class MyMoblet : public Moblet, public TimerListener {
public:

	MyMoblet() {
		printf("Hello World!\n");
		addTimer(this, 20, 0);
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("Button %d pressed.  Native %d",keyCode,nativeCode);
		if (keyCode == MAK_0){
			maExit(0);
		}
	}

	void keyReleaseEvent(int keyCode, int nativeCode) {
	}

	void runTimerEvent() {
		maResetBacklight();	//does not help
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
