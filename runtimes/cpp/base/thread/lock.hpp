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
 * File:   lock.hpp
 * Author: Ali Mosavian
 *
 * Created on July 13, 2009
 */

#ifndef __LOCK_HPP__
#define	__LOCK_HPP__

#include "mutex.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * Platform independent lock class for mutexes. 
         *
         */
        class Lock
        {
            friend class Mutex;
            friend class ReLocker;
            
        protected:
            Mutex *     m_mutex;
            bool        m_locked;

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


        public:
            /**
             * Constructor, locks the mutex
             *
             * @param m     The mutex to lock
             */
            Lock ( Mutex *m )
            {
                m_mutex = m;
                m_mutex->lock( );
                m_locked = true;
            }

            /**
             * Constructor, locks the mutex
             *
             * @param m     The mutex to lock
             */
            Lock ( Mutex &m )
            {
                m_mutex = &m;
                m_mutex->lock( );
                m_locked = true;
            }

            /**
             * Destructor, unlocks mutex
             *
             */
            ~Lock ( void )
            {
                if ( m_locked == true )
                    m_mutex->unlock( );
            }
        };
    }
}


#endif	/* __LOCK_HPP__ */

