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
 * @file HelloOpenGLES.cpp
 * @author Mattias Frånberg and Chris Hughes
 *
 * This application provides a very basic example of how to use
 * OpenGL for Embedded Systems (OpenGLES) in your MoSync application.
 * It creates a spinning box on the screen with 3D shading. The code is
 * very well commented so that you can see exactly what's happening at
 * each step. The application makes use of MoSync's Moblet framework to
 * handle events.
 */

// Include MoSync syscalls.
#include <maapi.h>
#include <ma.h>
#include <maheap.h> //malloc() free()

// Include NativeUI so that we can create an OpenGL view.
#include <IX_WIDGET.h>

// Include header file for Moblets.
#include <MAUtil/Moblet.h>

// Include header file for OpenGL.
#include <GLES/gl.h>

// Include widget utility functions. These functions simplify
// getting and setting widget properties.
#include "WidgetUtil.h"

// Include resource identifiers (in this case the texture to be
// used as the surface of the spinning box).
#include "MAHeaders.h"

#include <conprint.h>
#include <maassert.h>

/**
 * A Moblet is the main object of MoSync application. In the Moblet
 * we manage the application and handle events.
 */
class GLBench :
public MAUtil::Moblet
{

	// First, we define the public methods.
public:

	// ================== Constructor ==================

	/**
	 * In the constructor we create the user interface.
	 */
	GLBench() :
		mGLViewInitialized(false),
		mXRotation(0.0f),
		mYRotation(0.0f),
		mZRotation(0.0f),
		mFrameCounter(0),
		mTimeSlot(0),
		mStartTime(0)
	{

		mT = 0; //starting time

		// Create a screen widget that will hold the OpenGL view.
		mScreen = maWidgetCreate(MAW_SCREEN);

		// Check if NativeUI is supported by the runtime platform.
		// For example, MoRE does not support NativeUI at the time
		// of writing this program.
		if (-1 == mScreen)
		{
			maPanic(0,
					"OpenGL is only available on Android and iPhone. "
					"You must run directly on the device or devices emulator.");
		}

		// Create a GL_VIEW widget and add it to the screen.
		// widgetSetPropertyInt is a helper function defined
		// in WidgetUtil.cpp.
		mGLView = maWidgetCreate(MAW_GL_VIEW);
		widgetSetPropertyInt(
				mGLView,
				MAW_WIDGET_WIDTH,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		widgetSetPropertyInt(
				mGLView,
				MAW_WIDGET_HEIGHT,
				MAW_CONSTANT_FILL_AVAILABLE_SPACE);
		maWidgetAddChild(mScreen, mGLView);

		// Show the screen.
		maWidgetScreenShow(mScreen);

		// Make the Moblet listen to custom events, so that we
		// know when the GLView widget is ready to be drawn.
		MAUtil::Environment::getEnvironment().addCustomEventListener(this);
	}

	// ================== Event methods ==================

	/**
	 * This method is called when a key is pressed. The method
	 * is inherited from the Moblet class, and is overridden here.
	 */
	void keyPressEvent(int keyCode, int nativeCode)
	{
		// Close the application if the back key or key 0 is pressed.
		if (MAK_BACK == keyCode || MAK_0 == keyCode)
		{
			// Call close to exit the application
			close();
		}
	}


	/*
	 * Print benchmark statistics to the screen
	 */
	void showStats()
	{
		printf("Resolution: %d x %d\n", mViewWidth, mViewHeight);
		printf("frames: %lu\n", mFrameCounter);
		printf("msecs: %lu\n", mTimeSlot);
		printf("FPS: %lu\n", mFrameCounter * 1000 / mTimeSlot);
	}

	/**
	 * Method that implements the custom event listener interface.
	 * Widget events are sent as custom events.
	 */
	void customEvent(const MAEvent& event)
	{
		// Check if this is a widget event.
		if (EVENT_TYPE_WIDGET == event.type)
		{
			// Get the widget event data structure.
			MAWidgetEventData* eventData = (MAWidgetEventData*) event.data;

			// MAW_EVENT_GL_VIEW_READY is sent when the GL view is
			// ready for drawing.
			if (MAW_EVENT_GL_VIEW_READY == eventData->eventType)
			{
				// Associate the OpenGL context with the GLView.
				maWidgetSetProperty(mGLView, MAW_GL_VIEW_BIND, "");

				// Create the texture we will use for rendering.
				createTexture();

				// Set the GL viewport.
				mViewWidth = widgetGetPropertyInt(mGLView, MAW_WIDGET_WIDTH);
				mViewHeight = widgetGetPropertyInt(mGLView, MAW_WIDGET_HEIGHT);
				setViewport(mViewWidth, mViewHeight);

				// Initialize OpenGL.
				initGL();

				// Flag that the GLView has been initialized.
				mGLViewInitialized = true;

				// Run tests
				mNumPlanes = 2; //start with 2 planes for the fill rate tests
				doTest(1);
				doTest(2);
				doTest(3);
				doTest(4);

				maWidgetScreenShow(MAW_CONSTANT_MOSYNC_SCREEN_HANDLE);
				maWidgetSetProperty(mGLView, MAW_WIDGET_VISIBLE, "false");
				maUpdateScreen();

			}
		}
	}

	/*
	 * Run performance tests
	 */
	void doTest(int test) {

		bool switch_dir = false; //switch direction
		mFrameCounter = 0;
		switch(test) {

		default: //fillrate test
			glEnable(GL_BLEND);
			glDisable(GL_TEXTURE_2D);
			mStartTime = maGetMilliSecondCount();
			while(true) {
				drawTextFillTest(1); // 0 equals texture fill test
				mFrameCounter += 1;
				if(mTimeSlot != 0 && (mFrameCounter * 1000 / mTimeSlot) > 30.0f){ //if FPS > 24
					mNumPlanes += 10; //double the number of planes
				}
				if(mYRotation > 2.99f) {
					switch_dir = true;
				}else if(mYRotation < -2.99f) {
					switch_dir = false;
				}
				if(switch_dir)
					mYRotation -= 0.6f/1000.0f * (maGetMilliSecondCount() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.3*20 units
				else
					mYRotation += 0.6f/1000.0f * (maGetMilliSecondCount() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.3*20 units

				if((mTimeSlot = maGetMilliSecondCount() - mStartTime) > 20000) {
					printf("-----------------------------\n");
					printf("Test #1: Fillrate (No texture)\n");
					showStats();
					printf("#planes (fill rate tests): %d", mNumPlanes);
					printf("#planes x FPS: %f\n", (float) mNumPlanes*((float) mFrameCounter / (float) mTimeSlot*1000.0f));
					mNumPlanes = 2; //reset the number of planes for the next fill rate test
					mTimeSlot = 0; //reset timer
					return;
				}

			}
			break;

		case 4: //texture test
			glEnable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
			mStartTime = maGetMilliSecondCount();
			while(true) {
				draw();
				mFrameCounter += 1;
				// Update rotation parameters.
				mXRotation += 1.0f;
				mYRotation += 0.8f;
				mZRotation += 0.6f;
				if((mTimeSlot = maGetMilliSecondCount() - mStartTime) > 20000) {
					printf("-----------------------------\n");
					printf("Test #4: Rotating textured box\n");
					showStats();
					return;
				}

			}
			break;

		case 3: //Dynamic Object test
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
			mStartTime = maGetMilliSecondCount();
			while(true) {
				drawDOTest();
				mFrameCounter += 1;
				if(mTimeSlot != 0 && (mFrameCounter * 1000 / mTimeSlot) > 30.0f){ //if FPS > 24
					mNumPolygons += 100; //double the number of planes
				}
				// Update rotation parameters.
				mXRotation += .3f;
				mYRotation += .24f;
				mZRotation += .18f;
				if((mTimeSlot = maGetMilliSecondCount() - mStartTime) > 20000) {
					printf("-----------------------------\n");
					printf("Test #3: Dynamic Object test\n");
					showStats();
					printf("#Vertices (dynamic object test): %d", mNumPolygons);
					printf("drawing time (msecs): %d\nobject coordinates calc time (msecs): %d", mGLTimeSlot, mMoTimeSlot);
					printf("drawing time/object coord calc time ratio: %f\n", (float) mGLTimeSlot/ (float) mMoTimeSlot);
					return;
				}

			}
			break;

		case 2: //Texture fill test
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_BLEND);
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f); //reset the color to white so that the texture don't get colored
			mStartTime = maGetMilliSecondCount();
			while(true) {
				drawTextFillTest(0); // 0 equals texture fill test
				mFrameCounter += 1;
				if(mTimeSlot != 0 && (mFrameCounter * 1000 / mTimeSlot) > 30.0f){ //if FPS > 24
					mNumPlanes += 10; //double the number of planes
				}
				if(mYRotation > 2.99f) {
					switch_dir = true;
				}else if(mYRotation < -2.99f) {
					switch_dir = false;
				}
				if(switch_dir)
					mYRotation -= 0.6f/1000.0f * (maGetMilliSecondCount() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.3*20 units
				else
					mYRotation += 0.6f/1000.0f * (maGetMilliSecondCount() - (mTimeSlot + mStartTime)); //if the test time is 20sec we rotate a total of 0.3*20 units

				if((mTimeSlot = maGetMilliSecondCount() - mStartTime) > 20000) {
					printf("-----------------------------\n");
					printf("Test #2: Fillrate (With texture)\n");
					showStats();
					printf("#planes (fill rate tests): %d", mNumPlanes);
					printf("#planes x FPS: %f\n", (float) mNumPlanes*((float) mFrameCounter / (float) mTimeSlot*1000.0f));
					mNumPlanes = 2; //reset the number of planes for the next fill rate test
					mTimeSlot = 0; //reset timer
					return;
				}

			}
			break;
		}

	}


	// Next, we define the private methods. These are methods
	// used within this class, not called from the framework.
private:

	// ================== OpenGL/rendering methods ==================

	/**
	 * Create the texture used for rendering.
	 */
	void createTexture()
	{
		// Create an OpenGL 2D texture from the R_BOX resource.
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &mBoxTextureHandle);
		glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);
		maOpenGLTexImage2D(R_BOX_TEXTURE);

		// Set texture parameters.
		glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	/**
	 * Setup the projection matrix.
	 */
	void setViewport(int width, int height)
	{
		// Protect against divide by zero.
		if (0 == height)
		{
			height = 1;
		}

		// Set viewport.
		glViewport(0, 0, (GLint)width, (GLint)height);

		// Select the projection matrix.
		glMatrixMode(GL_PROJECTION);

		// Reset the projection matrix.
		glLoadIdentity();

		// Set the perspective (updates the projection
		// matrix to use the perspective we define).
		GLfloat ratio = (GLfloat)width / (GLfloat)height;
		gluPerspective(45.0f, ratio, 0.1f, 100.0f);
	}

	/**
	 * Standard OpenGL initialization.
	 */
	void initGL()
	{
		// Enable texture mapping.
		glEnable(GL_TEXTURE_2D);

		// Enable smooth shading.
		glShadeModel(GL_SMOOTH);

		//enable blending (to make materials translucent)
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Set the depth value used when clearing the depth buffer.
		glClearDepthf(1.0f);

		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);

		// Set the type of depth test.
		glDepthFunc(GL_LEQUAL);

		// Use the best perspective correction method.
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		// Enable lighting and light source LIGHT0
		//glEnable(GL_LIGHTING);
		//glEnable(GL_LIGHT0);

		//GLfloat lightval[3] = {1.0f, 1.0f, 1.0f};
		//glLightfv(GL_LIGHT0, GL_SPECULAR, lightval);

	}

	/**
	 * Render the 3D model.
	 */
	void draw()
	{
		// The GL_View must be initialized before we can do any drawing.
		if (!mGLViewInitialized)
		{
			return;
		}

		// Array used for object coordinates.
		GLfloat vcoords[4][3];

		// Array used for texture coordinates.
		GLfloat tcoords[4][2];

		// Array used to convert from QUAD to TRIANGLE_STRIP.
		// QUAD is not available on the OpenGL implementation
		// we are using.
		GLubyte indices[4] = {0, 1, 3, 2};

		// Set the background color to be used when clearing the screen.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Clear the screen and the depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the model matrix.
		glMatrixMode(GL_MODELVIEW);

		// Reset the model matrix.

		glPushMatrix();
		glLoadIdentity();

		GLfloat position[4] = { -2.0f, -2.0f, 2.0f, 1.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		// Specify rotation along the X, Y, and Z axes.
		glRotatef(0, 1.0f, 0.0f, 0.0f);
		glRotatef(0, 0.0f, 1.0f, 0.0f);
		glRotatef(0, 0.0f, 0.0f, 1.0f);
		glPopMatrix();

		glPushMatrix();
		glLoadIdentity();
		// Move into the screen 5 units.
		glTranslatef(0.0f, 0.0f, -5.0f);

		// Specify rotation along the X, Y, and Z axes.
		glRotatef(mXRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
		glRotatef(mZRotation, 0.0f, 0.0f, 1.0f);

		// Select the texture to use when rendering the box.
		glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);

		// Set pointers to vertex coordinates and texture coordinates.
		glVertexPointer(3, GL_FLOAT, 0, vcoords);
		glTexCoordPointer(2, GL_FLOAT, 0, tcoords);

		// Enable vertex and texture coord arrays.
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		// Define the front face of the box.
		tcoords[0][0] = 1.0f;  tcoords[0][1] = 0.0f;
		vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = 1.0f;
		tcoords[1][0] = 0.0f;  tcoords[1][1] = 0.0f;
		vcoords[1][0] = 1.0f;  vcoords[1][1] = -1.0f; vcoords[1][2] = 1.0f;
		tcoords[2][0] = 0.0f;  tcoords[2][1] = 1.0f;
		vcoords[2][0] = 1.0f;  vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
		tcoords[3][0] = 1.0f;  tcoords[3][1] = 1.0f;
		vcoords[3][0] = -1.0f; vcoords[3][1] = 1.0f;  vcoords[3][2] = 1.0f;

		// This draws one textured plane using a strip of two triangles.
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

		// Define the back face of the box.
		tcoords[0][0] = 0.0f;  tcoords[0][1] = 0.0f;
		vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
		tcoords[1][0] = 0.0f;  tcoords[1][1] = 1.0f;
		vcoords[1][0] = -1.0f; vcoords[1][1] = 1.0f;  vcoords[1][2] = -1.0f;
		tcoords[2][0] = 1.0f;  tcoords[2][1] = 1.0f;
		vcoords[2][0] = 1.0f;  vcoords[2][1] = 1.0f;  vcoords[2][2] = -1.0f;
		tcoords[3][0] = 1.0f;  tcoords[3][1] = 0.0f;
		vcoords[3][0] = 1.0f;  vcoords[3][1] = -1.0f; vcoords[3][2] = -1.0f;

		// Draw the plane.
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

		// Define the top face of the box.
		tcoords[0][0] = 1.0f;  tcoords[0][1] = 1.0f;
		vcoords[0][0] = -1.0f; vcoords[0][1] = 1.0f; vcoords[0][2] = -1.0f;
		tcoords[1][0] = 1.0f;  tcoords[1][1] = 0.0f;
		vcoords[1][0] = -1.0f; vcoords[1][1] = 1.0f; vcoords[1][2] = 1.0f;
		tcoords[2][0] = 0.0f;  tcoords[2][1] = 0.0f;
		vcoords[2][0] = 1.0f;  vcoords[2][1] = 1.0f; vcoords[2][2] = 1.0f;
		tcoords[3][0] = 0.0f;  tcoords[3][1] = 1.0f;
		vcoords[3][0] = 1.0f;  vcoords[3][1] = 1.0f; vcoords[3][2] = -1.0f;

		// Draw the plane.
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

		// Define the bottom face of the box.
		tcoords[0][0] = 0.0f;  tcoords[0][1] = 1.0f;
		vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
		tcoords[1][0] = 1.0f;  tcoords[1][1] = 1.0f;
		vcoords[1][0] = 1.0f;  vcoords[1][1] = -1.0f; vcoords[1][2] = -1.0f;
		tcoords[2][0] = 1.0f;  tcoords[2][1] = 0.0f;
		vcoords[2][0] = 1.0f;  vcoords[2][1] = -1.0f; vcoords[2][2] = 1.0f;
		tcoords[3][0] = 0.0f;  tcoords[3][1] = 0.0f;
		vcoords[3][0] = -1.0f; vcoords[3][1] = -1.0f; vcoords[3][2] = 1.0f;

		// Draw the plane.
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

		// Define the right face of the box.
		tcoords[0][0] = 0.0f; tcoords[0][1] = 0.0f;
		vcoords[0][0] = 1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
		tcoords[1][0] = 0.0f; tcoords[1][1] = 1.0f;
		vcoords[1][0] = 1.0f; vcoords[1][1] = 1.0f;  vcoords[1][2] = -1.0f;
		tcoords[2][0] = 1.0f; tcoords[2][1] = 1.0f;
		vcoords[2][0] = 1.0f; vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
		tcoords[3][0] = 1.0f; tcoords[3][1] = 0.0f;
		vcoords[3][0] = 1.0f; vcoords[3][1] = -1.0f; vcoords[3][2] = 1.0f;

		// Draw the plane.
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

		// Define the left face of the box.
		tcoords[0][0] = 1.0f;  tcoords[0][1] = 0.0f;
		vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = -1.0f;
		tcoords[1][0] = 0.0f;  tcoords[1][1] = 0.0f;
		vcoords[1][0] = -1.0f; vcoords[1][1] = -1.0f; vcoords[1][2] = 1.0f;
		tcoords[2][0] = 0.0f;  tcoords[2][1] = 1.0f;
		vcoords[2][0] = -1.0f; vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
		tcoords[3][0] = 1.0f;  tcoords[3][1] = 1.0f;
		vcoords[3][0] = -1.0f; vcoords[3][1] = 1.0f;  vcoords[3][2] = -1.0f;

		// Draw the plane.
		glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

		// Disable texture and vertex arrays.
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// Wait (blocks) until all GL drawing commands to finish.
		glFinish();
		glPopMatrix();

		// Update the GLView.
		maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");
	}

	/*
	 * Draw lots of planes with different opacity (and with or without texture depending on the type param)
	 * on top of each other to test the fill rate of the GPU.
	 * */
	void drawTextFillTest(int type) {
		// The GL_View must be initialized before we can do any drawing.
		if (!mGLViewInitialized){
			return;
		}

		// Array used for object coordinates.
		//GLfloat fill_vcoords[40][3];
		GLfloat *fill_vcoords = (GLfloat*) malloc(4*mNumPlanes*3*sizeof(GLfloat));
		// Array used for coloring vertices
		GLfloat *colors = (GLfloat*) malloc(4*mNumPlanes*4*sizeof(GLfloat));
		// Array used for texture coordinates.
		GLfloat *tcoords = (GLfloat*) malloc(4*mNumPlanes*2*sizeof(GLfloat));

		int j = 0; //used to simulate second array index

		if(type == 0){ //texture test
			for(int i = 0; i < 4*mNumPlanes; ++i) { //fill the color array with colors
				colors[j] = 1.0f; // Go from maximum red to none
				colors[j+1] = 1.0f;
				colors[j+2] = 1.0f; // Go from no blue to maximum
				colors[j+3] = 1.0f - i*(0.25f/mNumPlanes); // Alpha value (from full opacity to almost none)
				j += 4;
			}
		}else{ //no texture
			for(int i = 0; i < 4*mNumPlanes; ++i) { //fill the color array with colors
				colors[j] = 1.0f - i*(0.5f/mNumPlanes); // Go from maximum red to none
				colors[j+1] = 0.0f;
				colors[j+2] = 0.0f + i*(0.5f/mNumPlanes); // Go from no blue to maximum
				colors[j+3] = 1.0f - i*(0.25f/mNumPlanes); // Alpha value (from full opacity to almost none)
				j += 4;
			}
		}

		// Array used to convert from QUAD to TRIANGLE_STRIP.
		// QUAD is not available on the OpenGL implementation
		// we are using.
		//GLubyte indices[4] = {0, 1, 3, 2};

		// Set the background color to be used when clearing the screen.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Clear the screen and the depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the model matrix.
		glMatrixMode(GL_MODELVIEW);

		// Reset the model matrix.
		glLoadIdentity();

		// Move into the screen 5 units.
		glTranslatef(0.0f, 0.0f, -10.0f);

		// Select the texture to use when rendering the box.
		glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);

		// Set pointers to vertex coordinates.
		glVertexPointer(3, GL_FLOAT, 0, fill_vcoords);
		glColorPointer(4, GL_FLOAT, 0, colors); //COLORS MUST BE DEFINED WITH RGBA IN THIS GL-IMPLEMENTATION HENCE THE 4.
		glTexCoordPointer(2, GL_FLOAT, 0, tcoords);

		// Enable vertex coord arrays and color array.
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);


		// Set the current color
		//glDisable(GL_DEPTH_TEST);

		// Define the front face of the box
		//GLfloat tdiff = 0.001;

		// Specify rotation
		glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
		j = 0;
		int k = 0;
		for(int i = 0; i < mNumPlanes; ++i) { // fill the vertex coord array
			//x									y								z
			fill_vcoords[j] = -0.5f; fill_vcoords[j+1] = -1.0f; fill_vcoords[j+2] = -1.0f + i*(20.0f/mNumPlanes); //go from inside the screen and outwards
			fill_vcoords[j+3] = 0.5f; fill_vcoords[j+4] = -1.0f; fill_vcoords[j+5] = -1.0f + i*(20.0f/mNumPlanes);
			fill_vcoords[j+9] = 0.5f; fill_vcoords[j+10] = 1.0f; fill_vcoords[j+11] = -1.0f + i*(20.0f/mNumPlanes);
			fill_vcoords[j+6] = -0.5f; fill_vcoords[j+7] = 1.0f; fill_vcoords[j+8] = -1.0f + i*(20.0f/mNumPlanes);
			tcoords[k] = -0.5f; tcoords[k+1] = -1.0f;
			tcoords[k+2] = 0.5f; tcoords[k+3] = -1.0f;
			tcoords[k+4] = 0.5f; tcoords[k+5] = 1.0f;
			tcoords[k+6] = -0.5f; tcoords[k+7] = 1.0f;
			glRotatef(mYRotation*0.1f, 0.0f, 1.0f, 0.0f); //turn each plane sideways

			j+=12;
			k+=8;

			// Do the drawing
			glDrawArrays(GL_TRIANGLE_STRIP, 4*i, 4); //draws the arrays enabled with glEnableClientState()
		}


		// Disable texture and vertex arrays.
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);

		// Wait (blocks) until all GL drawing commands to finish.
		glFinish();

		// Update the GLView.
		maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");

		// free all heap mem
		free(fill_vcoords);
		free(colors);
		free(tcoords);

	}

	/*
	 * Test the performance while drawing an object that changes it's form
	 * Measure CPU time and GPU time.
	 */
	void drawDOTest() {
		// The GL_View must be initialized before we can do any drawing.
		if (!mGLViewInitialized)
		{
			return;
		}

		// Allocate space for colors and vertice arrays
		GLfloat *colors = (GLfloat*) malloc(4*mNumPolygons*4*sizeof(GLfloat));
		GLfloat *vcoords = (GLfloat*) malloc(4*mNumPolygons*3*sizeof(GLfloat));


		for(int i = 0; i < mNumPolygons; ++i){
			if(i%3){
				colors[4*i+0] = 1.0f;
				colors[4*i+1] = 0.0f;
				colors[4*i+2] = 0.0f;
				colors[4*i+3] = 1.0f;
			}else if(i%2){
				colors[4*i+0] = 0.0f;
				colors[4*i+1] = 1.0f;
				colors[4*i+2] = 0.0f;
				colors[4*i+3] = 1.0f;
			}else{
				colors[4*i+0] = 0.0f;
				colors[4*i+1] = 0.0f;
				colors[4*i+2] = 1.0f;
				colors[4*i+3] = 1.0f;
			}

		}

		// Set the background color to be used when clearing the screen.
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

		// Clear the screen and the depth buffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use the model matrix.
		glMatrixMode(GL_MODELVIEW);

		// Reset the model matrix.
		glLoadIdentity();

		// Move into the screen 5 units.
		glTranslatef(0.0f, 0.0f, -3.0f);

		// Specify rotation along the X, Y, and Z axes.
		glRotatef(mXRotation, 1.0f, 0.0f, 0.0f);
		glRotatef(mYRotation, 0.0f, 1.0f, 0.0f);
		glRotatef(mZRotation, 0.0f, 0.0f, 1.0f);

		// Set pointers to vertex coordinates and colors.
		glVertexPointer(3, GL_FLOAT, 0, vcoords);
		glColorPointer(4, GL_FLOAT, 0, colors);

		// Enable vertex coord array and color array.
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// Set the current color
		//glDisable(GL_DEPTH_TEST);

		// Define the front face of the box
		GLfloat tdiff = 0.2;
		GLfloat phi, thet;

		//set the color
		//glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

		mT+=tdiff;

		if(mT > 1.0f){
			mT = 0.0f;
		}

		//start timing
		int tmpTime = maGetMilliSecondCount();

		for(int i = 0; i < mNumPolygons; ++i) {
			phi = i*2*3.14159/mNumPolygons;
			thet = phi/2.0f;
			vcoords[i*3+0] = sin(thet)*cos(phi); vcoords[i*3+1] = sin(thet)*sin(phi); vcoords[i*3+2] = cos(thet);
		}

		mMoTimeSlot += maGetMilliSecondCount() - tmpTime;
		tmpTime = maGetMilliSecondCount();

		// Draw the plane.
		glDrawArrays(GL_TRIANGLE_FAN, 0, mNumPolygons);

		// Disable texture and vertex arrays.
		//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_COLOR_ARRAY);

		// Wait (blocks) until all GL drawing commands to finish.
		glFinish();

		mGLTimeSlot += maGetMilliSecondCount() - tmpTime;

		// Update the GLView.
		maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");
		// free the allocated memory
		free(colors);
		free(vcoords);

	}

	/**
	 * Standard OpenGL utility function for setting up the
	 * perspective projection matrix.
	 */
	void gluPerspective(
			GLfloat fovy,
			GLfloat aspect,
			GLfloat zNear,
			GLfloat zFar)
	{
		const float M_PI = 3.14159;

		GLfloat ymax = zNear * tan(fovy * M_PI / 360.0);
		GLfloat ymin = -ymax;
		GLfloat xmin = ymin * aspect;
		GLfloat xmax = ymax * aspect;

		glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
	}

	// Finally, we declare the instance variables used within this class.
