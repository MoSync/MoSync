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
