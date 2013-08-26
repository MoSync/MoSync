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
