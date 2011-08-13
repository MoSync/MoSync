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

/**
 * MoSyncTouchHandler
 * Abstract class for handling touch events.
 */
public abstract class MoSyncTouchHandler
{

	/**
	 * The current event being handled
	 */
	MotionEvent mMotionEvent;
	
	/**
	 * The current number of events
	 */
	int mNumEvents;
	
	/**
	 * The int array used for sending back event data
	 */
	int[] mEventData = new int[3];
	
	public MoSyncTouchHandler()
	{}
	
	/**
	 * Loads the current event to the handler and returns
	 * the number of touches in the event
	 * 
	 * @param event The MotionEvent received from Android OS
	 * @return The number of touches
	 */
	public abstract int loadEvent(MotionEvent event);
	
	/**
	 * Returns the i:th touch in this event
	 * 
	 * @param index The event index
	 * @return An int array containing the event
	 *         returns null if the event was incorrect
	 */
	public abstract int[] parseEvent(int index);
}
