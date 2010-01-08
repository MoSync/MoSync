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

#include "Syscall.h"

#include <helpers/helpers.h>
#include <net/net.h>

#define NETWORKING_H
#include "networking.h"


//***************************************************************************
//Variables
//***************************************************************************

ConnMap* gpConnections = NULL;
int gConnNextHandle;
ThreadPool* gpThreadPool = NULL;
#define gThreadPool (*gpThreadPool)
MoSyncMutex* gpConnMutex = NULL;
#define gConnMutex (*gpConnMutex)

//***************************************************************************
//Initialization
//***************************************************************************

void MANetworkInit() {
	gConnNextHandle = 1;
	gpConnMutex = new MoSyncMutex;
	gpThreadPool = new ThreadPool;
	gpConnections = new ConnMap;
	gConnMutex.init();
}

void MANetworkReset() {
	//stop all operations
	while(true) {
		ConnItr itr = gConnections.begin();
		if(itr == gConnections.end())
			break;
		MAHandle conn = itr->first;
		maConnClose(conn);
	}
	gConnNextHandle = 1;
}

void MANetworkClose() {
	MANetworkReset();

	gThreadPool.close();
	gConnMutex.close();
	SAFE_DELETE(gpConnections);
	SAFE_DELETE(gpThreadPool);
	SAFE_DELETE(gpConnMutex);
}

//***************************************************************************
//Helpers
//***************************************************************************

static MAConn& getConn(MAHandle conn) {
	MAConn* macp;
	gConnMutex.lock();
	{
		MYASSERT(!gConnections.empty(), ERR_CONN_HANDLE_INVALID);
		ConnItr itr = gConnections.find(conn);
		MYASSERT(itr != gConnections.end(), ERR_CONN_HANDLE_INVALID);
		macp = itr->second;
	}
	gConnMutex.unlock();
	return *macp;
}

static MAStreamConn& getStreamConn(MAHandle conn) {
	MAConn& mac(getConn(conn));
	MYASSERT(mac.type == eStreamConn, ERR_CONN_NOT_STREAM);
	return (MAStreamConn&)mac;
}

int rtspCreateConnection(const char* url, RtspConnection*& conn) {
	Uint16 port;
	const char *path;
	std::string hostname;
	if(parseProtocolURL(rtsp_string, url, &port, 554, &path, hostname)!=SUCCESS) return CONNERR_URL;
	conn = new RtspConnection(hostname, port, path);
	return 1;
}

//******************************************************************************
//Connection methods
//******************************************************************************

//RtspConnection
//*****************
#define RTSP_INITIAL 0
#define RTSP_DESCRIBE 1
#define RTSP_SETUP 2
#define RTSP_PLAY 3
#define RTSP_PAUSE 4
#define RTSP_TEARDOWN 5

RtspConnection::RtspConnection(const std::string& hostname, Uint16 port, const std::string& path) :
ProtocolConnection(hostname, port, path), mMethod(RTSP_INITIAL), CSeq(1), gotSessionId(false)
{
	methodMutex.init();
}

void RtspConnection::cleanup() {
	methodMutex.close();
}

std::string RtspConnection::methodString() {
	switch(mMethod) {
	case RTSP_DESCRIBE: return "DESCRIBE";
	case RTSP_SETUP: return "SETUP";
	case RTSP_PLAY: return "PLAY";
	case RTSP_PAUSE: return "PAUSE"; 
	case RTSP_TEARDOWN: return "TEARDOWN";
	default:
		DEBIG_PHAT_ERROR;
	}
}

std::string RtspConnection::protocolVersion() {
	return "RTSP/1.0";
}

std::string RtspConnection::pathString() {
	if(mMethod == RTSP_SETUP) {
		return setupPathString;
	} else {
		return ProtocolConnection::pathString();
	}
}

int RtspConnection::readResponseCode(const char* line, int len) {
	int responseCode;
	TLTZ_PASS(responseCode = readProtocolResponseCode("RTSP/", line, len));
	return responseCode;
}

static const char* intToString(int i) {
	static char temp[16];
	sprintf(temp, "%i", i);
	return temp;
}

bool RtspConnection::setMethod(int method) {
	methodMutex.lock();
	if(mMethod == method) {
		methodMutex.unlock();
		return false;
	}
	this->mMethod = method;
	methodMutex.unlock();
	return true;
}

