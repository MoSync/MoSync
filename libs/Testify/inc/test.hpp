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
 * test.hpp
 *
 *  Created on: 28 feb 2010
 *      Author: Ali Mosavian [mosavian at kth dot se]
 */

#ifndef 	__TESTIFY_TEST_HPP__
#define 	__TESTIFY_TEST_HPP__

#include "common.h"
#include "bind.hpp"

/**
* \brief MoSync test framework
*/

namespace Testify
{

/**
 * @breif A base class for all tests. It is wrapped by
 * TestCase and TestFunction to handle the different cases.
 *
 */
class Test
{
public:
	/**
	 * Destructor
	 */
	virtual ~Test ( void ) { };

	/**
	 * Runs tests
	 *
	 */
	virtual void run ( void ) = 0;

	/**
	 * Creates a test function
	 *
	 * @param f Pointer to a functor which contains the test
	 * @param n Test name, default value is 'no_name_function'
	 */
	static Test * create ( Functor *f,
						   const char *n = "no_name_function" );

};

}

#endif 	// __TESTIFY_TEST_HPP__
