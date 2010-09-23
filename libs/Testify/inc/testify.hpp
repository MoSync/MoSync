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

/** \file testify.hpp
 * \brief Asserts and macros for Testify.
 *
 * \author Ali Mosavian
 *
 * Created on: 1 mar 2010
 */

#ifndef __TESTIFY_HPP__
#define __TESTIFY_HPP__

#include "common.h"
#include "bind.hpp"
#include "test.hpp"
#include "testhook.hpp"
#include "testcase.hpp"
#include "testfunction.hpp"
#include "testmanager.hpp"

#define PTESTMANAGER (Testify::TestManager::getInstance( ))

//
// Asserts
//
#define TESTIFY_ASSERT( cond ) \
	if ( (cond) == false ) { \
		PTESTMANAGER->testFailed( __FILE__, __LINE__, #cond ); \
		return; \
	}

#define TESTIFY_ASSERT_FALSE( cond ) TESTIFY_ASSERT( !cond )

#define TESTIFY_ASSERT_EQUAL( a, b ) TESTIFY_ASSERT( a == b )

#define TESTIFY_ASSERT_NOT_EQUAL( a, b ) TESTIFY_ASSERT( a != b )

#define TESTIFY_ASSERT_LESS( a, b ) TESTIFY_ASSERT( a < b )

#define TESTIFY_ASSERT_LESS_EQUAL( a, b ) TESTIFY_ASSERT( a <= b )

#define TESTIFY_ASSERT_GREATER( a, b ) TESTIFY_ASSERT( a > b )

#define TESTIFY_ASSERT_GREATER_EQUAL( a, b ) TESTIFY_ASSERT( a >= b )


//
// Help macros
//
#define TESTIFY_DECL_TEST_FUNC( name ) \
	void name ( void ); \
	static TestHook hook_##name( Testify::bind( name ), #name ); \
	void name ( void )


#endif /* __TESTIFY_HPP__ */
