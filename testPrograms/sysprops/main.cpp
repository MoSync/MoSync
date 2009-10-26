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
#include <IX_SYSPROP.h>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World!\n");
		dumpProp("microedition.platform");
		dumpProp("com.sonyericsson.net.lac");
		dumpProp("com.sonyericsson.net.cellid");
		dumpProp("com.sonyericsson.net.mcc");
		//dumpProp("com.sonyericsson.net.cmcc");
		dumpProp("com.sonyericsson.net.mnc");
		//dumpProp("com.sonyericsson.net.cmnc");
		dumpProp("com.sonyericsson.net.status");
		dumpProp("com.sonyericsson.net.isonhomeplmn");
		dumpProp("com.sonyericsson.net.rat");
	}

	void dumpProp(const char* key) {
		char buf[256];
		printf("%s\n", key);
		int res = maGetSystemProperty(key, buf, sizeof(buf));
		if(res < 0) {
			printf("error: %i\n", res);
		} else if(res > (int)sizeof(buf)) {
			printf("too long: %i\n", res);
		} else {
			printf("%s\n", buf);
		}
		
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			close();
		}
	}
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
};
