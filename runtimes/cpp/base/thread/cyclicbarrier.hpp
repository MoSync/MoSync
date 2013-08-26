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
 * File:   cyclicbarrier.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __CYCLICBARRIER_HPP__
#define	__CYCLICBARRIER_HPP__

#include "mutexfactory.hpp"
#include "conditionfactory.hpp"
#include "../allocationfailedexception.hpp"

namespace Base
{
    namespace Thread
    {
        class CyclicBarrier
        {
        private:
            int         m_count;
            int         m_barrierLimit;
            Mutex*      m_mutex;
            Condition*  m_cond;

        public:
            /**
             * Constructor
             *
             * @param l     The barrier limit before all the waiting
             *              threads are released.
             *
             * @throws AllocationFailedException if the barrier fails to
             *         create its resources.
             */
            CyclicBarrier ( int l );

            /**
             * Destructor
             *
             */
             ~CyclicBarrier ( void );

            /**
             * Waits for all the threads to reach the barrier and
             * resets the barrier.
             *
             */
            void wait ( void );
        };
    }
}

#endif	/* _CYCLICBARRIER_HPP */

