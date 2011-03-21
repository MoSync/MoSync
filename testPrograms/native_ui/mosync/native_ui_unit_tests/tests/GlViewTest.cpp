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
