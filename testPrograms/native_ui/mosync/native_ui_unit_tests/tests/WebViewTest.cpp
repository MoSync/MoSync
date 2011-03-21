/*
 * WebViewTest.cpp
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
	{ MAW_WEB_VIEW_URL, "http://www.google.se/", "Testing url property.", MAW_RES_OK },
	{ NULL, NULL, NULL }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a webview.
 */
class WebViewTest
: public WidgetTest
{
public:
	WebViewTest()
	: WidgetTest( MAW_WEB_VIEW, "WebViewTest" )
	{
		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}
};

static Testify::TestHook hook( new WebViewTest( ), "WebViewTest" );