static const char* gotoNextLine(const char *desc) {
	while(*desc) {
		if(*desc==0xd && desc[1] && desc[1]==0xa) {
			desc+=2;
			break;
		} else {
			if(*desc==0xa) {
				desc++;
				break;
			}
		}

		desc++;
	}

	return desc;
}

void RtspConnection::parseDescribeData(const char *desc) {
	streams.clear();
	while(*desc) {
		int type = *desc;
		desc+=2;
		switch(type) {
			case 'a': 
				{
					const char *str = "control:trackID="; 
					if(strncmp(desc, str, strlen(str)-1)==0) {
						desc+=strlen(str);
						sscanf(desc, "%d", &streams[streams.size()-1].trackID);
					}
				}
				break;
			case 'm': 
				{
					//Stream stream;
					streams.resize(streams.size()+1);
					char temp[64];
					char typeBuf[64];
					sscanf(desc, "%s %s %s %s\n", typeBuf, temp, streams[streams.size()-1].packetType, temp);
					if(strcmp(typeBuf, "video")==0) streams[streams.size()-1].streamType = Stream::VIDEO;
					else if(strcmp(typeBuf, "audio")==0) streams[streams.size()-1].streamType = Stream::AUDIO;
				}
				break;
		}
		desc=gotoNextLine(desc);
	}
}

int RtspConnection::sendAndVerify(int method) {
	setMethod(method);
	SetRequestHeader("CSeq", intToString(CSeq));

	if(gotSessionId) {
		SetRequestHeader("Session", sessionId);
	}

	if(finish()<0) return CONNERR_GENERIC;
	const std::string *cseqStr = GetResponseHeader("CSeq");
	if(!cseqStr) return CONNERR_GENERIC;
	int recvCSeq = atoi(cseqStr->c_str());
	if(recvCSeq != CSeq) return CONNERR_GENERIC;

	const std::string *recvSessionId = GetResponseHeader("Session");	
	if(recvSessionId) {
		std::string temp = *recvSessionId + ";";
		if(gotSessionId) {
			if(temp != sessionId) {
				return CONNERR_GENERIC;
			}
		} else {
			gotSessionId = true;
			sessionId = temp;
		}
	}

	return 0;
}

void RtspConnection::parseTransportData(char *temp, int stream) {
	char *pairs[64];
	int i = 0;
	char *beginning = temp;
	while(beginning) {
		pairs[i++] = beginning;
		beginning = strtok(beginning, ";");
	}

	for(int j = 0; j < i; j++) {
		if(strncmp("source", pairs[i], 6)==0) {
			streams[stream].sourceAddress = inet_addr(pairs[i]+7);
		} else if(strncmp("client_port", pairs[i], 11)==0) {
			sscanf(pairs[i]+12, "%d-%d", &streams[stream].sourcePort[0], &streams[stream].sourcePort[1]);
		}
	}
}

int RtspConnection::setupStream(int stream) {
	char temp[1024];
	int res;
	sprintf(temp, "%s/trackID=%d", ProtocolConnection::pathString().c_str(), streams[stream].trackID);
	setupPathString = temp;

	sprintf(temp, "%s;unicast;client_port=9000-9001", streams[stream].packetType);
	// set temp to something

	if((res=sendAndVerify(RTSP_SETUP))<0) return res;

	const std::string *recvTransport = GetResponseHeader("Transport");
	if(!recvTransport) return CONNERR_GENERIC;
	strcpy(temp, recvTransport->c_str());
	parseTransportData(temp, stream);

	return 1;
}

int RtspConnection::setup() {

	int res;
	if((res=sendAndVerify(RTSP_DESCRIBE))<0) return res;

	const std::string *contentLengthStr = GetResponseHeader("Content-Length");
	if(!contentLengthStr) return CONNERR_GENERIC;
	int contentLength = atoi(contentLengthStr->c_str());
	
	char *describeData = new char[contentLength+1];
	if((res=read(describeData, contentLength))<0) return res;	//TODO: error handling
	describeData[contentLength] = 0;
	parseDescribeData(describeData);
	delete []describeData;

	int audio = -1;
	int video = -1;
	for(size_t i = 0; i < streams.size(); i++) {
		if(audio==-1 && streams[i].streamType == Stream::AUDIO) audio = i;
		if(video==-1 && streams[i].streamType == Stream::VIDEO) video = i;
	}

	if((res=setupStream(audio))<0) return res;
	if((res=setupStream(video))<0) return res;

	return 1;
}

