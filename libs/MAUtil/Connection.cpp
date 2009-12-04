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

#include <conprint.h>
#include "Connection.h"
#include "util.h"

namespace MAUtil {

void ConnectionListener::connectFinished(Connection* conn, int result) {
	PANIC_MESSAGE("ConnectionListener::connectFinished unimplemented!");
}
void ATTRIB(noreturn) ConnectionListener::connRecvFinished(Connection* conn, int result) {
	PANIC_MESSAGE("ConnectionListener::connRecvFinished unimplemented!");
}
void ATTRIB(noreturn) ConnectionListener::connWriteFinished(Connection* conn, int result) {
	PANIC_MESSAGE("ConnectionListener::connWriteFinished unimplemented!");
}
void ATTRIB(noreturn) ConnectionListener::connReadFinished(Connection* conn, int result) {
	PANIC_MESSAGE("ConnectionListener::connReadFinished unimplemented!");
}


//if mConn == 0 then it's uninitialized
Connection::Connection(ConnectionListener* listener) : mConn(0), mListener(listener), mRemain(0) {
}

Connection::~Connection() {
	close();
}

bool Connection::isOpen() const {
	return mConn > 0;
}

int Connection::connect(const char* url) {
	ASSERT_MSG(mConn <= 0, "already connected");
	mConn = maConnect(url);
	if(mConn < 0)
		return mConn;
	Environment::getEnvironment().setConnListener(mConn, this);
	return 1;
}

void Connection::close() {
	if(mConn > 0) {
		maConnClose(mConn);
		Environment::getEnvironment().removeConnListener(mConn);
		mConn = 0;
	}
}

void Connection::write(const void* src, int len) {
	maConnWrite(mConn, src, len);
}
void Connection::writeFromData(MAHandle data, int offset, int len) {
	maConnWriteFromData(mConn, data, offset, len);
}

void Connection::recv(void* dst, int maxlen) {
	maConnRead(mConn, dst, maxlen);
}
void Connection::recvToData(MAHandle data, int offset, int maxlen) {
	maConnReadToData(mConn, data, offset, maxlen);
}

void Connection::read(void* dst, int len) {
	ASSERT_MSG(len > 0, "invalid length");
	mDst = (byte*)dst;
	mRemain = len;
	mRecv = &Connection::recvMem;
	dummy(0);	//gcc bug causes crash unless a function is called.
	(this->*mRecv)(0);
}
void Connection::recvMem(int prevRes) {
	mDst += prevRes;
	maConnRead(mConn, mDst, mRemain);
}

void Connection::readToData(MAHandle data, int offset, int len) {
	ASSERT_MSG(len > 0, "invalid length");
	mData = data;
	mOffset = offset;
	mRemain = len;
	mRecv = &Connection::recvData;
	dummy(0);	//gcc bug causes crash unless a function is called.
	(this->*mRecv)(0);
}
void Connection::recvData(int prevRes) {
	mOffset += prevRes;
	maConnReadToData(mConn, mData, mOffset, mRemain);
}

void Connection::connEvent(const MAConnEventData& data) {
	//printf("cE %i %i\n", data.handle, mConn);
	ASSERT_MSG(data.handle == mConn, "didn't register for this connection handle");
	switch(data.opType) {
	case CONNOP_CONNECT:
		mListener->connectFinished(this, data.result);
		break;
	case CONNOP_READ:
		if(mRemain > 0) {
			if(data.result < 0) {
				mRemain = 0;
				mListener->connReadFinished(this, data.result);
				return;
			}
			mRemain -= data.result;
			MAASSERT(mRemain >= 0);	//failure here means this lib or the MoSync runtime is broken.
			if(mRemain == 0) {
				mListener->connReadFinished(this, 1);
			} else {
				(this->*mRecv)(data.result);
			}
		} else {
			mListener->connRecvFinished(this, data.result);
		}
		break;
	case CONNOP_WRITE:
		mListener->connWriteFinished(this, data.result);
		break;
	}
}

void Connection::setListener(ConnectionListener* listener) {
	mListener = listener;
}


//if mConn == 0 then it's uninitialized
HttpConnection::HttpConnection(HttpConnectionListener* listener) : Connection(listener) {
}

int HttpConnection::create(const char* url, int method) {
	ASSERT_MSG(mConn <= 0, "already connected");
	mConn = maHttpCreate(url, method);
	if(mConn < 0)
		return mConn;
	Environment::getEnvironment().setConnListener(mConn, this);
	return 1;
}

void HttpConnection::setRequestHeader(const char* key, const char* value) {
	maHttpSetRequestHeader(mConn, key, value);
}

int HttpConnection::getResponseHeader(const char* key, String* str) {
	int res = maHttpGetResponseHeader(mConn, key, str->pointer(), 0);
	if(res < 0)
		return res;
	str->resize(res);
	return maHttpGetResponseHeader(mConn, key, str->pointer(), str->capacity() + 1);
}

void HttpConnection::finish() {
	maHttpFinish(mConn);
}

void HttpConnection::connEvent(const MAConnEventData& data) {
	ASSERT_MSG(data.handle == mConn, "didn't register for this connection handle");
	if(data.opType == CONNOP_FINISH)
		((HttpConnectionListener*)mListener)->httpFinished(this, data.result);
	else
		Connection::connEvent(data);
}

}	//namespace MAUtil
