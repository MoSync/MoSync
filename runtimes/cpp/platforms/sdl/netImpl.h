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
