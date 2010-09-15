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

#ifndef _TOUCH_MOTION_TRACKER_H_
#define _TOUCH_MOTION_TRACKER_H_

#include <ma.h>

namespace MAUI {

// Calculates motion velocity over time.
class TouchMotionTracker {
public:
	TouchMotionTracker();
	void reset();
	void addPoint(MAPoint2d p);
	void addPoint(MAPoint2d p, int &relX, int &relY);
	void calculateVelocity(double &dirx, double &diry, double &velocityX, double &velocityY);
private:
	int mStartTime;
	MAPoint2d mStart;
	MAPoint2d mLast;
	double mDirx, mDiry;
	bool mStarted;
};

} // namespace MAUI

#endif // _TOUCH_MOTION_TRACKER_H_
