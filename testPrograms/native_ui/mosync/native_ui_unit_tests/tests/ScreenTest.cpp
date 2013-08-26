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
