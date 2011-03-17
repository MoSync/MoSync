/*
 * WidgetWithTextTest.cpp
 *
 *  Created on: Mar 16, 2011
 *      Author: fmattias
 */

#include "WidgetWithTextTest.h"

/**
 * Somes tests for text properties
 *
 * Note: That hard coded properties are used, these should be
 * the same among text widgets to have the API consistent.
 */
static widget_property_test_t g_property_tests[] =
{
	{ "fontColor", "0xffffff", "Testing color with 0x.", MAW_RES_OK },
	{ "fontColor", "ffffff", "Testing color without 0x.", MAW_RES_OK },
	{ "fontSize", "13.4", "Testing font size.", MAW_RES_OK },
	{ "fontSize", "13", "Testing integer font size.", MAW_RES_OK },
	{ "text", "text", "Testing text property.", MAW_RES_OK },
	{ "textHorizontalAlignment", "center", "Testing center horizontal.", MAW_RES_OK },
	{ "textHorizontalAlignment", "left", "Testing left horizontal.", MAW_RES_OK },
	{ "textHorizontalAlignment", "right", "Testing right horizontal.", MAW_RES_OK },
	{ "textVerticalAlignment", "center", "Testing center vertical.", MAW_RES_OK },
	{ "textVerticalAlignment", "top", "Testing top vertical.", MAW_RES_OK },
	{ "textVerticalAlignment", "bottom", "Testing bottom vertical.", MAW_RES_OK },
	{ NULL, NULL, NULL }
};

WidgetWithTextTest::WidgetWithTextTest(const char *type, const char *title)
: WidgetTest::WidgetTest( type, title )
{
	addTest( bind( &WidgetWithTextTest::testTextProperty, this, "text" ),
			"Tests get text property." );

	addSetPropertyTests( g_property_tests );
}

void
WidgetWithTextTest::testTextProperty(const char *textProperty)
{
	TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), textProperty, "text" ), MAW_RES_OK );

	char textBuffer[256];
	TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), textProperty, textBuffer, 256 ), strlen( "text" ) );
	TESTIFY_ASSERT_EQUAL( strcmp( "text", textBuffer ), 0 );

	TESTIFY_ASSERT_EQUAL( maWidgetGetProperty( getTestWidgetHandle( ), textProperty, textBuffer, 1 ), MAW_RES_INVALID_STRING_BUFFER_SIZE );
}
