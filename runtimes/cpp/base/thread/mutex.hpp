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
 * File:   Mutex.hpp
 * Author: Ali Mosavian
 *
 * Created on July 13, 2009
 */

#ifndef __MUTEX_HPP__
#define	__MUTEX_HPP__

#include <cstdlib>

namespace Base
{
    namespace Thread
    {
        class MutexFactory;

        /**
         * Platform independent mutex class. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the virtual methods
         *      - lock method
         *      - unlock method
         *
         * 3. Include that implementation when compiled for the new platform
         *    either by using the build system ( prefered ) or the preprocessor
         */
        class Mutex
        {            
        protected:
            MutexFactory*   m_factInstance;

            /**
             * Default constructor
             *
             * @param i     Pointer to the factory
             */
            Mutex ( MutexFactory *i = NULL );

            /**
             * Checks if the OS mutex was created, for internal use.
             *
             * @return true if it the OS mutex was created and this is
             *         a valid mutex.
             */
            virtual bool isValid ( void ) = 0;

        public:
            /**
             * Destructor
             *
             */
            virtual ~Mutex ( void );

        private:
            friend class Lock;
            friend class TimedLock;
            friend class Condition;

            /**
             * Copy constructor (not allowed!!)
             *
             */
            Mutex ( const Mutex &o );

            /**
             * Locks the mutex
             *
             */
            virtual void lock ( void ) = 0;

            /**
             * Unlocks the mutex
             *
             */
            virtual void unlock ( void ) = 0;
        };
    }
}


#endif	/* _MUTEX_HPP */