int RtspConnection::play() {
	if(!setMethod(RTSP_PLAY)) return CONNERR_GENERIC;
	userRequest.post();
	return 1;
}
int RtspConnection::pause() {
	if(!setMethod(RTSP_PAUSE)) return CONNERR_GENERIC;
	userRequest.post();
	return 1;
}
int RtspConnection::teardown() {
	if(!setMethod(RTSP_TEARDOWN)) return CONNERR_GENERIC;
	userRequest.post();
	return 1;
}

void RtspConnection::run() {
	setup();
	bool running = true;
	while(running) {
		userRequest.wait();
		methodMutex.lock();
		if(sendAndVerify(mMethod)<0) {
			// error;
		}
		if(mMethod == RTSP_TEARDOWN) {
			running = false;
		}
		methodMutex.unlock();
	}
	cleanup();
}

//******************************************************************************
//Proper syscalls
//******************************************************************************

SYSCALL(MAHandle, maConnect(const char* url)) {
	LOGST("Connect %i %s", gConnNextHandle, url);
	if(gConnections.size() >= CONN_MAX)
		return CONNERR_MAX;
	Connection* conn;
	if(sstrcmp(url, http_string) == 0) {
		HttpConnection* http;
		TLTZ_PASS(httpCreateConnection(url, http, HTTP_GET));
		http->mState = HttpConnection::FINISHING;
		conn = http;
	} else if(sstrcmp(url, socket_string) == 0) {
		//allowed forms:
		// socket://
		// socket://:%i
		// socket://%s:%i
		//only the last one is useful/used for now.

		//extract address and port
		const char* parturl = url + sizeof(socket_string) - 1;
		const char* port_m1 = strchr(parturl, ':');
		if(!port_m1) {
			return CONNERR_URL;
		}
		std::string hostname(parturl, size_t(port_m1 - parturl));
		int port = atoi(port_m1 + 1);
		if(port <= 0 || port >= 1 << 16) {
			return CONNERR_URL;
		}
		conn = new TcpConnection(hostname, port);
	} else if(sstrcmp(url, btspp_string) == 0) {
		//allowed forms:
		// btspp://localhost:%32x
		// btspp://localhost:%32x;name=%s
		// btspp://%12x:%i
		//the first two are for servers, without and with service name.
		//the last is for clients.

		//extract address and port
		const char* parturl = url + sizeof(btspp_string) - 1;
		const char* first_colon = strchr(parturl, ':');
		if(!first_colon) {
			return CONNERR_URL;
		}

#ifndef _WIN32_WCE	//not yet supported
		if(strstr(parturl, "localhost") == parturl) {
			//server
			static const char name_string[] = "name=";
			const char* uuidStr = first_colon + 1;
			const char* semicolon = strchr(uuidStr, ';');
			const char* name;
			int uuidLen;
			if(semicolon == NULL) {
				uuidLen = strlen(uuidStr);
				name = NULL;
			} else {
				uuidLen = semicolon - uuidStr;

				const char* paramPtr = semicolon + 1;
				if(strstr(paramPtr, name_string) == paramPtr) {
					//we have a name
					name = paramPtr + sizeof(name_string) - 1;
				} else {
					name = NULL;
				}
			}
			if(uuidLen != 32)
				return CONNERR_URL;
			MAUUID uuid;
			//todo, optional: check that every uuid char isxdigit().
			if(4 != sscanf(uuidStr, "%8x%8x%8x%8x",
				&uuid.i[0], &uuid.i[1], &uuid.i[2], &uuid.i[3]))
			{
				return CONNERR_URL;
			}

			//we have the data. let's create the server
			BtSppServer* serv = new BtSppServer;
			int res = serv->open(uuid, name);
			if(res < 0) {
				delete serv;
				return res;
			}
			gConnMutex.lock();
			{
				MAServerConn* mac = new MAServerConn(gConnNextHandle, serv);
				gConnections.insert(ConnPair(gConnNextHandle, mac));
				mac->state = 0;
				res = gConnNextHandle++;
			}
			gConnMutex.unlock();
			return res;
		} else
#endif	//_WIN32_WCE
		{
			//client

			int purlLen = first_colon - parturl;
			if(purlLen != 12) {
				return CONNERR_URL;
			}
			int ai[6];	//address ints
			if(6 != sscanf(parturl, "%2x%2x%2x%2x%2x%2x",
				&ai[0], &ai[1], &ai[2], &ai[3], &ai[4], &ai[5]))
			{
				return CONNERR_URL;
			}
			MABtAddr address;
			for(int i=0; i<BTADDR_LEN; i++) {
				address.a[i] = (byte)ai[i];
			}

			int port = atoi(first_colon + 1);
			if(port <= 0 || port > 30) {
				return CONNERR_URL;
			}

			conn = createBtSppConnection(&address, port);
		}
	} else {
		return CONNERR_URL;
	}
	//success. let's store our new connection.
	int result;
	gConnMutex.lock();
	{
		MAStreamConn* mac = new MAStreamConn(gConnNextHandle, conn);
		gConnections.insert(ConnPair(gConnNextHandle, mac));
		mac->state = CONNOP_CONNECT;
		gThreadPool.execute(new Connect(*mac));
		result = gConnNextHandle++;
	}
	gConnMutex.unlock();
	return result;
}

