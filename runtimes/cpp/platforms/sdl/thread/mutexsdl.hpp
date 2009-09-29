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
 * File:   mutexsdl.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __MUTEXSDL_HPP__
#define	__MUTEXSDL_HPP__

#include "SDL/SDL_mutex.h"
#include <thread/mutex.hpp>
#include <allocationfailedexception.hpp>

namespace Base
{
    namespace Thread
    {
        class MutexSDL : public Mutex
        {
            friend class MutexFactorySDL;
            friend class ConditionFactorySDL;
            friend class SemaphoreFactorySDL;
            friend class ThreadFactorySDL;

        private:            
            SDL_mutex*  m_mutex;

            /**
             * Constructor, creates a SDL mutex
             *
             */
            MutexSDL ( MutexFactory *i = NULL );

            /**
             * Locks the mutex
             *
             */
            virtual void lock ( void );

            /**
             * Unlocks the mutex
             *
             */
            virtual void unlock ( void );

            /**
             * Checks if the OS mutex was created, for internal use.
             *
             * @return true if it the OS mutex was created and this is
             *         a valid mutex.
             */
            virtual bool isValid ( void );

        public:
            /**
             * Destructor
             * Frees platform dependent resources.
             *
             */
            virtual ~MutexSDL ( void );
        };
    }
}


#endif	/* _MUTEXSDL_HPP */

