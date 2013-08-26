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
