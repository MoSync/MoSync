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

#include "AnimatedImage.h"

#ifdef MOBILEAUTHOR
#include <stdio.h>
#endif

#include <MAUtil/Graphics.h>

namespace MAUI {

	AnimatedImage::AnimatedImage(int x, int y, int width, int height, Widget *parent) :
		Widget(x, y, width, height, parent),
		mLoop(true),
		mCurrentFrame(0),
		mMsPf(50),
		mFrameHeight(32),
		mNumFrames(5),
		mDirection(0), 
		mResource(0)
	{
		start();
	}


	AnimatedImage::AnimatedImage(int x, int y, int width, int height, Widget* parent, MAHandle res) 
		: Widget(x, y, width, height, parent),
		mLoop(true),
		mCurrentFrame(0),
		mMsPf(50),
		mFrameHeight(32),
		mNumFrames(5),
		mDirection(0),
	    mResource(res)
		
	{ 
		setResource(res);
		start();
	}

	void AnimatedImage::drawWidget() {
		if(mResource) {
			// void maDrawImageRegion(MAHandle image, const MARect* srcRect, const MAPoint2d* dstPoint, int transformMode);
			MARect srcRect;
			srcRect.left  = 0;
			srcRect.width = EXTENT_X(mResSize);
			srcRect.top   = mCurrentFrame * mPaddedBounds.height;
			srcRect.height = mPaddedBounds.height;
			//MAPoint2d destPoint = {mPaddedBounds.x, mPaddedBounds.y};
			MAPoint2d destPoint = {0, 0};

			//maDrawImageRegion(resource, &srcRect, &destPoint, TRANS_NONE);
			Gfx_drawImageRegion(mResource, &srcRect, &destPoint, TRANS_NONE);
		}
	}

	void AnimatedImage::setResource(MAHandle res) {
		mResource = res;
		if(res == 0) return;
		mResSize = maGetImageSize(res);
	}

	MAHandle AnimatedImage::getResource() const {
		return mResource;
	}

	void AnimatedImage::start() {
		//MAUtil::Environment::getEnvironment().addIdleListener(this);
		MAUtil::Environment::getEnvironment().addTimer(this, mMsPf, -1);		
	}

	void AnimatedImage::stop() {
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}
	
	void AnimatedImage::stepForward() {
		//printf("stepping fwd (%d)!!!\n", currentFrame);
		mCurrentFrame++;
		if(mLoop) {
			if(mNumFrames)
				mCurrentFrame %= mNumFrames;
			else
				mCurrentFrame = 0;
		}
		else
			if(mCurrentFrame >= mNumFrames)
				mCurrentFrame = mNumFrames - 1;
	}

	void AnimatedImage::stepBack() {
		//printf("stepping back!!!\n");
		mCurrentFrame--;
		if(mCurrentFrame < 0)
		{
			if(mLoop)
				mCurrentFrame = mNumFrames - mCurrentFrame;
			else
				mCurrentFrame = 0;
		}

		if(mNumFrames)
			mCurrentFrame %= mNumFrames;
		else
			mCurrentFrame = 0;
	}

	void AnimatedImage::setFrameHeight(int height) {
		mBounds.width = EXTENT_X(mResSize);
		setWidth(mBounds.width);
		mBounds.height = height;
		setHeight(height);
	}

	void AnimatedImage::setNumFrames(int numFrames) {
		mNumFrames = numFrames;
	}

	void AnimatedImage::setFps(int fps) {
		if(fps) 
			mMsPf = 1000/fps;
		else
			mMsPf = 0;
		stop();
		start();
	}
	
	void AnimatedImage::setFrame(int frame) {
		mCurrentFrame = frame;
	}

	void AnimatedImage::setDirection(int dir) {
		mDirection = dir;
	}

	void AnimatedImage::runTimerEvent() {
		stepForward();
	}

}
