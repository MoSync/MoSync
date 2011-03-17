/*
 * WidgetTest.h
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#ifndef WIDGETTEST_H_
#define WIDGETTEST_H_

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

/**
 * Data structure for a property test, that enables you
 * to make lists of tests that can verify the return
 * value of a set property.
 */
struct widget_property_test_t
{
	/**
	 * The widget property to pass to maWidgetSetProperty.
	 */
	const char *property;

	/**
	 * The value of the widget property to pass to
	 * maWidgetSetProperty.
	 */
	const char *value;

	/**
	 * Short description of the test.
	 */
	const char *description;

	/**
	 * The expected return value.
	 */
	int expectedReturn;
};

/**
 * General test for widget. Tests that require a valid widget
 * handle before each test, should subclass this test and
 * call the getTestWidgetHandle method when they need the handle.
 *
 * Using this class also makes sure that you get a 'clean' widget,
 * before each test.
 */
class WidgetTest
: public Testify::TestCase
{
public:
	/**
	 * Constructor.
	 *
	 * @param type The widget type being tested.
	 * @param title The optional title of the test case.
	 */
	WidgetTest(const char *type, const char *title = "WidgetTest");

	/**
	 * Creates a widget for each test.
	 */
	virtual void testSetUp(void);

	/**
	 * Destroys the widget after each test.
	 */
	virtual void testTearDown(void);

	/**
	 * Adds tests for a list of properties.
	 *
	 * @param propertyList List of properties, that ends with an element whose property
	 *                     is NULL.
	 */
	void addSetPropertyTests(struct widget_property_test_t *propertyList);

	/**
	 * Adds a set of tests that tests for properties that all widgets
	 * except screens should obey.
	 */
	void addGeneralSetPropertyTests();

	/**
	 * Tests the given property by setting the given value to it.
	 *
	 * @param property The property to test.
	 * @param value The value to set the property to.
	 * @param expectedReturn The return value you expect when setting the property.
	 */
	void testSetProperty(const char *property, const char *value, int expectedReturn);

	/**
	 * Returns the widget handle for the current test.
	 *
	 * @return A widget handle to a widget of the type specified
	 *         in the constructor if you are in a test. Otherwise,
	 *         -1 is returned.
	 */
	MAWidgetHandle getTestWidgetHandle();

private:
	/**
	 * The type of widget that is being tested.
	 */
	const char *m_type;

	/**
	 * Handle to a valid widget.
	 */
	MAWidgetHandle m_testWidgetHandle;
};

#endif /* WIDGETTEST_H_ */
