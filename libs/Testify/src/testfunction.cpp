/* Copyright (C) 2009 Ali Mosavian

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
#include "common.h"
#include "bind.hpp"
#include "test.hpp"
#include "testfunction.hpp"
#include "testmanager.hpp"


NAMESPACE_BEGIN( Testify )

/**
 * Constructor
 *
 * @param f Pointer to a functor which contains the test
 * @param n Test name, default value is 'no_name_function'
 */
TestFunction::TestFunction ( Functor *f,
							 const char *n )
{
	m_test = f;
	m_name = n;
}

/**
 * Destructor
 *
 */
TestFunction::~TestFunction ( void )
{
	delete m_test;
}

/**
 * Runs test
 *
 */
void TestFunction::run ( void )
{
	TestManager::getInstance( )->beginTest( m_name );
	(*m_test)( );
	TestManager::getInstance( )->endTest( );
}

NAMESPACE_END
