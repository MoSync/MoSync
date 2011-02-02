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
