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
 * File:   semaphoresdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __SEMAPHORESDL_HPP__
#define	__SEMAPHORESDL_HPP__

#include <SDL/SDL_thread.h>
#include "thread/semaphore.hpp"

namespace Base
{
    namespace Thread
    {
        class SemaphoreSDL : public Semaphore
        {
        private:
            friend class SemaphoreFactorySDL;

            SDL_sem *   m_sem;

            /**
             * Constructor
             *
             * @param v     Initial semaphore valure
             *
             */
            SemaphoreSDL ( int v, SemaphoreFactory *i = NULL );

            /**
             * Checks if the OS mutex was created, for internal use.
             *
             * @return true if it the OS mutex was created and this is
             *         a valid mutex.
             */
            virtual bool isValid ( void );

        public:

            /**
             * Destructor
             *
             */
            ~SemaphoreSDL ( void );

            /**
             * Wait for a "signal", if there's one available, it will
             * return immediatly, if not it will block until some other
             * thread performs a notify.
             *
             */
            virtual void wait ( void );

            /**
             * Posts a notification
             *
             */
            virtual void post ( void );
        };
    }
}


#endif	/* _SEMAPHORESDL_HPP */

