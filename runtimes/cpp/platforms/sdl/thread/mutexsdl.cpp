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
