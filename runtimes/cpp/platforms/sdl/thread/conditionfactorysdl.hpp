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

