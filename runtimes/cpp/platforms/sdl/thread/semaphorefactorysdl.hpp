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
 * File:   semaphorefactorysdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __SEMAPHOREFACTORYSDL_HPP__
#define	__SEMAPHOREFACTORYSDL_HPP__

#include "mutexsdl.hpp"
#include <thread/semaphorefactory.hpp>

namespace Base
{
    namespace Thread
    {
        /**
         * Implementation of SemaphoreFactory for the SDL platform.
         *
         */
        class SemaphoreFactorySDL : public SemaphoreFactory
        {
        private:
            static MutexSDL     m_mutex;

            /**
             * Default constructor
             *
             */
            SemaphoreFactorySDL ( void );

        public:
            /**
             * Destructor
             *
             */
            virtual ~SemaphoreFactorySDL ( void );

            /**
             * Creates a platform independent Semaphore
             *
             * @param v     Initial value (defualt is 0)
             *
             * @return Pointer to semaphore if allocation was succesfull,
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


#endif	/* _SEMAPHOREFACTORYSDL_HPP */

