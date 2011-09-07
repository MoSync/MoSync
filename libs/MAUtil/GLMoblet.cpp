/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
*/

#include <ma.h>
#include <IX_WIDGET.h>
#include <IX_OPENGL_ES.h>
#include "GLMoblet.h"

namespace MAUtil {

class GLMobletListener : public TimerListener, public IdleListener, public FocusListener, public CustomEventListener {
private:
	GLMoblet* mMoblet;
	bool mWasDrawing;

public:

	GLMobletListener(GLMoblet* moblet) : mMoblet(moblet), mWasDrawing(false) {
	}

	void idle() {
		mMoblet->draw();
		maUpdateScreen();
		mMoblet->mFramesSinceStartDrawing++;
	}

	void runTimerEvent() {
		mMoblet->draw();
		maUpdateScreen();
		mMoblet->mFramesSinceStartDrawing++;
	}

	void focusLost() {
		mWasDrawing = mMoblet->isDrawing();
		mMoblet->stopDrawing();
	}

	void focusGained() {
		if(mWasDrawing)
			mMoblet->startDrawing();
	}

	void customEvent(const MAEvent& event) {

		if(event.type == EVENT_TYPE_WIDGET) {
			MAWidgetEventData* data = (MAWidgetEventData*)event.data;

			// maybe we need to send an event whenever a gl
			// surface is recreated (and textures have to be uploaded etc.)
			if( data->eventType == MAW_EVENT_GL_VIEW_READY )
			{
				maWidgetSetProperty(data->widgetHandle, "bind", "");
				mMoblet->init();
				mMoblet->startDrawing();
				mWasDrawing = true;
			}
		}
	}
};

GLMoblet::GLMoblet(int apiVersions) :
	mPreferredMillisecondsPerFrame(50),
	mFramesSinceStartDrawing(0),
	mMillisOfStartDrawing(0),
	mIsDrawing(false)
{
	bool glInitialized = false;

	if(apiVersions & GL2) {
		if(maOpenGLInitFullscreen(MA_GL_API_GL2) < 0) {

		} else {
			mApiVersion = GL2;
			glInitialized = true;
		}
	}

	if(!glInitialized) {
		if(!glInitialized && (apiVersions & GL1)) {
			if(maOpenGLInitFullscreen(MA_GL_API_GL1) < 0) {
			} else {
				mApiVersion = GL1;
				glInitialized = true;
			}
		}

	}

	if(!glInitialized) {
		maPanic(1, "Failed to initialize OpenGL|ES. OpenGL|ES is only available on Android and iOS. Also, please check that the device is able to run the version of OpenGL|ES you requested.");
	}

	mListener = new GLMobletListener(this);

	Environment::getEnvironment ().addCustomEventListener(mListener);

}

GLMoblet::ApiVersion GLMoblet::getApiVersion() const {
	return mApiVersion;
}

void GLMoblet::startDrawing() {
	if(mIsDrawing) return;
//	Environment::getEnvironment().addIdleListener(mListener);
	Environment::getEnvironment().addTimer(mListener, mPreferredMillisecondsPerFrame, -1);
	mIsDrawing = true;

	mFramesSinceStartDrawing = 0;
	mMillisOfStartDrawing = maGetMilliSecondCount();
}

void GLMoblet::stopDrawing() {
	if(!mIsDrawing) return;
//	Environment::getEnvironment().removeIdleListener(mListener);
	Environment::getEnvironment().removeTimer(mListener);
	mIsDrawing = false;
}

bool GLMoblet::isDrawing() const {
	return mIsDrawing;
}

float GLMoblet::getActualFramesPerSecond() const {
	if(!isDrawing()) 
		return 0.0f;
	else {
		int millisSinceStartDrawing = maGetMilliSecondCount() - mMillisOfStartDrawing;
		if(millisSinceStartDrawing == 0) return 0;
		return mFramesSinceStartDrawing / (0.001f * (float)millisSinceStartDrawing);
	}
}

void GLMoblet::setPreferredFramesPerSecond(float fps) {
	if((int)fps == 0) maPanic(1, "Invalid fps!");
	// we want milliseconds per frame.
	float secondsPerFrame = 1.0f/fps;
	float millisecondsPerFrame = 1000.0f * secondsPerFrame;
	mPreferredMillisecondsPerFrame = (int)millisecondsPerFrame;
	stopDrawing();
	startDrawing();
}

} // namespace MAUtil
