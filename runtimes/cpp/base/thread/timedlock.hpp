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
 * File:   timedlock.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __TIMEDLOCK_HPP__
#define	__TIMEDLOCK_HPP__


#include "timedmutex.hpp"
#include "lock.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * Platform independent lock class for mutexes.
         *
         */
        class TimedLock
        {
        private:
            TimedMutex *    m_mutex;
            bool            m_locked;

        public:
            /**
             * Constructor, locks the mutex with a time out
             *
             * @param m     The mutex to lock
             * @param ms    Milliseconds before time out
             */
            TimedLock ( const TimedMutex *m, int ms )
            {
                m_mutex = m;
                m_mutex->lock( ms );
                m_locked = true;
            }

            /**
             * Destructor, unlocks mutex
             *
             */
            ~TimedLock ( void )
            {
                if ( m_locked == true )
                    m_mutex->unlock( );
            }
            

            /**
             * Manual lock
             *
             */
            void lock ( void )
            {
                if ( m_locked == true )
                    return;

                m_locked = true;
                m_mutex->lock( );
            }

            /**
             * Manual unlock
             *
             */
            void unlock ( void )
            {
                if ( m_locked == false )
                    return;

                m_locked = false;
                m_mutex->unlock( );
            }
        };
    }
}





#endif	/* _TIMEDLOCK_HPP */

