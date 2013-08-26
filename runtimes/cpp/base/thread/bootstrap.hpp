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
 * File:   bootstrap.hpp
 * Author: Ali Mosavian
 *
 * Created on July 20, 2009
 */

#ifndef __BOOTSTRAP_HPP__
#define	__BOOTSTRAP_HPP__

namespace Base
{
    namespace Thread
    {
        /**
         * Inits the thread sub systems in the correct order.
         * Note: This should be called first in MALibInit
         *
         */
        void startup ( void );

        /**
         * Shuts down the thread sub systems in the correct order.
         * Note: This should be called last in MALibQuit
         *
         */
        void shutdown ( void );
    }
}


#endif	/* __BOOTSTRAP_HPP__ */

