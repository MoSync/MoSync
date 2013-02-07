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

#ifndef NETWORKING_H
#error "Bad include!"
#endif	//NETWORKING_H

//This file is designed to be included in the private section of class Syscall.

//******************************************************************************
//Declarations
//******************************************************************************

//******************************************************************************
//Definitions
//******************************************************************************

#define FS(base,unit) base(unit##_F, unit##_S)
#define FS_NULA2(base,unit) base(unit##_F, unit##_S, NULA2)

#define DECLARE_ARG_F(type,name) type _##name
#define DECLARE_ARG_S(type,name) ,DECLARE_ARG_F(type,name)

#define CDECL_ARG_F DECLARE_ARG_S
#define CDECL_ARG_S DECLARE_ARG_S

#define INIT_ARG(type,name) ,name(_##name)

#define DECLARE_VAR(type,name) type name;

#define REFERENCE_ARG_F(type,name) ,_##name
#define REFERENCE_ARG_S(type,name) ,_##name

//A class describing a single asynchronous connection operation
//(that is, it must have access to the active scheduler loop)
class ConnSubOp : public FifoQueLink {
public:
	const byte type;

	virtual ~ConnSubOp() {
		//LOGD("~ConnSubOp 0x%08X\n", this);
	}
protected:
	ConnSubOp(byte _type) : type(_type) { /*LOGST("ConnSubOp");*/ }
private:
	ConnSubOp();
	ConnSubOp(const ConnSubOp&);
	ConnSubOp& operator=(const ConnSubOp&);
};

#define CONN_SUBOPS(f, s) f(SockConnect) s(Read) s(Write) s(HttpReadHeaders)\
	s(StartNetworking) s(Accept) s(SecureHandshake)

#define CSV_SockConnect(f, s) f(TSockAddr&,addr) s(int,port) s(CBaseSocket&,sock)
#define CSV_Read(f, s) f(TDes8&,des)
#define CSV_Write(f, s) f(const TDesC8&,desc)
#define CSV_HttpReadHeaders(f, s) f(CHttpConnection&,http)
#define CSV_StartNetworking(f, s)
#define CSV_Accept(f, s) f(CServerSocket&,server) s(CSocket*,newSock)
#define CSV_SecureHandshake(f, s) f(CMySecureSocket&,sock) s(const TDesC*,hostname)

#define CSOC_F(a) CSOC_##a
#define CSOC_S(a) ,CSOC_##a

enum ConnSubOpCode { FS(CONN_SUBOPS, CSOC), CSOC_Resolve };

#define DEFINE_CSO(name) class CSO_##name : public ConnSubOp {\
public: CSO_##name(FS(CSV_##name, DECLARE_ARG)):\
	ConnSubOp(CSOC_##name) CSV_##name(INIT_ARG,INIT_ARG) {}\
	virtual ~CSO_##name() { LOGD("~CSO_" #name "\n"); }\
	CSV_##name(DECLARE_VAR,DECLARE_VAR)\
	};

CONN_SUBOPS(DEFINE_CSO,DEFINE_CSO)

//special requirements (resolver), so this class is defined manually.
class CSO_Resolve : public ConnSubOp {
public:
	CSO_Resolve(const TDesC& aHostname, TNameEntry& aResult) :
		ConnSubOp(CSOC_Resolve), hostname(aHostname), result(aResult) {}

	virtual ~CSO_Resolve() {
		LOGD("~CSO_Resolve\n");
		resolver.Close();
	}

	const TDesC& hostname;
	TNameEntry& result;
	RHostResolver resolver;
};


//A class describing a complete connection operation,
//analogue to a MoSync syscall.
class ConnOp : public CPublicActive {
public:
	TDblQueLink mLink;
	const byte mType;
	FifoQue<ConnSubOp> mSubOps;
	Syscall& mSyscall;
	const MAHandle mHandle;
	CConnection& mConn;

	virtual ~ConnOp() {
	//	LOGD("~ConnOp 0x%08X\n", this);

		//causes crash
		//since DoCancel accesses sub-classes' data, which has been destructed by now,
		//Cancel();
		//LOGD("ConnOp 0x%08X canceled\n", this);
	}
	void StartConnSubOpL();
protected:
	ConnOp(Syscall& _s, MAHandle h, CConnection& c, byte _type)
		: mType(_type), mSyscall(_s), mHandle(h), mConn(c) {
		//LOGST("ConnOp");
	}
	void RunL();
	void DoCancel();

	//for constructors
	void addHttpSendHeaders();
	void addHttpReadHeaders();
	void addSockConnect(CBaseSocket& sock, TSockAddr& addr, int port, const TDesC* hostname);
private:
	ConnOp();
	ConnOp(const ConnOp&);
	ConnOp& operator=(const ConnOp&);

	void SendResult(int result);
};

#define CONN_OPS(f, s) f(AddrConnect) s(NameConnect) s(Read) s(Write)\
	s(ReadToData) s(WriteFromData) s(HttpFinish) s(Accept)

#define CV_AddrConnect(f, s, e) f(TSockAddr,addr) s(int,port) s(CSocket&,sock)
#define CV_NameConnect(f, s, e) f(Smartie<HBufC>,hostname) s(int,port)\
	s(CSocket&,sock) e(TNameEntry,nameEntry)
