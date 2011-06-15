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

/**
 * @file ScreenSpinningCube.h
 * @author TODO: Add your name.
 *
 * TODO: Add comment.
 */

#ifndef SCREEN_SPINNING_CUBE_H_
#define SCREEN_SPINNING_CUBE_H_

#include <GLES/gl.h>
#include <MAUtil/Environment.h>
#include "UIWrapper/Widgets.h"

using namespace MoSync::UI;

/**
 * TODO: Add comment.
 */
class ScreenSpinningCube : public WidgetEventListener, public MAUtil::TimerListener
{
private:
	bool mGlViewInitialized;
	GLfloat mXRotation;
	GLfloat mYRotation;
	GLfloat mZRotation;

	GLfloat mXDirection;
	GLfloat mYDirection;

	GLuint	mTextureHandle;

	GLView* mGLViewWidget;

public:
	/**
	 * TODO: Add comment.
	 */
	static Screen* create();

	/*
	 * Update the application
	 */
	void runTimerEvent();

	/**
	 * This method is called when there is an event for this widget.
	 * @param widget The widget object of the event.
	 * @param widgetEventData The low-level event data.
	 */
	void handleWidgetEvent(Widget* widget, MAWidgetEventData* widgetEventData);

private:
	/*
	 * Constructor
	 */
	ScreenSpinningCube();

	static const GLfloat mLightAmbient[];
	static const GLfloat mLightDiffuse[];
	static const GLfloat mLightPosition[];

	/*
	 * Method for initializing open gl
	 */
	void initGL();

	/*
	 * Resize And Initialize The GL Window
	 */
	void resizeGLScene(int width, int height);

	/*
	 * Standard OpenGL code for setting up a perspective projection matrix.
	 */
	void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

	/*
	 * Method for drawing into the open GL screen
	 */
	void drawGLScene();

//	/*
//	 * Update the application
//	 */
//	void runTimerEvent();

};

#endif
