/* Copyright (C) 2009 Mobile Sorcery AB

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
		AnimatedImage(int x=0, int y=0, int width=0, int height=0, MAHandle res=0);
		void setResource(MAHandle res);
		MAHandle getResource() const;
		
		void start();
		void stop();
		void stepForward();
		void stepBack();
		void setNumFrames(int numFrames);
		
		void setFrameWidth(int width);
		void setFrameHeight(int height);
		void setFps(int fps);
		void setFrame(int frame);
		void setDirection(int dir);
		
		virtual bool isTransparent() const;
		

	protected:
		void drawWidget();
		void runTimerEvent();

		BOOL mLoop;
		int mCurrentFrame;
		int mMsPf;
		int mFrameWidth;
		int mFrameHeight;
		int mNumFrames;
		int mDirection;
		MAHandle mResource;
		MAExtent mResSize;
	};

}

#endif /* _SE_MSAB_MAUI_ANIMATEDIMAGE_H_ */