#define CV_Read(f, s, e) f(TPtr8,des)
#define CV_Write(f, s, e) f(TPtrC8,desc)
#define CV_ReadToData(f, s, e) f(Smartie<Stream>,dst) s(MAHandle,handle)\
	s(int,offset) s(int,size) e(TMyPtr8,des)
#define CV_WriteFromData(f, s, e) f(Smartie<Stream>,src) s(MAHandle,handle)\
	s(int,offset) s(int,size) e(TPtrC8,desc) e(Smartie<MemStream>,buffer)
#define CV_HttpFinish(f, s, e) f(CHttpConnection&,http) s(bool,connect)
#define CV_Accept(f, s, e) f(CServerSocket&,server) s(CSocket*,newSock)

#define COC_F(a) COC_##a
#define COC_S(a) ,COC_##a

enum ConnOpCode { FS(CONN_OPS, COC) };

#define DEFINE_CO(name) class CO_##name : public ConnOp {\
public: static CO_##name* NewL(bool aNetworkingIsStarted,\
	Syscall& s, MAHandle h, CConnection& c FS_NULA2(CV_##name, CDECL_ARG));\
	CV_##name(DECLARE_VAR,DECLARE_VAR,DECLARE_VAR)\
	virtual ~CO_##name();\
private: CO_##name(Syscall& s, MAHandle h, CConnection& c\
	FS_NULA2(CV_##name, CDECL_ARG))\
	: ConnOp(s, h, c, COC_##name) CV_##name(INIT_ARG,INIT_ARG,NULA2) {}\
	void ConstructL(bool aNetworkingIsStarted);\
	};

CONN_OPS(DEFINE_CO,DEFINE_CO)


#if 0
class CConnCleanupQue : public CActive {
public:
	CConnCleanupQue() : CActive(MyPrioStandard), mQue(_FOFF(ConnOp,iLink)) {
		CActiveScheduler::Add(this);
	}
	~CConnCleanupQue() {
		LOGS("~CConnCleanupQue()\n");
		Cancel();
	}
	void move(ConnOp& op) {
		LOGS("CCCQ move 0x%08X\n", &op);
		op.iLink.Deque();
		mQue.AddLast(op);
		if(!IsActive()) {
			iStatus = KRequestPending;
			SetActive();
			TRequestStatus* rsp = &iStatus;
			User::RequestComplete(rsp, KErrNone);
		}
	}
protected:	//CActive
	void RunL() { clean(); }
	void DoCancel() { clean(); }
private:
	TDblQue<ConnOp> mQue;

	void clean() {
		LOGS("CConnCleanupQue.clean %i\n", iStatus.Int());
		while(!mQue.IsEmpty()) {
			ConnOp* opp = mQue.First();
			LOGS("CCCQ deque/lete 0x%08X\n", opp);
			opp->iLink.Deque();
			delete opp;
		}
	}
};
#endif

//******************************************************************************
//Functions
//******************************************************************************
#if defined(__MARM_ARMI__) || defined(__MARM_THUMB__)	//buggy compiler
public:
#endif

void ClearNetworkingVariables();
void ConstructNetworkingL();
void StopNetworking();
void DestructNetworking();

void StartConnOpL(ConnOp* op);
void CancelConnOps(MAHandle conn);

enum SocketType {
	SSL, TCP, UDP
};
CSocket* createSocket(SocketType);
int httpCreateConnectionLC(const TDesC8& parturl, CHttpConnection*& conn,
	int method, SocketType);

CHttpConnection& GetHttp(MAHandle conn);
void StartNetworkingL(ConnOp& connOp);

void FinishNetworkingStartL();

void maAccept(MAHandle conn);

bool getSavedIap(TUint& iap);

int maIapSave();
int maIapReset();
int maIapShutdown();

#ifdef __SERIES60_3X__
#ifdef SUPPORT_MOSYNC_SERVER
int maNetworkStatus();
void NetworkStatusChangeHandlerL(TInt result);
#endif

int maIapSetMethod(int method);
int maIapSetFilter(int filter);
#endif	//__SERIES60_3X__

//******************************************************************************
//Variables
//******************************************************************************

RConnection gConnection;
int gConnNextHandle;
HashMap<CConnection> gConnections;
TDblQue<ConnOp> gConnOps;
CActiveEnder* gConnCleanupQue;
RStringPool gHttpStringPool;
#ifdef __SERIES60_3X__
#ifdef SUPPORT_MOSYNC_SERVER
CTelephony::TNetworkRegistrationV1 gNetworkRegistration;
CTelephony::TNetworkRegistrationV1Pckg gNetworkRegistrationPckg;
CClassSynchronizer<Syscall>* gNetworkStatusSync;
#endif
int gIapMethod;
int gIapFilter;
#endif
TUint gIapId;
TBuf16<KMaxPath> gIapPath16;
TBuf8<KMaxPath> gIapPath8;

//TDblQue<ConnOp> gConnOpsWaitingForNetworkingStart;	//error; can't be in two Ques at once.
//just a little inefficient, but since this is used only once,
//and for a short period, it's cool.
RPointerArray<ConnOp> gConnOpsWaitingForNetworkingStart;

enum NetworkingState {
	EIdle, EStarting, EStarted
} gNetworkingState;
