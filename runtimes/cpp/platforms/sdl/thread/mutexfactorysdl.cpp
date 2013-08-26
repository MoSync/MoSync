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
 * File:   mutexfactorysdl.cpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include "config_platform.h"
#include "mutexsdl.hpp"
#include "mutexfactorysdl.hpp"
#include "thread/lock.hpp"
#include "thread/mutex.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexSDL MutexFactorySDL::m_mutex;


/**
 * Default constructor
 *
 */
MutexFactorySDL::MutexFactorySDL ( void )
{

}


/**
 * Destructor
 *
 */
MutexFactorySDL::~MutexFactorySDL ( void )
{
    MutexFactory::deleteMutexes( );
}


/**
 * Creates a platform independent mutex
 *
 * @return Pointer to mutex if allocation was succesfull,
 *         NULL otherwise.
 *
 * @internal Do not forget to add the newly created mutex
 *           to the live list before returning.
 */
Mutex * MutexFactorySDL::createMutex ( void )
{
    Lock l( m_mutex );
    MutexSDL* m = new MutexSDL( this );

    if ( m->isValid( ) == false )
    {
        delete m;
        return NULL;
    }

    m_liveMutexList.push_back( m );
    return m;
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
MutexFactory * MutexFactorySDL::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock    l( m_mutex );

    if ( m_instance == NULL )
        m_instance = new MutexFactorySDL( );

    return m_instance;
}

