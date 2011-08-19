
#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_VIDEO_VIEW_PATH, "/sdcard/bluetooth/heineken.wmv", "Testing set path .", MAW_RES_OK },
	{ MAW_VIDEO_VIEW_URL, "http://www.jhepple.com/SampleMovies/niceday.wmv", "Testing set URL.", MAW_RES_OK },
	{ MAW_VIDEO_VIEW_ACTION, "1", "Testing set action to PLAY", MAW_RES_OK },
	{ MAW_VIDEO_VIEW_ACTION, "3", "Testing set action to STOP", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a video view.
 */
class VideoViewTest
: public WidgetTest
{
public:
	VideoViewTest()
	: WidgetTest( MAW_VIDEO_VIEW, "VideoViewTest" )
	{
		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

};

static Testify::Test *tests[] = { new VideoViewTest( ), new WidgetWithTextTest( MAW_VIDEO_VIEW, "VideoViewTest" ) };
static Testify::TestHook hook( tests, "VideoViewTest" );
