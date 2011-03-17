/*
 * WidgetTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

/**
 * A set of tests that can be applied to all widgets except
 * screens.
 */
static struct widget_property_test_t g_general_widget_tests[] =
{
	{ MAW_WIDGET_ALPHA, "0.0", "Testing alpha 0.0.", MAW_RES_OK },
	{ MAW_WIDGET_ALPHA, "1.0", "Testing alpha 1.0.", MAW_RES_OK },
	{ MAW_WIDGET_ALPHA, "0.5", "Testing alpha 0.5.", MAW_RES_OK },
	{ MAW_WIDGET_ALPHA, "2.0", "Testing alpha 2.0.", MAW_RES_INVALID_PROPERTY_VALUE },
	{ MAW_WIDGET_BACKGROUND_COLOR, "0xffffff", "Testing background color with 0x.", MAW_RES_OK },
	{ MAW_WIDGET_BACKGROUND_COLOR, "ffffff", "Testing background color without 0x.", MAW_RES_OK },
	{ MAW_WIDGET_BACKGROUND_COLOR, "fffff", "Testing background color without 0x.", MAW_RES_INVALID_PROPERTY_VALUE },
	{ MAW_WIDGET_HEIGHT, "40", "Testing height with 40.", MAW_RES_OK },
	{ MAW_WIDGET_HEIGHT, "-1", "Testing height with fill available space.", MAW_RES_OK },
	{ MAW_WIDGET_HEIGHT, "-2", "Testing height with wrap content.", MAW_RES_OK },
	{ MAW_WIDGET_WIDTH, "40", "Testing width with 40.", MAW_RES_OK },
	{ MAW_WIDGET_WIDTH, "-1", "Testing width with fill available space.", MAW_RES_OK },
	{ MAW_WIDGET_WIDTH, "-2", "Testing width with wrap content.", MAW_RES_OK },
	{ MAW_WIDGET_LEFT, "50", "Testing left with 50.", MAW_RES_OK },
	{ MAW_WIDGET_TOP, "50", "Testing left with 50.", MAW_RES_OK },
	{ MAW_WIDGET_VISIBLE, "true", "Testing visible with true.", MAW_RES_OK },
	{ MAW_WIDGET_VISIBLE, "false", "Testing visible with false.", MAW_RES_OK },
	{ NULL, NULL, NULL, -1 }
};

WidgetTest::WidgetTest(const char *type, const char *title)
: 	Testify::TestCase( title ),
	m_type( type ),
	m_testWidgetHandle( -1 )
{

}

void
WidgetTest::testSetUp(void)
{
	// Keep default behavior
	Testify::TestCase::testSetUp( );

	MAWidgetHandle widgetHandle = maWidgetCreate( m_type );
	TESTIFY_ASSERT( widgetHandle >= 0 );
	m_testWidgetHandle = widgetHandle;
}

void
WidgetTest::testTearDown(void)
{
	TESTIFY_ASSERT( m_testWidgetHandle >= 0 );
	TESTIFY_ASSERT_EQUAL( maWidgetDestroy( m_testWidgetHandle ), MAW_RES_OK );
	m_testWidgetHandle = -1;

	// Keep default behavior
	Testify::TestCase::testTearDown( );
}

void
WidgetTest::addSetPropertyTests(struct widget_property_test_t *propertyList)
{
	for(int i = 0; propertyList[ i ].property != NULL; i++)
	{
		Testify::Functor *testFunction = bind( &WidgetTest::testSetProperty,
				this,
				propertyList[ i ].property,
				propertyList[ i ].value,
				propertyList[ i ].expectedReturn );

		addTest( testFunction, propertyList[i].description );
	}
}

void
WidgetTest::addGeneralSetPropertyTests()
{
	addSetPropertyTests( g_general_widget_tests );
}

void
WidgetTest::testSetProperty(const char *property, const char *value, int expectedReturn)
{
	TESTIFY_ASSERT_EQUAL( maWidgetSetProperty( getTestWidgetHandle( ), property, value ), expectedReturn );
}

MAWidgetHandle
WidgetTest::getTestWidgetHandle()
{
	return m_testWidgetHandle;
}

