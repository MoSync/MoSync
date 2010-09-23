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

/** \file testmanager.hpp
 * \brief Test manager for Testify.
 *
 * \author Ali Mosavian
 *
 * Created on: 28 feb 2010
 */

#ifndef __TESTMANAGER_HPP__
#define __TESTMANAGER_HPP__

#include "common.h"
#include <mastring.h>
#include <MAUtil/Vector.h>
#include <MAUtil/collection_common.h>

NAMESPACE_BEGIN( Testify )

class Test;
class TestCase;
class TestHook;
class TestFunction;
class TestListener;

using MAUtil::Pair;
using MAUtil::Vector;


/**
 * @brief The Testify test manager.
 *
 * This class is the most important class in Testify. All the test hooks
 * add the tests to the singleton instance of this class. It manages all
 * the listeners as well as running the tests that have been hooked.
 *
 *
 */
class TestManager
{
	friend class TestHook;
	friend class TestCase;
	friend class TestFunction;


private:
	int	m_testCount;
	int m_testFailures;


	static TestManager *    m_instance;
	Vector<Test *>          m_testList;
	Vector<TestListener *>  m_listenerList;
	Vector<Pair<char *, Vector<Test *> *> >   m_suiteMap;


	/**
	 * Constructor, private since this is a singleton class
	 *
	 */
	TestManager ( void );

	/**
	 * Destructor, removes all listeners and all tests
	 *
	 */
	~TestManager ( void );

	/**
	 * Adds a new test
	 *
	 * @param t Pointer to test to add
	 * @param s Suite name [Optional]
	 */
	void addTest ( Test *t,
                   const char *s = "default" );

	/**
	 * Notifies the listeners that a test suite has started
	 *
	 * @param n Suite name
	 */
	void beginSuite ( const char *n );

	/**
	 * Notifies the listeners that a test suite has ended
	 *
	 */
	void endSuite ( void );

	/**
	 * Notifies the listeners that a test case has started
	 *
	 * @param n Test case name
	 */
	void beginTestCase ( const char *n );

	/**
	 * Notifies the listeners that a test case has ended
	 *
	 */
	void endTestCase ( void );

	/**
	 * Notifies the listeners that a test has started
	 *
	 * @param n Test name
	 */
	void beginTest ( const char *n );

	/**
	 * Notifies the listeners that a test has ended
	 *
	 */
	void endTest ( void );

	/**
	 * Adds a test to a suite
	 *
	 * @param n Name of the suite
	 * @param t Pointer to test to add to suite
	 */
	void addToSuite ( const char *n, Test *t )
	{
		Vector<Test *> *suiteList = NULL;

		// Search for suite
		for ( int i = 0; i < m_suiteMap.size( ); i++ )
		{
			if ( stricmp(m_suiteMap[i].first, n ) == 0 )
			{
				suiteList = m_suiteMap[i].second;
				break;
			}
		}

		// Suite exists ?
		if ( suiteList == NULL )
		{
			suiteList = new Vector<Test *>( );
			Pair< char *, Vector<Test *> *> pair;

			int len = strlen( n );
			pair.first = new char[len+1];
			strcpy( pair.first, n );

			pair.second= suiteList;
			m_suiteMap.add( pair );
		}

		// Add test
		suiteList->add( t );
	}

	/**
	 * Deletes this instance, should only be called form
	 * atexit( ... ).
	 */
	static void destroy ( void );

public:
	/**
	 * Adds a new test listener
	 *
	 * @param l Pointer to dynamically allocated listener
	 */
	void addTestListener ( TestListener * l );

	/**
	 * Runs the tests
	 *
	 */
	void run ( void );

	/**
	 * Tells the test manager that the current
	 * test has failed
	 *
	 * @param f File name
	 * @param l File line
	 * @param a Assertion string that failed
	 */
	void testFailed ( const char *f,
					  int l,
					  const char *a );


	/**
	 * Returns singleton instance.
	 * Note: This method is not thread safe.
	 *
	 * @return Pointer to instance
	 */
	static TestManager *getInstance ( void );
};

NAMESPACE_END

#endif /* __TESTMANAGER_HPP__ */
