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
 * File:   semaphore.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */
#include "semaphore.hpp"
#include "semaphorefactory.hpp"


using namespace Base::Thread;

/**
 * Default constructor
 *
 * @param i     Pointer to the factory
 */
Semaphore::Semaphore ( SemaphoreFactory *i )
: m_factInstance( i )
{

}

/**
 * Copy constructor (not allowed!!)
 *
 */
Semaphore::Semaphore ( const Semaphore &o )
{
    
}

/**
 * Destructor
 *
 */
Semaphore::~Semaphore ( void )
{
    //
    // Tell the factory that I'm no longer live
    //
    if ( m_factInstance != NULL )
        m_factInstance->deleteSemaphore( this );
}

