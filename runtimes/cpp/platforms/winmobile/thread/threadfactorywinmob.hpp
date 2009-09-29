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
 * File:   threadfactorywinmob.hpp
 * Author: Ali Mosavian
 *
 * Created on July 20, 2009
 */

#ifndef __THREADFACTORYWINMOB_HPP__
#define	__THREADFACTORYWINMOB_HPP__

#include "mutexwinmob.hpp"
#include "threadwinmob.hpp"
#include "thread/threadfactory.hpp"

namespace Base
{
    namespace Thread
    {
        class ThreadFactoryWinMob : public ThreadFactory
        {
            static MutexWinMob  m_mutex;

            /**
             * Default constructor
             *
             */
            ThreadFactoryWinMob ( void );

        public:
            /**
             * Destructor
             *
             */
            virtual ~ThreadFactoryWinMob ( void );

            /**
             * Creates a platform independent thread
             *
             * @return Pointer to thread if allocation was succesful,
             *         NULL otherwise.
             *
             * @internal Do not forget to add the newly created mutex
             *           to the live list before returning.
             */
            virtual Thread* createThread ( void );


            /**
             * Returns the singleton instance of the factory.
             * the method is thread safe.
             *
             */
            static ThreadFactory* getInstance ( void );
        };
    }
}

#endif	/* __THREADFACTORYWINMOB_HPP__ */

