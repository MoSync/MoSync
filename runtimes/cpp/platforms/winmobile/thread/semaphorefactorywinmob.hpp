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
 * File:   semaphorefactorywinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __SEMAPHOREFACTORYWINMOB_HPP__
#define	__SEMAPHOREFACTORYWINMOB_HPP__

#include "mutexwinmob.hpp"
#include "thread/semaphorefactory.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * Implementation of SemaphoreFactory for the Windows
         * mobile platform.
         *
         */
        class SemaphoreFactoryWinMob : public SemaphoreFactory
        {
        private:
            static MutexWinMob	m_mutex;

            /**
             * Default constructor
             *
             */
            SemaphoreFactoryWinMob ( void );

        public:
            /**
             * Destructor
             *
             */
            virtual ~SemaphoreFactoryWinMob ( void );

            /**
             * Creates a platform independent Semaphore
             *
             * @param v     Initial value (defualt is 0)
             *
             * @return Pointer to semaphore if allocation was succesful,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created mutex
             *           to the live list before returning.
             */
            virtual Semaphore * createSemaphore ( int v = 0 );


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static SemaphoreFactory * getInstance ( void );
        };
    }
}


#endif	/* _SEMAPHOREFACTORYWINMOB_HPP */

