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
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>

using namespace MAUtil;

#define BUFSIZE 1024

class MyMoblet : public Moblet, private HttpConnectionListener {
public:
	MyMoblet() : mHttp(this) {
		printf("Hello World!\n");

		int res = mHttp.create("http://www.example.com/", HTTP_GET);
		if(res < 0) {
			printf("http.create fail %i\n", res);
		} else {
			mHttp.finish();
		}
	}

	virtual void httpFinished(HttpConnection* http, int result) {
		printf("HTTP %i\n", result);
		String cl;
		int res = mHttp.getResponseHeader("content-length", &cl);
		printf("cl code %i\n", res);
		if(res > 0) {
			printf("cl strlen %i\n", cl.length());
			printf("cl: %s\n", cl.c_str());
			int len = atoi(cl.c_str());
			if(len >= BUFSIZE) {
				printf("Buffer too small, can't read.\n");
				return;
			}
			mBuffer[len] = 0;
			mHttp.read(mBuffer, len);
		}
	}
	virtual void connReadFinished(Connection* conn, int result) {
		printf("connReadFinished %i\n", result);
	}

	void keyPressEvent(int keyCode) {
		if(keyCode == MAK_0)
			maExit(0);
	}

private:
	char mBuffer[BUFSIZE];
	HttpConnection mHttp;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
