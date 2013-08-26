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
 * File:   threadwinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */
#include "config_platform.h"
#include "threadwinmob.hpp"
#include "allocationfailedexception.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Constructor, creates an SDL Thread
 *
 * @throws AllocationFailedException if creation fails
 */
ThreadWinMob::ThreadWinMob ( ThreadFactory *i )
: Thread( i ),
  m_barrier( 2 )
{
    m_thread =  CreateThread( NULL,
                              0,
                              ThreadWinMob::funct,
                              (LPVOID)this,
                              0,
                              NULL );
    /* Exceptions not allowed on Symbian
    if ( m_thread == NULL )
        throw AllocationFailedException( "CreateThread" );
    */
}

/**
 * Destructor
 * Frees platform dependent resources.
 *
 */
ThreadWinMob::~ThreadWinMob ( void )
{
    if ( m_thread != NULL )
        CloseHandle( m_thread );
}

/**
 * Starts the thread
 *
 * @param f     Pointer to the functor to invoke in
 *              the thread.
 */
void ThreadWinMob::start ( Functor *f )
{
    m_functor = f;    
    m_barrier.wait( );
}

/**
 * Starts the thread, virtual
 *
 */
void ThreadWinMob::join ( void )
{
    DWORD status;

    MAASSERT( WaitForSingleObject( m_thread, INFINITE ) != WAIT_FAILED );
    MAASSERT( GetExitCodeThread( m_thread, &status ) != 0 );
}

/**
 * Puts the thread to sleep
 *
 * @param ms    The number of miliseconds to put the
 *              thread to sleep.
 */
 void Thread::sleep ( unsigned int ms )
{
    Sleep( (DWORD) ms );
}


/**
 * Thread stub
 *
 * @param i     "this" pointer to current instance
 */
DWORD WINAPI ThreadWinMob::funct ( LPVOID i )
{
     ThreadWinMob *o = static_cast<ThreadWinMob *>( i );

     // Wait for start signal
     o->m_barrier.wait( );

     // Invoke functor
     if ( o->m_functor != NULL )
         (*(o->m_functor))( );

     return 0;
}


/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool ThreadWinMob::isValid ( void )
{
    return m_thread != NULL;
}