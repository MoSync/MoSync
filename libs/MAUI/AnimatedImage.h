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

/** 
* \file AnimatedImage.h
* \brief Animated image widget with stepping, speed and direction control
* \author Patrick Broman and Niklas Nummelin
*/

#ifndef _SE_MSAB_MAUI_ANIMATEDIMAGE_H_
#define _SE_MSAB_MAUI_ANIMATEDIMAGE_H_

#include "Widget.h"
#include <MAUtil/Environment.h>
#ifndef MOBILEAUTHOR
#include <ma.h>
#else
#include "../../mosync-trunk/intlibs/helpers/cpp_defs.h"
#include "../../mosync-trunk/intlibs/helpers/cpp_maapi.h"
#include <stdio.h>
#define MAAPI_H
#endif


namespace MAUI {

/** 
* \brief Animated image widget with stepping, speed and direction control
*/

	class AnimatedImage : public Widget, public MAUtil::TimerListener {
	public:
		AnimatedImage(int x, int y, int width, int height, Widget *parent=NULL);
		AnimatedImage(int x, int y, int width, int height, Widget* parent=NULL, MAHandle res=0);
		void setResource(MAHandle res);
		MAHandle getResource() const;
		
		void start();
		void stop();
		void stepForward();
		void stepBack();
		void setNumFrames(int numFrames);
		void setFrameHeight(int height);
		void setFps(int fps);
		void setFrame(int frame);
		void setDirection(int dir);

	protected:
		void drawWidget();
		void runTimerEvent();

		BOOL mLoop;
		int mCurrentFrame;
		int mMsPf;
		int mFrameHeight;
		int mNumFrames;
		int mDirection;
		MAHandle mResource;
		MAExtent mResSize;
	};

}

#endif /* _SE_MSAB_MAUI_ANIMATEDIMAGE_H_ */
