package com.mosync.java.android;

import java.nio.ByteBuffer;

//import com.google.android.c2dm.C2DMBaseReceiver;
//import com.google.android.c2dm.C2DMessaging;

import com.google.android.c2dm.C2DMBaseReceiver;
import com.google.android.c2dm.C2DMessaging;
import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;
import com.mosync.nativeui.util.HandleTable;

//import com.google.android.c2dm.C2DMBaseReceiver;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_NOT_REGISTERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ALREADY_REGISTERED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_REGISTRATION_NOT_CALLED;

/**
 * The Notifications Manager that holds all the notifications that were
 * received from C2Dm server.
 * Receives all C2DM events.
 */
public class PushNotificationsManager {//extends C2DMBaseReceiver{

	/**
	 * Constructor.
	 */
	public PushNotificationsManager(MoSyncThread mThread, Context context)
	{
//		super("dummy@google.com");
		mMosyncThread = mThread;
		mAppContext = context;
	}
/*
	@Override
	protected void onMessage(Context context, Intent intent) {
		Log.e("C2DM", "manager Message: Fantastic!!!");

		// create new PushNotificationObject that holds the payload.

		// TODO raise event

		// Launch local notification
		final String message = intent.getStringExtra("payload");
		createNotification(context, message);

	}

	@Override
	public void onError(Context context, String errorId) {
		Log.e("@@MoSync", "manager C2DmBaseReceiver onError " + errorId);
		//TODO case for err codes to set strings.
		mRegistrationInfo.registrationInProgress = false;
		mRegistrationInfo.errorMessage = errorId;
		mRegistrationInfo.registrationSuccess = false;
	}

	@Override
	public void onRegistered(Context context, String registrationId)
			throws java.io.IOException {
		Log.e("C2DM", "manager   Registration ID arrived: Fantastic!!!");
		Log.e("@@MoSync", "C2DM onRegistered " + registrationId);
		mRegistrationInfo.registrationInProgress = false;
		mRegistrationInfo.registrationID = registrationId;
		mRegistrationInfo.registrationSuccess = true;
		// SEND EVENT
	};
*/
	// delete this, handled in onRegistered.
	public static void setRegId(String regId)
	{
		mRegistrationInfo.registrationID = regId;
		// Raise an event.
	}

	/**
	 * Launch local notification for a received message.
	 * @param context
	 * @param message
	 */
	public void createNotification(Context context, String message)
	{
//		int icon = getResources().getIdentifier(
//				"icon",
//				"drawable",
//				getPackageName());
		int icon = -1;
		PushNotificationObject notification = new PushNotificationObject(message, icon);
		m_NotificationTable.add(notification);

		notification.triggerNotification(context);
	}

	/**
	 * Register to the C2DM server.
	 * @param accountID
	 * @return
	 */
	public int register(String accountID)
	{
		Log.e("EMMA", "Manager register");
		if ( mRegistrationInfo.registrationSuccess )
		{
			Log.e("@@MoSync", "Application is already registered to C2DM");
			return MA_NOTIFICATION_RES_ALREADY_REGISTERED;
		}
		if ( mRegistrationInfo.registrationInProgress )
		{
			Log.e("@@MoSync","One registration is already in progress.");
			return -1;
		}

		mRegistrationInfo.registrationInProgress = true;
		mRegistrationInfo.registrationAttempted = true;
		mSenderID = accountID;
		try {
			Class.forName("C2DMMessaging");

		}catch (ClassNotFoundException cnf)
		{
			Log.e("EMMA not found","");
		}
		Log.e("EMMA", "BEFORE");
		C2DMessaging.register(mMosyncThread.getActivity(), "emma.tresanszki@mobilesorcery.com");
		Log.e("EMMA","AFTER");
		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Unregister from the C2DM server if already registered.
	 * @return
	 */
	public int unregister()
	{
		if ( !mRegistrationInfo.registrationSuccess )
			return MA_NOTIFICATION_RES_NOT_REGISTERED;

		C2DMessaging.unregister(mAppContext);
		mRegistrationInfo.registrationSuccess = false;
		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Get the values for a given notification.
	 */
	public int getPushData(int notificationHandle, int payload, int payloadLength)
	{
		PushNotificationObject notification = m_NotificationTable.get(notificationHandle);
		if ( notification == null )
		{
			Log.e("@@MoSync Notification", "maNotificationPushGetData received invalid handle " + notificationHandle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}

		String messagePayload = notification.getMessage();
		if( messagePayload.length( ) + 1 > payloadLength )
		{
			Log.e( "@@MoSync Notification", "maNotificationPushGetData: Buffer size " + payloadLength +
					" too short to hold buffer of size: " + messagePayload.length( ) + 1 );
			return MA_NOTIFICATION_RES_INVALID_STRING_BUFFER_SIZE;
		}

		writeToMoSyncMemory(messagePayload, payload);

		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Get the latest registration data.
	 */
	public int getRegistrationData(int registrationBuf, int registrationBufSize)
	{
		if ( mRegistrationInfo.registrationAttempted)
		{
			if ( !mRegistrationInfo.registrationSuccess )
			{
				// Return the err code and specific message.
				writeToMoSyncMemory(mRegistrationInfo.errorMessage,registrationBuf);
				return mRegistrationInfo.errorCode;
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

	private void writeToMoSyncMemory(String value, int dst)
	{
		byte[] ba = value.getBytes();

		// Write string to MoSync memory.
		mMosyncThread.mMemDataSection.position( dst );
		mMosyncThread.mMemDataSection.put( ba );
		mMosyncThread.mMemDataSection.put( (byte)0 );
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
	 * Is the ID of the account authorized to send messages to the application,
	 * typically the email address of an account set up by the application's developer.
	 */
	private String mSenderID = null;

	/**
	 * The app context.
	 */
	private Context mAppContext;

	/**
	 * A table that contains a mapping between a handle and a push notification.
	 */
	private HandleTable<PushNotificationObject> m_NotificationTable = new HandleTable<PushNotificationObject>();
}
