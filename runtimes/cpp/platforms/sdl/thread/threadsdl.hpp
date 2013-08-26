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
 * File:   threadsdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */

#ifndef __THREADSDL_HPP__
#define	__THREADSDL_HPP__

#include "SDL/SDL_thread.h"
#include "thread/thread.hpp"
#include "thread/cyclicbarrier.hpp"

namespace Base
{
    namespace Thread
    {
        class ThreadSDL : public Thread
        {
            friend class ThreadFactorySDL;

        private:
            CyclicBarrier   m_barrier;
            SDL_Thread*     m_thread;


            /**
             * Constructor, creates an SDL Thread
             *
             */
            ThreadSDL ( ThreadFactory *i = NULL );

            /**
             * Thread stub
             *
             * @param i     "this" pointer to current instance
             */
             static int funct ( void *i );

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
             * Frees platform dependent resources.
             *
             */
            virtual ~ThreadSDL ( void );

            /**
             * Starts the thread
             *
             * @param f     Pointer to the functor to invoke in
             *              the thread.
             */
            virtual void start ( Functor *f );

            
            /**
             * Starts the thread, virtual
             *
             */
            virtual void join ( void );
        };
    }
}


#endif	/* __THREADSDL_HPP__ */

