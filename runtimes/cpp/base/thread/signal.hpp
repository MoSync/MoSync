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
 * File:   signal.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __SIGNAL_HPP__
#define	__SIGNAL_HPP__

#include "mutexfactory.hpp"
#include "conditionfactory.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * This platform independent class is used for signaling
         * threads.
         *
         */
        class Signal
        {
        private:
            Mutex*      m_mutex;
            Condition*  m_cond;
            bool        m_hasBeenSignaled;


        public:
            /**
             * Constructor
             *
             * @throws AllocationFailedException if the instance fails to
             *         create its resources.
             */
            Signal ( void );

            /**
             * Destructor, will free all the allocated resources.
             *
             */
            ~Signal ( void );

            /**
             * Sends a signal to all the waiting threads.
             *
             */
            void signal ( void );

            /**
             * Returns immediately if the a signal has been sent previously,
             * otherwise it blocks for until a signal is received.
             *
             */
            void wait ( void );

            /**
             * Checks if a signal has been received without blocking
             *
             * @return true if signal has been received.
             */
            bool hasBeenSignaled ( void );

            /**
             * Resets the signal, if there is one.
             *
             */
            void reset ( void );
        };
    }
}

#endif	/* __SIGNAL_HPP__ */

