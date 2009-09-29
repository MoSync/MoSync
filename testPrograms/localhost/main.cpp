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
#include <MAUtil/Connection.h>
#include <conprint.h>

using namespace MAUtil;

class MyMoblet : public Moblet, ConnectionListener {
public:
	Connection mConn;
	MyMoblet() : mConn(this) {
		printf("Connecting...\n");
		int res = mConn.connect("socket://localhost:3690");
		printf("Result: %i\n", res);
	}

	void connectFinished(Connection* conn, int result) {
		printf("cF %i\n", result);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			close();
		}
	}
};

extern "C" int MAMain() {
	Moblet::run(new MyMoblet());
	return 0;
};
