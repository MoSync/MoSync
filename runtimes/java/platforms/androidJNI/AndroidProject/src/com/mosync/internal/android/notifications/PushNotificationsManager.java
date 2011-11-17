package com.mosync.internal.android.notifications;

import java.nio.ByteBuffer;

import com.google.android.c2dm.C2DMessaging;
import com.mosync.internal.android.MoSyncThread;

import com.mosync.nativeui.util.HandleTable;


import android.os.Build;
import android.content.Context;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_NOT_REGISTERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ALREADY_REGISTERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_REGISTRATION_NOT_CALLED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_UNSUPPORTED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_REGISTRATION_IN_PROGRESS;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PUSH_NOTIFICATION;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PUSH_NOTIFICATION_UNREGISTRATION;
import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION;


/**
 * The Notifications Manager that holds all the notifications that were
 * received from C2Dm server.
 * Receives all C2DM events.
 * @author emma tresanszki
 */
public class PushNotificationsManager
{

	/**
	 * Constructor.
	 */
	public PushNotificationsManager(MoSyncThread mThread, Context context)
	{
		// C2DM supports only devices with Android 2.2 and higher.
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.FROYO)
		{
			mMosyncThread = mThread;
			mAppContext = context;
			ref = this;
		}
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
//		mRegistrationInfo.registrationAttempted = true;
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
		// POST MoSync event, notify that the request was processed.
		postEventRegistration(EVENT_TYPE_PUSH_NOTIFICATION_REGISTRATION);
	}

	/**
	 * The manager is notified when the application
	 * was unregistered from C2DM service.
	 */
	public void unregistered()
	{
		postEventRegistration(EVENT_TYPE_PUSH_NOTIFICATION_UNREGISTRATION);
	}

	/**
	 * The manager is notified by a new incoming message.
	 * @param mess The C2DM message.
	 */
	public void messageReceived(String message)
	{
		int newHandle = createNotification(mMosyncThread.getActivity(), message);
		postEventNotificationReceived(newHandle);

	}

	/**
	 * Launch local notification for a received message.
	 * @param context
	 * @param message The content body of the notification.
	 * @return The notification handle.
	 */
	public int createNotification(Context context, String message)
	{
		int icon = context.getResources().getIdentifier(
				"icon",
				"drawable",
				context.getPackageName());
		PushNotificationObject notification = new PushNotificationObject(
				message, icon, mTickerText, mMessageTitle);

		// Add the notification to the map.
		int handle = m_NotificationTable.add(notification);

		notification.triggerNotification(context);

		Log.e("EMMA","push notification HANDLE IS " + handle);
		return handle;
	}

	/**
	 * Post a message to the MoSync event queue.
	 * Send the handle of the push notification.
	 * @param index The notification handle.
	 */
	private void postEventNotificationReceived(int handle)
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
	 * Register to the C2DM server.
	 * @param accountID The ID of the account authorized to send messages to the application,
	 * typically the email address of an account set up by the application's developer.
	 *
	 * @return a result code.
	 */
	public int register(String accountID)
	{
		Log.e("EMMA", "Manager register");

		if (Build.VERSION.SDK_INT < Build.VERSION_CODES.FROYO)
		{
			// No need to send registration request, it can be handled locally.
			Log.e("@@MoSync", "Current Android version does not support C2DM. Use Android 2.2 or higher.");
			registrationFail(PushRegistrationData.REG_ERR_MESSAGE_PHONE_ERROR);
			return MA_NOTIFICATION_RES_UNSUPPORTED;
		}
		else if ( mRegistrationInfo.registrationInProgress )
		{
			Log.e("@@MoSync","One registration is already in progress.");
			return MA_NOTIFICATION_RES_REGISTRATION_IN_PROGRESS;
		}
		else if ( mRegistrationInfo.registrationSuccess )
		{
			Log.e("@@MoSync", "Application is already registered to C2DM");
			return MA_NOTIFICATION_RES_ALREADY_REGISTERED;
		}

		// Start requesting registration.
		mRegistrationInfo.registrationInProgress = true;
		mRegistrationInfo.registrationAttempted = true;
		// Register with the ID of the account authorized to send push messages.

		C2DMessaging.register(mMosyncThread.getActivity(), accountID);

		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Unregister from the C2DM server if already registered.
	 * @return a result code.
	 */
	public int unregister()
	{
		if ( !mRegistrationInfo.registrationSuccess )
		{
			Log.e("@@MoSync", "maNotificationPushUnregister error: application is not registered.");
			return MA_NOTIFICATION_RES_NOT_REGISTERED;
		}

		C2DMessaging.unregister(mAppContext);
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
		mMosyncThread.mMemDataSection.position( dst );
		mMosyncThread.mMemDataSection.put( ba );
		mMosyncThread.mMemDataSection.put( (byte)0 );
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
		PushNotificationObject notification = m_NotificationTable.get(notificationHandle);
		if ( notification == null )
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
			else
			{
				if( mRegistrationInfo.registrationID.length( ) + 1 > registrationBufSize )
				{
					Log.e( "@@MoSync Notification", "getPushData: Buffer size " + registrationBufSize +
							" too short to hold buffer of size: " + mRegistrationInfo.registrationID.length( ) + 1 );
					return MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
				}

				writeToMoSyncMemory(mRegistrationInfo.registrationID, registrationBuf);

				return MA_NOTIFICATION_RES_OK;
			}
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
		mTickerText = text;
	}

	/**
	 * Set message title for incoming notifications.
	 * @param title Message title.
	 */
	public void setMessageTitle(String title)
	{
		mMessageTitle = title;
	}

	private ByteBuffer getMemoryAt(int offset, int bound) {
		mMosyncThread.mMemDataSection.position(offset);
		ByteBuffer slice = mMosyncThread.mMemDataSection.slice();
		slice.limit(bound);
		return slice;
	}
	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	private MoSyncThread mMosyncThread;

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
	private HandleTable<PushNotificationObject> m_NotificationTable = new HandleTable<PushNotificationObject>();

	/**
	 * A static reference to this object.
	 */
	private static PushNotificationsManager ref;

	/**
	 * The ticker text for incoming notifications.
	 * It is set by maNotificationPushSetTickerText.
	 */
	private String mTickerText = null;

	/**
	 * The message title for incoming notifications.
	 * It is set bymaNotificationPushSetMessageTitle.
	 */
	private String mMessageTitle = null;
}