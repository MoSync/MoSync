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