private:

	// ================== Instance variables ==================

	/** Handle to the GLView widget. */
	MAHandle mGLView;

	/** GLView state (true = initialized and ready to be drawn). */
	bool mGLViewInitialized;

	/** Handle to the texture. */
	GLuint mBoxTextureHandle;

	/** X axis rotation. */
	GLfloat mXRotation;

	/** Y axis rotation. */
	GLfloat mYRotation;

	/** Z axis rotation. */
	GLfloat mZRotation;

	/** msec counter */
	long long int mStartTime;

	/** frame draw counter */
	long int mFrameCounter;

	int mTimeSlot;
	int mMoTimeSlot;
	int mGLTimeSlot;

	/** current color */
	GLfloat mR, mG, mB;
	GLfloat mT;

	/** Screen handle and screen resolution */
	int mViewWidth;
	int mViewHeight;
	int mScreen;
	int mWaveIndex;
	GLfloat mVCoords[10000][3];
	GLfloat mColors[10000][4];

	int mNumPlanes; //the number of planes to draw in the fill tests (this will vary to make sure that the device will have to sweat!)
	int mNumPolygons; //the number of polygons in the dynamic object test (this will vary to make sure that the device will have to sweat!)

};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Start the application by creating and running a Moblet.
	MAUtil::Moblet::run(new GLBench());


	// The Moblet will run until it is closed by the user.
	// Returning zero indicates a controlled exit.
	return 0;
}
