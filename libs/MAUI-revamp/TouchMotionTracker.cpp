/* Copyright (C) 2010 MoSync AB

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

#include "TouchMotionTracker.h"
#include <madmath.h>
#include <mastdlib.h>

namespace MAUI {

TouchMotionTracker::TouchMotionTracker() : mStarted(false), mFrictionPerSecond(0.1) {
}

void TouchMotionTracker::reset() {
	mStarted = false;
}

void normalize(double &x, double &y) {
	double len = sqrt(x*x+y*y);
	if(len<0.000000001)len=0.000000001;
	double len_recip = 1.0/len;
	x*=len_recip;
	y*=len_recip;
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

void TouchMotionTracker::calculateVelocity(double &directionX, double &directionY, double &velocityX, double &velocityY) {

	// take unit into acount ? (pixels per second or ms).
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

	// normalize directions
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
