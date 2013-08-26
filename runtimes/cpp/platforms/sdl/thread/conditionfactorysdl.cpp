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
 * File:   conditionfactorysdl.cpp
 * Author: Ali Mosavian
 *
 * Created on July 18, 2009
 */
#include "config_platform.h"
#include "conditionfactorysdl.hpp"
#include "thread/lock.hpp"
#include "helpers/helpers.h"

using namespace Base::Thread;

/**
 * Initialize static fields
 */
MutexSDL ConditionFactorySDL::m_mutex;


/**
 * Returns the singleton instance of the factory.
 * the method is thread safe.
 *
 */
ConditionFactory* ConditionFactorySDL::getInstance ( void )
{
    MAASSERT( m_mutex.isValid( ) == true );
    Lock    l( &m_mutex );

    if ( m_instance == NULL )
        m_instance = new ConditionFactorySDL( );

    return m_instance;
}


