/*
 * ScreenTest.cpp
 *
 *  Created on: Mar 14, 2011
 *      Author: fmattias
 */
#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

#include "res/Resources.h"

/**
 * The purpose of this test case is to test the functionality
 * related to a screen.
 */
class ScreenTest
: public WidgetTest
{
public:
	ScreenTest()
	: WidgetTest( MAW_SCREEN, "ScreenTest" )
	{
		addTest( bind( &ScreenTest::testTitleProperty, this ),
				"Tests title property." );

		addTest( bind( &ScreenTest::testIconProperty, this ),
						"Tests icon property" );
	}

	/**
	 * Tests the title property.
	 */
	void testTitleProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_SCREEN_TITLE, "title" ), MAW_RES_OK );
	}

	/**
	 * The purpose of this test is to test all properties that
	 * you can set on a screen.
	 */
	void testIconProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_SCREEN_ICON, getIconHandleAsString( ) ), MAW_RES_OK );
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_SCREEN_ICON, "-1" ), MAW_RES_INVALID_PROPERTY_VALUE );
	}
};

static Testify::TestHook hook( new ScreenTest( ), "ScreenTest" );
