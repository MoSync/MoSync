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
 * File:   mutexwinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include "config_platform.h"
#include "mutexwinmob.hpp"
#include "helpers/helpers.h"


using namespace Base::Thread;


/**
 * Constructor, creates a Windows mobile mutex
 *
 */
MutexWinMob::MutexWinMob ( MutexFactory *i )
: Mutex( i )
{
    m_mutex = CreateMutex( NULL, FALSE, NULL );
    /* Exceptions are not allows on symbian
    if ( m_mutex == NULL )
        throw AllocationFailedException( "CreateMutex" );
    */
}

/**
 * Destructor
 * Frees platform dependent resources.
 *
 */
MutexWinMob::~MutexWinMob ( void )
{
    if ( m_mutex != NULL )
        CloseHandle( m_mutex );
    
}

/**
 * Locks the mutex
 *
 */
void MutexWinMob::lock ( void )
{
    MAASSERT( WaitForSingleObject( m_mutex, INFINITE ) != WAIT_FAILED );
}

/**
 * Unlocks the mutex
 *
 */
void MutexWinMob::unlock ( void )
{
    ReleaseMutex( m_mutex );
}

/**
 * Checks if the OS mutex was created, for internal use.
 *
 * @return true if it the OS mutex was created and this is
 *         a valid mutex.
 */
bool MutexWinMob::isValid ( void )
{
    return m_mutex != NULL;
}