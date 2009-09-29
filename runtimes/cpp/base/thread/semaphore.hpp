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
 * File:   semaphore.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __SEMAPHORE_HPP__
#define	__SEMAPHORE_HPP__

#include <cstdlib>

namespace Base
{
    namespace Thread
    {
        class SemaphoreFactory;
        /**
         * Platform independent mutex class. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the virtual methods
         *      - wait method
         *      - post method
         *
         * 3. Include that implementation when compiled for the new platform
         *    either by using the build system ( prefered ) or the preprocessor
         */
        class Semaphore
        {
        private:
            friend class SemaphoreFactory;
            
            /**
             * Copy constructor (not allowed!!)
             *
             */
            Semaphore ( const Semaphore &o );

        protected:
            SemaphoreFactory*   m_factInstance;

            /**
             * Default constructor
             *
             * @param i     Pointer to the factory
             */
            Semaphore ( SemaphoreFactory *i = NULL );
            
        public:
            static const int MAXVALUE   = 32*1024;

            /**
             * Destructor
             *
             */
            virtual ~Semaphore ( void );

            /**
             * Wait for a "signal", if there's one available, it will
             * return immediatly, if not it will block until some other
             * thread performs a notify.
             *
             */
            virtual void wait ( void ) = 0;

            /**
             * Posts a notification.
             *
             * Note: Maximum semphore value can not be greater
             *       then Semaphore::MAXVALUE on some platforms.
             *
             */
            virtual void post ( void ) = 0;
        };
    }
}

#endif	/* _SEMAPHORE_HPP */

