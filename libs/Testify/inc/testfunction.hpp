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

/** \file testfunction.hpp
 * \brief Wrapper for a test function for Testify.
 *
 * \author Ali Mosavian
 *
 * Created on: 28 feb 2010
 */

#ifndef 	__TESTIFY_TESTFUNCTION_HPP__
#define 	__TESTIFY_TESTFUNCTION_HPP__

#include "common.h"
#include "bind.hpp"
#include "test.hpp"

NAMESPACE_BEGIN( Testify )

/**
 * @brief A wrapper for a test function. It's main reason for
 * existing is to provide homogeneous interface without
 * special cases.
 *
 */
class TestFunction : public Test
{
private:
	const char *m_name;
	Functor *	m_test;

public:
	/**
	 * Constructor
	 *
	 * @param f Pointer to a functor which contains the test
	 * @param n Test name, default value is 'no_name_function'
	 */
	TestFunction ( Functor *f,
				   const char *n = "no_name_function" );

	/**
	 * Destructor
	 */
	virtual ~TestFunction ( void );

	/**
	 * Runs test
	 *
	 */
	virtual void run ( void );
};

NAMESPACE_END

#endif 	// __TESTIFY_TESTFUNCTION_HPP__
