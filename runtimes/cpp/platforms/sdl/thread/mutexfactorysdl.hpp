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
 * File:   mutexfactorysdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __MUTEXFACTORYSDL_HPP__
#define	__MUTEXFACTORYSDL_HPP__

#include "mutexsdl.hpp"
#include "thread/mutexfactory.hpp"

namespace Base
{
    namespace Thread
    {
        class MutexFactorySDL : public MutexFactory
        {
        private:
            static MutexSDL     m_mutex;

            /**
             * Default constructor
             *
             */
            MutexFactorySDL ( void );

        public:
            /**
             * Destructor
             *
             */
            virtual ~MutexFactorySDL ( void );

            /**
             * Creates a platform independent mutex.
             *
             * @return Pointer to mutex if allocation was succesfull,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created mutex
             *           to the live list before returning.
             */
            virtual Mutex * createMutex ( void );


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static MutexFactory * getInstance ( void );
        };
    }
}


#endif	/* _MUTEXFACTORYSDL_HPP */

