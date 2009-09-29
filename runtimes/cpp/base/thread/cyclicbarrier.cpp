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
 * File:   cyclicbarrier.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "lock.hpp"
#include "cyclicbarrier.hpp"
#include "helpers/helpers.h"


using namespace Base::Thread;


/**
 * Constructor
 *
 * @param l     The barrier limit before all the waiting
 *              threads are released.
 *
 * @throws AllocationFailedException if the barrier fails to
 *         create its resources.
 */
CyclicBarrier::CyclicBarrier ( int l )
{
    m_count         = 0;
    m_barrierLimit  = l;
    m_mutex         = MutexFactory::getInstance( )->createMutex( );
    m_cond          = ConditionFactory::getInstance( )->createCondition( );

    MAASSERT ( m_mutex != NULL );
    MAASSERT ( m_cond  != NULL );
}

/**
 * Destructor
 *
 */
 CyclicBarrier::~CyclicBarrier ( void )
{
    delete m_mutex;
    delete m_cond;
}


/**
 * Waits for all the threads to reach the barrier and
 * resets the barrier.
 *
 */
void CyclicBarrier::wait ( void )
{
    Lock    l( m_mutex );
    
    m_count++;
    if ( m_count < m_barrierLimit )
        m_cond->wait( l );
    else
    {
        m_count = 0;
        m_cond->notifyAll( );
    }
}

