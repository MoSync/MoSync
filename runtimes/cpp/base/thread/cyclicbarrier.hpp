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

