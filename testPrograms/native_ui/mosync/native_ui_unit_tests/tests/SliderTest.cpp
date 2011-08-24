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
