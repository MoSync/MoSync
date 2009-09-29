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
