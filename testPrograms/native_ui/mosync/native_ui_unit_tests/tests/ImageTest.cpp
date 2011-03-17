/*
 * ImageTest.cpp
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
 * related to a image.
 */
class ImageTest
: public WidgetTest
{
public:
	ImageTest()
	: WidgetTest( MAW_IMAGE, "ImageTest" )
	{
		addTest( bind( &ImageTest::testImageProperty, this ),
						"Tests to set an image." );
		addGeneralSetPropertyTests( );
	}

	/**
	 * The purpose of this test is to test all properties that
	 * you can set on a screen.
	 */
	void testImageProperty()
	{
		TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), MAW_IMAGE_IMAGE, getIconHandleAsString( ) ), MAW_RES_OK );
	}

};

static Testify::TestHook hook( new ImageTest( ), "ImageTest" );
