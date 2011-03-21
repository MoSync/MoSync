/*
 * EditBoxTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_EDIT_BOX_PLACEHOLDER, "placeholder", "Testing placeholder property.", MAW_RES_OK },
	{ MAW_EDIT_BOX_SHOW_KEYBOARD, "false", "Testing show keyboard with false.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a check box.
 */
class EditBoxTest
: public WidgetTest
{
public:
	EditBoxTest()
	: WidgetTest( MAW_EDIT_BOX, "EditBoxTest" )
	{
		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

};

static Testify::Test *tests[] = { new EditBoxTest( ), new WidgetWithTextTest( MAW_EDIT_BOX, "EditBoxTest" ) };
static Testify::TestHook hook( tests, "EditBoxTest" );
