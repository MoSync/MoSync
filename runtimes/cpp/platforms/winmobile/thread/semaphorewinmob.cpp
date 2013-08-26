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
 * File:   semaphorewinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "semaphorewinmob.hpp"
#include "allocationfailedexception.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Constructor
 *
 * @param v     Initial semaphore valure
 *
 * @throws AllocationFailedException if creation of the
 *         semaphore fails.
 */
SemaphoreWinMob::SemaphoreWinMob ( int v, SemaphoreFactory *i )
: Semaphore( i )
{
    m_sem = CreateSemaphore( NULL, v, MAXVALUE, NULL );
    /* Exceptions not allowed on Symbian
    if ( m_sem == NULL )
        throw AllocationFailedException( "CreateSemaphore" );
    */
}


/**
 * Destructor
 *
 */
SemaphoreWinMob::~SemaphoreWinMob ( void )
{
    if ( m_sem != NULL )
        CloseHandle( m_sem );
}

/**
 * Wait for a "signal", if there's one available, it will
 * return immediatly, if not it will block until some other
 * thread performs a notify.
 *
 */
void SemaphoreWinMob::wait ( void )
{
    MAASSERT( WaitForSingleObject( m_sem, INFINITE ) != WAIT_FAILED );
}

/**
 * Posts a notification
 *
 */
void SemaphoreWinMob::post ( void )
{
    ReleaseSemaphore( m_sem, 1, NULL );
}

/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool SemaphoreWinMob::isValid ( void )
{
    return m_sem != NULL;
}
