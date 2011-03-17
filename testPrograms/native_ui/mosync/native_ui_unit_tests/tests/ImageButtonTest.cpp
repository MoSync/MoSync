/*
 * ImageButtonTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "res/Resources.h"

#include "WidgetTest.h"

/**
 * The purpose of this test case is to test the functionality
 * related to a image button.
 */
class ImageButtonTest
: public WidgetTest
{
public:
	ImageButtonTest()
	: WidgetTest( MAW_IMAGE_BUTTON, "ImageButtonTest" )
	{
		addTest( bind( &ImageButtonTest::testImageBackgroundProperty, this ),
						"Tests to set an image background." );
		addGeneralSetPropertyTests( );
	}

	/**
	 * The purpose of this test is to test all properties that
	 * you can set on a screen.
	 */
	void testImageBackgroundProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_IMAGE_BUTTON_BACKGROUND_IMAGE, getIconHandleAsString( ) ), MAW_RES_OK );
	}

};

static Testify::TestHook hook( new ImageButtonTest( ), "ImageButtonTest" );
