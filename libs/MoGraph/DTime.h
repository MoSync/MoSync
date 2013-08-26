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
