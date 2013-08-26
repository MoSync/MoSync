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

#ifndef _NET_IMPL_H_
#define _NET_IMPL_H_

#include <net/net.h>

#include <windows.h>
#include <winsock.h>

#define WM_ADD_EVENT (WM_USER+1)
#define WM_DEFLUX_BINARY (WM_USER+2)

class MoSyncMutex {
public:
	MoSyncMutex();
	~MoSyncMutex();
	void init();
	void lock();
	void unlock();
	void close();
private:
	HANDLE mMutex;
};

extern bool winSockInitialized;

typedef SOCKET MoSyncSocket;
typedef unsigned short Uint16;

class SslConnection : public TcpConnection {
public:
	SslConnection(const std::string& hostname, u16 port)
		: TcpConnection(hostname, port) {}
	virtual ~SslConnection();
	virtual int connect();
	const char* hostname() { return mHostname.c_str(); }

	int mSslError;
};

#endif /* _NET_IMPL_H_ */
