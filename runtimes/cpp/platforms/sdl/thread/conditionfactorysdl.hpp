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
 * File:   conditionfactorysdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 18, 2009
 */

#ifndef __CONDITIONFACTORYSDL_HPP__
#define	__CONDITIONFACTORYSDL_HPP__

#include "thread/conditionfactory.hpp"
#include "mutexsdl.hpp"

namespace Base
{
    namespace Thread
    {
        class ConditionFactorySDL : public ConditionFactory
        {
        private:
            static MutexSDL     m_mutex;

        public:
            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static ConditionFactory * getInstance ( void );
        };
    }
}

#endif	/* __CONDITIONFACTORYSDL_HPP__ */

