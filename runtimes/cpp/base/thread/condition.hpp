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
 * File:   condition.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __CONDITION_HPP__
#define	__CONDITION_HPP__

#include "lock.hpp"
#include "mutex.hpp"
#include "semaphore.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * Platform independent condition variable class.
         *
         * Note: The implementation is high level using semaphores and
         *       mutexes as not all OS:es have native support for condition
         *       variables.
         */
        class Condition
        {
            friend class ConditionFactory;
            
        private:
            Mutex*          m_mutex;
            Semaphore*      m_semaphore;
            int             m_threadsWaiting;

            /**
             * Constructor
             *
             * @throws AllocationFailedException if the allocation of internal
             *         resources failes.
             */
            Condition ( void );

            /**
             * Checks if the condition variable was created, for internal use.
             *
             * @return true if this is a valid condition variable.
             */
            bool isValid ( void );

        public:
            /**
             * Destructor
             *
             */
             ~Condition ( void );

            /**
             * Waits for a notification
             *
             * @param l     A lock which is used as a guard for
             *              the shared resource. It will be unlocked
             *              while waiting for a notification and
             *              relocked before returning.
             */
            void wait ( Lock &l );

            /**
             * Notifies one waiting thread
             *
             */
            void notify ( void );

            /**
             * Notifies all waiting threads
             * 
             */
            void notifyAll ( void );
        };
    }
}

#endif	/* __CONDITION_HPP__ */

