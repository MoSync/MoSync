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
#include <MAUtil/String.h>
#include <conprint.h>
#include <IX_CAMERA.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World\n");
		dumpAll();
	}
	
	void dumpAll() {
		dump("supports.video.capture");
		dump("video.snapshot.encodings");
		dump("camera.orientations");
		dump("camera.resolutions");
	}
	
	void dump(const char* key) {
		int len = maGetSystemProperty(key, NULL, 0);
		printf("%s: %i\n", key, len);
		if(len < 0) {
			return;
		}
		String s(len);
		maGetSystemProperty(key, s.pointer(), len);
		printf("%s\n", s.c_str());
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		printf("%i %i\n", keyCode, nativeCode);
		if(keyCode == MAK_0)
			close();
		if(keyCode == MAK_FIRE)
			dumpAll();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
