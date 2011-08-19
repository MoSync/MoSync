/*
 * VideoViewTest.cpp
 *
 *  Created on: May 06, 2011
 *      Author: emma
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

static widget_property_test_t g_property_tests[] =
{
	{ MAW_PROGRESS_BAR_INCREMENT_PROGRESS, "10", "Testing increment progress property with 10.", MAW_RES_OK },
	{ MAW_PROGRESS_BAR_MAX, "100", "Testing set max property with 100.", MAW_RES_OK },
	{ MAW_PROGRESS_BAR_PROGRESS, "50", "Testing progress property with 50.", MAW_RES_OK },
	{ NULL, NULL, NULL, 0 }
};

/**
 * The purpose of this test case is to test the functionality
 * related to a progress bar.
 */
class VideoViewTest
: public WidgetTest
{
public:
	VideoViewTest()
	: WidgetTest( MAW_SLIDER, "ProgressBarTest" )
	{
		addTest( bind( &VideoViewTest::testGetProgressProperty, this ),
				"Tests get progress value property." );

		addTest( bind( &VideoViewTest::testGetMaxProperty, this ),
				"Tests get max property." );

		addSetPropertyTests( g_property_tests );
		addGeneralSetPropertyTests( );
	}

	void testGetProgressProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_PROGRESS_BAR_PROGRESS, "50" ), MAW_RES_OK );

		char valueBuffer[256];
		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_PROGRESS_BAR_PROGRESS, valueBuffer, 256 ), strlen( "false" ) );
		TESTIFY_ASSERT_EQUAL( strcmp( valueBuffer, "50" ), 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_PROGRESS_BAR_PROGRESS, valueBuffer, 1 ), MAW_RES_INVALID_STRING_BUFFER_SIZE );
	}

	void testGetMaxProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_PROGRESS_BAR_MAX, "100" ), MAW_RES_OK );

		char maxBuffer[256];
		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_PROGRESS_BAR_MAX, maxBuffer, 256 ), strlen( "false" ) );
		TESTIFY_ASSERT_EQUAL( strcmp( maxBuffer, "100" ), 0 );

		TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), MAW_PROGRESS_BAR_MAX, maxBuffer, 1 ), MAW_RES_INVALID_STRING_BUFFER_SIZE );
	}
};

static Testify::Test *tests[] = { new VideoViewTest( ), new WidgetWithTextTest( MAW_PROGRESS_BAR, "ProgressBarTest" ) };
static Testify::TestHook hook( tests, "ProgressBarTest" );
