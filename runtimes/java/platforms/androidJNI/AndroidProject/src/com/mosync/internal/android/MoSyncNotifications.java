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

import android.app.Activity;
import android.content.Context;
import android.os.Build;
import android.util.Log;

import com.google.android.gcm.GCMRegistrar;

import com.mosync.internal.android.notifications.LocalNotificationsManager;
import com.mosync.internal.android.notifications.PushNotificationsManager;
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
	 * GCM/C2DM supports only devices with Android 2.2 and higher.
	 * Version.SDK_INT is not available on sdk 3.
	 * @param thread
	 *            The MoSync thread.
	 */
	public MoSyncNotifications(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mLocalNotificationsManager = new LocalNotificationsManager(thread);

		try{
			if ( Build.VERSION.SDK_INT >= Build.VERSION_CODES.FROYO )
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
	void panicIfPushPermissionsAreNotSet()
	{
		try{
		    GCMRegistrar.checkManifest(getActivity());
		}catch (java.lang.IllegalStateException ise)
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
			Log.e("@@MoSync", "Current Android version does not support GCM/C2DM. Use Android 2.2 or higher.");
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