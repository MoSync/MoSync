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
#include <SecureSocket.h>

#include <helpers/helpers.h>

#include "CbSync.h"
#include "symbian_helpers.h"
#include "string2map.h"

class CMySecureSocket;

class CBaseSocket : public CConnection {
public:
	virtual void Connect(TSockAddr& aAddr, CPublicActive& op) = 0;
	virtual CMySecureSocket* ssl() = 0;
};

class CSocket : public CBaseSocket {
public:
	enum Type {
		ETcp, ERfcomm, EBlank
	};

	CSocket(RSocketServ& aServer, Type type) : mClosed(true) {
		switch(type) {
		case ETcp:
			LHEL(mSocket.Open(aServer, KAfInet, KSockStream, KProtocolInetTcp));
			break;
		case ERfcomm:
			LHEL(mSocket.Open(aServer, KBTAddrFamily, KSockStream, KRFCOMM));
			break;
		case EBlank:
			LHEL(mSocket.Open(aServer));
			break;
		}
		mClosed = false;
	}
	virtual ~CSocket() {
		LOGD("~CSocket()\n");
		if(!mClosed) {
			mSocket.CancelAll();
			mSocket.Close();
			mClosed = true;
		}
	}

	virtual void Connect(TSockAddr& aAddr, CPublicActive& op);
	virtual bool Write(const TDesC8& aDesc, CPublicActive& op);
	virtual void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op);
	virtual void CancelAll() {
		mSocket.CancelAll();
	}
	CMySecureSocket* ssl() { return NULL; }
	
	void GetAddr(MAConnAddr* addr);
	
	RSocket& socket() { return mSocket; }

protected:
	RSocket mSocket;
	TSockXfrLength mDummyLength;
	bool mClosed;
};

class CMySecureSocket : public CSocket {
public:
	CMySecureSocket(RSocketServ& aServer) : CSocket(aServer, ETcp), mSS(NULL) {}
	virtual ~CMySecureSocket();
	virtual bool Write(const TDesC8& aDesc, CPublicActive& op);
	virtual void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op);
	virtual void CancelAll();
	CMySecureSocket* ssl() { return this; }
	void Handshake(const TDesC* hostname, CPublicActive& op);
private:
	CSecureSocket* mSS;
};

class CServerSocket : public CConnection {
public:
	~CServerSocket() {
		mSocket.CancelAll();
		mSocket.Close();
	}
	void Accept(RSocket& aBlankSocket, CPublicActive& op);

	//CConnection
	void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op);
	bool Write(const TDesC8& aDesc, CPublicActive& op);
	void CancelAll();
	void GetAddr(MAConnAddr* addr);
	CServerSocket* server() { return this; }
protected:
	RSocket mSocket;
};

class CBtServerSocket : public CServerSocket {
public:
	CBtServerSocket(RSdpDatabase& db)
	: mSdpDB(db), mHandle(0)
	{}
	~CBtServerSocket();
	void init(RSocketServ& aServer, const TUUID& uuid, bool hasName,
		const TDesC8& name = KNullDesC8);

	//CConnection
	//CBtServerSocket* btServer() { return this; }
private:
	RSdpDatabase& mSdpDB;
	TSdpServRecordHandle mHandle;
};


class CHttpConnection : public CBaseSocket {
public:	//CConnection
	//only allowed on FINISHED sockets
	void RecvOneOrMoreL(TDes8& aDes, CPublicActive& op);

	//only allowed on POST sockets in WRITING state
	bool Write(const TDesC8& aDesc, CPublicActive& op);
	
	CHttpConnection* http() { return this; }
	
	void CancelAll() { mTransport->CancelAll(); }
	void GetAddr(MAConnAddr* addr) { mTransport->GetAddr(addr); }

	//CBaseSocket
	void Connect(TSockAddr& aAddr, CPublicActive& op) { mTransport->Connect(aAddr, op); }
	CMySecureSocket* ssl() { return mTransport->ssl(); }
public:	//CHttpConnection
	CHttpConnection(CBaseSocket* transport, int method, uint port, RStringPool& pool)
	: mState(SETUP), mMethod(method), mPort(port),
	mRequestHeaders(pool), mResponseHeaders(pool),
	mSync(TSyncCallback(SyncCallbackL, this)), mSyncOp(NULL), mLineHandler(NULL),
	mTransport(transport) {}

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

	const int mMethod;
	const uint mPort;
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
	CBaseSocket* mTransport;
};

#endif  //SOCKET_H
