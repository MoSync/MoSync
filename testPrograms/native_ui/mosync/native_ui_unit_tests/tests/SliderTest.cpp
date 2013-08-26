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
 * SliderTest.cpp
 *
 *  Created on: May 06, 2011
 *      Author: emma
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_SLIDER_INCREASE_VALUE, "10", "Testing increment value by 10 property.", MAW_RES_OK },
	{ MAW_SLIDER_MAX, "100", "Testing slider max with 100.", MAW_RES_OK },
	{ MAW_SLIDER_VALUE, "50", "Testing slider value with 50.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a slider.
 */
class SliderTest
: public WidgetTest
{
public:
	SliderTest()
	: WidgetTest( MAW_SLIDER, "SliderTest" )
	{
		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

};

static Testify::Test *tests[] = { new SliderTest( ), new WidgetWithTextTest( MAW_SLIDER, "SliderTest" ) };
static Testify::TestHook hook( tests, "SliderTest" );
