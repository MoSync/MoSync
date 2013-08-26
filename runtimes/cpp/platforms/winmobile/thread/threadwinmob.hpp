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
 * File:   threadwinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */

#ifndef __THREADWINMOB_HPP__
#define	__THREADWINMOB_HPP__

#include <windows.h>
#include "thread/thread.hpp"
#include "thread/cyclicbarrier.hpp"

namespace Base
{
    namespace Thread
    {
        class ThreadWinMob : public Thread
        {
            friend class ThreadFactoryWinMob;

        private:
            HANDLE          m_thread;
            CyclicBarrier   m_barrier;

            /**
             * Constructor, creates a Windows Mobile Thread
             *
             */
            ThreadWinMob ( ThreadFactory *i = NULL );

            /**
             * Thread stub
             *
             * @param i     The "this" pointer
             */
             static DWORD WINAPI funct ( LPVOID i );

            /**
             * Checks if the OS thread was created, for internal use.
             *
             * @return true if it the OS thread was created and this is
             *         a valid thread.
             */
            virtual bool isValid ( void );

        public:
            /**
             * Destructor
             * Frees platform dependent resources.
             *
             */
            virtual ~ThreadWinMob ( void );

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


#endif	/* __THREADWINMOB_HPP__ */

