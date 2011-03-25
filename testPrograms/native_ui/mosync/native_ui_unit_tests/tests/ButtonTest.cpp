/*
 * ButtonTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */
#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

/**
 * The purpose of this test case is to test the functionality
 * related to a button.
 */
class ButtonTest
: public WidgetTest
{
public:
	ButtonTest()
	: WidgetTest( MAW_BUTTON, "ButtonTest" )
	{
		addGeneralSetPropertyTests( );
	}
};

static Testify::Test *tests[] = { new ButtonTest( ), new WidgetWithTextTest( MAW_BUTTON, "ButtonTest" ) };
static Testify::TestHook hook( tests, "ButtonTest" );
