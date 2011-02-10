#include <maapi.h>

#include <mavsprintf.h>

#include <IX_WIDGET.h>
#include <GLES/gl.h>

#include <mastdlib.h>
#include "MAHeaders.h"

int maWidgetSetPropertyInt(MAHandle handle, const char *property, int value)
{
	char buffer[256];
	sprintf( buffer, "%d", value );
	maWidgetSetProperty( handle, property, buffer );
}

int maWidgetGetPropertyInt(MAHandle handle, const char *property) {
	char buffer[256];
	maWidgetGetProperty( handle, property, buffer, 256);

	lprintfln("buffer: %s\n", buffer);

	return atoi(buffer);
}

int createListScreen();
int createWebScreen();
int createScreen(const char *title, int icon, const char *text);
int createOpenGLScreen(int& openglView);


void
gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar)
{
	GLfloat xmin, xmax, ymin, ymax;

	const float M_PI = 3.14159;

   ymax = zNear * tan(fovy * M_PI / 360.0);
   ymin = -ymax;
   xmin = ymin * aspect;
   xmax = ymax * aspect;


   glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

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

    /* Setup our viewport. */
    glViewport(0, 0, (GLint)width, (GLint)height);

    /*
     * change to the projection matrix and set
     * our viewing volume.
     */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    /* Set our perspective */
    gluPerspective(45.0f, ratio, 0.1f, 100.0f);

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode(GL_MODELVIEW);

    /* Reset The View */
    glLoadIdentity();
}

void initGL(GLvoid)
{
    /* Enable Texture Mapping ( NEW ) */
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

GLfloat xrot = 0.0f;
GLfloat yrot = 0.0f;
GLfloat zrot = 0.0f;
GLuint textureHandle;

/* Here goes our drawing code */
void drawGLScene(GLvoid)
{
    /* These are to calculate our fps */
    static GLint T0=0;
    static GLint Frames=0;
    GLfloat texcoords[4][2];
    GLfloat vertices[4][3];
    GLubyte indices[4]={0, 1, 3, 2}; /* QUAD to TRIANGLE_STRIP conversion; */

    /* Clear The Screen And The Depth Buffer */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /* Move Into The Screen 5 Units */
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    glRotatef(xrot, 1.0f, 0.0f, 0.0f); /* Rotate On The X Axis */
    glRotatef(yrot, 0.0f, 1.0f, 0.0f); /* Rotate On The Y Axis */
    glRotatef(zrot, 0.0f, 0.0f, 1.0f); /* Rotate On The Z Axis */

    /* Select Our Texture */
    glBindTexture(GL_TEXTURE_2D, textureHandle);

    /* Set pointers to vertices and texcoords */
    glVertexPointer(3, GL_FLOAT, 0, vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

    /* Enable vertices and texcoords arrays */
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    /* Front Face */
    texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=1.0f;
    texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Back Face */
    /* Normal Pointing Away From Viewer */
    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=-1.0f;
    texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=1.0f; vertices[3][1]=-1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Top Face */
    texcoords[0][0]=1.0f; texcoords[0][1]=1.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=0.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Bottom Face */
    texcoords[0][0]=0.0f; texcoords[0][1]=1.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=1.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=-1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=0.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=-1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=0.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Right face */
    texcoords[0][0]=0.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=1.0f;  vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=0.0f; texcoords[1][1]=1.0f;
    vertices[1][0]=1.0f;  vertices[1][1]=1.0f; vertices[1][2]=-1.0f;
    texcoords[2][0]=1.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=1.0f;  vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=1.0f; texcoords[3][1]=0.0f;
    vertices[3][0]=1.0f;  vertices[3][1]=-1.0f; vertices[3][2]=1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Left Face*/
    texcoords[0][0]=1.0f; texcoords[0][1]=0.0f;
    vertices[0][0]=-1.0f; vertices[0][1]=-1.0f; vertices[0][2]=-1.0f;
    texcoords[1][0]=0.0f; texcoords[1][1]=0.0f;
    vertices[1][0]=-1.0f; vertices[1][1]=-1.0f; vertices[1][2]=1.0f;
    texcoords[2][0]=0.0f; texcoords[2][1]=1.0f;
    vertices[2][0]=-1.0f; vertices[2][1]=1.0f; vertices[2][2]=1.0f;
    texcoords[3][0]=1.0f; texcoords[3][1]=1.0f;
    vertices[3][0]=-1.0f; vertices[3][1]=1.0f; vertices[3][2]=-1.0f;

    /* Draw one textured plane using two stripped triangles */
    glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_BYTE, indices);

    /* Disable texcoords and vertices arrays */
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);

    /* Flush all drawings */
    glFinish();

    xrot+=1.0f; /* X Axis Rotation */
    yrot+=0.8f; /* Y Axis Rotation */
    zrot+=0.6f; /* Z Axis Rotation */
}

