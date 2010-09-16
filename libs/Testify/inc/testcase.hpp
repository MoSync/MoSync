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
 * testcase.hpp
 *
 *  Created on: 28 feb 2010
 *      Author: Ali Mosavian [mosavian at kth dot se]
 */

#ifndef __TESTCASE_HPP__
#define __TESTCASE_HPP__

#include <MAUtil/Vector.h>

#include "common.h"
#include "bind.hpp"
#include "test.hpp"


NAMESPACE_BEGIN( Testify )

/**
 * @brief A test case. This class should be overloaded to handle test cases.
 *
 * It has global set up and tear down, as well as a set up
 * and tear down for each test. Using them are optional,
 * to use them one has to overload
 *  - caseSetUp()
 *  - caseTearDown()
 *  - testSetUp()
 *  - testTearDown()
 *
 * To add a test method, one invokes the addTest(...) method
 * from the constructor once for each test. It's arguments are
 *  - A functor which wraps the test method
 *  - An optional name for the test method
 *
 */
class TestCase : public Test
{
private:
	const char *		 	m_name;
	MAUtil::Vector<Test *> 	m_testList;


public:
	/**
	 * Constructor
	 *
	 * @param n Test case name, default value is 'no_name_case'
	 */
	TestCase ( const char *n = "no_name_testcase" );

	/**
	 * Destructor
	 *
	 */
	virtual ~TestCase ( void );

	/**
	 * Called once before any test has run
	 * Note: Overload if you need a custom version
	 *
	 */
	virtual void caseSetUp ( void );

	/**
	 * Called once after all tests have run
	 * Note: Overload if you need a custom version
	 *
	 */
	virtual void caseTearDown ( void );

	/**
	 * Called once before every test
	 * Note: Overload if you need a custom version
	 *
	 */
	virtual void testSetUp ( void );

	/**
	 * Called once after each test
	 * Note: Overload if you need a custom version
	 */
	virtual void testTearDown ( void );

	/**
	 * Adds a test case
	 *
	 * @param f Pointer to functor which contains the test
	 * @param n Name of test, default value is 'no_name_test_method'
	 */
	void addTest ( Functor *f,
				   const char *n = "no_name_test_method" );

	/**
	 * Runs tests
	 *
	 */
	virtual void run ( void );
};


NAMESPACE_END


#endif /* __TESTSUITE_HPP__ */
