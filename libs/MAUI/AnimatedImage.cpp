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
		loop(true),
		currentFrame(0),
		msPf(50),
		frameHeight(32),
		numFrames(5),
		direction(0), 
		resource(0) {
			start();
		
	}


	AnimatedImage::AnimatedImage(int x, int y, int width, int height, Widget* parent, MAHandle res) 
		: Widget(x, y, width, height, parent),
		loop(true),
		currentFrame(0),
		msPf(50),
		frameHeight(32),
		numFrames(5),
		direction(0),
	    resource(res)
		
	{ 
		setResource(res);
		start();
	}

	void AnimatedImage::drawWidget() {
		if(resource) {
			// void maDrawImageRegion(MAHandle image, const MARect* srcRect, const MAPoint2d* dstPoint, int transformMode);
			MARect srcRect;
			srcRect.left  = 0;
			srcRect.width = EXTENT_X(resSize);
			srcRect.top   = currentFrame * paddedBounds.height; 
			srcRect.height = paddedBounds.height;
			//MAPoint2d destPoint = {paddedBounds.x, paddedBounds.y};
			MAPoint2d destPoint = {0, 0};

			//maDrawImageRegion(resource, &srcRect, &destPoint, TRANS_NONE);
			Gfx_drawImageRegion(resource, &srcRect, &destPoint, TRANS_NONE);
		}
	}

	void AnimatedImage::setResource(MAHandle res) {
		resource = res;
		if(res == 0) return;
		resSize = maGetImageSize(res);
	}

	MAHandle AnimatedImage::getResource() const {
		return resource;
	}

	void AnimatedImage::start() {
		//MAUtil::Environment::getEnvironment().addIdleListener(this);
		MAUtil::Environment::getEnvironment().addTimer(this, msPf, -1);		
	}

	void AnimatedImage::stop() {
		MAUtil::Environment::getEnvironment().removeTimer(this);
	}
	
	void AnimatedImage::stepForward() {
		//printf("stepping fwd (%d)!!!\n", currentFrame);
		currentFrame++;
		if(loop) {
			if(numFrames)
				currentFrame %= numFrames;
			else
				currentFrame = 0;
		}
		else
			if(currentFrame >= numFrames)
				currentFrame = numFrames - 1;
	}

	void AnimatedImage::stepBack() {
		//printf("stepping back!!!\n");
		currentFrame--;
		if(currentFrame < 0)
		{
			if(loop)
				currentFrame = numFrames - currentFrame;
			else
				currentFrame = 0;
		}

		if(numFrames)
			currentFrame %= numFrames;
		else
			currentFrame = 0;
	}

	void AnimatedImage::setFrameHeight(int height) {
		this->bounds.width = EXTENT_X(resSize);
		this->setWidth(this->bounds.width);
		this->bounds.height = height;
		this->setHeight(height);
	}

	void AnimatedImage::setNumFrames(int numFrames) {
		this->numFrames = numFrames;
	}

	void AnimatedImage::setFps(int fps) {
		if(fps) 
			msPf = 1000/fps;
		else
			msPf = 0;
		stop();
		start();
	}
	
	void AnimatedImage::setFrame(int frame) {
		currentFrame = frame;
	}

	void AnimatedImage::setDirection(int dir) {
		direction = dir;
	}

	void AnimatedImage::runTimerEvent() {
		stepForward();
	}

}
