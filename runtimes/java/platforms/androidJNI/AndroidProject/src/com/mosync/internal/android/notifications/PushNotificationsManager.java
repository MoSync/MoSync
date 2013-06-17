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

package com.mosync.internal.android.notifications;

import java.nio.ByteBuffer;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.google.android.gcm.GCMRegistrar;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.internal.android.notifications.PushNotificationsUtil;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ALREADY_REGISTERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_REGISTRATION_NOT_CALLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_REGISTRATION_IN_PROGRESS;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PUSH_NOTIFICATION;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PUSH_NOTIFICATION_UNREGISTRATION;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME;

/**
 * The Notifications Manager that holds all the notifications that were
 * received from GCM server.
 * Receives all GCM events ( registration response, push notifications).
 * GCM supports only devices with Android 2.2 and higher.
 * Note that C2DM server was deprecated and GCM is available instead.
 * This manager is not created on unsupported platforms.
 * @author emma tresanszki
 */
public class PushNotificationsManager
{
	/**
	 * Constructor.
	 */
	@SuppressWarnings("deprecation")
	public PushNotificationsManager(MoSyncThread mThread, Context context)
	{
		Log.e("@@MoSync","GCM Create PushNotificationManager");
		mMosyncThread = mThread;
		mAppContext = context;
		ref = this;
		// Store the default display flag in SharedPreferences.
		PushNotificationsUtil.setPushNotificationDisplayFlag(
				mMosyncThread.getActivity(), MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT);
		mRegistrationInfo.registrationInProgress = false;
	}

	/**
	 * Handles an GCM/C2DM intent.
	 * Routine: GCM/C2DM message is received.
	 * If the MoSync activity is started by clicking on the
	 * notification this function is called.
	 * @param intent
	 * @return {@code true} If the intent was a GCM/C2DM intent and handled.
	 */
	public static boolean handlePushNotificationIntent(Intent intent)
	{
		Log.e("@@MoSync", "handlePushNotificationIntent");

		PushNotificationsManager instance = getRef();
		if ( null == instance )
		{
			return false;
		}

		int notificationHandle = intent.getIntExtra(
				PushNotificationsUtil.MOSYNC_INTENT_EXTRA_NOTIFICATION_HANDLE, -1);
		if ( -1 != notificationHandle )
		{
			// Post a message to MoSync queue.
			postEventNotificationReceived(notificationHandle);
		}
		// Some error occurred and the notification handle that was passed
		// was invalid.
		return true;
	}

	/**
	 * Get a reference to the current object.
	 */
	public static PushNotificationsManager getRef()
	{
		return ref;
	}

	/**
	 * The manager is notified of a registration
	 * @param regId The registration id of the device.
	 */
	public void registrationReady(String regId)
	{
		mRegistrationInfo.registrationInProgress = false;
		mRegistrationInfo.registrationSuccess = true;
		mRegistrationInfo.registrationID = regId;

		// Raise a MoSync event, notify that the request was processed.
		postEventRegistration(EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION);
	}

	/**
	 * The manager is notified in case of registration fail.
	 * @param regError The reason for registration failed.
	 */
	public void registrationFail(String regError)
	{
		mRegistrationInfo.registrationSuccess = false;
		mRegistrationInfo.registrationInProgress = false;
		mRegistrationInfo.errorMessage = regError;

		// Raise a MoSync event, notify that the request was processed,
		// unconditioned by the result.
		postEventRegistration(EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION);
	}

	/**
	 * The manager is notified when the application
	 * was unregistered from GCM/C2DM service.
	 */
	public void unregistered()
	{
		postEventRegistration(EVENT_TYPE_PUSH_NOTIFICATION_UNREGISTRATION);
	}

	/**
	 * The manager is notified by a new incoming message.
	 * @param message The GCM/C2DM message.
	 * @param showNotification True if the notification should be
	 * displayed to the user.
	 */
	public void messageReceived(String message, Boolean showNotification)
	{
		// Create local notification object.
		int newHandle = createNotification(mMosyncThread.getActivity(), message);

		if ( showNotification )
		{
			// Launch the notification now.
			triggerNotification(mMosyncThread.getActivity(), newHandle);
		}

		// Post a message to MoSync queue.
		postEventNotificationReceived(newHandle);
	}

