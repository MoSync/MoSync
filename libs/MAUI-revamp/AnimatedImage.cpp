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

#include "AnimatedImage.h"

#ifdef MOBILEAUTHOR
#include <stdio.h>
#endif

#include <MAUtil/Graphics.h>

namespace MAUI
{
	bool AnimatedImage::isTransparent() const
	{
		return true;
	}

	AnimatedImage::AnimatedImage(	int x, int y, int width,
									int height, MAHandle res) 
		: Widget(x, y, width, height),
		mLoop(true),
		mCurrentFrame(0),
		mMsPf(50),
		mFrameWidth(32),
		mFrameHeight(32),
		mNumFrames(5),
		mDirection(0),
		mResource(res)
	{ 
		setWidth(mFrameWidth);
		setHeight(mFrameHeight);
		setResource(res);
		start();
	}

	void AnimatedImage::drawWidget()
	{
		if(mResource)
		{
			MARect srcRect;
			srcRect.left  = (mCurrentFrame*mFrameWidth)%(EXTENT_X(mResSize));
			srcRect.width = mFrameWidth;
			srcRect.top   = ((mCurrentFrame*mFrameWidth)/(EXTENT_X(mResSize)))
							*mFrameHeight;
			srcRect.height = mFrameHeight;
			
			//MAPoint2d destPoint = {mPaddedBounds.x, mPaddedBounds.y};
			MAPoint2d destPoint = {0, 0};

			Gfx_drawImageRegion(mResource, &srcRect, &destPoint, TRANS_NONE);
		}
	}

	void AnimatedImage::setResource(MAHandle res)
	{
		mResource = res;
		if(res == 0) return;
		mResSize = maGetImageSize(res);
		requestRepaint();		
	}

	MAHandle AnimatedImage::getResource() const
	{
		return mResource;
	}

	void AnimatedImage::start()
	{
		MAUtil::Environment::getEnvironment().addTimer(this, mMsPf, -1);	
	}

	void AnimatedImage::stop()
	{
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}
	
	void AnimatedImage::stepForward()
	{
		//printf("stepping fwd (%d)!!!\n", currentFrame);
		mCurrentFrame++;
		if(mLoop)
		{
			if(mNumFrames)
				mCurrentFrame %= mNumFrames;
			else
				mCurrentFrame = 0;
		}
		else
			if(mCurrentFrame >= mNumFrames)
				mCurrentFrame = mNumFrames - 1;
		requestRepaint();
	}

	void AnimatedImage::stepBack()
	{
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
		requestRepaint();			
	}

	void AnimatedImage::setFrameWidth(int width)
	{
		mFrameWidth = width;
		setWidth(width);
	}
	
	void AnimatedImage::setFrameHeight(int height)
	{
		mFrameHeight = height;
		setHeight(height);
	}

	void AnimatedImage::setNumFrames(int numFrames)
	{
		mNumFrames = numFrames;
	}

	void AnimatedImage::setFps(int fps)
	{
		if(fps) 
			mMsPf = 1000/fps;
		else
			mMsPf = 0;
		stop();
		start();
	}
	
	void AnimatedImage::setFrame(int frame)
	{
		mCurrentFrame = frame;
	}

	void AnimatedImage::setDirection(int dir)
	{
		mDirection = dir;
	}

	void AnimatedImage::runTimerEvent()
	{
		stepForward();
	}

}
