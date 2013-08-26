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
