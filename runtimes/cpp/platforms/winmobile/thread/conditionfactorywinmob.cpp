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
 * File:   conditionfactorywinmob.cpp
 * Author: Ali Mosavian
 *
 * Created on July 18, 2009
 */
#include "config_platform.h"
#include "conditionfactorywinmob.hpp"
#include "thread/lock.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexWinMob ConditionFactoryWinMob::m_mutex;


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
ConditionFactory* ConditionFactoryWinMob::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock    l( &m_mutex );

    if ( m_instance == NULL )
        m_instance = new ConditionFactoryWinMob( );

    return m_instance;
}



