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

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		Handle store = maOpenStore("store.bin", MAS_CREATE_IF_NECESSARY);
		printf("open: %i\n", store);
		if(store < 0)
			return;
		Handle data = maCreatePlaceholder();
		printf("phc: 0x%x", data);
		maCreateData(data, BUFSIZE);
		char buf[BUFSIZE];
		maWriteData(data, buf, 0, BUFSIZE);
		int res = maWriteStore(store, data);
		printf("write: %i\n", res);
		maCloseStore(store, 0);
		printf("done.\n");
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			close();
		}
	}

	void keyReleaseEvent(int keyCode) {
		// todo: handle key releases
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
