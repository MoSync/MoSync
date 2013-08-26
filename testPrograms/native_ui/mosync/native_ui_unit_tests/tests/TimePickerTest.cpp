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

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_TIME_PICKER_CURRENT_HOUR, "20", "Testing set hour property to 20.", MAW_RES_OK },
	{ MAW_TIME_PICKER_CURRENT_MINUTE, "10", "Testing set minute to 10.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a time picker.
 */
class TimePickerTest
: public WidgetTest
{
public:
	TimePickerTest()
	: WidgetTest( MAW_TIME_PICKER, "TimePickerTest" )
	{

		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

};

static Testify::Test *tests[] = { new TimePickerTest( ), new WidgetWithTextTest( MAW_TIME_PICKER, "TimePickerTest" ) };
static Testify::TestHook hook( tests, "TimePickerTest" );
