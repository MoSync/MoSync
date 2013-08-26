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
