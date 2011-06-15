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
 * @file ScreenSpinningCube.cpp
 * @author TODO: Add your name.
 *
 * TODO: Add comment.
 */

#include "MAHeaders.h"
#include "ScreenSpinningCube.h"

/**
 * TODO: Add comment.
 */
Screen* ScreenSpinningCube::create()
{
	Screen* screen = new Screen();
	screen->setTitle("Cube");
	if (WidgetManager::isAndroid())
	{
		screen->setIcon(RES_TAB_ICON_CUBE_ANDROID);
	}
	else
	{
		screen->setIcon(RES_TAB_ICON_CUBE);
	}
	mGLViewWidget = new GLView();
	screen->setMainWidget(mGLViewWidget);

	return screen;
}

ScreenSpinningCube::ScreenSpinningCube():
	mGlViewInitialized(false),
	mXRotation(0), mYRotation(0), mZRotation(0)
{
}

const GLfloat ScreenSpinningCube::mLightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
const GLfloat ScreenSpinningCube::mLightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat ScreenSpinningCube::mLightPosition[]=		{ 0.0f, 0.0f, 2.0f, 1.0f };

/*
 * Method for initializing open gl
 */
void ScreenSpinningCube::initGL()
{
	/* Enable Texture Mapping */
	glEnable(GL_TEXTURE_2D);

	/* Enable smooth shading */
	glShadeModel(GL_SMOOTH);

	/* Set the background black */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);

	/* Depth buffer setup */
	glClearDepthf(1.0f);

	/* Enables Depth Testing */
	//glEnable(GL_DEPTH_TEST);

	/* The Type Of Depth Test To Do */
	glDepthFunc(GL_LEQUAL);

	/* Turn Blending on */
	glEnable(GL_BLEND);

	/* Really Nice Perspective Calculations */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glLightfv(GL_LIGHT1, GL_AMBIENT, mLightAmbient);		// Setup The Ambient Light
	glLightfv(GL_LIGHT1, GL_DIFFUSE, mLightDiffuse);		// Setup The Diffuse Light
	glLightfv(GL_LIGHT1, GL_POSITION,mLightPosition);	// Position The Light
	glEnable(GL_LIGHT1);								// Enable Light One

	glColor4f(1.0f, 1.0f, 1.0f, 0.5);					// Full Brightness.  50% Alpha
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
}

/*
 * Resize And Initialize The GL Window
 */
void ScreenSpinningCube::resizeGLScene(int width, int height)
{
	/* Height / width ration */
	GLfloat ratio;

	/* Protect against a divide by zero */
	if (height==0)
	{
		height = 1;
	}

	ratio=(GLfloat)width/(GLfloat)height;

	/* Setup our view port. */
	glViewport(0, 0, (GLint)width, (GLint)height);

	/*
	 * Change to the projection matrix and set
	 * our viewing volume.
	 */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* Set our perspective */
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	/* Make sure we're changing the model view and not the projection */
	glMatrixMode(GL_MODELVIEW);

	/* Reset The View */
	glLoadIdentity();
}

/*
 * Standard OpenGL code for setting up a perspective projection matrix.
 */
