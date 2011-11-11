package com.mosync.internal.android;

import android.app.Activity;
import android.content.Context;

import com.mosync.java.android.LocalNotificationsManager;
import com.mosync.java.android.PushNotificationsManager;
//import com.mosync.internal.android.notifications.LocalNotificationsManager;
//import com.mosync.internal.android.notifications.PushNotificationsManager;

/**
 * Wrapper for Notifications Syscalls to avoid cluttering the MoSyncSyscalls file.
 */
class MoSyncNotifications {

	/**
	 * Constructor.
	 * @param thread
	 *            The MoSync thread.
	 */
	public MoSyncNotifications(MoSyncThread thread) {
		mMoSyncThread = thread;
		mLocalNotificationsManager = new LocalNotificationsManager();
		mPushNotificationManager = new PushNotificationsManager(thread,thread.getActivity());
		mLocalNotificationsManager.mMoSyncThread = thread;
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity() {
		return mMoSyncThread.getActivity();
	}

	/**
	 * Notifications related methods
	 */
	int maNotificationCreate(Context appContext)
	{
		return mLocalNotificationsManager.maNotificationCreate(appContext);
	}

	int maNotificationDestroy(int handle, Activity activity)
	{
		return mLocalNotificationsManager.maNotificationDestroy(handle, activity);
	}

	int maNotificationSetProperty(
			int handle,
			String property,
			String value)
	{
		return mLocalNotificationsManager.maNotificationSetProperty(handle, property, value);
	}

	int maNotificationGetProperty(
			int handle,
			String property,
			int memBuffer,
			int memBufferSize)
	{
		return mLocalNotificationsManager.maNotificationGetProperty(handle, property, memBuffer, memBufferSize);
	}

	int maNotificationLocalRegister(int handle, Context appContext)
	{
		return mLocalNotificationsManager.maNotificationLocalRegister(handle, appContext);
	}

	int maNotificationLocalUnregister(int handle)
	{
		return mLocalNotificationsManager.maNotificationLocalUnregister(handle);
	}

	int maNotificationPushRegister(String accountID)
	{
		return mPushNotificationManager.register(accountID);
	}

	int maNotificationPushUnregister()
	{
		return mPushNotificationManager.unregister();
	}

	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	/**
	 * The handle to the local notifications manager.
	 */
	private LocalNotificationsManager mLocalNotificationsManager;

	/**
	 * The handle to the push notifications manager.
	 */
	private PushNotificationsManager mPushNotificationManager;
}