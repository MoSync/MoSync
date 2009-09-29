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
 * File:   conditionfactory.cpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */
#include <cstdlib>
#include "config_platform.h"
#include "conditionfactory.hpp"



using namespace Base::Thread;

/**
 * Initialize singleton instance field
 */
ConditionFactory * ConditionFactory::m_instance = NULL;


/**
 * Constructor
 *
 */
ConditionFactory::ConditionFactory ( void )
{

}


/**
 * Destructor
 *
 */
ConditionFactory::~ConditionFactory ( void )
{
    deleteConditions( );
    m_instance = NULL;
}


/**
 * Removes a condition variable from the live list
 *
 * @param c     The condition to remove
 *
 */
void ConditionFactory::removeCondition ( Condition *c )
{
    m_liveCondList.remove( c );
}


/**
 * Deletes all the condition variables, called from
 * the factory destructor.
 *
 */
void ConditionFactory::deleteConditions ( void )
{
    //
    // The destructor of implementation of the condition should
    // free all the platform dependent resources. The destructor
    // of the base class will remove it from the live list.
    //
    while ( m_liveCondList.empty( ) == false )
        delete m_liveCondList.front( );
}

/**
 * Creates a platform independent condition variable
 *
 * @return Pointer to mutex if allocation was succesfull,
 *         NULL otherwise.
 *
 * @internal Do not forget to add the newly created condition
 *           to the live list before returning.
 */
Condition * ConditionFactory::createCondition ( void )
{
    Condition *c = new Condition( );

    if ( c->isValid( ) == false )
    {
        delete c;
        return NULL;
    }

    m_liveCondList.push_back( c );
    return c;
}


/**
 * Returns the singleton instance of the factory.
 * the method is not thread safe.
 *
 */
ConditionFactory * ConditionFactory::getInstance ( void )
{
    if ( m_instance == NULL )
        m_instance = new ConditionFactory( );
    
    return m_instance;
}

