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
	* @brief Animated image widget with stepping, speed and direction control
	*
	*/
	class AnimatedImage : public Widget, public MAUtil::TimerListener {
	public:
		AnimatedImage(int x=0, int y=0, int width=0, 
					  int height=0, MAHandle res=0);
		void setResource(MAHandle res);
		MAHandle getResource() const;
		
		/**
		 * @brief Starts the amimation 
		 */
		void start();
		
		/**
		 * @brief Stops the animation
		 */
		void stop();
		
		/**
		 * @brief Shows the next frame in the animation
		 */
		void stepForward();
		
		/**
		 * @brief Shows the previous frame in the animation 
		 */
		void stepBack();
		
		/**
		 * @brief Sets the number of frames
		 *
		 * @param numFrames The number of frames
		 */
		void setNumFrames(int numFrames);
		
		/**
		 * @brief Sets the width of this animation
		 *
		 * @param width The width
		 */
		void setFrameWidth(int width);
		
		/**
		 * @brief Sets the height of this animation
		 *
		 * @param height The height
		 */
		void setFrameHeight(int height);
		
		/**
		 * @brief Sets the amonut of frames per second
		 *
		 * @param fps The number of frames per second
		 */
		void setFps(int fps);
		
		/**
		 * @brief Sets which frame in the animation which should be visible
		 *
		 * @param frame The frame which should be visible.
		 *              Should be a number between 0 and the number of frames
		 */
		void setFrame(int frame);
		
		/**
		 * @brief In which direction should it animate
		 *
		 * @param dir The direction
		 */
		void setDirection(int dir);
		
		/**
		 * @brief Returns wether or not this widget is transparent
		 *
		 * @return true is transparent
		 *				false is fully opaque
		 */
		virtual bool isTransparent() const;
		

	protected:
		
		/**
		 * @brief Draws this widget
		 */
		void drawWidget();
		
		/**
		 * @brief Updates the animation
		 *
		 * This timer event is triggered when this animation has been started.
		 * Each update steps the animation forward
		 */
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
