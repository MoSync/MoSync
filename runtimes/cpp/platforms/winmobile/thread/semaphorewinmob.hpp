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
 * File:   semaphorewinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 15, 2009
 */

#ifndef __SEMAPHOREWINMOB_HPP__
#define	__SEMAPHOREWINMOB_HPP__

#include <windows.h>
#include "thread/semaphore.hpp"

namespace Base
{
    namespace Thread
    {
        class SemaphoreWinMob : public Semaphore
        {
        private:
            friend class SemaphoreFactoryWinMob;
            HANDLE  m_sem;

            /**
             * Constructor
             *
             * @param v     Initial semaphore valure
             *
             */
            SemaphoreWinMob ( int v, SemaphoreFactory *i = NULL );

            /**
             * Checks if the OS semaphore was created, for internal use.
             *
             * @return true if it the OS semaphore was created and this is
             *         a valid semaphore.
             */
            virtual bool isValid ( void );

        public:

            /**
             * Destructor
             *
             */
            ~SemaphoreWinMob ( void );

            /**
             * Wait for a "signal", if there's one available, it will
             * return immediatly, if not it will block until some other
             * thread performs a notify.
             *
             */
            virtual void wait ( void );

            /**
             * Posts a notification
             *
             */
            virtual void post ( void );
        };
    }
}


#endif	/* _SEMAPHOREWINMOB_HPP */

