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

