package com.mosync.internal.android;

import android.view.MotionEvent;

public class MoSyncMultiTouchHandler extends MoSyncTouchHandler
{

	MoSyncMultiTouchHandler()
	{}
	
	/**
	 * Check abstract class MoSyncTouchHandler
	 */
	int loadEvent(MotionEvent event)
	{
		mMotionEvent = event;
	
		mNumEvents = event.getPointerCount();
		
		return mNumEvents;
	}
	
	/**
	 * Check abstract class MoSyncTouchHandler
	 */
	int[] parseEvent(int index)
	{
		if(index < 0 || index >= mNumEvents) return null;
			
		mEventData[0] = (int)mMotionEvent.getX(index);
		mEventData[1] = (int)mMotionEvent.getY(index);
		mEventData[2] = (int)mMotionEvent.getPointerId(index);
		
		return mEventData;
	}
	
}