SYSCALL(void, maConnClose(MAHandle conn)) {
	LOGST("ConnClose %i", conn);
	MAConn& mac = getConn(conn);
	mac.close();	//may take too long
	delete &mac;
	gConnMutex.lock();
	{
		size_t result = gConnections.erase(conn);
		DEBUG_ASSERT(result == 1);
	}
	gConnMutex.unlock();
}

void Base::maAccept(MAHandle conn) {
	LOGST("Accept %i", conn);
	MAConn& mac = getConn(conn);
	MYASSERT(mac.type == eServerConn, ERR_CONN_NOT_SERVER);
#ifdef _WIN32_WCE
	DEBIG_PHAT_ERROR;
#else
	MAServerConn& masc((MAServerConn&)mac);
	MYASSERT((mac.state & CONNOP_ACCEPT) == 0, ERR_CONN_ALREADY_ACCEPTING);
	mac.state |= CONNOP_ACCEPT;
	gThreadPool.execute(new Accept(masc));
#endif	//_WIN32_WCE
}

SYSCALL(int, maConnGetAddr(MAHandle conn, MAConnAddr* addr)) {
	LOGST("ConnGetAddr %i", conn);
	if(conn == HANDLE_LOCAL) {
		if(addr->family == CONN_FAMILY_BT) {
			return Bluetooth::getLocalAddress(addr->bt.addr);
		}
		return CONNERR_INTERNAL;
	}
	MAConn& mac = getConn(conn);
	return mac.clo->getAddr(*addr);
}

SYSCALL(void, maConnRead(MAHandle conn, void* dst, int size)) {
	LOGST("ConnRead %i %i", conn, size);
	SYSCALL_THIS->ValidateMemRange(dst, size);
	MAStreamConn& mac = getStreamConn(conn);
	MYASSERT((mac.state & CONNOP_READ) == 0, ERR_CONN_ALREADY_READING);
	mac.state |= CONNOP_READ;
	gThreadPool.execute(new ConnRead(mac, dst, size));
}

SYSCALL(void, maConnWrite(MAHandle conn, const void* src, int size)) {
	LOGST("ConnWrite %i %i", conn, size);
	SYSCALL_THIS->ValidateMemRange(src, size);
	MAStreamConn& mac = getStreamConn(conn);
	MYASSERT((mac.state & CONNOP_WRITE) == 0, ERR_CONN_ALREADY_WRITING);
	mac.state |= CONNOP_WRITE;
	gThreadPool.execute(new ConnWrite(mac, src, size));
}

SYSCALL(void, maConnReadToData(MAHandle conn, MAHandle data, int offset, int size)) {
	LOGST("ConnReadToData %i %i %i %i", conn, data, offset, size);
	MYASSERT(offset >= 0, ERR_DATA_OOB);
	MYASSERT(size > 0, ERR_DATA_OOB);
	MYASSERT(offset + size > 0, ERR_DATA_OOB);

	MAStreamConn& mac = getStreamConn(conn);
	MYASSERT((mac.state & CONNOP_READ) == 0, ERR_CONN_ALREADY_READING);

	Stream& stream = *SYSCALL_THIS->resources.extract_RT_BINARY(data);
	MYASSERT(stream.ptr() != NULL, ERR_DATA_READ_ONLY);
	{
		int sLength;
		MYASSERT(stream.length(sLength), ERR_DATA_OOB);
		MYASSERT(sLength >= offset + size, ERR_DATA_OOB);
		ROOM(SYSCALL_THIS->resources.add_RT_FLUX(data, (void*)sLength));
	}

	mac.state |= CONNOP_READ;
	gThreadPool.execute(new ConnReadToData(mac, (MemStream&)stream, data, offset, size));
}

