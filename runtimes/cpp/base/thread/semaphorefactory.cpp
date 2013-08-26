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
 * File:   semaphorefactory.cpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include <cstdlib>
#include "config_platform.h"
#include "semaphorefactory.hpp"
#ifdef  __SDL__
#include "../../platforms/sdl/thread/semaphorefactorysdl.hpp"
#elif defined( __WINMOBILE__ )
#include "../../platforms/winmobile/thread/semaphorefactorywinmob.hpp"
#else
    //#warning "No platform has been defined!"
#endif


using namespace Base::Thread;


/**
 * Initialize singleton instance field
 */
SemaphoreFactory *SemaphoreFactory::m_instance = NULL;


/**
 * Constructor
 *
 *
 */
SemaphoreFactory::SemaphoreFactory ( void )
{

}


/**
 * Destructor
 *
 *
 */
SemaphoreFactory::~SemaphoreFactory ( void )
{
    m_instance = NULL;
}

/**
 * Removes a semaphore from the live list
 *
 * @param m     The mutex to remove
 *
 */
void SemaphoreFactory::deleteSemaphore ( Semaphore *m )
{
    m_liveSemaphoreList.remove( m );
}

/**
 * Deletes all the mutexes
 *
 */
void SemaphoreFactory::deleteSemaphores ( void )
{
    //
    // The destructor of implementation of the mutex should
    // free all the platform dependent resources. The destructor
    // of the base class will remove it from the live list.
    //
    while ( m_liveSemaphoreList.empty( ) == false )
        delete m_liveSemaphoreList.front( );
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
SemaphoreFactory * SemaphoreFactory::getInstance ( void )
{
    if ( m_instance != NULL )
        return m_instance;

    //
    // Call thread safe platform dependent implementation of
    // of getInstance here, such as MutexFactorySDL::getInstance
    // MutexFactoryS60::getInstance etc etc
    //
#ifdef  __SDL__
    return SemaphoreFactorySDL::getInstance( );
#elif defined( __WINMOBILE__ )
    return SemaphoreFactoryWinMob::getInstance( );
#else
    return NULL;
#endif
}
