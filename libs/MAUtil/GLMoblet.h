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

/** \file GLMoblet.h
* \brief OpenGL|ES fullscreen moblet.
*
* The GLMoblet class provides an object-oriented wrapper around the
* C event system in MoSync. It automatically sets up a fullscreen
* OpenGL|ES 1.1 or 2.0 view and calls the function init. Afterwards
* it calls the function continuously until stopDrawing has been
* called. It automatically binds the gl context when the gl view is
* ready and then calls init. No gl calls are allowed until init has
* been invoked.
*
* \author Niklas Nummelin
*
*/

#ifndef _MAUTIL_GL_MOBLET_H_
#define _MAUTIL_GL_MOBLET_H_

#include <MAUtil/Moblet.h>

namespace MAUtil {

class GLMobletListener;

/**
* \brief OpenGL|ES fullscreen moblet.
*/
class GLMoblet : public Moblet
{
public:

	/**
	* Enum of the different OpenGL|ES versions that may be supported on the system.
	*/
	enum ApiVersion {
		GL1 = 1,
		GL2 = 2
	};

	/**
	* Constructor for the GLMoblet.
	* @param apiVersion A mask of the api versions supported by the application. The latest api version that is available
	* on the system will be initialized. If none of the api versions are available, the system sends a panic.
	*/
	GLMoblet(int apiVersions);

	/**
	* This function will be called when the OpenGL|ES fullscreen window is initialized
	* and the context is bound. Override it to do initialization for the application.
	* No OpenGL|ES calls may be used until this function has been invoked by the system.
	* startDrawing is automatically executed after this function has been invoked.
	* Use getApiVersion to check which version has been initialized.
	*/
	virtual void init() = 0;

	/**
	* This function will be invoked continuously after startDrawing has been invoked.
	* Override it to do the application drawing. When the execution returns it automatically
	* flips the screen.
	*/
	virtual void draw() = 0;

	/**
	* Use this function to enable drawing, i.e. set the system to start calling 'draw' continuously.
	* This function is called automatically if the application focus is gained.
	*/
	void startDrawing();

	/**
	* Use this function to disable drawing, i.e. set the system to stop calling 'draw' continuously.
	* This function is called automatically if the application focus is lost.
	*/
	void stopDrawing();

	/**
	* Use this function to ask the system whether it has drawing enabled or disabled.
	*/
	bool isDrawing() const;

	/**
	* Returns the api version the GLMoblet was initialized to.
	* Use this to choose code paths if you want to support
	* both OpenGL|ES 1.1 and 2.0
	*/
	ApiVersion getApiVersion() const;

	/**
	* Get the current frames per second. Will take some time since 
	* drawing has started to become stable. Will be 0 if isDrawing 
	* equals to false.
	*/
	float getActualFramesPerSecond() const;

	/**
	* Sets the amount of calls to 'draw' per second. This is the optimal
	* amount of frames per second. If 'draw' do complex drawing it might
	* get invoked less times per second. Default is as fast as possible.
	*/
	void setPreferredFramesPerSecond(float fps);

private:
	friend class GLMobletListener;

	int mPreferredMillisecondsPerFrame;
	int mFramesSinceStartDrawing;
	int mMillisOfStartDrawing;

	GLMobletListener *mListener;
	bool mIsDrawing;

	ApiVersion mApiVersion;
};

}

#endif // _MAUTIL_GL_MOBLET_H_
