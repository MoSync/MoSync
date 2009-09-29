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
 * File:   mutexsdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include "config_platform.h"
#include "mutexsdl.hpp"
#include "helpers/helpers.h"


using namespace Base::Thread;


/**
 * Constructor, creates a SDL mutex
 *
 */
MutexSDL::MutexSDL ( MutexFactory *i )
: Mutex( i )
{
    m_mutex = SDL_CreateMutex( );

    /* Exceptions not supported on Symbian.
    if ( m_mutex == NULL )
        throw AllocationFailedException( "SDL_CreateMutex" );
    */
}

/**
 * Destructor
 * Frees platform dependent resources.
 *
 */
MutexSDL::~MutexSDL ( void )
{
    if ( m_mutex != NULL )
        SDL_DestroyMutex( m_mutex );
}

/**
 * Locks the mutex
 *
 */
void MutexSDL::lock ( void )
{
    SDL_mutexV( m_mutex );
}

/**
 * Unlocks the mutex
 *
 */
void MutexSDL::unlock ( void )
{
    SDL_mutexP( m_mutex );
}

/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool MutexSDL::isValid ( void )
{
    return m_mutex != NULL;
}
