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
 * testhook.hpp
 *
 *  Created on: 28 feb 2010
 *      Author: Ali Mosavian
 */

#ifndef __TESTHOOK_HPP__
#define __TESTHOOK_HPP__

#include "common.h"

#include "test.hpp"
#include "testfunction.hpp"
#include "testmanager.hpp"


NAMESPACE_BEGIN( Testify )

/**
 * @brief A hook used to add a test or a
 * collection of tests. It uses global constructors to
 * add the actual tests to the test manager.
 *
 */
class TestHook
{
public:
	/**
	 * Hook for adding one test function.
	 * Note: The functor will be deleted by the test manager.
	 *
	 * @param f Pointer to functor which wraps the test
	 * @param n Name of the test [Optional]
	 * @param s Name of the suite it belongs to [Optional]
	 */
	TestHook ( Functor *f,
			   const char *n = "noname",
			   const char *s = "default" )
	{
		TestFunction *f2 = new TestFunction( f, n );
		TestManager::getInstance( )->addTest( f2, s );
	}

	/**
	 * Hook for adding one test.
	 * Note: The test will be deleted by the test manager.
	 *
	 * @param f Pointer to the test
	 * @param s Name of the suite it belongs to [Optional]
	 */
	TestHook ( Test *f,
			   const char *s = "default" )
	{
		TestManager::getInstance( )->addTest( f, s );
	}

	/**
	 * Hook for adding multiple test
	 * Note: The tests will be deleted by the test manager.
	 *
	 * @param f Array of pointers to tests
	 * @param s Name of the suite they belongs to [Optional]
	 */
	TestHook ( Test **f,
			   const char *s = "default" )
	{
		while ( *f != NULL )
			TestManager::getInstance( )->addTest( *(f++), s );
	}
};

NAMESPACE_END

#endif /* __TESTHOOK_HPP__ */
