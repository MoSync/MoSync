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
 * File:   semaphorefactorywinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __SEMAPHOREFACTORYWINMOB_HPP__
#define	__SEMAPHOREFACTORYWINMOB_HPP__

#include "mutexwinmob.hpp"
#include "thread/semaphorefactory.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * Implementation of SemaphoreFactory for the Windows
         * mobile platform.
         *
         */
        class SemaphoreFactoryWinMob : public SemaphoreFactory
        {
        private:
            static MutexWinMob	m_mutex;

            /**
             * Default constructor
             *
             */
            SemaphoreFactoryWinMob ( void );

        public:
            /**
             * Destructor
             *
             */
            virtual ~SemaphoreFactoryWinMob ( void );

            /**
             * Creates a platform independent Semaphore
             *
             * @param v     Initial value (defualt is 0)
             *
             * @return Pointer to semaphore if allocation was succesful,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created mutex
             *           to the live list before returning.
             */
            virtual Semaphore * createSemaphore ( int v = 0 );


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static SemaphoreFactory * getInstance ( void );
        };
    }
}


#endif	/* _SEMAPHOREFACTORYWINMOB_HPP */

