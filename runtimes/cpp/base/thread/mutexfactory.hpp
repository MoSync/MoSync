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
 * File:   mutexfactory.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __MUTEXFACTORY_HPP__
#define	__MUTEXFACTORY_HPP__

#include <list>
#include <cstdlib>
#include "mutex.hpp"
#include "timedmutex.hpp"
#include "../allocationfailedexception.hpp"


namespace Base
{
    namespace Thread
    {
        /**
         * This singleton class creates new mutexes in a platform
         * independent fashion. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the methods
         *      - createMutex method
         *      - createTimedMutex method (no SDL support, so skipped)
         *      - Thread safe getInstance method
         *
         * 3. Add the call to YourImplementation::getInstance in the
         *    getInstance method of this class by using the preprocessor.
         *
         * 4. Include that implementation when compiled for the new platform
         *    by using the build system.
         *
         */
        class MutexFactory
        {
            friend class Mutex;
            friend class TimedMutex;


        protected:
            static MutexFactory*    m_instance;
            std::list<Mutex *>      m_liveMutexList;


            /**
             * Deletes all the mutexes
             *
             */
            void deleteMutexes ( void );

            /**
             * Constructor
             *
             */
            MutexFactory ( void );

            /**
             * Removes a mutex from the live list
             *
             * @param m     The mutex to remove
             *
             */
            void deleteMutex ( Mutex *m );

        public:
            /**
             * Destructor
             *
             * @internal Do not forget to call deleteMutexes in
             *           the destructor of the implementation.
             */
            virtual ~MutexFactory ( void );

            /**
             * Creates a platform independent mutex
             *
             * @return Pointer to mutex if allocation was succesfull,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created mutex
             *           to the live list before returning.
             */
            virtual Mutex * createMutex ( void ) = 0;


            /**
             * Creates a platform independent timed mutex
             *
             */
            /*virtual TimedMutex * createTimedMutex ( void ) = 0;*/


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static MutexFactory * getInstance ( void );
        };

    }
}

#endif	/* _MUTEXFACTORY_HPP */

