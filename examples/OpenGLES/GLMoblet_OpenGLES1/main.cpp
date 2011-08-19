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

#include <MAUtil/GLMoblet.h>
#include <GLES/gl.h>

using namespace MAUtil;

/**
 * A simple OpenGL|ES 1.1 GLMoblet application that
 * draws a rotating red square. Shows how to
 * setup the viewport, a perspective projection matrix,
 * initialize the gl state and do some simple drawing.
 */

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

/**
 * Default OpenGL initialization.
 */
void initGL()
{
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

class MyGLMoblet : public GLMoblet {
private:
public:
	MyGLMoblet() : GLMoblet(GLMoblet::GL1) {
	}

	// setup viewport with a default perspective matrix and viewport.
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

	void init() {
		setViewport(EXTENT_X(maGetScrSize()), EXTENT_Y(maGetScrSize()));
		initGL();
	}

	void draw() {
	    float zrotation = maGetMilliSecondCount()*0.06f;

	    // Array used for object coordinates.
	    GLfloat vcoords[4][3];

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
	    glRotatef(zrotation, 0.0f, 0.0f, 1.0f);

	    glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

	    // Set pointers to vertex coordinates and texture coordinates.
	    glVertexPointer(3, GL_FLOAT, 0, vcoords);

	    // Enable vertex arrays.
	    glEnableClientState(GL_VERTEX_ARRAY);

	    // Define the front face of the box.
	    vcoords[0][0] = -1.0f; vcoords[0][1] = -1.0f; vcoords[0][2] = 1.0f;
	    vcoords[1][0] = 1.0f;  vcoords[1][1] = -1.0f; vcoords[1][2] = 1.0f;
	    vcoords[2][0] = -1.0f; vcoords[2][1] = 1.0f;  vcoords[2][2] = 1.0f;
	    vcoords[3][0] = 1.0f;  vcoords[3][1] = 1.0f;  vcoords[3][2] = 1.0f;

	    // This draws one textured plane using a strip of two triangles.
	    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	    // Disable vertex arrays.
	    glDisableClientState(GL_VERTEX_ARRAY);

	    // Wait (blocks) until all GL drawing commands to finish.
	    glFinish();
	}
};

/**
 * Entry point of the program. The MAMain function
 * needs to be declared as extern "C".
 */
extern "C" int MAMain()
{
	Moblet::run(new MyGLMoblet());
	return 0;
}
