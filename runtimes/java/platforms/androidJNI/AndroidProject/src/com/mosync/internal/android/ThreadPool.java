/* Copyright (C) 2010 MoSync AB

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING.  If not, write to the Free
Software Foundation, 59 Temple Place - Suite 330, Boston, MA
02111-1307, USA.
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