void ScreenSpinningCube::gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLfloat xmin, xmax, ymin, ymax;

	const float M_PI = 3.14159;

	ymax = zNear * tan(fovy * M_PI / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

/*
 * Method for drawing into the open GL screen
 */
void ScreenSpinningCube::drawGLScene()
{
	//Calculate the frames per second.
	GLfloat texcoords[4][2];
	GLfloat vertices[4][3];
	GLubyte indices[4]={0, 1, 3, 2}; /* QUAD to TRIANGLE_STRIP conversion; */

	//Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Move into the screen 5 units.
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -5.0f);

	glRotatef(mXRotation, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis */
	glRotatef(mYRotation, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis */
	glRotatef(mZRotation, 0.0f, 0.0f, 1.0f); /* Rotate On The Z Axis */

	//Select our texture.
	glBindTexture(GL_TEXTURE_2D, mTextureHandle);

	//Set pointers to vertices and texcoords.
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	//Enable vertices and texcoords arrays.
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Front face.
	texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
	vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
	texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
	vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
	texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
	vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
	texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
	vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;

	//Draw one textured plane using two stripped triangles.
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

	//Back face.
	//Normal pointing away from viewer.
	texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
	vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
	texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
	vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
	texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
	vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
	texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
	vertices[3][0]=1.0f; vertices[3][1]=-1.0f; vertices[3][2]=-1.0f;

	//Draw one textured plane using two stripped triangles.
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

	//Top Face.
	texcoords[0][0]=1.0f; texcoords[0][1]=1.0f;
	vertices[0][0]=-1.0f; vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
	texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
	vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=1.0f;
	texcoords[2][0]=0.0f; texcoords[2][1]=0.0f;
	vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
	texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
	vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

	//Draw one textured plane using two stripped triangles.
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

	//Bottom Face.
	texcoords[0][0]=0.0f; texcoords[0][1]=1.0f;
	vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
	texcoords[1][0]=1.0f; texcoords[1][1]=1.0f;
	vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
	texcoords[2][0]=1.0f; texcoords[2][1]=0.0f;
	vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=1.0f;
	texcoords[3][0]=0.0f; texcoords[3][1]=0.0f;
	vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

	//Draw one textured plane using two stripped triangles.
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

	//Right face.
	texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
	vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
	texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
	vertices[1][0]=1.0f;  vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
	texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
	vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
	texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
	vertices[3][0]=1.0f;  vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

	//Draw one textured plane using two stripped triangles.
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

	//Left Face.
	texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
	vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
	texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
	vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
	texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
	vertices[2][0]=-1.0f; vertices[2][1]=1.0f; vertices[2][2]=1.0f;
	texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
	vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

	//Draw one textured plane using two stripped triangles.
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

	//Disable texcoords and vertices arrays.
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	//Flush all drawings.
	glFinish();

	mXRotation += 0.5f;//mXDirection; /* X Axis Rotation */
	mYRotation += 0.5f;//mYDirection; /* Y Axis Rotation */
	mZRotation += 0.5f; /* Z Axis Rotation */

	//Commit the changes to GLView.
	mGLViewWidget->setProperty(MAW_GL_VIEW_INVALIDATE, "");
}

/*
 * Update the application
 */
void ScreenSpinningCube::runTimerEvent()
{
	// if gl view is active, redraw it
	if( mGlViewInitialized )
	{
		drawGLScene();
	}
}

/**
 * This method is called when there is an event for this widget.
 * @param widget The widget object of the event.
 * @param widgetEventData The low-level event data.
 */
void ScreenSpinningCube::handleWidgetEvent(Widget* widget, MAWidgetEventData* widgetEventData)
{
	if( widgetEventData->eventType == MAW_EVENT_GL_VIEW_READY )
	{
		//Make the OpenGL context associated with the GLView active.
		mGLViewWidget->setProperty(MAW_GL_VIEW_BIND, "");

		//Create an OpenGL 2D texture from the R_BOX resource.
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &mTextureHandle);
		glBindTexture(GL_TEXTURE_2D, mTextureHandle);
		maOpenGLTexImage2D(RES_IMAGE_TEX);

		//Set texture parameters.
		glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Get width and height of the GLView widget, and resize.
		int glViewWidth = mGLViewWidget->getProperty(MAW_WIDGET_WIDTH);
		int glViewHeight = mGLViewWidget->getProperty(MAW_WIDGET_HEIGHT);
		resizeGLScene(glViewWidth, glViewHeight);

		//Initialize OpenGL.
		initGL();

		//Commit the changes to GLView ("invalidate" tells the screen
		//that it is out-of-date and needs to be redrawn).
		mGLViewWidget->setProperty(MAW_GL_VIEW_INVALIDATE, "");

		//Record that the GLView has been initialized.
		mGlViewInitialized = true;
	}
}
