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

package com.mosync.nativeui.util;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

import com.mosync.internal.android.MoSyncThread;

/**
 * A utility class that simplifies the waiting for asynchronous
 * events. This class is only intended to be used for sharing
 * a single result between two threads.
 *
 * @param <T> The result type.
 */
public class AsyncWait<T>
{
	/**
	 * A semaphore that is used to wait for the result being
	 * posted.
	 */
	private ArrayBlockingQueue<T> mResult = new ArrayBlockingQueue<T>( 1 );

	/**
	 * Since the ArrayBlockingQueue can't handle null input we need
	 * another mechanism to help us handle that
	 */
	private boolean mActive = true;

	/**
	 * Set the result, marking the object ready for result.
	 *
	 * @param result The result to set.
	 */
	public void setResult(T result)
	{
		try
		{
			mResult.add( result );
		}
		catch(NullPointerException npe)
		{
			mActive = false;
			// Cause an InterruptedException in getResult(), ensuring that it returns null.
			MoSyncThread.getInstance().interrupt();
		}
	}

	/**
	 * Waits for the result and returns it.
	 * Since the result might be null it will
	 * wait until either there is an item added
	 * to the result array, or when the setter
	 * function sets the mActive flag to false.
	 *
	 * @return The result that has been set.
	 */
	public T getResult()
	{
		while(mActive)
		{
			try
			{
				// Will return null if timeout is hit.
				T r = mResult.poll(5, TimeUnit.SECONDS);
				if(r == null)
				{
					throw new Error("AsyncWait timeout hit");
				}
				return r;
			}
			catch(InterruptedException ie)
			{
			}
		}
		return null;
	}
}
