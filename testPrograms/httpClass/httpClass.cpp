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
#include <mautil/environment.h>
#include <mautil/connection.h>
#include <maassert.h>
#include <mastdlib.h>

using namespace MAUtil;

namespace MAUtil {

#if 0
//An HTTP POST operation
class HttpPostListener : public HttpListener {
public:
	virtual void httpPostComplete() = 0;
};

class HttpPost : public Http {
public:
	HttpPost(HttpPostListener& listener);
	int setup(const char* url, const void* src, int len);
};
#endif


//TODO, immediate: move all of this shit into the Conn class itself.
//32 more bytes per connection is nothing. :)

#if 0
//Uses a Conn to read a specific amount of data, no more or less.
class ConnReadListener {
public:
	virtual void connReadFinished(int result) = 0;
};

class ConnReadBase : public ConnListener {
public:
	ConnReadBase();
	~ConnReadBase();
	void setup(Conn* conn, ConnReadListener* listener);
protected:
	Conn* mConn;
	ConnReadListener* mListener;
	int mRemain;

	virtual void recv(int prevRes) = 0;
private:	//ConnListener
	void connectFinished(Conn* conn, int result);
	void connRecvFinished(Conn* conn, int result);
	void connWriteFinished(Conn* conn, int result);
};

void ConnReadBase::connRecvFinished(Conn* conn, int result) {
	MAASSERT(conn == mConn);
	if(result < 0) {
		mListener->connReadFinished(result);
		return;
	}
	mRemain -= result;
	MAASSERT(mRemain >= 0);
	if(mRemain == 0) {
		mListener->connReadFinished(1);
	} else {
		recv(result);
	}
}


class	ConnReadMem : public ConnReadBase {
public:
	void read(void* dst, int len);

protected:
	byte* mDst;

	void recv(int prevRes);
};

void ConnReadMem::read(void* dst, int len) {
	mDst = (byte*)dst;
	mRemain = len;
	recv(0);
}

void ConnReadMem::recv(int prevRes) {
	mDst += prevRes;
	mConn->recv(mDst, mRemain);
}


class	ConnReadData : public ConnReadBase {
public:
	void read(Handle data, int offset, int len);

protected:
	Handle mData;
	int mOffset;
	int mRemain;

	void recv(int prevRes);
};

void ConnReadData::read(Handle data, int offset, int len) {
	mData = data;
	mOffset = offset;
	mRemain = len;
	recv(0);
}

void ConnReadData::recv(int prevRes) {
	mOffset += prevRes;
	mConn->recvToData(mData, mOffset, mRemain);
}
#endif
}	//namespace MAUtil

#include <MAUtil/Moblet.h>
#include <conprint.h>

using namespace MAUtil;

#define BUFSIZE 1024*1024*2
#define MIN(a, b) ((a) < (b) ? (a) : (b))


class MyMoblet : public Moblet, private HttpConnectionListener {
public:
#pragma warning(disable:4355)
	MyMoblet() : mHttp(this) {
		printf("Hello World!\n");

		int res = mHttp.create("http://ftp.sunet.se/pub/Networked.Information.Retrieval/Hyper-G/Tools/Win32/HyperWaveTools.zip", HTTP_GET);
		if(res < 0) {
			printf("http.create fail %i\n", res);
		} else {
			mHttp.finish();
		}
	}

	virtual void httpFinished(HttpConnection* http, int result) {
		printf("httpFinished %i\n", result);
		String cl;
		int res = mHttp.getResponseHeader("content-length", &cl);
		printf("cl code %i\n", res);
		if(res > 0) {
			printf("cl strlen %i\n", cl.length());
			printf("cl: %s\n", cl.c_str());
			int len = atoi(cl.c_str());
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
}
