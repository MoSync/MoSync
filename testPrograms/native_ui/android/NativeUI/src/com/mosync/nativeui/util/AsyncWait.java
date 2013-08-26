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
