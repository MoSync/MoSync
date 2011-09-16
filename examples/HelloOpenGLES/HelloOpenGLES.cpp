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

/**
 * A Moblet is the main object of MoSync application. In the Moblet
 * we manage the application and handle events.
 */
class HelloGLMoblet :
	public MAUtil::Moblet,
	public MAUtil::TimerListener
{

// First, we define the public methods.
public:

	// ================== Constructor ==================

	/**
	 * In the constructor we create the user interface.
	 */
	HelloGLMoblet() :
		mGLViewInitialized(false),
		mXRotation(0.0f),
		mYRotation(0.0f),
		mZRotation(0.0f)
	{
		// Create a screen widget that will hold the OpenGL view.
		int screen = maWidgetCreate(MAW_SCREEN);

		// Check if NativeUI is supported by the runtime platform.
		// For example, MoRE does not support NativeUI at the time
		// of writing this program.
		if (-1 == screen)
		{
			maPanic(0, "OpenGL is only available on Android and iOS.");
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
		maWidgetAddChild(screen, mGLView);

		// Show the screen.
		maWidgetScreenShow(screen);

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
			// Call close to exit the application.
			close();
		}
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
				int viewWidth = widgetGetPropertyInt(mGLView, MAW_WIDGET_WIDTH);
				int viewHeight = widgetGetPropertyInt(mGLView, MAW_WIDGET_HEIGHT);
				setViewport(viewWidth, viewHeight);

				// Initialize OpenGL.
				initGL();

				// Flag that the GLView has been initialized.
				mGLViewInitialized = true;

				// Draw the initial scene.
				draw();

				// Start timer that will redraw the scene.
				// This calls runTimerEvent each 20 ms.
				MAUtil::Environment::getEnvironment().addTimer(this, 20, -1);
			}
		}
	}

	/**
	 * Called on a timer event. Implements the interface in TimerListener.
	 */
	void runTimerEvent()
	{
		// Draw the 3D scene.
		draw();

	    // Update rotation parameters.
	    mXRotation += 1.0f;
	    mYRotation += 0.8f;
	    mZRotation += 0.6f;
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

		// Set the depth value used when clearing the depth buffer.
		glClearDepthf(1.0f);

		// Enable depth testing.
		glEnable(GL_DEPTH_TEST);

		// Set the type of depth test.
		glDepthFunc(GL_LEQUAL);

		// Use the best perspective correction method.
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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
	    glClearColor(0.3f, 0.0f, 0.0f, 1.0f);

	    // Clear the screen and the depth buffer.
	    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	    // Use the model matrix.
	    glMatrixMode(GL_MODELVIEW);

	    // Reset the model matrix.
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

		// Update the GLView.
	    maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");
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
};

/**
 * Main function that is called when the program starts.
 */
extern "C" int MAMain()
{
	// Start the application by creating and running a Moblet.
	MAUtil::Moblet::run(new HelloGLMoblet());

	// The Moblet will run until it is closed by the user.
	// Returning zero indicates a controlled exit.
	return 0;
}