SYSCALL(void, maConnWriteFromData(MAHandle conn, MAHandle data, int offset, int size)) {
	LOGST("ConnWriteFromData %i %i %i %i", conn, data, offset, size);
	MYASSERT(offset >= 0, ERR_DATA_OOB);
	MYASSERT(size > 0, ERR_DATA_OOB);
	MYASSERT(offset + size > 0, ERR_DATA_OOB);

	MAStreamConn& mac = getStreamConn(conn);
	MYASSERT((mac.state & CONNOP_WRITE) == 0, ERR_CONN_ALREADY_WRITING);

	Stream& stream = *SYSCALL_THIS->resources.extract_RT_BINARY(data);
	{
		int sLength;
		MYASSERT(stream.length(sLength), ERR_DATA_OOB);
		MYASSERT(sLength >= offset + size, ERR_DATA_OOB);
		ROOM(SYSCALL_THIS->resources.add_RT_FLUX(data, (void*)sLength));
	}

	mac.state |= CONNOP_WRITE;
	gThreadPool.execute(new ConnWriteFromData(mac, stream, data, offset, size));
}

SYSCALL(MAHandle, maHttpCreate(const char* url, int method)) {
	LOGST("Connect %i %s", gConnNextHandle, url);
	if(gConnections.size() >= CONN_MAX)
		return CONNERR_MAX;
	HttpConnection* conn;
	if(sstrcmp(url, http_string) == 0) {
		TLTZ_PASS(httpCreateConnection(url, conn, method));
	} else {
		return CONNERR_URL;
	}
	MAConn* mac = new MAStreamConn(gConnNextHandle, conn);
	gConnections.insert(ConnPair(gConnNextHandle, mac));
	return gConnNextHandle++;
}

SYSCALL(void, maHttpSetRequestHeader(MAHandle conn, const char* key, const char* value)) {
	LOGS("HttpSetRequestHeader %i %s: %s\n", conn, key, value);
	MAStreamConn& mac = getStreamConn(conn);
	HttpConnection* http = mac.conn->http();
	MYASSERT(http != NULL, ERR_CONN_NOT_HTTP);
	MYASSERT(http->mState == HttpConnection::SETUP, ERR_HTTP_NOT_SETUP);
	http->SetRequestHeader(key, value);
}

SYSCALL(int, maHttpGetResponseHeader(MAHandle conn, const char* key, char* buffer, int bufSize)) {
	SYSCALL_THIS->ValidateMemRange(buffer, bufSize);
	MAStreamConn& mac = getStreamConn(conn);
	HttpConnection* http = mac.conn->http();
	MYASSERT(http != NULL, ERR_CONN_NOT_HTTP);
	MYASSERT(http->mState == HttpConnection::FINISHED, ERR_HTTP_NOT_FINISHED);

	const std::string* valueP = http->GetResponseHeader(key);
	if(valueP == NULL)
		return CONNERR_NOHEADER;

	if(bufSize > (int)valueP->length()) {
		memcpy(buffer, valueP->c_str(), valueP->length() + 1);
	}

	return valueP->length();
}

SYSCALL(void, maHttpFinish(MAHandle conn)) {
	MAStreamConn& mac = getStreamConn(conn);
	MYASSERT(mac.state == 0, ERR_CONN_ACTIVE);
	HttpConnection* http = mac.conn->http();
	MYASSERT(http != NULL, ERR_CONN_NOT_HTTP);
	MYASSERT(http->mState == HttpConnection::SETUP || http->mState == HttpConnection::WRITING,
		ERR_HTTP_ALREADY_FINISHED);
	mac.state = CONNOP_FINISH;
	http->mState = HttpConnection::FINISHING;
	gThreadPool.execute(new HttpFinish(mac, *http));
}
