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

/**
* This example shows how to do basic Moblet based HTTP/HTTPS communication.
*
* If content-length is returned as an response header, then the first 1024
* bytes of the content will be read.
*
*/

#include <maapi.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>

using namespace MAUtil;

#define BUFSIZE 1024
#define EXAMPLE_URL "http://www.example.com/"

class MyMoblet : public Moblet, private HttpConnectionListener {
public:
	MyMoblet() : mHttp(this) {
		mRestart = false;
		showInformation();
	}

	void showInformation()
	{
		printf("Fire: HTTP\n");
		printf("0: EXIT\n");
		printf("1: IAP save\n");
		printf("2: IAP reset\n");
		printf("3: IAP shutdown\n");
		printf("4: start loop\n");
		printf("5: method standard\n");
		printf("6: method wlan\n");
		printf("7: filter not wlan\n");
		printf("8: filter wlan\n");
		printf("9: filter all\n");
	}

	void start(const char* url) {
		printf("Connect %s...\n", url);

		mHttp.close();
		int res = mHttp.create(url, HTTP_GET);
		if(res < 0) {
			printf("http.create fail %i\n", res);
		} else {
			mHttp.finish();
		}
	}

	virtual void httpFinished(HttpConnection* http, int result) {
		printf("HTTP %i\n", result);
		if(result <= 0) {
			return;
		}
		String cl;
		int res = mHttp.getResponseHeader("content-length", &cl);

		if(res > 0) {
			printf("content-length: %s bytes\n", cl.c_str());
			int len = atoi(cl.c_str());
			if(len >= BUFSIZE) {
				printf("Buffer too small, can't read everything.\n");
				len = BUFSIZE-1;
			}
			mBuffer[len] = 0;
			mHttp.read(mBuffer, len);
		}
		else
		{
			printf("No content-length!\n");
			showInformation();
		}
		if(mRestart) {
			mHttp.close();
			int res = maIapShutdown();
			printf("maIapShutdown: %i\n", res);
			start(EXAMPLE_URL);
		}
	}

	virtual void connReadFinished(Connection* conn, int result) {
		printf("connReadFinished result: %i\n\n", result);
		showInformation();
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		mRestart = false;
		if(keyCode == MAK_0)
			maExit(0);
		if(keyCode == MAK_FIRE)
			start(EXAMPLE_URL);
		if(keyCode == MAK_1) {
			int res = maIapSave();
			printf("maIapSave: %i\n", res);
		} else if(keyCode == MAK_2) {
			int res = maIapReset();
			printf("maIapReset: %i\n", res);
		} else if(keyCode == MAK_3) {
			mHttp.close();
			int res = maIapShutdown();
			printf("maIapShutdown: %i\n", res);
		} else if(keyCode == MAK_4) {
			mHttp.close();
			int res = maIapShutdown();
			printf("maIapShutdown: %i\n", res);
			start(EXAMPLE_URL);
			mRestart = true;
		} else if(keyCode == MAK_5) {
			int res = maIapSetMethod(MA_IAP_METHOD_STANDARD);
			printf("method standard: %i\n", res);
		} else if(keyCode == MAK_6) {
			int res = maIapSetMethod(MA_IAP_METHOD_WLAN);
			printf("method wlan: %i\n", res);
		} else if(keyCode == MAK_7) {
			int res = maIapSetFilter(MA_IAP_FILTER_NOT_WLAN);
			printf("filter not wlan: %i\n", res);
		} else if(keyCode == MAK_8) {
			int res = maIapSetFilter(MA_IAP_FILTER_WLAN);
			printf("filter wlan: %i\n", res);
		} else if(keyCode == MAK_9) {
			int res = maIapSetFilter(MA_IAP_FILTER_ALL);
			printf("filter all: %i\n", res);
		}
	}

private:
	char mBuffer[BUFSIZE];
	HttpConnection mHttp;
	bool mRestart;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
