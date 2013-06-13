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

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_HOMESCREEN_HIDDEN;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_HOMESCREEN_SHOWN;

import java.util.List;
import java.util.Set;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.ActivityManager.RecentTaskInfo;
import android.app.ActivityManager.RunningTaskInfo;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.util.Log;

/**
 * Class that handles home screen related syscalls.
 * @author Mikael Kindborg
 */
public class MoSyncHomeScreen
{
	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMoSyncThread;

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncHomeScreen(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/**
	 * Add a shortcut icon to the home screen. If called multiple times
	 * multiple icons will be added. The shortcut launches the current
	 * application.
	 * @param name The text that will be used for the shortcut label.
	 * @return <0 on error
	 */
	int maHomeScreenShortcutAdd(String name)
	{
		homeScreenPanicIfPermissionsAreNotSet();

		// Create intent for the shortcut launcher.
		Intent shortcutIntent = new Intent(Intent.ACTION_MAIN);
		shortcutIntent.setClassName(getActivity(), getActivity().getClass().getName());
		shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP);
		shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT);
		shortcutIntent.addFlags(Intent.FLAG_ACTIVITY_SINGLE_TOP);

		// Get icon resource.
		int icon = getActivity().getResources().getIdentifier(
			"icon",
			"drawable",
			getActivity().getPackageName());

		// Create intent for adding a shortcut icon.
		Intent intent = new Intent();
		intent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
		intent.putExtra(Intent.EXTRA_SHORTCUT_NAME, name);
		intent.putExtra(
			Intent.EXTRA_SHORTCUT_ICON_RESOURCE,
			Intent.ShortcutIconResource.fromContext(getActivity(), icon));
		intent.setAction("com.android.launcher.action.INSTALL_SHORTCUT");
		// Alternative form: intent.setAction(Intent.ACTION_CREATE_SHORTCUT);

		// Broadcast the intent.
		getActivity().getApplicationContext().sendBroadcast(intent);

