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
 * File:   mutexfactory.hpp
 * Author: Ali Mosavian
 *
 * Created on July 14, 2009
 */

#ifndef __MUTEXFACTORY_HPP__
#define	__MUTEXFACTORY_HPP__

#include <list>
#include <cstdlib>
#include "mutex.hpp"
#include "timedmutex.hpp"
#include "../allocationfailedexception.hpp"


namespace Base
{
    namespace Thread
    {
        /**
         * This singleton class creates new mutexes in a platform
         * independent fashion. To create an implementation for
         * a new platform you need to:
         *
         * 1. Create a seperate file for the implementation
         *
         * 2. Implement the methods
         *      - createMutex method
         *      - createTimedMutex method (no SDL support, so skipped)
         *      - Thread safe getInstance method
         *
         * 3. Add the call to YourImplementation::getInstance in the
         *    getInstance method of this class by using the preprocessor.
         *
         * 4. Include that implementation when compiled for the new platform
         *    by using the build system.
         *
         */
        class MutexFactory
        {
            friend class Mutex;
            friend class TimedMutex;


        protected:
            static MutexFactory*    m_instance;
            std::list<Mutex *>      m_liveMutexList;


            /**
             * Deletes all the mutexes
             *
             */
            void deleteMutexes ( void );

            /**
             * Constructor
             *
             */
            MutexFactory ( void );

            /**
             * Removes a mutex from the live list
             *
             * @param m     The mutex to remove
             *
             */
            void deleteMutex ( Mutex *m );

        public:
            /**
             * Destructor
             *
             * @internal Do not forget to call deleteMutexes in
             *           the destructor of the implementation.
             */
            virtual ~MutexFactory ( void );

            /**
             * Creates a platform independent mutex
             *
             * @return Pointer to mutex if allocation was succesfull,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created mutex
             *           to the live list before returning.
             */
            virtual Mutex * createMutex ( void ) = 0;


            /**
             * Creates a platform independent timed mutex
             *
             */
            /*virtual TimedMutex * createTimedMutex ( void ) = 0;*/


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static MutexFactory * getInstance ( void );
        };

    }
}

#endif	/* _MUTEXFACTORY_HPP */

