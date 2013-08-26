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
