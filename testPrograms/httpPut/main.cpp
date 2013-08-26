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
