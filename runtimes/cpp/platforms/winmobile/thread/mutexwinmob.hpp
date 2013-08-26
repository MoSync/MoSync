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
 * File:   mutexwinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __MUTEXWINMOB_HPP__
#define	__MUTEXWINMOB_HPP__

#include <windows.h>
#include "thread/mutex.hpp"
#include "allocationfailedexception.hpp"

namespace Base
{
    namespace Thread
    {
        class MutexWinMob : public Mutex
        {
			friend class MutexFactoryWinMob;            
            friend class SemaphoreFactoryWinMob;
            friend class ConditionFactoryWinMob;
            friend class ThreadFactoryWinMob;

        private:
            HANDLE  m_mutex;

            /**
             * Constructor, creates a Windows mobile mutex
             *
             */
            MutexWinMob ( MutexFactory *i = NULL );

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
            virtual ~MutexWinMob ( void );
        };
    }
}


#endif	/* _MUTEXWINMOB_HPP */

