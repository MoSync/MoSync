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
 * File:   thread.hpp
 * Author: Ali Mosavian
 *
 * Created on August 3, 2009
 */
#ifndef __THREAD_HPP__
#define	__THREAD_HPP__

#include "ThreadPoolImpl.h"
#include "thread/bind.hpp"

class Thread : public MoSyncThread
{
private:
    /**
     * Thread stub
     *
     * @param i     "this" pointer to current instance
     */
     static int funct ( void *i );

public:
    /**
     * Starts the thread.
     *
     * @param f     Pointer to the functor to invoke in
     *              the thread.
     */
    void start ( Base::Thread::Functor *f );
};

#endif	/* __THREAD_HPP__ */

