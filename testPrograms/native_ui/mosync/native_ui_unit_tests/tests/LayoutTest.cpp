/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * LayoutTest.cpp
 *
 *  Created on: Mar 29, 2011
 *      Author: fmattias
 */
#include <testify/testify.hpp>

#include <maapi.h>

#include <IX_WIDGET.h>

/**
 * The purpose of this class is to test general
 * properties of layouts. Like adding and removing
 * children.
 */
class LayoutTest
: public Testify::TestCase
{
public:
	/**
	 * Constructor.
	 */
	LayoutTest()
	{
		addTest( Testify::bind( &LayoutTest::testDestroyLayoutWithChildren, this ),
						"Testing to destroy a layout with children" );
	}

	/**
	 * Tests to remove a layout along with its children.
	 */
	void testDestroyLayoutWithChildren()
	{
		MAWidgetHandle layout = maWidgetCreate( MAW_VERTICAL_LAYOUT );
		TESTIFY_ASSERT( layout > 0 );

		MAWidgetHandle child1 = maWidgetCreate( MAW_LABEL );
		TESTIFY_ASSERT( child1 > 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetAddChild( layout, child1 ), MAW_RES_OK );

		MAWidgetHandle child2 = maWidgetCreate( MAW_LABEL );
		TESTIFY_ASSERT( child2 > 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetAddChild( layout, child2 ), MAW_RES_OK );

		MAWidgetHandle child3 = maWidgetCreate( MAW_LABEL );
		TESTIFY_ASSERT( child3 > 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetAddChild( layout, child3 ), MAW_RES_OK );

		TESTIFY_ASSERT_EQUAL( maWidgetDestroy( layout ), MAW_RES_OK );
	}
};

static Testify::Test *tests[] = { new LayoutTest( ) };
static Testify::TestHook hook( tests, "LayoutTest" );
