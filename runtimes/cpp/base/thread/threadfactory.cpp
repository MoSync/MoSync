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
 * File:   threadfactory.cpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */
#include <cstdlib>
#include "config_platform.h"
#include "threadfactory.hpp"
#ifdef  __SDL__
#include "../../platforms/sdl/thread/threadfactorysdl.hpp"
#elif defined( __WINMOBILE__ )
#include "../../platforms/winmobile/thread/threadfactorywinmob.hpp"
#else
//#warning "No platform has been defined!"
#endif

using namespace Base::Thread;


/**
 * Initialize singleton instance field
 */
ThreadFactory *ThreadFactory::m_instance = NULL;


/**
 * Constructor
 *
 *
 */
ThreadFactory::ThreadFactory( void )
{

}


/**
 * Destructor
 *
 *
 */
ThreadFactory::~ThreadFactory( void )
{
    m_instance = NULL;
}


/**
 * Removes a thread from the live list
 *
 * @param t     The thread to remove
 *
 */
void ThreadFactory::removeThread ( Thread *t )
{
    m_liveThreadList.remove( t );
}


/**
 * Deletes all the threads
 *
 */
void ThreadFactory::deleteThreads ( void )
{
    //
    // The destructor of implementation of the thread should
    // free all the platform dependent resources. The destructor
    // of the base class will remove it from the live list.
    //
    while ( m_liveThreadList.empty( ) == false )
        delete m_liveThreadList.front( );
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
ThreadFactory* ThreadFactory::getInstance ( void )
{
    if ( m_instance != NULL )
        return m_instance;

    //
    // Call thread safe platform dependent implementation of
    // of getInstance here, such as ThreadFactorySDL::getInstance
    // ThreadFactoryS60::getInstance etc etc
    //
#ifdef  __SDL__
    return ThreadFactorySDL::getInstance( );
#elif defined( __WINMOBILE__ )
    return ThreadFactoryWinMob::getInstance( );
#else
    return NULL;
#endif
}
