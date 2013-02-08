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

package com.mosync.internal.android.notifications;

import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.util.Log;

import com.mosync.java.android.MoSync;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_TICKER_TEXT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_BODY;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_TITLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_PLAY_SOUND;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_SOUND_PATH;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;

/**
 * The service that displays the status bar notification.
 * A status bar notification should be used for any case in which a background
 * service needs to alert the user about an event that requires a response.
 * Service used to give MoSync applications higher priority and display
 * a notification icon that can be used to launch the application.
 * Start service in foreground with notification icon.
 *
 * @author emma tresanszki
 */
public class LocalNotificationsService extends Service
{
	/**
	 * This intent describes a scheduled local notification.
	 */
	public final static String ACTION_NOTIFICATION_RECEIVED = "LocalNotificationReceived";

	/**
	 *  Notification information sent via the BroadcastReceiver, also used when storing
	 *  data to SharedPreferences is based on the properties from the idl.
	 */
	public final static String LOCAL_NOTIFICATION_ID = "local_notification_id";
	public final static int LOCAL_NOTIFICATION_ID_DEFAULT = -1;

	/**
	 * Call this method to start the service and display a notification icon.
	 * @param context Application's context.
	 * @param notification The local notification object.
	 */
	public static void startService(
		Context context,
		int notificationId,
		String notificationTitle,
		String notificationContent,
		String notificationTicker,
		Boolean notificationPlaySound,
		String notificationSoundPath,
		Boolean notificationVibrate,
		long notificationVibrateDuration,
		Boolean notificationFlashingLights,
		String notificationFlashingPattern,
		int notificationFlag)
	{
		Log.i("@@@MoSync", "NotificationsService.startService");

		// The service is already running.
		if (null != sMe)
		{
			Log.i("@@@MoSync", "NotificationsService.startService - service is already running");
		}

		// Get the notification object based on it's handle.
		mLatestNotification = new LocalNotificationObject(context);
		mLatestNotification.setId(notificationId);
		mLatestNotification.setProperty(
				MA_NOTIFICATION_LOCAL_TICKER_TEXT, notificationTicker);
		mLatestNotification.setProperty(
				MA_NOTIFICATION_LOCAL_CONTENT_TITLE, notificationTitle);
		mLatestNotification.setProperty(
				MA_NOTIFICATION_LOCAL_CONTENT_BODY, notificationContent);
		if (notificationFlag != 0)
			mLatestNotification.setFlag(notificationFlag);
		mLatestNotification.setProperty(
				MA_NOTIFICATION_LOCAL_PLAY_SOUND, Boolean.toString(notificationPlaySound));
		if ( !notificationSoundPath.isEmpty() )
		{
			mLatestNotification.setProperty(
					MA_NOTIFICATION_LOCAL_SOUND_PATH, notificationSoundPath);
		}
		mLatestNotification.setProperty(
				MA_NOTIFICATION_LOCAL_VIBRATE, Boolean.toString(notificationVibrate));
		mLatestNotification.setVibrateDuration(notificationVibrateDuration);
		mLatestNotification.setProperty(
				MA_NOTIFICATION_LOCAL_FLASH_LIGHTS, Boolean.toString(notificationFlashingLights));
		if ( !notificationFlashingPattern.isEmpty() )
		{
			mLatestNotification.setProperty(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN, notificationFlashingPattern);
		}

		LocalNotificationsUtil.setLocalNotificationInfo(context, mLatestNotification);

		if ( mLatestNotification == null )
		{
			SYSLOG("@@MoSync The Local Notification Service cannot be started");
			return;
		}

		Intent serviceIntent = new Intent(
				context, com.mosync.internal.android.notifications.LocalNotificationsService.class);

		// Here we set a flag to signal that the service was started
		// from the MoSync application.
		serviceIntent.putExtra("StartedByTheMoSyncApplication", true);
		context.startService(serviceIntent);
	}

	/**
	 * Called by the Android runtime to stop the service.
	 * @return MA_NOTIFICATION_RES_OK or MA_NOTIFICATION_RES_ERROR.
	 */
	public static int stopService()
	{
		SYSLOG("@@@MoSync LocalNotificationsService.stopService");

		if (null != sMe)
		{
			SYSLOG("@@@MoSync LocalNotificationsService.stopService - stopSelf");
			// Cancel the pending notifications.
			sMe.stopSelf();
			sMe = null;
			return MA_NOTIFICATION_RES_OK;
		}
		Log.e("@@@MoSync", "LocalNotificationsService is not started");
		return MA_NOTIFICATION_RES_ERROR;
	}

