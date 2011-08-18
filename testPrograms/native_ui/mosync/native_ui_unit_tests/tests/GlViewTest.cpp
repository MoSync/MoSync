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
 * GLViewTest.cpp
 *
 *  Created on: Mar 15, 2011
 *      Author: fmattias
 */

#include <ma.h>
#include <Testify/testify.hpp>
#include <IX_WIDGET.h>

#include "WidgetTest.h"

/**
 * The purpose of this test case is to test the functionality
 * related to a glview.
 */
class GLViewTest
: public WidgetTest
{
public:
	GLViewTest()
	: WidgetTest( MAW_GL_VIEW, "GLViewTest" )
	{
		addTest( bind( &GLViewTest::testGLView, this ),
						"Tests to setup a GLView." );
		addGeneralSetPropertyTests( );
	}

	void testGLView()
	{
	}

};

static Testify::TestHook hook( new GLViewTest( ), "GLViewTest" );
