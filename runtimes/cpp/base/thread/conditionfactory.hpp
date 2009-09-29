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

