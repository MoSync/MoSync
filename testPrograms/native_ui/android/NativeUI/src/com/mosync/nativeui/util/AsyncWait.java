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
	void setResult(T result)
	{
		synchronized( m_result )
		{
			m_result.add( result );
		}
	}
	
	/**
	 * Waits for the result and returns it.
	 * 
	 * @return The result that has been set.
	 * @throws InterruptedException
	 */
	T getResult() throws InterruptedException
	{
		return m_result.take( );
	}
}