/*
 * main.cpp
 *
 *  Created on: Dec 8, 2010
 *      Author: fmattias
 */
extern "C" int MAMain()
{
	int screenHeight = EXTENT_Y( maGetScrSize( ) );

	int tabScreen = maWidgetCreate( WIDGET_TYPE_TAB_SCREEN );

	int listScreen = createListScreen( );
	maWidgetAddChild( tabScreen, listScreen );

	int webScreen = createWebScreen( );
	maWidgetAddChild( tabScreen, webScreen );

	int startScreen = createScreen( "Start", R_START, "Start..." );
	maWidgetAddChild( tabScreen, startScreen );

	int resultsScreen = createScreen( "Results", R_RESULTS, "Results..." );
	maWidgetAddChild( tabScreen, resultsScreen );

	int openglView;
	int openglScreen = createOpenGLScreen(openglView);
	maWidgetAddChild(tabScreen, openglScreen);

	maWidgetScreenShow( tabScreen );



	maWidgetSetProperty(openglView, "bind", "");

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &textureHandle);
	glBindTexture(GL_TEXTURE_2D, textureHandle);
	maOpenGLTexImage2D(R_TEXTURE);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int oglWidth = maWidgetGetPropertyInt(openglView, "width");
	int oglHeight = maWidgetGetPropertyInt(openglView, "height");

	lprintfln("w: %d, h: %d\n");

	resizeWindow(oglWidth, oglHeight);
	initGL();

	int currentTab = 0;

	// Wait for close event
	while(1)
	{
		MAEvent event;

		//maWait( 0 );


		if(currentTab == 4) {
			maWidgetSetProperty(openglView, "bind", "");

			float time = (float)(maGetMilliSecondCount()%1000) * 0.001f;

			//glClearColorx(maGetMilliSecondCount(), 0xffff, 0, 0xffff);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			drawGLScene();

			maWidgetSetProperty(openglView, "invalidate", "");
		}

		while(maGetEvent( &event ) != 0) {
			if( event.type == EVENT_TYPE_FOCUS_LOST )
			{
				maExit( 0 );
			}
			if( event.type == EVENT_TYPE_WIDGET) {

				MAWidgetEventData* data = (MAWidgetEventData*)event.data;

				if(data->eventType == WIDGET_EVENT_TAB_CHANGED) {
					currentTab = data->tabIndex;
				}

			}
			else if(event.type == EVENT_TYPE_CLOSE) {
				maExit( 0 );
			}
		}
	}
}

int createListScreen()
{
	int listScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( listScreen, "title", "RSS" );
	maWidgetSetPropertyInt( listScreen, "icon", R_NEWS );

	// Create List
	int list = maWidgetCreate( WIDGET_TYPE_LIST_VIEW );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_HEIGHT, -1 );
	maWidgetSetPropertyInt( list, WIDGET_PROPERTY_WIDTH, -1 );

	// Add 10 buttons to the list
	for(int i = 0; i < 10; i++)
	{
		int listItem = maWidgetCreate( WIDGET_TYPE_LIST_VIEW_ITEM );
		maWidgetSetProperty( listItem, "text", "Click me" );
		maWidgetSetPropertyInt( listItem, "icon", R_ICON );
		maWidgetAddChild( list, listItem );
	}

	// Add the tree to the root
	maWidgetAddChild( listScreen, list );

	return listScreen;
}

int createWebScreen()
{
	int webScreen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( webScreen, "title", "Web" );
	maWidgetSetPropertyInt( webScreen, "icon", R_TV );

	int webView = maWidgetCreate( WIDGET_TYPE_WEB_VIEW );
	maWidgetSetPropertyInt( webView, WIDGET_PROPERTY_WIDTH, -1 );
	maWidgetSetPropertyInt( webView, WIDGET_PROPERTY_HEIGHT, -1 );
	maWidgetSetProperty( webView, "url", "http://www.google.se/" );

	maWidgetAddChild( webScreen, webView );

	return webScreen;
}

int createOpenGLScreen(int& openglView)
{
	int screen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( screen, "title", "OpenGL" );
	openglView = maWidgetCreate(WIDGET_TYPE_GL_VIEW);
	maWidgetAddChild( screen, openglView );
	return screen;
}

int createScreen(const char *title, int icon, const char *text)
{
	int screen = maWidgetCreate( WIDGET_TYPE_SCREEN );
	maWidgetSetProperty( screen, "title", title );
	maWidgetSetPropertyInt( screen, "icon", icon );

	int textView = maWidgetCreate( WIDGET_TYPE_LABEL );
	maWidgetSetProperty( textView, WIDGET_PROPERTY_LABEL_TEXT, text );

	maWidgetAddChild( screen, textView );

	return screen;
}
