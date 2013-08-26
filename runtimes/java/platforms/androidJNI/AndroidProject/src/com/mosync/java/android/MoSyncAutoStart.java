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

package com.mosync.java.android;

import java.util.concurrent.ScheduledThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * This class will be notified on system boot.
 * @author Mikael Kindborg
 */
public class MoSyncAutoStart extends BroadcastReceiver
{
	/**
	 * Method that gets called at boot time. Here we launch the
	 * application via an intent.
	 */
	@Override
	public void onReceive(final Context context, final Intent intent) 
	{
		Log.i("MoSync", "MoSyncAutoStart.onReceive");
		
		Runnable task = new Runnable()
		{
			@Override
			public void run()
			{
				Log.i("MoSync", "MoSyncAutoStart -> launching application");
				Intent launcherIntent = new Intent(context, MoSync.class);
				launcherIntent.addFlags(
					Intent.FLAG_ACTIVITY_NEW_TASK | 
					Intent.FLAG_ACTIVITY_REORDER_TO_FRONT |
					Intent.FLAG_ACTIVITY_SINGLE_TOP);
				context.startActivity(launcherIntent);
			}
		};

		// Start delayed.
		ScheduledThreadPoolExecutor executor = new ScheduledThreadPoolExecutor(3);
		executor.schedule(task, 5L, TimeUnit.SECONDS);
	}
}
