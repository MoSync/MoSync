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
#include <IX_NATIVE_INPUT.h>

using namespace MAUtil;

#define MIN(a, b) ((a) < (b) ? (a) : (b))

class NKMoblet : public Moblet {
public:
	NKMoblet() {
		printf("Hello World!\n");
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
		printf("k: %i\n", keyCode);
	}

	void customEvent(const MAEvent& event) {
		if(event.type == EVENT_TYPE_NATIVE_KEY_PRESSED) {
			printf("nk: %i\n", event.key);
		}
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new NKMoblet());
	return 0;
}
