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

