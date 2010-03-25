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
 * File:   lock.hpp
 * Author: Ali Mosavian
 *
 * Created on July 13, 2009
 */

#ifndef __LOCK_HPP__
#define	__LOCK_HPP__

#include "mutex.hpp"

/**
 * Platform independent lock class for mutexes.
 *
 */
class Lock
{
private:
    Mutex *     mMutex;
    bool        mLocked;

public:
    /**
     * Constructor, locks the mutex
     *
     * @param m     The mutex to lock
     */
    Lock ( Mutex *m )
    : mMutex( m )
    {
        mMutex->lock( );
    }

    /**
     * Constructor, locks the mutex
     *
     * @param m     The mutex to lock
     */
    Lock ( Mutex &m )
    : mMutex( &m )
    {
        mMutex->lock( );
    }

    /**
     * Destructor, unlocks mutex
     *
     */
    ~Lock ( void )
    {
        mMutex->unlock( );
    }
};


#endif	/* __LOCK_HPP__ */

