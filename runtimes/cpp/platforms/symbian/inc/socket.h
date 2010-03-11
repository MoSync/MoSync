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

#ifndef SOCKET_H
#define SOCKET_H

#include <ES_SOCK.H>
#include <in_sock.h>
#include <bt_sock.h>

#include <helpers/helpers.h>

#include "CbSync.h"
#include "symbian_helpers.h"
#include "string2map.h"

class CSocket : public CConnection {
public:
	enum Type {
		ETcp, ERfcomm
	};

	CSocket(RSocketServ& aServer, Type type) {
		switch(type) {
		case ETcp:
			LHEL(mSocket.Open(aServer, KAfInet, KSockStream, KProtocolInetTcp));
			break;
		case ERfcomm:
			LHEL(mSocket.Open(aServer, KBTAddrFamily, KSockStream, KRFCOMM));
			break;
		}
	}
	~CSocket() {
		mSocket.CancelAll();
		mSocket.Close();
	}

	void Connect(TSockAddr& aAddr, CPublicActive& op);
	bool Write(const TDesC8& aDesc, CPublicActive& op);
	void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op);
	void CancelAll() {
		mSocket.CancelAll();		
	}

private:
	RSocket mSocket;
	TSockXfrLength mDummyLength;
};


class CHttpConnection : public CSocket {
public:	//CConnection
	//only allowed on FINISHED sockets
	void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op);

	//only allowed on POST sockets in WRITING state
	bool Write(const TDesC8& aDesc, CPublicActive& op);
	
	CHttpConnection* http() { return this; }

public:	//CHttpConnection
	CHttpConnection(RSocketServ& aServer, int method, TUint16 port, RStringPool& pool)
	: CSocket(aServer, CSocket::ETcp), mState(SETUP), mPort(port), mMethod(method),
	mRequestHeaders(pool), mResponseHeaders(pool),
	mSync(TSyncCallback(SyncCallbackL, this)), mSyncOp(NULL), mLineHandler(NULL) {}

	void ConstructL(const TDesC8& hostname, const TDesC8& path);
	
	~CHttpConnection();

	void SetRequestHeaderL(const TDesC8& key, const TDesC8& value);
	const TDesC8* GetResponseHeaderL(const TDesC8& key) const;
	int GetResponseCode() const { return mResponseCode; };

	void FormatRequestL();
	
	//result value is either a negative symbian error code,
	//or a positive number which is a negated CONNERR code,
	//or, on success, zero.
	void ReadHeadersL(CPublicActive& op);	//goes from any to FINISHED

	enum State {
		SETUP=1, WRITING, FINISHING, FINISHED
	} mState;

	const TUint16 mPort;
	const int mMethod;
	Smartie<HBufC8> mPath;
	Smartie<HBufC> mHostname;
	Smartie<CBufFlat> mBuffer;
	TNameEntry mNameEntry;
	TMyPtr8 mBufPtr;

private:
	//int readLine(const char*& lineP);
	//int sendHeaders();	//goes from SETUP to WRITING
	
	static void SyncCallbackL(TAny* aPtr, TInt aResult);
	void RunL(TInt aResult);
	void ReadMoreHeadersL();
	void StatusLineHandlerL(const TDesC8& aLine);
	void HeaderLineHandlerL(const TDesC8& aLine);
	void CompleteReadHeaders(int aConnErr, int aResult);

	int mPos;
	String2Map mRequestHeaders, mResponseHeaders;
	CCBSynchronizer mSync;
	CPublicActive* mSyncOp;
	void (CHttpConnection::*mLineHandler)(const TDesC8& aLine);
	int mResponseCode;
	TMyPtr8 mRecvPtr;
};

#endif  //SOCKET_H
