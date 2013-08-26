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
 * File:   thread.cpp
 * Author: Ali Mosavian
 *
 * Created on August 3, 2009
 */
#include "thread.hpp"

using namespace Base::Thread;

/**
 * Starts the thread.
 *
 * @param f     Pointer to the functor to invoke in
 *              the thread.
 */
void Thread::start ( Functor *f )
{
    MoSyncThread::start( funct, (void *)f );
}


/**
 * Thread stub
 *
 * @param i     "this" pointer to current instance
 */
int Thread::funct ( void *i )
{
     Functor *f = static_cast<Functor *>( i );

     (*f)( );

     return 0;
}