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

#include <MAUtil/util.h>
#include <MAUtil/Connection.h>
#include <conprint.h>
#include "common.h"
#include "MAHeaders.h"
#include "conn_common.h"

//HTTP: use all known headers. add in a bunch of random ones too.
//check for filtering

//all conn types:
//read x bytes
//compare

//write x bytes
//server will compare, reply success or failure

//torture test:
//recv and write different sizes (1-16, powers of 2 up to perhaps 2^13, random values in between).
//this should find any hidden requirements of buffer size.

//create maximum number of connections (sockets, bluetooth, http) individually and mixed.
//try to write/read from all of them at once.

#define BT_HOST "0080984474c8"	//TDK @ MS-FREDRIK
#define IP_HOST "modev.mine.nu" //"192.168.0.173"	//"localhost"	//

#define SOCKET_URL(port) ("socket://" IP_HOST ":" + integerToString(port)).c_str()
#define HTTP_GET_URL(port) ("http://" IP_HOST ":" +integerToString(port)+ "/server_data.bin").c_str()
#define HTTP_POST_URL ("http://" IP_HOST ":5004/post")
#define BT_URL(port) ("btspp://" BT_HOST ":" +integerToString(port)).c_str()

#ifdef _MSC_VER
#pragma warning(disable:4355)
#endif

class SocketSizeCase : public TestCase, public ConnectionListener {
private:
	//1, 2, 3, 4,  6, 7, 8,  14, 15, 16, ...32,64,128,156,512,1024,2048,4096
	//sizes <= 0 should result in a panic. we'll test that elsewhere.
#define NSIZES (1 + 2 + 9*3)
	int mSizes[NSIZES];
	char mServerData[DATA_SIZE];

	int mCurrentSizeIndex;
	Connection mConn;
	char mReadBuffer[DATA_SIZE];
public:
	SocketSizeCase() : TestCase("socketSize"), mConn(this) {
		mSizes[0] = 1;
		mSizes[1] = 2048;
		mSizes[2] = 4096;
		for(int i=1; i<=9; i++) {
			mSizes[i*3+0] = (2 << i) - 2;
			mSizes[i*3+1] = (2 << i) - 1;
			mSizes[i*3+2] = (2 << i) - 0;
		}

		maReadData(SERVER_DATA, mServerData, 0, DATA_SIZE);
	}

	//TestCase
	void start() {
		printf("Socket size test\n");
		mCurrentSizeIndex = -1;
		int res = mConn.connect(SOCKET_URL(SOCKET_SIZE_PORT));
		if(res <= 0) {
			printf("connect %i\n", res);
			fail();
		}
	}
	void close() {
		mConn.close();
	}

	//ConnectionListener
	virtual void connectFinished(Connection* conn, int result) {
		printf("Connected %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		next();
	}
	virtual void connReadFinished(Connection* conn, int result) {
		printf("Read %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		if(memcmp(mReadBuffer, mServerData, mSizes[mCurrentSizeIndex]) != 0) {
			printf("Data diff!\n");
			fail();
			return;
		}
		next();
	}

	void next() {
		if(mCurrentSizeIndex < NSIZES) {
			mCurrentSizeIndex++;
			mConn.read(mReadBuffer, mSizes[mCurrentSizeIndex]);
		} else {
			succeed();
		}
	}

	void fail() {
		assert(name, false);
		suite->runNextCase();
	}

	void succeed() {
		assert(name, true);
		suite->runNextCase();
	}
};

struct PAIR {
	const char* key, *value;
};
static const PAIR sHeaders[] = {	//I suppose we should test all known header keys here
	{"foo", "bar"},
	{"w00t", "b4k4"},
	{"Cookie", "WAAAGH!"}
};
static const int snHeaders = sizeof(sHeaders) / sizeof(PAIR);

class SingleHttpPostCase : public TestCase, public HttpConnectionListener {
private:
	HttpConnection mHttp;
	char* mReadBuffer;
	char* mClientData;
	const int mMultiple;
public:
	SingleHttpPostCase(int multiple) : TestCase("singlePost"), mHttp(this),
		mReadBuffer(NULL), mClientData(NULL), mMultiple(multiple)
	{
	}

	void fail() {
		assert(name, false);
		suite->runNextCase();
	}

	//TestCase
	void start() {
		printf("Single HTTP test (%d)\n", mMultiple);
		int res = mHttp.create("http://www.example.com/", HTTP_POST);//HTTP_POST_URL, HTTP_POST);
		printf("res:%d\n", res);
		if(res <= 0) {
			printf("create %i\n", res);
			fail();
			return;
		}
		//allocate source data
		mClientData = new char[mMultiple * DATA_SIZE];
		for(int i=0; i<mMultiple; i++) {
			maReadData(CLIENT_DATA, mClientData + (i * DATA_SIZE), 0, DATA_SIZE);
		}
/*
		//set a bunch of headers
		for(int i=0; i<snHeaders; i++) {
			mHttp.setRequestHeader(sHeaders[i].key, sHeaders[i].value);
		}
*/
		char buffer[64];
		sprintf(buffer, "%i", DATA_SIZE * mMultiple);
		printf("cl:%d\n", DATA_SIZE*mMultiple);
		mHttp.setRequestHeader("Content-Length", buffer);
		//write some data
		mHttp.write(mClientData, DATA_SIZE * mMultiple);

	}
	void close() {
		mHttp.close();
		delete mClientData;
		mClientData = NULL;
		delete mReadBuffer;
		mReadBuffer = NULL;
	}

