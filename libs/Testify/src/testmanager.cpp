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
#include "test.hpp"
#include "testmanager.hpp"
#include "testlistener.hpp"
#include "defaultlistener.hpp"
#include "testfunction.hpp"

extern "C" int atexit ( void ( * function ) (void) );



NAMESPACE_BEGIN( Testify )


/**
 * Constructor, private since this is a singleton class
 *
 */
TestManager::TestManager ( void )
{
	atexit( TestManager::destroy );
}

/**
 * Destructor, removes all listeners and all tests
 *
 */
TestManager::~TestManager ( void )
{
	// Remove tests
	for ( int i = 0; i < m_suiteMap.size( ); i++ )
	{
		Vector<Test *> &l = *(m_suiteMap[i].second);

		// Delete tests in suite
		for ( int j = 0; j < l.size( ); j++ )
			delete l[j];

		// Delete data
		delete m_suiteMap[i].first;
		delete m_suiteMap[i].second;
	}
	m_suiteMap.clear( );

	// Remove listeners
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		delete m_listenerList[i];

}

/**
 * Adds a new test
 *
 * @param t Pointer to test to add
 * @param s Suite name [Optional]
 */
void TestManager::addTest ( Test *t,
                            const char *s )
{
	// Add to suite
	addToSuite( s, t );
}

/**
 * Notifies the listeners that a test suite has started
 *
 * @param n Suite name
 */
void TestManager::beginSuite ( const char *n )
{
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->beginSuite( n );
}

/**
 * Notifies the listeners that a test suite has ended
 *
 */
void TestManager::endSuite ( void )
{
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->endSuite( );
}


/**
 * Notifies the listeners that a test case has started
 *
 * @param n Test case name
 */
void TestManager::beginTestCase ( const char *n )
{
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->beginTestCase( n );
}

/**
 * Notifies the listeners that a test case has ended
 *
 */
void TestManager::endTestCase ( void )
{
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->endTestCase( );
}

/**
 * Notifies the listeners that a test has started
 *
 * @param n Test name
 */
void TestManager::beginTest ( const char *n )
{
	m_testCount++;

	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->beginTest( n );
}

/**
 * Notifies the listeners that a test has ended
 *
 */
void TestManager::endTest ( void )
{
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->endTest( );
}


/**
 * Tells the test manager that the current
 * test has failed
 *
 * @param f File name
 * @param l File line
 * @param a Assertion string that failed
 */
void TestManager::testFailed ( const char *f,
					 		   int l,
							   const char *a )
{
	m_testFailures++;
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->testFailed( f, l, a );
}

/**
 * Adds a new test listener
 *
 * @param l Pointer to dynamically allocated listener
 */
void TestManager::addTestListener ( TestListener * l )
{
	m_listenerList.add( l );
}

/**
 * Runs the tests
 *
 */
void TestManager::run ( void )
{
	m_testCount    = 0;
	m_testFailures = 0;

	// Notify listeners that testing will starting
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->beginUnit( );

	// Run suites
	for ( int i = 0; i < m_suiteMap.size( ); i++ )
	{
		Pair<char *, Vector<Test *> *> &it = m_suiteMap[i];
		const char * n = it.first;
		Vector<Test *> &l = *(it.second);

		// Notify listeners
		for ( int j = 0; j < m_listenerList.size( ); j++ )
			m_listenerList[j]->beginSuite( n );

		// Run tests
		for ( int j = 0; j < l.size( ); j++ )
			l[j]->run( );

		// Notify listeners
		for ( int j = 0; j < m_listenerList.size( ); j++ )
			m_listenerList[j]->endSuite( );
	}

	// Notify listeners that testing has finished
	for ( int i = 0; i < m_listenerList.size( ); i++ )
		m_listenerList[i]->endUnit( m_testCount, m_testFailures );
}


/**
 * Deletes this instance, should only be called form
 * atexit( ... ).
 *
 */
void TestManager::destroy ( void )
{
	delete TestManager::getInstance( );
}


/**
 * Returns singleton instance.
 * Note: This method is not thread safe.
 *
 * @return Pointer to instance
 */
TestManager *TestManager::m_instance = NULL;
TestManager *TestManager::getInstance ( void )
{
	if ( m_instance == NULL )
		m_instance = new TestManager( );

	return m_instance;
}


NAMESPACE_END
