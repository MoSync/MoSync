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
 * File:   timedmutex.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __TIMEDMUTEX_HPP__
#define	__TIMEDMUTEX_HPP__

#include "mutex.hpp"
#include "timeoutexception.hpp"

namespace Base
{
    namespace Thread
    {
        class TimedMutex : public Mutex
        {
        private:
            friend class TimedLock;
            
            /**
             * Timed lock operation, if a lock is not accquired
             * before the operation has timed out an exception
             * is thrown. If the method returns without an exception
             * then the lock has been accquired.
             *
             * @param ms    Number of milliseconds before time out.
             *
             * @throws TimeoutException when the operation has timed
             *         out without succeeding.
             */
            virtual void lock ( int ms ) = 0;
        };
    }
}

#endif	/* _TIMEDMUTEX_HPP */

