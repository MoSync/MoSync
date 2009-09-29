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
