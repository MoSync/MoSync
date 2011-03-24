/*
 * ListViewItemTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */
#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "res/Resources.h"

#include "WidgetTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_LIST_VIEW_ITEM_TEXT, "text", "Testing text property.", MAW_RES_OK },
	{ NULL, NULL, NULL }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a list view item.
 */
class ListViewItemTest
: public WidgetTest
{
public:
	ListViewItemTest()
	: WidgetTest( MAW_LIST_VIEW_ITEM, "ListViewItemTest" )
	{
		addSetPropertyTests( g_property_tests );

		addTest( bind( &ListViewItemTest::testIconProperty, this ),
						"Tests to set an icon." );
		addGeneralSetPropertyTests( );
	}

	/**
	 * The purpose of this test is to test all properties that
	 * you can set on a screen.
	 */
	void testIconProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_LIST_VIEW_ITEM_ICON, getIconHandleAsString( ) ), MAW_RES_OK );
	}

};

static Testify::TestHook hook( new ListViewItemTest( ), "ListViewItemTest" );
