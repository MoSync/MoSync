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

