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
 * File:   mutexfactory.cpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include <cstdlib>
#include "config_platform.h"
#include "mutexfactory.hpp"
#ifdef  __SDL__
#include "../../platforms/sdl/thread/mutexfactorysdl.hpp"
#elif defined(__WINMOBILE__)
#include "../../platforms/winmobile/thread/mutexfactorywinmob.hpp"
#else
//#warning "No platform has been defined!"
#endif

using namespace Base::Thread;


/**
 * Initialize singleton instance field
 */
MutexFactory *MutexFactory::m_instance = NULL;


/**
 * Constructor
 *
 *
 */
MutexFactory::MutexFactory( void )
{

}


/**
 * Destructor
 *
 *
 */
MutexFactory::~MutexFactory( void )
{
    m_instance = NULL;
}


/**
 * Removes a mutex from the live list
 *
 * @param m     The mutex to remove
 *
 */
void MutexFactory::deleteMutex ( Mutex *m )
{
    m_liveMutexList.remove( m );
}


/**
 * Deletes all the mutexes
 *
 */
void MutexFactory::deleteMutexes ( void )
{
    //
    // The destructor of implementation of the mutex should
    // free all the platform dependent resources. The destructor
    // of the base class will remove it from the live list.
    //
    while ( m_liveMutexList.empty( ) == false )
        delete m_liveMutexList.front( );
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
MutexFactory* MutexFactory::getInstance ( void )
{
    if ( m_instance != NULL )
        return m_instance;

    //
    // Call thread safe platform dependent implementation of
    // of getInstance here, such as MutexFactorySDL::getInstance
    // MutexFactoryS60::getInstance etc etc
    //
#ifdef  __SDL__
    return MutexFactorySDL::getInstance( );
#elif defined(__WINMOBILE__)
    return MutexFactoryWinMob::getInstance( );
#else
    return NULL;
#endif
}
