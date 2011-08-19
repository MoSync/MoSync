
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
