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
 * SearchBarTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_EDIT_BOX_TEXT, "text", "Testing text property.", MAW_RES_OK },
	{ MAW_EDIT_BOX_PLACEHOLDER, "placeholder", "Testing placeholder property.", MAW_RES_OK },
	{ MAW_EDIT_BOX_SHOW_KEYBOARD, "false", "Testing show keyboard with false.", MAW_RES_OK },
	{ NULL, NULL, NULL }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a search bar.
 */
class SearchBarTest
: public WidgetTest
{
public:
	SearchBarTest()
	: WidgetTest( MAW_SEARCH_BAR, "SearchBarTest" )
	{
		addTest( bind( &SearchBarTest::testTextProperty, this ),
				"Testing to get text property." );

		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

	/**
	 * Tests the get text property
	 */
	void testTextProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_SEARCH_BAR_TEXT, "text" ), MAW_RES_OK );

		char textBuffer[256];
		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_SEARCH_BAR_TEXT, textBuffer, 256 ), strlen( "text" ) );
		TESTIFY_ASSERT_EQUAL( strcmp( "text", textBuffer ), 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_SEARCH_BAR_TEXT, textBuffer, 1 ), MAW_RES_INVALID_STRING_BUFFER_SIZE );
	}
};

static Testify::TestHook hook( new SearchBarTest( ), "SearchBarTest" );
