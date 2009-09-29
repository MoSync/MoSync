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
#include <IX_CONNSERVER.H>

using namespace MAUtil;

class MyMoblet : public Moblet {
public:
	MyMoblet() {
		printf("Hello World!\n");
		ConnAddr addr;
		addr.family = CONN_FAMILY_BT;
		int res = maConnGetAddr(HANDLE_LOCAL, &addr);
		printf("maConnGetAddr res: %i\n", res);
		byte* a = addr.bt.addr.a;
		printf("%02x%02x%02x%02x%02x%02x\n", a[0], a[1], a[2], a[3], a[4], a[5]);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			close();
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
