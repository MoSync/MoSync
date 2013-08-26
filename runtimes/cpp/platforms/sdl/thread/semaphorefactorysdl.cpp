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
 * File:   semaphorefactorysdl.cpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include "config_platform.h"
#include "semaphoresdl.hpp"
#include "semaphorefactorysdl.hpp"
#include "thread/lock.hpp"
#include "thread/semaphore.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexSDL SemaphoreFactorySDL::m_mutex;


/**
 * Default constructor
 *
 */
SemaphoreFactorySDL::SemaphoreFactorySDL ( void )
{
}


/**
 * Destructor
 *
 */
SemaphoreFactorySDL::~SemaphoreFactorySDL ( void )
{
	SemaphoreFactory::deleteSemaphores( );
}


/**
 * Creates a platform independent Semaphore
 *
 * @param v     Initial value (defualt is 0)
 *
 * @return Pointer to semaphore if allocation was succesfull,
 *         NULL otherwise.
 * 
 * @internal Do not forget to add the newly created mutex
 *           to the live list before returning.
 */
Semaphore * SemaphoreFactorySDL::createSemaphore ( int v )
{
    Lock l( m_mutex );
    SemaphoreSDL* s = new SemaphoreSDL( v, this );

    if ( s->isValid( ) == false )
    {
        delete s;
        return NULL;
    }

    m_liveSemaphoreList.push_back( s );
    return s;
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
SemaphoreFactory * SemaphoreFactorySDL::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock    l( m_mutex );

    if ( m_instance == NULL )
        m_instance = new SemaphoreFactorySDL( );

    return m_instance;
}


