/* Copyright (C) 2011 MoSync AB

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License,
version 2, as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
MA 02110-1301, USA.
*/

package com.mosync.internal.android;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.content.pm.PermissionInfo;
import android.os.Build;
import android.util.Log;

import com.mosync.internal.android.notifications.LocalNotificationsManager;
import com.mosync.internal.android.notifications.PushNotificationsManager;
import com.mosync.internal.android.notifications.PushRegistrationData;
import com.mosync.nativeui.util.properties.FloatConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_UNSUPPORTED;

/**
 * Wrapper for Notifications Syscalls to avoid cluttering the MoSyncSyscalls file.
 * @author emma tresanszki
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
		mMoSyncThread = thread;
		mLocalNotificationsManager = new LocalNotificationsManager(thread);
		// C2DM supports only devices with Android 2.2 and higher.
		// Version.SDK_INT is not available on sdk 3.
		try{
			int target = IntConverter.convert( Build.VERSION.SDK );
			if ( target >= 8 )
			{
				mPushNotificationManager = new PushNotificationsManager(thread, getActivity());
			}
		}
		catch(PropertyConversionException pce )
		{
			return;
		}
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
	boolean isPushPermissionSet()
	{
		return
			(PackageManager.PERMISSION_GRANTED ==
				getActivity().checkCallingOrSelfPermission(getActivity().getPackageName() + ".permission.C2D_MESSAGE"))
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
		if (!isPushPermissionSet())
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
		if ( mPushNotificationManager != null )
		{
			return mPushNotificationManager.register(accountID);
		}
		else
		{
			// No need to send registration request, it can be handled locally.
			Log.e("@@MoSync", "Current Android version does not support C2DM. Use Android 2.2 or higher.");
			return MA_NOTIFICATION_RES_UNSUPPORTED;
		}
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

	int maNotificationPushSetDisplayFlag(int displayFlag)
	{
		panicIfPushPermissionsAreNotSet();
		if ( mPushNotificationManager != null )
		{
			return mPushNotificationManager.setDisplayFlag(displayFlag);
		}
		else
		{
			Log.e("@@MoSync","maNotificationPushSetDisplayFlag Platform unsupported");
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