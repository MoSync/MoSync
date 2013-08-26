/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
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
