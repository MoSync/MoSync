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
 * File:   semaphorefactorywinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "semaphorewinmob.hpp"
#include "semaphorefactorywinmob.hpp"
#include "thread/semaphore.hpp"
#include "thread/lock.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexWinMob SemaphoreFactoryWinMob::m_mutex;


/**
 * Default constructor
 *
 */
SemaphoreFactoryWinMob::SemaphoreFactoryWinMob ( void )
{

}


/**
 * Destructor
 *
 */
SemaphoreFactoryWinMob::~SemaphoreFactoryWinMob ( void )
{
	SemaphoreFactory::deleteSemaphores( );
}


/**
 * Creates a platform independent Semaphore
 *
 * @param v     Initial value (defualt is 0)
 *
 * @return Pointer to semaphore if allocation was succesful,
 *         NULL otherwise.
 *
 * @internal Do not forget to add the newly created mutex
 *           to the live list before returning.
 */
Semaphore * SemaphoreFactoryWinMob::createSemaphore ( int v )
{
    Lock	l( m_mutex );
    SemaphoreWinMob* m = new SemaphoreWinMob( v );

    if ( m->isValid( ) == false )
    {
        delete m;
        return NULL;
    }
    
    m_liveSemaphoreList.push_back( m );
    return m;
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
SemaphoreFactory * SemaphoreFactoryWinMob::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock    l( m_mutex );

    if ( m_instance == NULL )
        m_instance = new SemaphoreFactoryWinMob( );

    return m_instance;
}


