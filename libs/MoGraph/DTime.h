/*
Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

#ifndef DTIME_H_
#define DTIME_H_

#include <maapi.h>

/**
 * \brief Time class,
 * generic class that handles time from x and measuring elapsed time generic usage.
 */

class Time
{
	unsigned int mStart;
	unsigned int mElapsed;
public:

	/**
	 * \brief Time,	constructor, by default starts a timer
	 */

	Time() {start();}

	/**
	 * \brief ~Time, destructor
	 */

	virtual ~Time() {}

	/**
	 * \brief start,	restart the timer.
	 */

	void start() {mStart = maGetMilliSecondCount();}

	/**
	 * \brief update,	udate elapsed time since start
	 * @return calculated elapsed
	 */

	float update() {return mElapsed = maGetMilliSecondCount() - mStart;}

	/**
	 * \brief getElapsed   get the elapsed time since start
	 * @return mElapsed member variable
	 */

	unsigned int getElapsed() {return mElapsed;}

	/**
	 * \brief, getStart get start member variable. where time was started
	 * @return mStart member variable
	 */

	unsigned int getStart() {return mStart;}
};

/**
 *	\brief Delta time class
 *	for more specific usage of fps time elapsed time since last frame.
 */

class DTime : public Time
{
public:

	/**
	 * \brief DTime, class Constructor
	 * Delta Time class restarts at every tick and calculates fps and delta time between ticks.
	 */

	DTime() : mDesiredFps(50.0f),mFps(0.0f),mDeltaMs(0.0f),mDelta(1.0f){start();}
	virtual ~DTime() {}

	/**
	 * \brief tick()
	 * heart beat function called once every frame.
	 */

	void 	tick()					// update time used as a tick called once every frame
	{
		float currentDelta = update();				// get elapsed time since last frame
		start();									// reset counter and start counting from here! affects mStart.
		mFps 			= 1000.0f/currentDelta;		// calculate current frame rate.
		mDelta 			= mDesiredFps/mFps;			// calculate scale from desired fps. that will be used as delta / frame
	}

	/**
	 * \brief getDeltaTime()
	 * gets the delta time in ms between from last tick() call
	 */

	float 	getDeltaTime() 				{return mDeltaMs;}	//get elapsed time since last tick (in our case frame)

	/**
	 * \brief setDesiredFps()
	 * sets the desired FPS to have.
	 * this will give you delta values based on desired fps
	 * e.g. if the app is running half speed from desired 50 fps,
	 * delta will then be 2.0 * 25 = 50 fps.
	 * so that the user can compensate for the time lost by
	 * using constant rotation etc to avoid slowdowns
	 * this is commonly used in the gaming industry
	 */

	void 	setDesiredFps(float fps)	{mDesiredFps = fps;}

	/**
	 * \brief currentFps()
	 * returns the actual fps
	 * @return fps
	 */

	float 	currentFps()				{return mFps;}

	/**
	 * \brief getDesiredFps()
	 * @return desired fps
	 */

	float 	getDesiredFps()				{return mDesiredFps;}

protected:
	float	mDesiredFps;	// Desired fps
	float	mFps;			// Current fps.
	float	mDeltaMs;		// Float delta time in ms elapsed since last frame
	float	mDelta;			// delta in frames by % , when maintaining desired fps delta is 1.0

};


#endif /* DTIME_H_ */
