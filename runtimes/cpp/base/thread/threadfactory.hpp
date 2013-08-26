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

