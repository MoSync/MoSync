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
