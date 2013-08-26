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
 * File:   semaphore.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __SEMAPHORE_HPP__
#define	__SEMAPHORE_HPP__

#include <cstdlib>

namespace Base
{
    namespace Thread
    {
        class SemaphoreFactory;
        /**
         * Platform independent mutex class. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the virtual methods
         *      - wait method
         *      - post method
         *
         * 3. Include that implementation when compiled for the new platform
         *    either by using the build system ( prefered ) or the preprocessor
         */
        class Semaphore
        {
        private:
            friend class SemaphoreFactory;
            
            /**
             * Copy constructor (not allowed!!)
             *
             */
            Semaphore ( const Semaphore &o );

        protected:
            SemaphoreFactory*   m_factInstance;

            /**
             * Default constructor
             *
             * @param i     Pointer to the factory
             */
            Semaphore ( SemaphoreFactory *i = NULL );
            
        public:
            static const int MAXVALUE   = 32*1024;

            /**
             * Destructor
             *
             */
            virtual ~Semaphore ( void );

            /**
             * Wait for a "signal", if there's one available, it will
             * return immediatly, if not it will block until some other
             * thread performs a notify.
             *
             */
            virtual void wait ( void ) = 0;

            /**
             * Posts a notification.
             *
             * Note: Maximum semphore value can not be greater
             *       then Semaphore::MAXVALUE on some platforms.
             *
             */
            virtual void post ( void ) = 0;
        };
    }
}

#endif	/* _SEMAPHORE_HPP */