	/**
	 * The manager is notified by a new incoming message.
	 * This will trigger a notification immediately,
	 * and after the MoSync activity is started the event will
	 * be received.
	 * @param message
	 * @param context
	 */
	public static void messageReceivedWhenAppNotRunning(String message, Context context)
	{
		Log.e("@@MoSync","GCM messageReceived when application is not running");

		int newHandle = createNotification(context, message);
		triggerNotification(context, newHandle);
		// MoSyncThread will be able to queue events only after the activity is started.
	}
	/**
	 * Launch local notification for a received message.
	 * @param id The notification handle.
	 * @return True if the local notification object exists.
	 */
	public static Boolean triggerNotification(Context context, int id)
	{
		PushNotificationObject notification =
			m_NotificationTable.get(id);
		if ( null == notification )
			return false;
		notification.triggerNotification(context, id);
		return true;
	}

	/**
	 * Create a local notification for a received message.
	 * @param context
	 * @param message The content body of the notification.
	 * @return The new notification handle.
	 */
	public static int createNotification(Context context, String message)
	{
		int icon = context.getResources().getIdentifier(
				"icon",
				"drawable",
				context.getPackageName());
		PushNotificationObject notification = new PushNotificationObject(
				message,
				icon,
				PushNotificationsUtil.getNotificationTicker(context),
				PushNotificationsUtil.getNotificationTitle(context));

		// Add the notification to the map.
		int handle = m_NotificationTable.add(notification);

		return handle;
	}

	/**
	 * Post a message to the MoSync event queue.
	 * Send the handle of the push notification.
	 * @param index The notification handle.
	 */
	private static void postEventNotificationReceived(int handle)
	{
		int[] event = new int[2];
		event[0] = EVENT_TYPE_PUSH_NOTIFICATION;
		event[1] = handle;

		mMosyncThread.postEvent(event);
	}

	/**
	 * Post a registration finished to the MoSync event queue.
	 * @param registrationState One of the constants:
	 *  - EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION if a registration
	 *  request was processed.
	 *  - EVENT_TYPE_PUSH_NOTIFICATION_UNREGISTRATION if an unregistration
	 *  was processed.
	 */
	private void postEventRegistration(int registrationState)
	{
		int[] event = new int[1];
		event[0] = registrationState;

		mMosyncThread.postEvent(event);
	}

