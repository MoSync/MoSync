/*
 * LabelTest.cpp
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
 * related to a label.
 */
class LabelTest
: public WidgetWithTextTest
{
public:
	LabelTest()
	: WidgetWithTextTest( MAW_LABEL, "LabelTest" )
	{
		addGeneralSetPropertyTests( );
	}
};

static Testify::Test *tests[] = { new LabelTest( ), new WidgetWithTextTest( MAW_LABEL, "LabelTest" ) };
static Testify::TestHook hook( tests, "LabelTest" );
