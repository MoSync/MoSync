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

#ifndef NETIMPL_H
#define NETIMPL_H

#include <SDL/SDL.h>

#include "net/net.h"

class MoSyncMutex {
public:
	MoSyncMutex();
	~MoSyncMutex();
	void init();
	void lock();
	void unlock();
	void close();
private:
	SDL_mutex* mMutex;
};

typedef unsigned short Uint16;
typedef struct ssl_st SSL;

class SslConnection : public TcpConnection {
public:
	SslConnection(const std::string& hostname, u16 port)
		: TcpConnection(hostname, port), mState(eIdle) {}
	virtual ~SslConnection();
	virtual int connect();
	virtual int read(void* dst, int max);
	virtual int write(const void* src, int len);
	virtual void close();
private:
	SSL* mSession;
	enum State { eIdle, eInit, eHandshook } mState;
};

#endif	//NETIMPL_H
