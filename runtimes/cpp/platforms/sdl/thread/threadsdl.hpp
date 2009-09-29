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

