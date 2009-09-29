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
 * File:   Mutex.hpp
 * Author: Ali Mosavian
 *
 * Created on July 13, 2009
 */

#ifndef __MUTEX_HPP__
#define	__MUTEX_HPP__

#include <cstdlib>

namespace Base
{
    namespace Thread
    {
        class MutexFactory;

        /**
         * Platform independent mutex class. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the virtual methods
         *      - lock method
         *      - unlock method
         *
         * 3. Include that implementation when compiled for the new platform
         *    either by using the build system ( prefered ) or the preprocessor
         */
        class Mutex
        {            
        protected:
            MutexFactory*   m_factInstance;

            /**
             * Default constructor
             *
             * @param i     Pointer to the factory
             */
            Mutex ( MutexFactory *i = NULL );

            /**
             * Checks if the OS mutex was created, for internal use.
             *
             * @return true if it the OS mutex was created and this is
             *         a valid mutex.
             */
            virtual bool isValid ( void ) = 0;

        public:
            /**
             * Destructor
             *
             */
            virtual ~Mutex ( void );

        private:
            friend class Lock;
            friend class TimedLock;
            friend class Condition;

            /**
             * Copy constructor (not allowed!!)
             *
             */
            Mutex ( const Mutex &o );

            /**
             * Locks the mutex
             *
             */
            virtual void lock ( void ) = 0;

            /**
             * Unlocks the mutex
             *
             */
            virtual void unlock ( void ) = 0;
        };
    }
}


#endif	/* _MUTEX_HPP */

