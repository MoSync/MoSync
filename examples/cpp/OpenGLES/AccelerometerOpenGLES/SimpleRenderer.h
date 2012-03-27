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

/** \file SimpleRenderer.h
*
* This file declares the interface for the Renderer class
* and an implementation for it called SimpleRenderer.
*
* \author Niklas Nummelin
*
*/

#ifndef _SIMPLE_RENDERER_H_
#define _SIMPLE_RENDERER_H_

class Renderer {
public:

	// Sets the rotation of the vector for each
	// axis in degrees.
	void setRotation(float x, float y, float z);

	// Initializes the renderer with the width
	// and height of the surface to be drawn to.
	virtual void init(int width, int height) = 0;

	// Let the renderer perform the drawing.
	virtual void draw() = 0;

protected:

	// Variables that keeps the current rotation
	// for each axis.
	float 	mRotationX,
			mRotationY,
			mRotationZ;
};

class SimpleRenderer : public Renderer {
public:
	void init(int width, int height);
	void draw();
};

#endif // _SIMPLE_RENDERER_H_
