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
