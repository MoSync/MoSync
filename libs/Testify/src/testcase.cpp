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
#include "test.hpp"
#include "testcase.hpp"
#include "testmanager.hpp"


NAMESPACE_BEGIN( Testify )


/**
 * Constructor
 *
 * @param n Test suite name, default value is 'no_name_suite'
 */
TestCase::TestCase ( const char *n )
{
	m_name = n;
}

/**
 * Destructor
 *
 */
TestCase::~TestCase ( void )
{
	// Delete tests
	for ( int i = 0; i < m_testList.size( ); i++ )
		delete m_testList[i];
}


/**
 * Called once before any test has run
 * Note: Overload if you need a custom version
 *
 */
void TestCase::caseSetUp ( void )
{
}

/**
 * Called once after all tests have run
 * Note: Overload if you need a custom version
 *
 */
void TestCase::caseTearDown ( void )
{
}


/**
 * Called once before every test
 * Note: Overload if you need a custom version
 *
 */
void TestCase::testSetUp ( void )
{
}

/**
 * Called once after each test
 * Note: Overload if you need a custom version
 */
void TestCase::testTearDown ( void )
{
}

/**
 * Adds a test case
 *
 * @param f Pointer to functor which contains the test
 * @param n Name of test, default value is 'no_name_testcase'
 */
void TestCase::addTest ( Functor *f,
						 const char *n )
{
	m_testList.add( Test::create( f, n ) );
}

/**
 * Runs tests
 *
 */
void TestCase::run ( void )
{
	TestManager::getInstance( )->beginTestCase( m_name );
	caseSetUp( );

	// Run individual tests
	for ( int i = 0; i < m_testList.size( ); i++ )
	{
		testSetUp( );
		m_testList[i]->run( );
		testTearDown( );
	}

	caseTearDown( );
	TestManager::getInstance( )->endTestCase( );
}


NAMESPACE_END

