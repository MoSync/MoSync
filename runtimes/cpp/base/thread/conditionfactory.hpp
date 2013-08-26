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
 * File:   conditionfactory.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __CONDITIONFACTORY_HPP__
#define	__CONDITIONFACTORY_HPP__

#include <list>
#include "condition.hpp"

namespace Base
{
    namespace Thread
    {
        class ConditionFactory
        {
            friend class Condition;

        protected:
            static ConditionFactory *   m_instance;
            std::list<Condition *>      m_liveCondList;


            /**
             * Constructor
             *
             */
            ConditionFactory ( void );


            /**
             * Removes a condition variable from the live list
             *
             * @param c     The condition to remove
             *
             */
            void removeCondition ( Condition *c );

            /**
             * Deletes all the condition variables, called from
             * the factory destructor.
             *
             */
            void deleteConditions ( void );

        public:
            /**
             * Destructor
             *
             */
            virtual ~ConditionFactory ( void );

            /**
             * Creates a platform independent condition variable
             *
             * @return Pointer to condition if allocation was succesfull,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created condition
             *           to the live list before returning.
             */
            Condition * createCondition ( void );


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static ConditionFactory * getInstance ( void );
        };
    };
};

#endif	/* __CONDITIONFACTORY_HPP__ */

