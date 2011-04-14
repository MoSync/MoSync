/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

/** @file helloopengles.cpp
*
* This application provides a very basic example of how to use OpenGL for
* Embedded Systems (OpenGLES) in your MoSync application. It creates a spinning
* box on the screen with 3D shading. The code is very well commented
* so that you can see exactly what's happening at each step.
* The application makes use of MoSync's Moblet framework to handle events.
*
* IMPORTANT! THIS APPLICATION CURRENTLY ONLY WORKS ON ANDROID AND IOS DEVICES.
* IT WILL NOT WORK ON THE MOSYNC EMULATOR, AS THE EMULATOR HAS NO NATIVE UI.
*
* @author Mattias Frånberg and Chris Hughes
*/

//Include the header files for MoSync Moblets and OpenGL so that we can
//access their libraries from our application.
#include <MAUtil/Moblet.h>
#include <GLES/gl.h>

//Include NativeUI so that we can create an OpenGL view widget.
#include <IX_WIDGET.h>

//Include MoSync syscall collection.
#include <maapi.h>

//Include our project's widget utilities. These utilities simplify
//getting and setting widget properties.
#include "widgetutil.h"

//Include our resources (in this case a texture to be used as the
//surface of our box).
#include "MAHeaders.h"

//Standard OpenGL code for setting up a perspective projection matrix.
void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
 GLfloat xmin, xmax, ymin, ymax;

 const float M_PI = 3.14159;

   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;

   glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

//Standard OpenGL code for initialization.
void initGL(GLvoid)
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
    glEnable(GL_DEPTH_TEST);

    /* The Type Of Depth Test To Do */
    glDepthFunc(GL_LEQUAL);

    /* Really Nice Perspective Calculations */
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

//Standard OpenGL code for setting camera angle.
void resizeWindow(int width, int height)
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

//Create the wrapper for the entire application. It is here that we will manage
//the application and handle events.
class MyMoblet : public MAUtil::Moblet, MAUtil::TimerListener {

//Define our new class's public methods.
public:

	//First, the constructor.
	MyMoblet()
	: mGlViewInitialized( false ),
	  mXRotation( 0.0f ),
	  mYRotation( 0.0f ),
	  mZRotation( 0.0f )
	{
		//Create a screen using NativeUI. 
		int screen = maWidgetCreate( MAW_SCREEN );
		
		//Check if this is supported on the device it's running on
		if(screen == -1)
		{
			maPanic(0,"Sorry but this program does not run currently on this platform");
		}
		
		//Create an OpenGL widget within the previously created screen.
		//Set the widget's width and height: -1 means the whole screen.
		//Make the widget a child of the screen, then show it.
		mGLView = maWidgetCreate( MAW_GL_VIEW );
		maWidgetSetPropertyInt( mGLView, MAW_WIDGET_WIDTH, -1 );
		maWidgetSetPropertyInt( mGLView, MAW_WIDGET_HEIGHT, -1 );
		maWidgetAddChild( screen, mGLView );
		maWidgetScreenShow( screen );

		//Make the moblet listen to custom events, so that we can know when our
		//GLView widget is ready to be drawn.
		MAUtil::Environment::getEnvironment( ).addCustomEventListener( this );

		//Make the moblet draw every 20 milliseconds.
		MAUtil::Environment::getEnvironment( ).addTimer( this, 20, -1 );
	}

	//Next, a method for detecting key presses. This is a method we have
	//inherited from the Moblet base class, and here we will override that
	//method with some processing of our own.
	void keyPressEvent(int keyCode, int nativeCode)
	{
		//Close the application if key 0 or the back key is pressed.
		if(keyCode == MAK_0 || keyCode == MAK_BACK)
		{
			close();
		}
	}

	//Now we implement the custom event listener interface.
	void customEvent(const MAEvent& event)
	{
		//Ignore events that do not come from widgets.
		if( event.type != EVENT_TYPE_WIDGET )
		{
			return;
		}

		// Check if the GLView is ready to be drawn.
		MAWidgetEventData *eventData = (MAWidgetEventData *) event.data;
		if( eventData->eventType == MAW_EVENT_GL_VIEW_READY )
		{
			//Make the OpenGL context associated with the GLView active.
			maWidgetSetProperty(mGLView, MAW_GL_VIEW_BIND, "");

			//Create an OpenGL 2D texture from the R_BOX resource.
			glEnable(GL_TEXTURE_2D);
			glGenTextures(1, &mBoxTextureHandle);
			glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);
			maOpenGLTexImage2D(R_BOX);

			//Set texture parameters.
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			//Get width and height of the GLView widget, and resize.
			int glViewWidth = maWidgetGetPropertyInt(mGLView, MAW_WIDGET_WIDTH);
			int glViewHeight = maWidgetGetPropertyInt(mGLView, MAW_WIDGET_HEIGHT);
			resizeWindow(glViewWidth, glViewHeight);

			//Initialize OpenGL.
			initGL();

			//Commit the changes to GLView ("invalidate" tells the screen
			//that it is out-of-date and needs to be redrawn).
			maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");

			//Record that the GLView has been initialized.
			mGlViewInitialized = true;
		}
	}

	void runTimerEvent()
	{
		// Only draw when the GLView is initialized
		if( !mGlViewInitialized )
		{
			return;
		}

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
	    glBindTexture(GL_TEXTURE_2D, mBoxTextureHandle);

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

	    mXRotation+=1.0f; /* X Axis Rotation */
	    mYRotation+=0.8f; /* Y Axis Rotation */
	    mZRotation+=0.6f; /* Z Axis Rotation */

		//Commit the changes to GLView.
	    maWidgetSetProperty(mGLView, MAW_GL_VIEW_INVALIDATE, "");
	}

private:
	//Handle to the GLView widget.
	MAHandle mGLView;

	//GLView state (true = initialized and ready to be drawn).
	bool mGlViewInitialized;

	//Handle for the box texture.
	GLuint mBoxTextureHandle;

	//Rotation angles.
	GLfloat mXRotation;
	GLfloat mYRotation;
	GLfloat mZRotation;
};

//The entry point for the application - the place where processing starts.
extern "C" int MAMain() {

	//Create the instance of MyMoblet
	MyMoblet myMoblet;

	// Run the Moblet to start the application.
	MAUtil::Moblet::run( &myMoblet );

	//MyMoblet will run until it is closed by the user pressing key 0. When
	//it's closed we end our program in a well-behaved way by returning zero.
	return 0;
};
