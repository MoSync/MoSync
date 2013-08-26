/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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