		return 1;
	}

	/**
	 * Remove a shortcut icon to the home screen.
	 * @param name The shortcut(s) with this label will be removed.
	 * @return <0 on error
	 */
	int maHomeScreenShortcutRemove(String name)
	{
		homeScreenPanicIfPermissionsAreNotSet();

		Intent shortcutIntent = new Intent(Intent.ACTION_MAIN);
		shortcutIntent.setClassName(getActivity(), getActivity().getClass().getName());

		Intent intent = new Intent();
		intent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, shortcutIntent);
		intent.putExtra(Intent.EXTRA_SHORTCUT_NAME, name);
		intent.setAction("com.android.launcher.action.UNINSTALL_SHORTCUT");

		getActivity().getApplicationContext().sendBroadcast(intent);

		return 1;
	}

	/**
	 * Thread that checks if the home screen is visible.
	 */
	HomeScreenDetectionThread mHomeScreenDetectionThread = null;

	/**
	 * Turn on/off sending of HomeScreen events. Off by default.
	 * @param eventsOn 1 = events on, 0 = events off
	 */
	int maHomeScreenEventsOnOff(final int eventsOn)
	{
		//Log.i("MoSync", "maHomeScreenEventsOnOff");

		homeScreenPanicIfPermissionsAreNotSet();

		// Start home screen event polling thread.
		if (1 == eventsOn)
		{
			// Thread should not be started.
			if (null != mHomeScreenDetectionThread)
			{
				//Log.i("MoSyncSyscalls",
				//	"maHomeScreenEventsOnOff - thread already running");
				return -2;
			}

			// Create and start thread.
	    	mHomeScreenDetectionThread = new HomeScreenDetectionThread();
			mHomeScreenDetectionThread.start();

			return 0;
		}
		// Stop home screen event polling thread.
		else if (0 == eventsOn)
		{
			mHomeScreenDetectionThread.stopThread();
			mHomeScreenDetectionThread = null;
		}

		return -2;
	}

	/**
	 * Check if permissions needed by home screen syscalls are set,
	 * and if not call maPanic().
	 */
	void homeScreenPanicIfPermissionsAreNotSet()
	{
		if ((PackageManager.PERMISSION_GRANTED !=
				getActivity().checkCallingOrSelfPermission(
						android.Manifest.permission.GET_TASKS))
			||
			(PackageManager.PERMISSION_GRANTED !=
				getActivity().checkCallingOrSelfPermission(
					"com.android.launcher.permission.INSTALL_SHORTCUT"))
			||
			(PackageManager.PERMISSION_GRANTED !=
				getActivity().checkCallingOrSelfPermission(
					"com.android.launcher.permission.UNINSTALL_SHORTCUT"))
			)
		{
			mMoSyncThread.maPanic(1,
				"Permission 'Home Screen access' " +
				"is not set in the MoSync project");
		}
	}

	class HomeScreenDetectionThread extends Thread
	{
		/**
		 * The id of the home screen task. -1 means that the id is unknown.
		 */
		int mHomeScreenTaskId = -1;

		/**
		 * Keep track of whether we are on the home screen or not.
		 */
		boolean mIsOnHomeScreen = false;

		/**
		 * This thread will run as long as this variable is true.
		 */
		boolean mIsRunning = false;

		/**
		 * Constructor.
		 */
		public HomeScreenDetectionThread()
		{
		}

		/**
		 * Stop the thread.
		 */
		public void stopThread()
		{
			mIsRunning = false;
		}

		public void run()
		{
			//Log.i("MoSyncSyscalls", "HomeScreenDetectionThread is starting");

			// Used for printing debug messages at a reduced interval.
			int debugPrintAliveCounter = 0;

			mIsRunning = true;
			while (mIsRunning)
			{
				try
				{
					// Will print every 10 seconds.
					if (debugPrintAliveCounter++ % 10 == 0)
					{
						//Log.i("MoSyncSyscalls",
						//	"HomeScreenDetectionThread is alive."
						//	+ " mHomeScreenTaskId: " + mHomeScreenTaskId
						//	+ " Timestamp: " + System.currentTimeMillis());
					}

					boolean isOnHomeScreen = isHomeScreenVisible();

					// Only post event if the state has changed.
					if (isOnHomeScreen != mIsOnHomeScreen)
					{
						// Update current state.
						mIsOnHomeScreen = isOnHomeScreen;

						// Post state change event.
						int[] event = new int[1];
						if (mIsOnHomeScreen)
						{
							//Log.i("MoSyncSyscalls",
							//	"HOMESCREEN IS SHOWN Timestamp"
							//	+ System.currentTimeMillis());
							event[0] = EVENT_TYPE_HOMESCREEN_SHOWN;
						}
						else
						{
							//Log.i("MoSyncSyscalls",
							//	"HOMESCREEN IS HIDDEN Timestamp: "
							//	+ System.currentTimeMillis());
							event[0] = EVENT_TYPE_HOMESCREEN_HIDDEN;
						}
						mMoSyncThread.postEvent(event);
					}

					Thread.sleep(1000);
				}
				catch (Exception ex)
				{
					Log.e("MoSyncSyscalls",
						"HomeScreenDetectionThread exception: " + ex);
					ex.printStackTrace();
				}
			}
		}

		/**
		 * @return true if the home screen is visible.
		 */
	    private boolean isHomeScreenVisible()
	    {
			// We will use the ActivityManager to get info about the top task.
			ActivityManager activityManager = (ActivityManager)
	    		getActivity().getSystemService(Context.ACTIVITY_SERVICE);
	    	if (null == activityManager) { return false; }

			// Get a list with the top task.
	    	List<RunningTaskInfo> taskList = activityManager.getRunningTasks(1);
	    	if (null == taskList) { return false; }

			//int i = 0;
			//for (RunningTaskInfo task : taskList)
			//{
			//	if (++i > 5) break;
			//	Log.i("@@@@@@@@", "@ " + task.baseActivity
			//		+ " " + task.topActivity + " id: " + task.id);
			//}

			// If the home screen task id is -1 it is not known.
			// Get it from the task list.
			if (-1 == mHomeScreenTaskId)
			{
				mHomeScreenTaskId = getHomeScreenTaskId();
			}

	    	// Is the first task in the task list the home screen?
	    	boolean isHomeScreen = mHomeScreenTaskId == taskList.get(0).id;

			// If we have not found the home screen, we make an extra
			// check of the class name. The home screen class is named
			// "Launcher" on the systems we have tested. It might be that
			// some other app, in another package, has the same name, and
			// then we will mistakenly think that we are on the home screen.
			// But if the mHomeScreenTaskId has been set this will not be
			// an issue. The background to this problem is that maSendToBack
			// (which calls moveTaskToBack) does not put the home screen task
			// in the RunningTaskInfo list. But pressingt the home key on the
			// device does so.
			if (-1 == mHomeScreenTaskId && !isHomeScreen)
			{
				isHomeScreen = taskList.get(0).baseActivity
					.getClassName().endsWith(".Launcher");
			}

	    	return isHomeScreen;
	    }

		/**
		 * @return The id of the home screen task, -1 on error or if not found.
		 */
	    private int getHomeScreenTaskId()
	    {
			// Get the ActivityManager.
			ActivityManager activityManager = (ActivityManager)
	    		getActivity().getSystemService(Context.ACTIVITY_SERVICE);
	    	if (null == activityManager) { return -1; }

			// Get a list of recent tasks. This list contains info about
			// The tasks so that we can determine which task is the
			// home screen.
	    	List<RecentTaskInfo> taskList = activityManager.getRecentTasks(
	    		100,
	    		ActivityManager.RECENT_WITH_EXCLUDED
	    		);

	    	if (null != taskList)
	    	{
				// Search for the home screen task.
		    	for (ActivityManager.RecentTaskInfo taskInfo : taskList)
		    	{
			    	Intent intent = taskInfo.baseIntent;
			    	if (null == intent) { break; }

			    	// Get task categories.
			    	Set<String> categories = intent.getCategories();
			    	if (null == categories) { break; }

			    	// If this is a CATEGORY_HOME intent we are on the home screen.
			    	boolean isHomeScreen  = categories.contains(Intent.CATEGORY_HOME);
		    		if (isHomeScreen)
		    		{
						// We found the id of the home screen task.
		    			return taskInfo.id;
		    		}
		    	}
	    	}

			// We did not find the id of the home screen task.
	    	return -1;
	    }
	}
}
