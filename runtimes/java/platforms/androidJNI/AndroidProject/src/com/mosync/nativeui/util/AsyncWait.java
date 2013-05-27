/* Copyright (C) 2012 MoSync AB

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

package com.mosync.nativeui.util;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.TimeUnit;

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
