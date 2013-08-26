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
 * File:   threadsdl.cpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */
#include <SDL/SDL.h>
#include "threadsdl.hpp"
#include "allocationfailedexception.hpp"

using namespace Base::Thread;

/**
 * Constructor, creates an SDL Thread
 *
 *
 * @throws AllocationFailedException if creation fails
 */
ThreadSDL::ThreadSDL ( ThreadFactory *i )
: Thread( i ),
  m_barrier( 2 )
{
    m_thread = SDL_CreateThread( ThreadSDL::funct, this );
    /* Exceptions not allowed on Symbian
    if ( m_thread == NULL )
        throw AllocationFailedException( "SDL_CreateThread" );
    */
}

/**
 * Destructor
 * Frees platform dependent resources.
 *
 */
ThreadSDL::~ThreadSDL ( void )
{

}

/**
 * Starts the thread, virtual
 *
 */
void ThreadSDL::join ( void )
{
    int status;
    SDL_WaitThread( m_thread, &status );
}

/**
 * Starts the thread
 *
 * @param f     Pointer to the functor to invoke in
 *              the thread.
 */
void ThreadSDL::start ( Functor *f )
{
    m_functor = f;
    m_barrier.wait( );
}

/**
 * Puts the thread to sleep
 *
 * @param ms    The number of miliseconds to put the
 *              thread to sleep.
 */
void Thread::sleep ( unsigned int ms )
{
    SDL_Delay( (Uint32) ms );
}


/**
 * Thread stub
 *
 * @param i     "this" pointer to current instance
 */
int ThreadSDL::funct ( void *i )
{
     ThreadSDL *o = static_cast<ThreadSDL *>( i );
     
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
bool ThreadSDL::isValid ( void )
{
    return m_thread != NULL;
}
