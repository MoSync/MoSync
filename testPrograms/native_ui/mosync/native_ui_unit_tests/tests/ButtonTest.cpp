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
 * ButtonTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */
#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"
#include "WidgetWithTextTest.h"

/**
 * The purpose of this test case is to test the functionality
 * related to a button.
 */
class ButtonTest
: public WidgetTest
{
public:
	ButtonTest()
	: WidgetTest( MAW_BUTTON, "ButtonTest" )
	{
		addGeneralSetPropertyTests( );
	}
};

static Testify::Test *tests[] = { new ButtonTest( ), new WidgetWithTextTest( MAW_BUTTON, "ButtonTest" ) };
static Testify::TestHook hook( tests, "ButtonTest" );
