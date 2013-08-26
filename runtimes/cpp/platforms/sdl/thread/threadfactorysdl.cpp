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
 * File:   threadfactorysdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 20, 2009
 */
#include "config_platform.h"
#include "threadfactorysdl.hpp"
#include "thread/lock.hpp"
#include "thread/thread.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;


/**
 * Initialize static fields
 */
MutexSDL ThreadFactorySDL::m_mutex;


/**
 * Default constructor
 *
 */
ThreadFactorySDL::ThreadFactorySDL ( void )
{
}


/**
 * Destructor
 *
 */
ThreadFactorySDL::~ThreadFactorySDL ( void )
{
    deleteThreads( );
}

/**
 * Creates a platform independent thread
 *
 * @return Pointer to thread if allocation was succesfull,
 *         NULL otherwise.
 *
 * @internal Do not forget to add the newly created mutex
 *           to the live list before returning.
 */
Thread* ThreadFactorySDL::createThread ( void )
{
    ThreadSDL *t = new ThreadSDL( this );
    
    if ( t->isValid( ) == false )
    {
        delete t;
        return NULL;
    }

    m_liveThreadList.push_back( t );
    return t;
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
ThreadFactory* ThreadFactorySDL::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock lck( m_mutex );

    if ( m_instance == NULL )
        m_instance = new ThreadFactorySDL( );

    return m_instance;
}
