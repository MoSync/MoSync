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
