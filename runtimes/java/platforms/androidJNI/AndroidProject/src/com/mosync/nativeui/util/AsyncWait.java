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

package com.mosync.nativeui.util;

import java.util.concurrent.ArrayBlockingQueue;

/**
 * A utility class that simplifies the waiting for asynchronous
 * events. This class is only intended to be used for sharing
 * a single result between two threads.
 * 
 * @author fmattias
 *
 * @param <T> The result type.
 */
public class AsyncWait<T>
{
	/**
	 * A semaphore that is used to wait for the result being
	 * posted.
	 */
	private ArrayBlockingQueue<T> m_result = new ArrayBlockingQueue<T>( 1 );
	
	/**
	 * Set the result, marking the object ready for result.
	 * 
	 * @param result The result to set.
	 */
	public void setResult(T result)
	{
		m_result.add( result );
	}
	
	/**
	 * Waits for the result and returns it.
	 * 
	 * TODO: Add timeout
	 * 
	 * @return The result that has been set.
	 * @throws InterruptedException
	 */
	public T getResult() throws InterruptedException
	{
		while(true)
		{
			try
			{
				return m_result.take( );
			}
			catch(InterruptedException ie)
			{
				
			}
		}
	}
}
