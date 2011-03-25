/*
 * LinearLayoutTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

static widget_property_test_t g_property_tests_horizontal[] =
{
	{ MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, "center", "Testing center horizontal.", MAW_RES_OK },
	{ MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, "left", "Testing left horizontal.", MAW_RES_OK },
	{ MAW_HORIZONTAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, "right", "Testing right horizontal.", MAW_RES_OK },
	{ MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, "center", "Testing center vertical.", MAW_RES_OK },
	{ MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, "top", "Testing top vertical.", MAW_RES_OK },
	{ MAW_HORIZONTAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, "bottom", "Testing bottom vertical.", MAW_RES_OK },
	{ NULL, NULL, NULL }
};

static widget_property_test_t g_property_tests_vertical[] =
{
	{ MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, "center", "Testing center horizontal.", MAW_RES_OK },
	{ MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, "left", "Testing left horizontal.", MAW_RES_OK },
	{ MAW_VERTICAL_LAYOUT_CHILD_HORIZONTAL_ALIGNMENT, "right", "Testing right horizontal.", MAW_RES_OK },
	{ MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, "center", "Testing center vertical.", MAW_RES_OK },
	{ MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, "top", "Testing top vertical.", MAW_RES_OK },
	{ MAW_VERTICAL_LAYOUT_CHILD_VERTICAL_ALIGNMENT, "bottom", "Testing bottom vertical.", MAW_RES_OK },
	{ NULL, NULL, NULL }
};

/**
 * The number of children to try to add.
 */
static const int NUM_CHILDREN = 10;

/**
 * The purpose of this test case is to test the functionality
 * related to the horizontal and vertical layout.
 */
class LineraLayoutTest
: public WidgetTest
{
public:
	/**
	 * Constructor.
	 *
	 * @param vertical True if the test should test the vertical layout, false
	 *                 if it should test the horizontal layout.
	 */
	LineraLayoutTest(bool vertical)
	: WidgetTest( vertical ? MAW_VERTICAL_LAYOUT : MAW_HORIZONTAL_LAYOUT,
				  vertical ? "VerticalLayoutTest" : "HorizontalLayoutTEst" )
	{
		addTest( bind( &LineraLayoutTest::testAddAndRemoveChildren, this ),
						"Testing to add and remove children" );

		if( vertical )
		{
			addSetPropertyTests( g_property_tests_vertical );
		}
		else
		{
			addSetPropertyTests( g_property_tests_horizontal );
		}
		addGeneralSetPropertyTests( );
	}

	/**
	 * Tests to add and remove children from the layout.
	 */
	void testAddAndRemoveChildren()
	{
		MAWidgetHandle children[ 10 ];
		for(int i = 0; i < NUM_CHILDREN; i++)
		{
			children[ i ] = maWidgetCreate( MAW_LABEL );
			TESTIFY_ASSERT( children[ i ] >= 0 );

			TESTIFY_ASSERT_EQUAL( maWidgetAddChild( getTestWidgetHandle( ), children[ i ] ), MAW_RES_OK );
		}

		for(int i = 0; i < NUM_CHILDREN; i++)
		{
			TESTIFY_ASSERT_EQUAL( maWidgetRemoveChild( children[ i ] ), MAW_RES_OK );
			TESTIFY_ASSERT_EQUAL( maWidgetDestroy( children[ i ] ), MAW_RES_OK );
		}
	}

};

static Testify::Test *tests[] = { new LineraLayoutTest( true ), new LineraLayoutTest( false ) };
static Testify::TestHook hook( tests, "VerticalLayoutTest" );
