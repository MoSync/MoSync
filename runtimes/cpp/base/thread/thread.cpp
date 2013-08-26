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
 * File:   thread.cpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */
#include <cstdlib>
#include "thread.hpp"
#include "threadfactory.hpp"

using namespace Base::Thread;

/**
 * Constructor
 *
 * @param i     Pointer to the factory
 */
Thread::Thread ( ThreadFactory *i )
: m_functor( NULL ),
m_factInstance( i )
{
    
}


/**
 * Destructor
 *
 */
Thread::~Thread ( void )
{
    if ( m_factInstance != NULL )
        m_factInstance->removeThread( this );
}
