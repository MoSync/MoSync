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
 * File:   semaphoresdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include "config_platform.h"
#include <SDL/SDL_thread.h>
#include "semaphoresdl.hpp"
#include <allocationfailedexception.hpp>
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
SemaphoreSDL::SemaphoreSDL ( int v, SemaphoreFactory *i )
: Semaphore( i )
{
    m_sem = SDL_CreateSemaphore( v );

    /* Exceptions not allowed on symbian
    if ( m_sem == NULL )
        throw AllocationFailedException( "SDL_CreateSemaphore" );
    */
}


/**
 * Destructor
 *
 */
SemaphoreSDL::~SemaphoreSDL ( void )
{
    if ( m_sem != NULL )
        SDL_DestroySemaphore( m_sem );
}

/**
 * Wait for a "signal", if there's one available, it will
 * return immediatly, if not it will block until some other
 * thread performs a notify.
 *
 */
void SemaphoreSDL::wait ( void )
{
    SDL_SemWait( m_sem );
}

/**
 * Posts a notification
 *
 */
void SemaphoreSDL::post ( void )
{
    SDL_SemPost( m_sem );
}

/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool SemaphoreSDL::isValid ( void )
{
    return m_sem != NULL;
}
