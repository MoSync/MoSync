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
#define EXAMPLE_URL "http://www.mosync.com/"
#define SECURE_URL "https://encrypted.google.com/"

static bool sRepeat = false;
static const char* sUrl;
static int sCount;

/**
* Moblet class.
* HttpConnectionListener inherites the http listener functions.
*/
class MyMoblet : public Moblet, private HttpConnectionListener {
public:

	/**
	* Constructor
	* mHttp(this) activates our http listeners.
	*/
	MyMoblet() : mHttp(this) {
		showInformation();
	}
	
	/**
	* Shows basic information on how to use the program
	*/
	void showInformation()
	{
		printf("HTTP connection -\n");
		printf("   Press fire key or\n");
		printf("   touch the screen\n");
		printf("HTTPS connection -\n");
		printf("   Press 1 key or\n");
		printf("   soft left key\n");
		printf("Toggle REPEAT -\n");
		printf("   Press 5 key\n");
		printf("To EXIT -\n");
		printf("   Press 0 key or\n");
		printf("   soft right key\n");
	}

	/**
	* Initiates and establishes a new connection with the given url.
	*/
	void start(const char* url) {
		sUrl = url;
		if(sRepeat) {
			sCount++;
			printf("%i\n", sCount);
		}
		printf("Connecting to,\n%s...\n", url);

		mHttp.close();
		int res = mHttp.create(url, HTTP_GET);
		if(res < 0) {
			printf("http.create fail %i\n", res);
		} else {
			mHttp.finish();
		}
	}

	/**
	* Listener that is fired when a connection has been established.
	* The function tries to read the content-length from the
	* resoonse headers. If successfull, the first 1024 bytes
	* will be read.
	*/
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
			printf("No content-length\n");
			mHttp.recv(mBuffer, BUFSIZE);
		}
	}

	/**
	* Listener that is fired when content with a known length has been read.
	*/
	virtual void connReadFinished(Connection* conn, int result) {
		printf("connReadFinished result: %i\n\n", result);
		if(sRepeat)
			start(sUrl);
		else
			showInformation();
	}

	/**
	* Listener that is fired when content with unknown length has been read.
	*/
	virtual void connRecvFinished(Connection* conn, int result) {
		printf("connRecvFinished result: %i\n\n", result);
		if(sRepeat)
			start(sUrl);
		else
			showInformation();
	}

	/**
	* Moblet listener that is fired when a key has been pressed.
	*/
	void keyPressEvent(int keyCode, int nativeCode) {
		if(keyCode == MAK_0 || keyCode == MAK_SOFTRIGHT)
			maExit(0);
		if(keyCode == MAK_FIRE)
			start(EXAMPLE_URL);
		if(keyCode == MAK_1 || keyCode == MAK_SOFTLEFT)
			start(SECURE_URL);
		if(keyCode == MAK_5) {
			sRepeat = !sRepeat;
			sCount = 0;
			printf("Repeat: %i\n", sRepeat);
		}
	}
	
	/**
	* Moblet listener that has been fired when the screen has been pressed.
	*/
	void pointerPressEvent(MAPoint2d p) {
		start(EXAMPLE_URL);
	}

	void closeEvent() {

	}

private:
	/**
	* The char buffer that is used for storing the content.
	*/
	char mBuffer[BUFSIZE];

	/**
	* The connection object used for the connection.
	*/
	HttpConnection mHttp;
};

/**
* Program entry point which initiates the console and starts the Moblet.
*/
extern "C" int MAMain() {
	InitConsole();
	gConsoleLogging = 1;

	Moblet::run(new MyMoblet());
	return 0;
}
