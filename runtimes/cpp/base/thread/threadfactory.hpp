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
 * File:   theadfactory.hpp
 * Author: Ali Mosavian
 *
 * Created on July 19, 2009
 */

#ifndef __THREADFACTORY_HPP__
#define	__THREADFACTORY_HPP__

#include <list>
#include "thread.hpp"
#include "../allocationfailedexception.hpp"


namespace Base
{
    namespace Thread
    {
        /**
         * This singleton class creates new threads in a platform
         * independent fashion. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the methods
         *      - createrThread method
         *      - Thread safe getInstance method
         *
         * 3. Add the call to YourImplementation::getInstance in the
         *    getInstance method of this class by using the preprocessor.
         *
         * 4. Include that implementation when compiled for the new
         *    platform by using the build system.
         *
         */
        class ThreadFactory
        {
            friend class Thread;

        protected:
            static ThreadFactory*   m_instance;
            std::list<Thread *>     m_liveThreadList;


            /**
             * Constructor
             *
             */
            ThreadFactory ( void );

            /**
             * Removes a thread from the live list
             *
             * @param t     The thread to remove
             *
             */
            void removeThread ( Thread *t );

            /**
             * Deletes all the threads
             *
             */
            void deleteThreads ( void );

        public:
            /**
             * Destructor
             *
             * @internal Do not forget to call deleteThreads in
             *           the destructor of the implementation.
             */
            virtual ~ThreadFactory ( void );

            
            /**
             * Creates a platform independent Thread
             *
             * @return Pointer to thread if allocation was succesful,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created thread
             *           to the live list before returning.
             */
            virtual Thread* createThread ( void ) = 0;


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static ThreadFactory* getInstance ( void );
        };
    }
}

#endif	/* __THREADFACTORY_HPP__ */

