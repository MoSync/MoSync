/*
 * ActivityIndicatorTest.cpp
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
	{ MAW_ACTIVITY_INDICATOR_IN_PROGRESS, "true", "Testing inProgress property.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to an activity indicator.
 */
class ActivityIndicatorTest
: public WidgetTest
{
public:
	ActivityIndicatorTest()
	: WidgetTest( MAW_SLIDER, "ActivityIndicatorTest" )
	{
		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

};

static Testify::Test *tests[] = { new ActivityIndicatorTest( ), new WidgetWithTextTest( MAW_ACTIVITY_INDICATOR, "ActivityIndicatorTest" ) };
static Testify::TestHook hook( tests, "ActivityIndicatorTest" );
