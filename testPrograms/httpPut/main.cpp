/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/**
* This example shows how to do basic Moblet based HTTP/HTTPS communication.
*
* If content-length is returned as an response header, then the first 1024
* bytes of the content will be read.
*
*/

#include <ma.h>
#include <MAUtil/Moblet.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include <maassert.h>
#include <mastdlib.h>

// You must #define CONFIG_URL in config.h.
#include "config.h"

using namespace MAUtil;

// uneven size, larger than a regular TCP packet.
#define DATA_SIZE 2045

static char sData[DATA_SIZE], sBuffer[DATA_SIZE];

enum State {
	eIdle,
	ePut,
	eGet,
	eDelete,
	eHead,
} state = eIdle;

#define TEST(func) { int res = (func); if(res < 0) { printf("%s: %i\n", #func, res); return; } }

class MyMoblet : public Moblet, private HttpConnectionListener {
public:
	MyMoblet() : mHttp(this) {
		srand(42);
		start();
	}

	void initData() {
		for(int i=0; i<DATA_SIZE; i++) {
			sData[i] = (byte) rand();
		}
	}

	void start() {
		printf("HTTP_PUT\n");
		mHttp.close();
		initData();
		state = ePut;
		TEST(mHttp.create(CONFIG_URL, HTTP_PUT));
		char buf[32];
		sprintf(buf, "%i", DATA_SIZE);
		mHttp.setRequestHeader("content-length", buf);
		mHttp.write(sData, DATA_SIZE);
	}

	virtual void connWriteFinished(Connection* conn, int result) {
		printf("connWriteFinished: %i\n", result);
		if(result <= 0) {
			return;
		}
		MAASSERT(state == ePut);
		mHttp.finish();
	}

	virtual void httpFinished(HttpConnection* http, int result) {
		printf("httpFinished: %i\n", result);
		if(result <= 0) {
			return;
		}
		switch(state) {
		case ePut:
		case eDelete:
			mHttp.recv(sBuffer, DATA_SIZE-1);
			break;
		case eGet:
			mHttp.read(sBuffer, DATA_SIZE);
			break;
		default:
			BIG_PHAT_ERROR;
		}
	}

	virtual void connRecvFinished(Connection* conn, int result) {
		printf("connRecvFinished: %i\n", result);
		if(result <= 0 && result != CONNERR_CLOSED) {
			return;
		}
		if(result > 0) {
			MAASSERT(result < DATA_SIZE);
			//sBuffer[result] = 0;
			//printf("%s\n", sBuffer);
			maWriteLog(sBuffer, result);
		}
		switch(state) {
		case ePut:
			mHttp.close();
			state = eGet;
			printf("HTTP_GET\n");
			TEST(mHttp.create(CONFIG_URL, HTTP_GET));
			mHttp.finish();
			break;
		case eDelete:
			printf("Done.\n");
			break;
		default:
			BIG_PHAT_ERROR;
		}
	}

	virtual void connReadFinished(Connection* conn, int result) {
		printf("connReadFinished: %i\n", result);
		if(result <= 0) {
			return;
		}
		MAASSERT(state == eGet);
		printf("memcmp: %i\n", memcmp(sData, sBuffer, DATA_SIZE));

#if 1	// Disable this code to enable examination of the target file.
		mHttp.close();
		printf("HTTP_DELETE\n");
		state = eDelete;
		TEST(mHttp.create(CONFIG_URL, HTTP_DELETE));
		mHttp.finish();
#endif
	}

	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0 || keyCode == MAK_SOFTRIGHT || keyCode == MAK_BACK)
			maExit(0);
		if(keyCode == MAK_FIRE)
			start();
	}

	void pointerPressEvent(MAPoint2d p) {
		start();
	}

private:
	HttpConnection mHttp;
};

extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
