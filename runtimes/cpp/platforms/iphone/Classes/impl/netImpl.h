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
/*
 Authors: Niklas Nummelin, Romain Chalant
 */

#ifndef _NET_IMPL_H_
#define _NET_IMPL_H_

#include <pthread.h>
#include "MemStream.h"
#include <net/net.h>

class MoSyncMutex {
public:
	MoSyncMutex();
	~MoSyncMutex();
	void init();
	void lock();
	void unlock();
	void close();
private:
	bool mInitialized;
	pthread_mutex_t mMutex;
	pthread_mutexattr_t mMutexAttr;
	
};

struct InternalEventDefluxBin {
	MAHandle handle;
	Base::Stream* stream;
};

typedef int MoSyncSocket;
typedef unsigned short Uint16;

//***************************************************************************
// Declaration of SslConnection
//***************************************************************************

/**
 * This class is an implementation of
 * an ssl socket.
 * Check TcpConnection for more info.
 */
class SslConnection : public TcpConnection {
public:
	/**
	 * Constructor.
	 * Calls the constructor of the parent class.
	 *
	 * @param hostname	Name of the host to connect to.
	 * @param port		Port to connect to.
	 */
	SslConnection(const std::string& hostname, u16 port)
	: TcpConnection(hostname, port) {}
	
	/**
	 * Destructor.
	 */
	virtual ~SslConnection();
	
	/**
	 * Establishes a connection to a defined host
	 * and port. See Constructor.
	 * Note: The highest level security protocol that can be negotiated
	 *		 is set as the security protocol.
	 *
	 * @return	1 If the connection could be established.
	 *			CONNERR_SSL if there was an error.
	 */
	virtual int connect();
	
	/**
	 * Reads up to a given number of bytes into a given buffer.
	 *
	 * @param dst	Pointer to the destination buffer
	 * @param max	Maximum number of bytes to read
	 * @return		A positive number indicates the number of bytes read;
	 *				0 indicates that the end of the buffer was reached;
	 *				CONNERR_SSL means that the operation failed.
	 */
	virtual int read(void* dst, int max);
	
	/**
	 * Writes the contents of a provided data buffer to the receiver.
	 *
	 * @param src	Pointer to the buffer to write
	 * @param len	Number of bytes to write
	 * @return		The number of bytes actually written, or CONNERR_SSL
	 *				if an error occurs.
	 *				If the receiver has reached its capacity, 0 is returned.
	 */
	virtual int write(const void* src, int len);
	
	/**
	 * Closes the connection.
	 */
	virtual void close();
private:
	NSInputStream *mInputStream;
	NSOutputStream *mOutputStream;
};

#endif /* _NET_IMPL_H_ */
