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

class MyMoblet : public Moblet, TimerListener, FocusListener {
private:
	int mStartTime;
public:
	MyMoblet() {
		mStartTime = maGetMilliSecondCount();
		printf("Hello World!\n");
		printf("Press 5 to hide for a few seconds.\n");
		addFocusListener(this);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_5) {
			int res = maSendToBackground();
			printf("stb %i @ %i ms\n", res, maGetMilliSecondCount() - mStartTime);
			addTimer(this, 5000, 1);
		}
		if(keyCode == MAK_0) {
			close();
		}
	}

	void keyReleaseEvent(int keyCode) {
		if(keyCode == MAK_5) {
			printf("r5 @ %i ms\n", maGetMilliSecondCount() - mStartTime);
		}
	}

	void runTimerEvent() {
		int res = maBringToForeground();
		printf("btf %i @ %i ms\n", res, maGetMilliSecondCount() - mStartTime);
	}

	void focusLost() {
		printf("focusLost @ %i ms\n", maGetMilliSecondCount() - mStartTime);
	}
	void focusGained() {
		printf("focusGained @ %i ms\n", maGetMilliSecondCount() - mStartTime);
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
};
