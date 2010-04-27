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

/*
 * testmanager.hpp
 *
 *  Created on: 28 feb 2010
 *      Author: Ali Mosavian [mosavian at kth dot se]
 */

#ifndef __TESTMANAGER_HPP__
#define __TESTMANAGER_HPP__

#include "common.h"
#include <MAUtil/Vector.h>

NAMESPACE_BEGIN( Testify )


class Test;
class TestCase;
class TestHook;
class TestFunction;
class TestListener;

/**
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

	static TestManager *			m_instance;
	MAUtil::Vector<Test *>			m_testList;
	MAUtil::Vector<TestListener *>	m_listenerList;


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
