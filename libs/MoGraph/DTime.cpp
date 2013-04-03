/*
Copyright (C) 2013 MoSync AB

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

#include "DTime.h"

/**
 * \brief tick()
 * heart beat function called once every frame.
 */

void DTime::tick()
{
	float currentDelta = update();				// get elapsed time since last frame
	start();									// reset counter and start counting from here! affects mStart.
	mFps 			= 1000.0f/currentDelta;		// calculate current frame rate.
	mDelta 			= mDesiredFps/mFps;			// calculate scale from desired fps. that will be used as delta / frame
}