	//HttpConnectionListener
	virtual void connWriteFinished(Connection*, int result) {
		printf("Write %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		//our POST is complete. ask for the response.
		mHttp.finish();
	}
	virtual void httpFinished(HttpConnection*, int result) {
		printf("Finish %i\n", result);
		if(result < 200 || result >= 300) {
			fail();
			return;
		}
		//check the response headers
		for(int i=0; i<snHeaders; i++) {
			String str;
			result = mHttp.getResponseHeader(sHeaders[i].key, &str);
			if(result < 0) {
				fail();
				return;
			}
			if(strcmp(str.c_str(), sHeaders[i].value) != 0) {
				fail();
				return;
			}
		}

		mReadBuffer = new char[DATA_SIZE * mMultiple];

		//read the response body
		mHttp.read(mReadBuffer, DATA_SIZE * mMultiple);
	}
	virtual void connReadFinished(Connection*, int result) {
		printf("Read %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		assert(name, memcmp(mReadBuffer, mClientData, DATA_SIZE * mMultiple) == 0);
		suite->runNextCase();
	}
};

class SingleSocketCase : public TestCase, public ConnectionListener {
private:
	Connection mConn;
	char mReadBuffer[DATA_SIZE];
	char mClientData[DATA_SIZE];
	char mServerData[DATA_SIZE];
	bool mReadFinished;
public:
	SingleSocketCase() : TestCase("singleSocket"), mConn(this) {
		maReadData(CLIENT_DATA, mClientData, 0, DATA_SIZE);
		maReadData(SERVER_DATA, mServerData, 0, DATA_SIZE);
	}

	void fail() {
		assert(name, false);
		suite->runNextCase();
	}

	//TestCase
	void start() {
		printf("Single Socket test\n");
		memset(mReadBuffer, 0, DATA_SIZE);
		mReadFinished = false;
		int res = mConn.connect(SOCKET_URL(SINGLE_SOCKET_PORT));
		if(res <= 0) {
			printf("connect %i\n", res);
			fail();
		}
	}
	void close() {
		mConn.close();
	}

	virtual void connectFinished(Connection* conn, int result) {
		printf("Connected %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		conn->read(mReadBuffer, DATA_SIZE);
	}
	virtual void connReadFinished(Connection* conn, int result) {
		printf("Read %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		if(!mReadFinished) {
			if(memcmp(mReadBuffer, mServerData, DATA_SIZE) != 0) {
				printf("Data diff!\n");
				fail();
				return;
			}
			mReadFinished = true;
			conn->write(mClientData, DATA_SIZE);
		} else {	//reply to write
			//success?
			assert(name, mReadBuffer[0] != 0x01);
			suite->runNextCase();
		}
	}
	virtual void connWriteFinished(Connection* conn, int result) {
		printf("Wrote %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		conn->read(mReadBuffer, 1);
	}
};


class SingleHttpGetCase : public TestCase, public HttpConnectionListener {
private:
	HttpConnection mHttp;
	char mReadBuffer[DATA_SIZE];
	char mServerData[DATA_SIZE];
public:
	SingleHttpGetCase() : TestCase("singleHttp"), mHttp(this) {
		maReadData(SERVER_DATA, mServerData, 0, DATA_SIZE);
	}

	void fail() {
		assert(name, false);
		suite->runNextCase();
	}

	//TestCase
	void start() {
		printf("Single HTTP get test\n");
		memset(mReadBuffer, 0, DATA_SIZE);
		int res = mHttp.create(HTTP_GET_URL(HTTP_PORT), HTTP_GET);
		if(res <= 0) {
			printf("connect %i\n", res);
			fail();
			return;
		}
		mHttp.finish();
	}
	void close() {
		mHttp.close();
	}

	//HttpConnectionListener
	virtual void httpFinished(HttpConnection* http, int result) {
		printf("Finish %i\n", result);
		if(result < 200 || result >= 300) {
			fail();
			return;
		}
		http->read(mReadBuffer, DATA_SIZE);
	}
	virtual void connReadFinished(Connection* conn, int result) {
		printf("Read %i\n", result);
		if(result <= 0) {
			fail();
			return;
		}
		assert(name, memcmp(mReadBuffer, mServerData, DATA_SIZE) == 0);
		suite->runNextCase();
	}
};

void addConnTests(TestSuite* suite);
void addConnTests(TestSuite* suite) {
	//suite->addTestCase(new SingleSocketCase);
	//suite->addTestCase(new SingleHttpGetCase);
	for(int i=0; i<5; i++) {
		suite->addTestCase(new SingleHttpPostCase(1 << i));
	}
}
