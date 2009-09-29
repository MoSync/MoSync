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

