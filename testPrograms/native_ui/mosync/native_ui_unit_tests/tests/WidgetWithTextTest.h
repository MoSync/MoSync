/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

/*
 * WidgetWithTextTest.h
 *
 *  Created on: Mar 16, 2011
 *      Author: fmattias
 */

#ifndef WIDGET_WITH_TEXT_TEST_H_
#define WIDGET_WITH_TEXT_TEST_H_

#include <ma.h>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

/**
 * This test tests the properties that are common
 * to widgets that contains text.
 */
class WidgetWithTextTest
: public WidgetTest
{
public:
	WidgetWithTextTest(const char *type, const char *title);

	/**
	 * Tests the get text property o
	 *
	 * @param textProperty Name of the text property, different widgets can have different
	 *                     names for it, e.g. MAW_LABEL_TEXT and MAW_BUTTON_TEXT.
	 */
	void testTextProperty(const char *textProperty);
};

#endif /* WIDGET_WITH_TEXT_TEST_H_ */
