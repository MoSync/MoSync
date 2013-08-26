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

import static com.mosync.internal.android.MoSyncHelpers.MYASSERT;

import java.util.Vector;

final class ThreadPool 
{
	@SuppressWarnings("unchecked")
	public void execute(Runnable runnable) 
	{
		WorkerThread worker = null;
		for (int i = 0; i < mThreads.size(); ++i) 
		{
			WorkerThread temp = (WorkerThread) mThreads.elementAt(i);
			if (temp.isAvailable()) 
			{
				worker = temp;
				break;
			}
		}
		if (worker == null) 
		{	
			//no available threads; we must make another.
			worker = new WorkerThread();
			mThreads.addElement(worker);
		}
		worker.start(runnable);
		// worker is now unavailable until runnable.run() returns.
	}

	public void stopEverything() 
	{
		for (int i = 0; i < mThreads.size(); ++i) 
		{
			WorkerThread worker = (WorkerThread) mThreads.elementAt(i);
			worker.stopWorkerThread();
		}
	}

	@SuppressWarnings("unchecked")
	private Vector mThreads = new Vector();
}

final class WorkerThread extends Thread 
{
	public WorkerThread() 
	{
		start();
	}

	public boolean isAvailable() 
	{
		return mRunnable == null;
	}
	
	synchronized public void start(Runnable r) 
	{
		MYASSERT(isAvailable());
		mRunnable = r;
		notify();
	}
	
	synchronized public void stopWorkerThread() 
	{
		quit = true;
		notify();
	}

	public void run() 
	{
		try 
		{
			while (!quit) 
			{
				synchronized (this) 
				{ 
					while (mRunnable == null) 
					{
						try 
						{
							wait();
						} 
						catch (InterruptedException e) 
						{
						}
						if (quit)
						{
							return;
						}
					} 
				}
				mRunnable.run();
				mRunnable = null;
			}
		} 
		catch (Throwable e) 
		{
			e.printStackTrace();
		}
	}

	private Runnable mRunnable;
	private boolean quit = false;
}
