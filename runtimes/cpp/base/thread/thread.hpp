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
 * File:   thread.hpp
 * Author: Ali Mosavian
 *
 * Created on July 13, 2009
 */

#ifndef __THREAD_HPP__
#define	__THREAD_HPP__

#include <cstdlib>
#include "bind.hpp"

namespace Base
{
    namespace Thread
    {
        class ThreadFactory;

        /**
         * Platform independent thread class. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the virtual methods
         *      - start method
         *      - join method
         *      - sleep method
         *
         * 3. Include that implementation when compiled for the new platform
         *    either by using the build system ( prefered ) or the preprocessor
         *
         */
        class Thread
        {
        protected:            
            Functor*        m_functor;
            ThreadFactory*  m_factInstance;
            
            /**
             * Constructor
             *
             * @param i     Pointer to the factory
             */
            Thread ( ThreadFactory *i = NULL );

        public:

            /**
             * Destructor
             *
             */
            virtual ~Thread ( void );

            /**
             * Starts the thread
             *
             * @param f     Pointer to the functor to invoke in
             *              the thread.
             */
            virtual void start ( Functor *f ) = 0;


            /**
             * Joins the thread, virtual
             *
             */
            virtual void join ( void ) = 0;


            /**
             * Puts the thread to sleep, virtual
             *
             * @param ms    The number of miliseconds to put the
             *              thread to sleep.
             */
            static void sleep ( unsigned int ms );
        };
    }
}

#endif	/* __MOSYNCTHREAD_HPP__ */

