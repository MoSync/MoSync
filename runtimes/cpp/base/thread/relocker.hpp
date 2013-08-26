/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/* 
 * File:   relocker.hpp
 * Author: Ali Mosavian
 *
 * Created on July 18, 2009
 */

#ifndef __RELOCKER_HPP__
#define	__RELOCKER_HPP__

#include "lock.hpp"

namespace Base
{
    namespace Thread
    {
        /**
         * This class is used for opening a lock at begining of a
         * block and re locking it at the end of the block.
         *
         */
        class ReLocker
        {
        private:
            Lock&   m_lock;

        public:
            /**
             * Constructor, unlocks
             *
             * @param l     The lock to relock
             */
            ReLocker ( Lock & l )
            : m_lock( l )
            {
                m_lock.unlock( );
            }

            /**
             * Destructor, re-locks the lock
             *
             */
            ~ReLocker ( void )
            {
                m_lock.lock( );
            }
        };
    }
}


#endif	/* __RELOCKER_HPP__ */

