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
