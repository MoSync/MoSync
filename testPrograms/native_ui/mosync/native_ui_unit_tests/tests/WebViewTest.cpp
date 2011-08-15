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
