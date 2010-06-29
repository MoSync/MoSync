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

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		char buf[64] = "";
		int res;
		printf("Hello World!\n");
		res = maGetSystemProperty("mosync.imei", buf, sizeof(buf));
		printf("IMEI:%i: %s\n", res, buf);
		buf[0] = 0;
		res = maGetSystemProperty("mosync.imsi", buf, sizeof(buf));
		printf("IMSI:%i: %s\n", res, buf);

#if 0
		if(res > 0) {
			// print byte values
			char buf2[128];
			char* ptr = buf2;
			for(int i=0; i<res; i++) {
				ptr += sprintf(ptr, "%i,", buf[i]);
			}
			printf("%s\n", buf2);

			//print individual chars
			ptr = buf2;
			for(int i=0; i<res; i++) {
				ptr += sprintf(ptr, "%c", buf[i]);
			}
			printf("%s\n", buf2);
		}
#endif	//0
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
