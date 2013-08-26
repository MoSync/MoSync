/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/** \file SimpleRenderer.cpp
*
* This file contains the implementation for a simple
* renderer that draws a finite plane (usually called a
* quad).
*
* \author Niklas Nummelin
*
*/


#include "SimpleRenderer.h"
#include <gles/gl.h>
#include <madmath.h>

//-------------------------------------------------------
// Renderer
//-------------------------------------------------------

void Renderer::setRotation(float x, float y, float z) {
	mRotationX = x;
	mRotationY = y;
	mRotationZ = z;
}

//-------------------------------------------------------
// SimpleRenderer
//-------------------------------------------------------

/**
 * Standard OpenGL utility function for setting up the
 * perspective projection matrix.
 */
void gluPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
	GLfloat ymax = zNear * tan(fovy * M_PI / 360.0);
	GLfloat ymin = -ymax;
	GLfloat xmin = ymin * aspect;
	GLfloat xmax = ymax * aspect;

	glFrustumf(xmin, xmax, ymin, ymax, zNear, zFar);
}

// setup viewport with a default perspective matrix and viewport.
void setViewport(int width, int height) {
	// Protect against divide by zero.
	if (0 == height) {
		height = 1;
	}

	// Set viewport.
	glViewport(0, 0, (GLint) width, (GLint) height);

	// Select the projection matrix.
	glMatrixMode( GL_PROJECTION);

	// Reset the projection matrix.
	glLoadIdentity();

	// Set the perspective (updates the projection
	// matrix to use the perspective we define).
	GLfloat ratio = (GLfloat) width / (GLfloat) height;
	gluPerspective(45.0f, ratio, 0.1f, 100.0f);
}

/**
 * Default OpenGL initialization.
 */
void initGL() {
	// Enable smooth shading.
	glShadeModel( GL_SMOOTH);

	// Set the depth value used when clearing the depth buffer.
	glClearDepthf(1.0f);

	// Enable depth testing.
	glEnable( GL_DEPTH_TEST);

	// Set the type of depth test.
	glDepthFunc( GL_LEQUAL);

	// Use the best perspective correction method.
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void SimpleRenderer::init(int width, int height) {
	setViewport(width, height);
	initGL();
}

void SimpleRenderer::draw() {
	// Set the background color to be used when clearing the screen.
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Clear the screen and the depth buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use the model matrix.
	glMatrixMode( GL_MODELVIEW);

	// Reset the model matrix.
	glLoadIdentity();

	// Move into the screen 5 units.
	glTranslatef(0.0f, 0.0f, -5.0f);

	// Specify rotation along the X, Y, and Z axes.
	glRotatef(mRotationX, 1.0f, 0.0f, 0.0f);
	glRotatef(mRotationY, 0.0f, 1.0f, 0.0f);
	glRotatef(mRotationZ, 0.0f, 0.0f, 1.0f);


	// Set up the vertex array.
	GLfloat vcoords[4*3] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,
	};

	// Set up the color array.
	GLfloat colors[4*4] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f
	};

	// Set pointers to vertex coordinates and colors.
	glVertexPointer(3, GL_FLOAT, 0, vcoords);
	glColorPointer(4, GL_FLOAT, 0, colors);

	// Enable vertex arrays.
	glEnableClientState( GL_VERTEX_ARRAY );

	// Enable color arrays.
	glEnableClientState( GL_COLOR_ARRAY );

	// This draws one textured plane using a strip of two triangles.
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Disable vertex arrays.
	glDisableClientState(GL_VERTEX_ARRAY);

	// Disable color arrays.
	glDisableClientState(GL_COLOR_ARRAY);

	// Wait (blocks) until all GL drawing commands to finish.
	glFinish();
}
