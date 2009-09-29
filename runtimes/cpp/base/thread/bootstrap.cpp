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
 * File:   bootstrap.cpp
 * Author: Ali Mosavian
 *
 * Created on July 20, 2009
 */
#include "bootstrap.hpp"
#include "mutexfactory.hpp"
#include "semaphorefactory.hpp"
#include "conditionfactory.hpp"
#include "threadfactory.hpp"


/**
 * Inits the thread sub systems in the correct order.
 * Note: This should be called first in MALibInit
 *
 */
void Base::Thread::startup ( void )
{
    MutexFactory::getInstance( );
    SemaphoreFactory::getInstance( );
    ConditionFactory::getInstance( );
    ThreadFactory::getInstance( );
}

/**
 * Shuts down the thread sub systems in the correct order.
 * Note: This should be called last in MALibQuit
 *
 */
void Base::Thread::shutdown ( void )
{
    delete ThreadFactory::getInstance( );
    delete ConditionFactory::getInstance( );
    delete SemaphoreFactory::getInstance( );
    delete MutexFactory::getInstance( );
}

