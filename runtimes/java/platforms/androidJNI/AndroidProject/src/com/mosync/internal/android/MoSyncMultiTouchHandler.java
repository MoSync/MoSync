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

package com.mosync.internal.android;

import android.view.MotionEvent;

public class MoSyncMultiTouchHandler extends MoSyncTouchHandler
{

	public MoSyncMultiTouchHandler()
	{}
	
	/**
	 * Check abstract class MoSyncTouchHandler
	 */
	public int loadEvent(MotionEvent event)
	{
		mMotionEvent = event;
	
		mNumEvents = event.getPointerCount();
		
		return mNumEvents;
	}
	
	/**
	 * Check abstract class MoSyncTouchHandler
	 */
	public int[] parseEvent(int index)
	{
		if(index < 0 || index >= mNumEvents) return null;
			
		mEventData[0] = (int)mMotionEvent.getX(index);
		mEventData[1] = (int)mMotionEvent.getY(index);
		mEventData[2] = (int)mMotionEvent.getPointerId(index);
		
		return mEventData;
	}
	
}
