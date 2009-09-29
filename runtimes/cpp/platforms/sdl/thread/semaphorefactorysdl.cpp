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
 * File:   semaphorefactorysdl.cpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include "config_platform.h"
#include "semaphoresdl.hpp"
#include "semaphorefactorysdl.hpp"
#include "thread/lock.hpp"
#include "thread/semaphore.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexSDL SemaphoreFactorySDL::m_mutex;


/**
 * Default constructor
 *
 */
SemaphoreFactorySDL::SemaphoreFactorySDL ( void )
{
}


/**
 * Destructor
 *
 */
SemaphoreFactorySDL::~SemaphoreFactorySDL ( void )
{
	SemaphoreFactory::deleteSemaphores( );
}


/**
 * Creates a platform independent Semaphore
 *
 * @param v     Initial value (defualt is 0)
 *
 * @return Pointer to semaphore if allocation was succesfull,
 *         NULL otherwise.
 * 
 * @internal Do not forget to add the newly created mutex
 *           to the live list before returning.
 */
Semaphore * SemaphoreFactorySDL::createSemaphore ( int v )
{
    Lock l( m_mutex );
    SemaphoreSDL* s = new SemaphoreSDL( v, this );

    if ( s->isValid( ) == false )
    {
        delete s;
        return NULL;
    }

    m_liveSemaphoreList.push_back( s );
    return s;
}


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
SemaphoreFactory * SemaphoreFactorySDL::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock    l( m_mutex );

    if ( m_instance == NULL )
        m_instance = new SemaphoreFactorySDL( );

    return m_instance;
}


