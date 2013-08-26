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
* \file TouchMotionTracker.h
* \brief Class that calculates touch motion. 
* \author Niklas Nummelin
*/

#include "TouchMotionTracker.h"
#include <madmath.h>
#include <mastdlib.h>

namespace MAUI {

/**
 * Local helper function that normalises a vector.
 */
 /*
static void normalize(double &x, double &y) {
	double len = sqrt(x*x+y*y);
	if(len<0.000000001)len=0.000000001;
	double len_recip = 1.0/len;
	x*=len_recip;
	y*=len_recip;
}
*/

TouchMotionTracker::TouchMotionTracker() :
	mStarted(false), 
	mFrictionPerSecond(0.1) 
{
}

void TouchMotionTracker::reset() {
	mStarted = false;
}

void TouchMotionTracker::addPoint(MAPoint2d p, int &relX, int &relY) {
	if(!mStarted) {
		mLast = mStart = p;
		mStartTime = maGetMilliSecondCount();
		mDirx = 0;
		mDiry = 0;
		relX = 0;
		relY = 0;
		mStarted = true;
	} else {
		relX = p.x-mLast.x;
		relY = p.y-mLast.y;
		double dx = relX;
		double dy = relY;
		mDirx+=dx;
		mDiry+=dy;
		mLast = p;
	}
}

void TouchMotionTracker::addPoint(MAPoint2d p) {
	int relX, relY;
	addPoint(p, relX, relY);
}

void TouchMotionTracker::setFrictionPerSecond(double fps) {
	mFrictionPerSecond = fps;
}

void TouchMotionTracker::calculateVelocity(
	double &directionX, 
	double &directionY, 
	double &velocityX, 
	double &velocityY) {

	// TODO: Take unit into acount? (pixels per second or ms).
	double time = maGetMilliSecondCount()-mStartTime;

	double timeInSeconds = time/1000.0;
	double friction = pow(1.0-mFrictionPerSecond, 1+timeInSeconds);
	if(friction<0.0) friction = 0.0;

	double dx = mDirx;
	double dy = mDiry;

	dx*=friction;
	dy*=friction;

	double dirX = dx;
	double dirY = dy;

	// Normalize directions.
	double len = sqrt(dx*dx+dy*dy);

	if(len < 0.0000000001) {
		velocityX = velocityY = 0.0;
		directionX = directionY = 0.0;
		return;
	}

	double len_recip = 1.0/len;
	dx*=len_recip;
	dy*=len_recip;

	directionX = dx;
	directionY = dy;

	if(time  < 0.0000000001) {
		velocityX = velocityY = 0.0;
		return;
	}

	velocityX = dirX/timeInSeconds;
	velocityY = dirY/timeInSeconds;
}

MAPoint2d TouchMotionTracker::getStartPoint() const {
	return mStart;
}

} // namespace MAUI
