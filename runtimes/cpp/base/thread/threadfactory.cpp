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
