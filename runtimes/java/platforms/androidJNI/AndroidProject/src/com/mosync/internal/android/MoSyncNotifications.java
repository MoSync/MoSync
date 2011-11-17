package com.mosync.internal.android;

import android.app.Activity;
import android.content.Context;

import com.mosync.internal.android.notifications.LocalNotificationsManager;
import com.mosync.internal.android.notifications.PushNotificationsManager;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;

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
	int maNotificationLocalCreate(Context appContext)
	{
		return mLocalNotificationsManager.create(appContext);
	}

	int maNotificationLocalDestroy(int handle, Activity activity)
	{
		return mLocalNotificationsManager.destroy(handle, activity);
	}

	int maNotificationLocalSetProperty(
			int handle,
			String property,
			String value)
	{
		return mLocalNotificationsManager.setProperty(handle, property, value);
	}

	int maNotificationLocalGetProperty(
			int handle,
			String property,
			int memBuffer,
			int memBufferSize)
	{
		return mLocalNotificationsManager.getProperty(handle, property, memBuffer, memBufferSize);
	}

	int maNotificationLocalSchedule(int handle, Context appContext)
	{
		return mLocalNotificationsManager.schedule(handle, appContext);
	}

	int maNotificationLocalUnschedule(int handle)
	{
		return mLocalNotificationsManager.unschedule(handle);
	}

	int maNotificationPushRegister(String accountID)
	{
		return mPushNotificationManager.register(accountID);
	}

	int maNotificationPushGetRegistration(int buf, int bufSize)
	{
		return mPushNotificationManager.getRegistrationData(buf, bufSize);
	}

	int maNotificationPushUnregister()
	{
		return mPushNotificationManager.unregister();
	}

	int maNotificationPushGetData(int pushNotificationHandle,
			int allertMessage, int allertMessageSize)
	{
		return mPushNotificationManager.getPushData(pushNotificationHandle, allertMessage, allertMessageSize);
	}

	int maNotificationPushSetTickerText(String text)
	{
		mPushNotificationManager.setTickerText(text);
		return MA_NOTIFICATION_RES_OK;
	}

	int maNotificationPushSetMessageTitle(String title)
	{
		mPushNotificationManager.setMessageTitle(title);
		return MA_NOTIFICATION_RES_OK;
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