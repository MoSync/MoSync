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
 * File:   mutexsdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __MUTEXSDL_HPP__
#define	__MUTEXSDL_HPP__

#include "SDL/SDL_mutex.h"
#include <thread/mutex.hpp>
#include <allocationfailedexception.hpp>

namespace Base
{
    namespace Thread
    {
        class MutexSDL : public Mutex
        {
            friend class MutexFactorySDL;
            friend class ConditionFactorySDL;
            friend class SemaphoreFactorySDL;
            friend class ThreadFactorySDL;

        private:            
            SDL_mutex*  m_mutex;

            /**
             * Constructor, creates a SDL mutex
             *
             */
            MutexSDL ( MutexFactory *i = NULL );

            /**
             * Locks the mutex
             *
             */
            virtual void lock ( void );

            /**
             * Unlocks the mutex
             *
             */
            virtual void unlock ( void );

            /**
             * Checks if the OS mutex was created, for internal use.
             *
             * @return true if it the OS mutex was created and this is
             *         a valid mutex.
             */
            virtual bool isValid ( void );

        public:
            /**
             * Destructor
             * Frees platform dependent resources.
             *
             */
            virtual ~MutexSDL ( void );
        };
    }
}


#endif	/* _MUTEXSDL_HPP */

