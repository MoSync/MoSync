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

#include <maapi.h>
#include <IX_STREAMING.H>
#include <conprint.h>
#include <MAUtil/Moblet.h>

using namespace MAUtil;

class StreaMoblet : public Moblet, public ConnListener {
	Handle mHttp;
public:
	StreaMoblet() {
#if 0
		printf("starting...\n");
		int result = maSoundPlayConn(0, "audio/mpeg");
		printf("spc: %i\n", result);
	}
#else
		printf("connecting...\n");
		mHttp = maConnect("http://localhost:5002/machinae_supremacy-sidology_2-trinity.mp3");
		setConnListener(mHttp, this);
	}

	void connEvent(const CONN_EVENT_DATA& data) {
		int result = data.result;
		printf("finish %i\n", result);
		if(result >= 200 && result <= 299) {
			char ctBuf[256];
			result = maHttpGetResponseHeader(mHttp, "content-type", ctBuf, 256);
			printf("hrh: %i\n", result);
			if(result > 0) {
				printf("ct: %s\n", ctBuf);
				result = maSoundPlayConn(mHttp, ctBuf);
				printf("spc: %i\n", result);
			}
		}
	}
#endif
	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0) {
			maExit(0);
		}
	}
};

void MAMain() {
	InitConsole();
	Moblet::run(new StreaMoblet);
}
