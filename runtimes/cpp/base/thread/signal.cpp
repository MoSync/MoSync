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


