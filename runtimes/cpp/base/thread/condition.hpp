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

