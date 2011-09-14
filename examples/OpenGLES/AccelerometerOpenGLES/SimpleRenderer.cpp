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
