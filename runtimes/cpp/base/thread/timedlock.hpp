/* Copyright (C) 2009 Mobile Sorcery AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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

