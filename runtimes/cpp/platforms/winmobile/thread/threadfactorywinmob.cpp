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
 * File:   threadfactorywinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 20, 2009
 */
#include "config_platform.h"
#include "threadfactorywinmob.hpp"
#include "thread/lock.hpp"
#include "thread/thread.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexWinMob ThreadFactoryWinMob::m_mutex;

/**
 * Default constructor
 *
 */
ThreadFactoryWinMob::ThreadFactoryWinMob ( void )
{

}


/**
 * Destructor
 *
 */
ThreadFactoryWinMob::~ThreadFactoryWinMob ( void )
{
    deleteThreads( );
}

/**
 * Creates a platform independent thread
 *
 * @return Pointer to thread if allocation was succesful,
 *         NULL otherwise.
 *
 * @internal Do not forget to add the newly created mutex
 *           to the live list before returning.
 */
Thread* ThreadFactoryWinMob::createThread ( void )
{
    ThreadWinMob *t = new ThreadWinMob( );

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
ThreadFactory* ThreadFactoryWinMob::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock lck( m_mutex );

    if ( m_instance == NULL )
        m_instance = new ThreadFactoryWinMob( );

    return m_instance;
}
