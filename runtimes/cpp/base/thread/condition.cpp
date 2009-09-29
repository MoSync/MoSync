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
 * File:   condition.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "condition.hpp"
#include "mutexfactory.hpp"
#include "semaphorefactory.hpp"
#include "conditionfactory.hpp"
#include "lock.hpp"
#include "relocker.hpp"

using namespace Base::Thread;


/**
 * Constructor
 *
 * @throws AllocationFailedException if the allocation of internal
 *         resources failes.
 */
Condition::Condition ( void )
{
    m_threadsWaiting = 0;

    m_mutex     = MutexFactory::getInstance( )->createMutex( );
    m_semaphore = SemaphoreFactory::getInstance( )->createSemaphore( );
}


/**
 * Destructor
 *
 */
Condition::~Condition ( void )
{
    if ( m_mutex != NULL )
        delete m_mutex;
    if ( m_semaphore )
        delete m_semaphore;

	ConditionFactory::getInstance( )->removeCondition( this );
}


/**
 * Waits for a notification
 *
 * @param l     A lock which is used as a guard for
 *              the shared resource. It will be unlocked
 *              while waiting for a notification and
 *              relocked before returning.
 */
void Condition::wait ( Lock &l )
{
    Lock lck( m_mutex );
    ReLocker relck( l );

    m_threadsWaiting++;
    {
        // Release lock while waiting and reaccuire it
        // once we're done.
        ReLocker waitLock( lck );
        m_semaphore->wait( );
    }

}

/**
 * Notifies one waiting thread
 *
 */
void Condition::notify ( void )
{
    // Operation needs to be atomic
    Lock lck( m_mutex );

    if ( m_threadsWaiting < 1 )
        return;

    m_threadsWaiting--;
    m_semaphore->post( );
}


/**
 * Notifies all waiting threads
 *
 */
void Condition::notifyAll ( void )
{
    // Operation needs to be atomic
    Lock lck( m_mutex );

    if ( m_threadsWaiting < 1 )
        return;

    // Notify all the threads
    int count = m_threadsWaiting;
    m_threadsWaiting = 0;
    for ( ; count > 0; count-- )
        m_semaphore->post( );
}

/**
 * Checks if the condition variable was created, for internal use.
 *
 * @return true if this is a valid condition variable.
 */
bool Condition::isValid ( void )
{
    return (m_mutex != NULL) && (m_semaphore != NULL );
}
