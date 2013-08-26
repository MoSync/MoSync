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
 * File:   signal.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "lock.hpp"
#include "signal.hpp"
#include "mutexfactory.hpp"
#include "conditionfactory.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;


/**
 * Constructor
 *
 * @throws AllocationFailedException if the instance fails to
 *         create its resources.
 */
Signal::Signal ( void )
{
    m_mutex = MutexFactory::getInstance( )->createMutex( );
    m_cond  = ConditionFactory::getInstance( )->createCondition( );
    m_hasBeenSignaled = false;

    MAASSERT ( m_mutex != NULL );
    MAASSERT ( m_cond  != NULL );
}

/**
 * Destructor, will free all the allocated resources.
 *
 */
Signal::~Signal ( void )
{
    delete m_mutex;
    delete m_cond;
}

/**
 * Sends a signal to all the waiting threads.
 *
 */
void Signal::signal ( void )
{
    Lock l( m_mutex );
    m_hasBeenSignaled = true;
    m_cond->notifyAll( );
}

/**
 * Returns immediately if the a signal has been sent previously,
 * otherwise it blocks for until a signal is received.
 *
 */
void Signal::wait ( void )
{
    Lock l( m_mutex );
    
    if ( m_hasBeenSignaled == true )
        return;

    m_cond->wait( l );
}

/**
 * Checks if a signal has been received without blocking
 *
 * @return true if signal has been received.
 */
bool Signal::hasBeenSignaled ( void )
{
    return m_hasBeenSignaled;
}

/**
 * Resets the signal, if there is one.
 *
 */
void Signal::reset ( void )
{
    Lock l( m_mutex );
    m_hasBeenSignaled = false;
}


