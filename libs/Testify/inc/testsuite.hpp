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
 * testsuite.hpp
 *
 *  Created on: 28 feb 2010
 *      Author: Ali Mosavian [mosavian at kth dot se]
 */

#ifndef __TESTSUITE_HPP__
#define __TESTSUITE_HPP__

#include <MAUtil/Vector.h>

#include "ustd/common.h"
#include "ustd/bind.hpp"

#include "test.hpp"


NAMESPACE_BEGIN( Testify )

class TestSuite : public Test
{
private:
	const char *		 	m_name;
	MAUtil::Vector<Test *> 	m_testList;


public:
	/**
	 * Constructor
	 *
	 * @param n Test suite name, default value is 'no_name_suite'
	 */
	TestSuite ( const char *n = "no_name_suite" );

	/**
	 * Destructor
	 *
	 */
	virtual ~TestSuite ( void );

	/**
	 * Called once before any test has run
	 * Note: Overload if you need a custom version
	 *
	 */
	virtual void suiteSetUp ( void );

	/**
	 * Called once after all tests have run
	 * Note: Overload if you need a custom version
	 *
	 */
	virtual void suiteTearDown ( void );

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
	 * @param n Name of test, default value is 'no_name_testcase'
	 */
	void addTest ( ustd::Functor *f,
				   const char *n = "no_name_testcase" );

	/**
	 * Runs tests
	 *
	 */
	virtual void run ( void );
};


NAMESPACE_END


#endif /* __TESTSUITE_HPP__ */