	/**
	 * Register to the GCM/C2DM server.
	 * @param senderId The projectId obtained from here:
	 * http://developer.android.com/guide/google/gcm/gs.html#create-proj
	 * deprecated: accountID The ID of the account authorized to send messages to the application,
	 * typically the email address of an account set up by the application's developer.
	 *
	 * @return a result code.
	 * The registration request is asynchronous. Get the request result using
	 * maNotificationPushGetRegistration.
	 */
	public int register(String senderId)
	{
		if ( mRegistrationInfo.registrationInProgress )
		{
			Log.e("@@MoSync","One registration is already in progress.");
			return MA_NOTIFICATION_RES_REGISTRATION_IN_PROGRESS;
		}
		if ( mRegistrationInfo.registrationSuccess )
		{
			Log.e("@@MoSync", "Application is already registered to GCM/C2DM");
			return MA_NOTIFICATION_RES_ALREADY_REGISTERED;
		}

		// Start requesting registration.
		mRegistrationInfo.registrationInProgress = true;
		mRegistrationInfo.registrationAttempted = true;

		GCMRegistrar.register(mMosyncThread.getActivity(), senderId);
		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Unregister from the GCM/C2DM server if already registered.
	 * @return a result code.
	 */
	public int unregister()
	{
		GCMRegistrar.unregister(mAppContext);

		mRegistrationInfo.registrationSuccess = false;
		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Write to MoSync memory.
	 * @param value The value to be written.
	 * @param dst The position on which to write the buffer.
	 */
	private void writeToMoSyncMemory(String value, int dst)
	{
		byte[] ba = value.getBytes();

		// Write string to MoSync memory.
		ByteBuffer buffer = mMosyncThread.getMemorySlice(dst, ba.length + 1);
		buffer.put( ba );
		buffer.put( (byte)0 );
	}

	/**
	 * Get the values for a given notification.
	 * @param notificationHandle
	 * @param type By default is 1. This is used only on iOS.
	 * @param payloadBuffer Address to buffer to receive the data.
	 * @param bufferSize The max size of the buffer.
	 * @return One of the values:
	 *  - MA_NOTIFICATION_RES_OK
	 *  - MA_NOTIFICATION_RES_INVALID_HANDLE
	 *  - MA_NOTIFICATION_RES_INVALIS_STRING_BUFFER_SIZE
	 */
	public int getPushData(int notificationHandle, int payloadBuffer, int bufferSize)
	{
		Log.e("@@MoSync","maNotificationPushGetData");

		PushNotificationObject notification = m_NotificationTable.get(notificationHandle);
		if ( null == notification )
		{
			Log.e("@@MoSync Notification", "maNotificationPushGetData received invalid handle " + notificationHandle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}

		String messagePayload = notification.getMessage();
		if( messagePayload.length( ) + 1 > bufferSize )
		{
			Log.e( "@@MoSync Notification", "maNotificationPushGetData: Buffer size " + bufferSize +
					" too short to hold buffer of size: " + messagePayload.length( ) + 1 );
			return MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
		}

		writeToMoSyncMemory(messagePayload, payloadBuffer);

		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Destroy a push notification object.
	 * This is not called explicitly by the user, but
	 * implicitly( in Notifications library) after a
	 * push notification is received.
	 * @param notificationHandle
	 * \returns One of the next constants:
	 * - #MA_NOTIFICATION_RES_OK if no error occurred.
	 * - #MA_NOTIFICATION_RES_INVALID_HANDLE if the notificationHandle is invalid.
	 */
	public int destroyNotification(int notificationHandle)
	{
		PushNotificationObject notification = m_NotificationTable.get(notificationHandle);
		if ( null == notification )
		{
			Log.e("@@MoSync","maNotificationPushDestroy Invalid handle");
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}

		m_NotificationTable.remove(notificationHandle);
		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Get the latest registration data.
	 * @param registrationBuf The address on which to write the registration value,or error message.
	 * @param registrationBufSize The size of the registration buffer.
	 * @return A result code.
	 */
	public int getRegistrationData(int registrationBuf, int registrationBufSize)
	{
		if ( mRegistrationInfo.registrationAttempted )
		{
			if ( !mRegistrationInfo.registrationSuccess )
			{
				if( mRegistrationInfo.errorMessage.length() + 1 > registrationBufSize )
				{
					Log.e( "@@MoSync Notification", "getPushData: Buffer size " + registrationBufSize +
							" too short to hold buffer of size: " + mRegistrationInfo.errorMessage.length( ) + 1 );
					return MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
				}

				// Return the err code and specific message.
				writeToMoSyncMemory(mRegistrationInfo.errorMessage,registrationBuf);
				return MA_NOTIFICATION_RES_ERROR;
			}
			if( mRegistrationInfo.registrationID.length( ) + 1 > registrationBufSize )
			{
				Log.e( "@@MoSync Notification", "getPushData: Buffer size " + registrationBufSize +
						" too short to hold buffer of size: " + mRegistrationInfo.registrationID.length( ) + 1 );
				return MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
			}

			writeToMoSyncMemory(mRegistrationInfo.registrationID, registrationBuf);
			return MA_NOTIFICATION_RES_OK;
		}
		else
		{
			Log.e("@@MoSync Notification", "Register was not called before");
			return MA_NOTIFICATION_RES_REGISTRATION_NOT_CALLED;
		}
	}

	/**
	 * Set ticker text for incoming notifications.
	 * @param text Ticker text for status bar.
	 */
	public void setTickerText(String text)
	{
		PushNotificationsUtil.setPushNotificationTicker(mMosyncThread.getActivity(), text);
	}

	/**
	 * Set message title for incoming notifications.
	 * @param title Message title.
	 */
	public void setMessageTitle(String title)
	{
		PushNotificationsUtil.setPushNotificationTitle(mMosyncThread.getActivity(), title);
	}

	/**
	 * Store the display flag in Preferences.
	 * @param displayFlag
	 * @return MA_NOTIFICATION_RES_OK or MA_NOTIFICATION_RES_ERROR.
	 */
	@SuppressWarnings("deprecation")
	public int setDisplayFlag(int displayFlag)
	{
		switch ( displayFlag)
		{
			case MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT:
				PushNotificationsUtil.setPushNotificationDisplayFlag(
						mMosyncThread.getActivity(), MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT);
				break;
			case MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME:
				PushNotificationsUtil.setPushNotificationDisplayFlag(
						mMosyncThread.getActivity(), MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME);
				break;
			default:
			{
				Log.e("@@MoSync", "maNotificationPushSetDisplayFlag Invalid flag");
				return MA_NOTIFICATION_RES_ERROR;
			}
		}
		return MA_NOTIFICATION_RES_OK;
	}
	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMosyncThread;

	/**
	 * Hold the latest registration information.
	 */
	public static PushRegistrationData mRegistrationInfo = new PushRegistrationData();

	/**
	 * The app context.
	 */
	private Context mAppContext;

	/**
	 * A table that contains a mapping between a handle and a push notification.
	 */
	private static HandleTable<PushNotificationObject> m_NotificationTable = new HandleTable<PushNotificationObject>();

	/**
	 * A static reference to this object.
	 */
	private static PushNotificationsManager ref;
}