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
