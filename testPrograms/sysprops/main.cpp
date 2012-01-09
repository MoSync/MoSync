/* Copyright (C) 2009 Mobile Sorcery AB
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

/**
 * A Moblet is a high-level class that defines the
 * behaviour of a MoSync program.
 */
class MyMoblet : public Moblet
{
public:
	/**
	 * Initialize the application in the constructor.
	 */
	MyMoblet()
	{
		printf("Press zero or back to exit\n");

		dumpProp("mosync.imei");
		dumpProp("mosync.imsi");
		dumpProp("mosync.iso-639-1");
		dumpProp("mosync.iso-639-2");
		dumpProp("mosync.device");
		dumpProp("mosync.device.name");
		dumpProp("mosync.device.UUID");
		dumpProp("mosync.device.OS");
		dumpProp("mosync.device.OS.version");
		dumpProp("mosync.network.type");
		dumpProp("mosync.path.local");
//		dumpProp("microedition.platform");
//		dumpProp("com.sonyericsson.net.lac");
//		dumpProp("com.sonyericsson.net.cellid");
//		dumpProp("com.sonyericsson.net.mcc");
//		//dumpProp("com.sonyericsson.net.cmcc");
//		dumpProp("com.sonyericsson.net.mnc");
//		//dumpProp("com.sonyericsson.net.cmnc");
//		dumpProp("com.sonyericsson.net.status");
//		dumpProp("com.sonyericsson.net.isonhomeplmn");
//		dumpProp("com.sonyericsson.net.rat");
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
			printf("    %s\n", buf);
		}

	}

	/**
	 * Called when a key is pressed.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application.
			close();
		}
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
