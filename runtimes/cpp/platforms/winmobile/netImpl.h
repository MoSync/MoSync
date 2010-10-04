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
	virtual int read(void* dst, int max);
	virtual int write(const void* src, int len);
	virtual void close();
};

#endif /* _NET_IMPL_H_ */
