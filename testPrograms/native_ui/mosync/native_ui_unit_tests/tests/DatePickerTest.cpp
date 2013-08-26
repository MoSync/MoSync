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
	{ MAW_DATE_PICKER_YEAR, "2000", "Testing set year property to 2000.", MAW_RES_OK },
	{ MAW_DATE_PICKER_MONTH, "2", "Testing set month to 2.", MAW_RES_OK },
	{ MAW_DATE_PICKER_DAY_OF_MONTH, "1", "Testing set day to 1.", MAW_RES_OK },
	{ MAW_TIME_PICKER_CURRENT_MINUTE, "10", "Testing set minute to 10.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a date picker.
 */
class DatePickerTest
: public WidgetTest
{
public:
	DatePickerTest()
	: WidgetTest( MAW_DATE_PICKER, "DatePickerTest" )
	{
		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

};


static Testify::Test *tests[] = { new DatePickerTest( ), new WidgetWithTextTest( MAW_DATE_PICKER, "DatePickerTest" ) };
static Testify::TestHook hook( tests, "DatePickerTest" );
