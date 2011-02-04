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

/**
* \file TouchMotionTracker.h
* \brief Class that calculates touch motion. 
* \author Niklas Nummelin
*/

#ifndef _TOUCH_MOTION_TRACKER_H_
#define _TOUCH_MOTION_TRACKER_H_

#include <ma.h>

namespace MAUI {

/**
 * Class that calculates motion velocity over time.
 */
class TouchMotionTracker {
public:
	/**
	 * Constructor.
	 */
	TouchMotionTracker();
	
	/**
	 * Reset the motion tracker.
	 */
	void reset();
	
	/**
	 * Add a point to the motion tracker data.
	 * \param point The point to add.
	 */
	void addPoint(MAPoint2d point);
	
	/**
	 * Add a point to the motion tracker data and return
	 * the relative distance since last point.
	 *
	 * \param point The point to add.
	 * \param relX The horizontal component of the relative distance 
	 * (return value).
	 * \param relY The vertical component of the relative distance 
	 * (return value).
	 */
	void addPoint(MAPoint2d point, int &relX, int &relY);

	/**
	 * Return the direction and velocity of the motion tracker,
	 * based on the current state of the tracker.
	 * The direction is a normalized direction between 0-1.
	 * The velocity is in pixels per second.
	 * \param dirX The horizontal component of the direction (return value).
	 * \param dirY The vertical component of the direction (return value).
	 * \param velocityX The horizontal component of the velocity 
	 * (return value).
	 * \param velocityY The vertical component of the velocity (return value).
	 */
	void calculateVelocity(
		double &dirX, 
		double &dirY, 
		double &velocityX, 
		double &velocityY);

	/**
	 * \return The starting point of the tracker.
	 */
	MAPoint2d getStartPoint() const;

	/**
	 * TODO: Document.
	 */
	void setFrictionPerSecond(double fps);
	
private:
	/**
	 * Starting time.
	 */
	int mStartTime;
	
	/**
	 * Starting point.
	 */
	MAPoint2d mStart;
	
	/**
	 * Last point.
	 */
	MAPoint2d mLast;
	
	/**
	 * TODO: Document.
	 */
	double mDirx;
	
	/**
	 * TODO: Document.
	 */
	double mDiry;
	
	/**
	 * Tracks if tracker is started.
	 */
	bool mStarted;
	
	/**
	 * TODO: Document.
	 */
	double mFrictionPerSecond;
};

} // namespace MAUI

#endif // _TOUCH_MOTION_TRACKER_H_
