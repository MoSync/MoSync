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
 * File:   mutexwinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "mutexwinmob.hpp"
#include "helpers/helpers.h"


using namespace Base::Thread;


/**
 * Constructor, creates a Windows mobile mutex
 *
 */
MutexWinMob::MutexWinMob ( MutexFactory *i )
: Mutex( i )
{
    m_mutex = CreateMutex( NULL, FALSE, NULL );
    /* Exceptions are not allows on symbian
    if ( m_mutex == NULL )
        throw AllocationFailedException( "CreateMutex" );
    */
}

/**
 * Destructor
 * Frees platform dependent resources.
 *
 */
MutexWinMob::~MutexWinMob ( void )
{
    if ( m_mutex != NULL )
        CloseHandle( m_mutex );
    
}

/**
 * Locks the mutex
 *
 */
void MutexWinMob::lock ( void )
{
    MAASSERT( WaitForSingleObject( m_mutex, INFINITE ) != WAIT_FAILED );
}

/**
 * Unlocks the mutex
 *
 */
void MutexWinMob::unlock ( void )
{
    ReleaseMutex( m_mutex );
}

/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool MutexWinMob::isValid ( void )
{
    return m_mutex != NULL;
}