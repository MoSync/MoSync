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

