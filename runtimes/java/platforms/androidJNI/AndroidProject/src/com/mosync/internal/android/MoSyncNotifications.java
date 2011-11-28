package com.mosync.internal.android;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Build;
import android.util.Log;

import com.mosync.internal.android.notifications.LocalNotificationsManager;
import com.mosync.internal.android.notifications.PushNotificationsManager;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;

/**
 * Wrapper for Notifications Syscalls to avoid cluttering the MoSyncSyscalls file.
 */
class MoSyncNotifications
{
	/**
	 * Constructor.
	 * @param thread
	 *            The MoSync thread.
	 */
	public MoSyncNotifications(MoSyncThread thread)
	{
//		Log.e("@@MoSync","MosyncNotifications constructor");

		mMoSyncThread = thread;
		mLocalNotificationsManager = new LocalNotificationsManager(thread);
		// C2DM supports only devices with Android 2.2 and higher.
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.FROYO)
		{
			Log.e("Emma", "froyo");
			mPushNotificationManager = new PushNotificationsManager(thread, getActivity());
		}
		else
			Log.e("emma","below");
	}

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/**
	 * Check if Push permissions are set, and if not call maPanic().
	 */
	boolean isPushPermissionsSet()
	{
		return
			(PackageManager.PERMISSION_GRANTED ==
				getActivity().checkCallingOrSelfPermission(
						"com.mosync.java.android.permission.C2D_MESSAGE"))
			&&
			(PackageManager.PERMISSION_GRANTED ==
				getActivity().checkCallingOrSelfPermission(
						"com.google.android.c2dm.permission.RECEIVE"));
	}

	/**
	 * Check if Push permissions are set, and if not call maPanic().
	 */
	void panicIfPushPermissionsAreNotSet()
	{
		if (!isPushPermissionsSet())
		{
			mMoSyncThread.maPanic(1,
				"Push Notifications permission is not set in the MoSync project");
		}
	}

	/**
	 * Notifications related methods
	 */
	int maNotificationLocalCreate(Context appContext)
	{
		return mLocalNotificationsManager.create(appContext);
	}

	int maNotificationLocalDestroy(int handle)
	{
		return mLocalNotificationsManager.destroy(handle);
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
		panicIfPushPermissionsAreNotSet();
		return mPushNotificationManager.register(accountID);
	}

	int maNotificationPushGetRegistration(int buf, int bufSize)
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null)
		{
			return mPushNotificationManager.getRegistrationData(buf, bufSize);
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushGetRegistration Platform unsupported");
			return MA_NOTIFICATION_RES_ERROR;
		}
	}

	int maNotificationPushUnregister()
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null)
		{
			return mPushNotificationManager.unregister();
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushUnregister Platform unsupported");
			return MA_NOTIFICATION_RES_ERROR;
		}
	}

	int maNotificationPushGetData(int pushNotificationHandle,
			int allertMessage, int allertMessageSize)
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null )
		{
			return mPushNotificationManager.getPushData(pushNotificationHandle, allertMessage, allertMessageSize);
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushGetData Platform unsupported");
			return MA_NOTIFICATION_RES_ERROR;
		}
	}

	int maNotificationPushDestroy(int pushNotificationHandle)
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null )
		{
			return mPushNotificationManager.destroyNotification(pushNotificationHandle);
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushDestroy Platform unsupported");
			return MA_NOTIFICATION_RES_ERROR;
		}
	}

	int maNotificationPushSetTickerText(String text)
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null )
		{
			mPushNotificationManager.setTickerText(text);
			return MA_NOTIFICATION_RES_OK;
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushSetTickerText Platform unsupported");
			return MA_NOTIFICATION_RES_ERROR;
		}
	}

	int maNotificationPushSetMessageTitle(String title)
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null )
		{
			mPushNotificationManager.setMessageTitle(title);
			return MA_NOTIFICATION_RES_OK;
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushSetMessageTitle Platform unsupported");
			return MA_NOTIFICATION_RES_ERROR;
		}
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