	/**
	 * Called by the Android runtime to remove the notification icon.
	 * @param notificationId Notification's unique Id.
	 */
	public static void removeServiceNotification(
		int notificationId, Context context)
	{
		SYSLOG("@@MoSync LocalNotification: remove service notification");

		NotificationManager mNotificationManager = (NotificationManager)
			context.getSystemService(Context.NOTIFICATION_SERVICE);
		mNotificationManager.cancel(notificationId);
	}

	/**
	 * Called when the service is created.
	 */
	@Override
	public void onCreate()
	{
		super.onCreate();

		SYSLOG("@@@MoSync NotificationsService.onCreate");

		// Set the single instance of this service.
		sMe = this;
	}

	@Override
	public IBinder onBind(Intent intent)
	{
		return null;
	}

	/**
	 * Called when a service is started on Android before version 2.0.
	 */
	@Override
	public void onStart(Intent intent, int startId)
	{
		SYSLOG("@@@MoSync NotificationsService.onStart");

		startMe(intent);
	}

	/**
	 * Called when a service is started on Android starting from version 2.0.
	 */
	@Override
	public int onStartCommand(Intent intent, int flags, int startId)
	{
		SYSLOG("@@@MoSync NotificationsService.onStartCommand");

		startMe(intent);

		// Don't restart if killed since it is not meaningful
		// to have this service running without the app running.
	    return START_NOT_STICKY;
	}

	@Override
	public void onDestroy()
	{
		super.onDestroy();

		SYSLOG("@@@MoSync LocalNotificationsService.onDestroy");

		stopService();
	}

	/**
	 * Start the service.
	 * @param intent Could be null.
	 */
	private void startMe(Intent intent)
	{
		// Stop the service if there is no intent.
		if (null == intent)
		{
			SYSLOG("@@@MoSync NotificationsService.startMe: "
				+ "stopping service because intent is null");
			stopSelf();
			return;
		}

		// Check that the MoSync start flag is set in the intent.
		boolean startFlag =
			intent.getBooleanExtra("StartedByTheMoSyncApplication", false);
		if (!startFlag)
		{
			SYSLOG("@@@MoSync NotificationsService.startMe: "
				+ "stopping service because startFlag is false");
			stopSelf();
			return;
		}

		// sMe must be set.
		if (null == sMe)
		{
			SYSLOG("@@@MoSync NotificationsService.startMe: "
				+ "stopping service because sMe is null");
			stopSelf();
			return;
		}

		triggerNotification();
	}

	/**
	 * Display the status bar notification after the time has elapsed.
	 */
	void triggerNotification()
	{
		Context context = getApplicationContext();
		Intent intent = new Intent(context, MoSync.class);
		intent.addFlags(
			Intent.FLAG_ACTIVITY_NEW_TASK |
			Intent.FLAG_ACTIVITY_REORDER_TO_FRONT |
			Intent.FLAG_ACTIVITY_SINGLE_TOP |
			Intent.FLAG_DEBUG_LOG_RESOLUTION);

		intent.setAction(ACTION_NOTIFICATION_RECEIVED);

		mLatestNotification = LocalNotificationsUtil.getLocalNotificationInfo(getApplicationContext());

		intent.putExtra(
				LOCAL_NOTIFICATION_ID,
				mLatestNotification.getId());

		// The notification is already created, just trigger it.
		mLatestNotification.trigger();

		PendingIntent contentIntent = PendingIntent.getActivity(
			context,
			mLatestNotification.getId(),
			intent,
			0
		    );

		mLatestNotification.getNotification().setLatestEventInfo(context,
			mLatestNotification.getTitle(),
			mLatestNotification.getText(),
			contentIntent);

		// We do not use a wrapper class anymore, since backwards compatibility
		// is fulfilled for all supported sdks.
		NotificationManager notificationManager = (NotificationManager)
			getSystemService(Context.NOTIFICATION_SERVICE);
		notificationManager.notify(
				(int)System.currentTimeMillis(),
				mLatestNotification.getNotification());
	}

	/************************ Class members ************************/

	/**
	 *  Instance of this service.
	 */
	static LocalNotificationsService sMe;

	/**
	 * The latest registered notification.
	 */
	private static LocalNotificationObject mLatestNotification;
}