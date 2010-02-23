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

#include <pthread.h>

class MoSyncMutex {
public:
	MoSyncMutex();
	~MoSyncMutex();
	void init();
	void lock();
	void unlock();
	void close();
private:
	pthread_mutex_t mMutex;
	pthread_mutexattr_t mMutexAttr;
	
};

typedef int MoSyncSocket;
typedef unsigned short Uint16;

#endif /* _NET_IMPL_H_ */
