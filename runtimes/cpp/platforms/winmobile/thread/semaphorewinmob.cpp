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
 * File:   semaphorewinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "semaphorewinmob.hpp"
#include "allocationfailedexception.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Constructor
 *
 * @param v     Initial semaphore valure
 *
 * @throws AllocationFailedException if creation of the
 *         semaphore fails.
 */
SemaphoreWinMob::SemaphoreWinMob ( int v, SemaphoreFactory *i )
: Semaphore( i )
{
    m_sem = CreateSemaphore( NULL, v, MAXVALUE, NULL );
    /* Exceptions not allowed on Symbian
    if ( m_sem == NULL )
        throw AllocationFailedException( "CreateSemaphore" );
    */
}


/**
 * Destructor
 *
 */
SemaphoreWinMob::~SemaphoreWinMob ( void )
{
    if ( m_sem != NULL )
        CloseHandle( m_sem );
}

/**
 * Wait for a "signal", if there's one available, it will
 * return immediatly, if not it will block until some other
 * thread performs a notify.
 *
 */
void SemaphoreWinMob::wait ( void )
{
    MAASSERT( WaitForSingleObject( m_sem, INFINITE ) != WAIT_FAILED );
}

/**
 * Posts a notification
 *
 */
void SemaphoreWinMob::post ( void )
{
    ReleaseSemaphore( m_sem, 1, NULL );
}

/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool SemaphoreWinMob::isValid ( void )
{
    return m_sem != NULL;
}
