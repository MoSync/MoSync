/* Copyright (C) 2011 MoSync AB

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

package com.mosync.internal.android;

import android.view.MotionEvent;

public class MoSyncSingleTouchHandler extends MoSyncTouchHandler
{

	MotionEvent mMotionEvent; 
	
	/**
	 * The single touch constructor
	 * Sets the number of events to 1
	 */
	public MoSyncSingleTouchHandler()
	{
		mNumEvents = 1;
	}
	
	/**
	 * Check abstract class MoSyncTouchHandler
	 */
	public int loadEvent(MotionEvent event)
	{
		mMotionEvent = event;
		return mNumEvents;
	}
	
	/**
	 * Check abstract class MoSyncTouchHandler
	 */
	public int[] parseEvent(int index)
	{
		if(index != 0) return null;
		
		mEventData[0] = (int)mMotionEvent.getX();
		mEventData[1] = (int)mMotionEvent.getY();
		mEventData[2] = 0;
		
		return mEventData;
	}
	
}
