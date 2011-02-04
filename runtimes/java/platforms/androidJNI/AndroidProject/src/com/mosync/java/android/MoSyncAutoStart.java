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
