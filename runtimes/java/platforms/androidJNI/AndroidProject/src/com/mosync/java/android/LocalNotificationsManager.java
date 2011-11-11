package com.mosync.java.android;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.java.android.MoSync;
import com.mosync.java.android.MoSyncService;
import com.mosync.nativeui.ui.widgets.Widget;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

//import static com.mosync.internal.generated.MA_NOTIFICATION_RES_INVALID_HANDLE;

/**
 * The Notifications Manager that holds all the notifications that were
 * created by the user.
 */
public class LocalNotificationsManager {

	/**
	 * Constructor.
	 */
	public LocalNotificationsManager()
	{}

	/**
	 *
	 * @param appContext
	 * @return
	 */
	public int maNotificationCreate(Context appContext)
	{
		mAppContext = appContext;
		return mService.createNotification(appContext.getApplicationContext());
	}

	/**
	 *
	 * @param handle
	 * @return
	 */
	public int maNotificationDestroy(int handle, Activity activity)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( notification != null )
		{
			// Remove the service notification.
			MoSyncService.removeServiceNotification(
					 m_NotificationTable.get(handle).getId(), activity);
//			return mService.stopService();
			// Stop the service.
			MoSyncService.stopService();
			m_NotificationTable.remove(handle);
			return 0;
		}
		Log.e("@@MoSync", "maNotificationDestroy: Invalid notification handle " + handle);
		return -1;
//		return MA_NOTIFICATION_RES_INVALID_HANDLE;
	}

	/**
	 * Set a specific property on a notification.
	 */
	public int maNotificationSetProperty(int handle, String property, String value)
	{
		LocalNotificationObject notif = m_NotificationTable.get(handle);
		if (  notif != null )
		{
			try{
				return notif.setProperty(property, value);
			}catch (PropertyConversionException pce){
				Log.e("@@MoSync", "maNotificationSetProperty: Error while converting property value "+ value + ":" + pce.getMessage());
				return -1;
//				return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
			}
		}
		else
		{
			Log.e("@@MoSync", "maNotificationSetProperty: Invalid notification handle: "+ handle);
			return -1;
//			return -1; //MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
	}

	/**
	 * Get a specific property of a notification.
	 */
	public int maNotificationGetProperty(
			int handle,
			String property,
			int memBuffer,
			int memBufferSize)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( notification == null )
		{
			Log.e("@@MoSync", "maNotificationGetProperty: Invalid notification handle: "+ handle);
			return -1;
//			return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
		}
		String result = notification.getProperty(property);
		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maWidgetGetProperty: Buffer size " + memBufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return IX_WIDGET.MAW_RES_INVALID_STRING_BUFFER_SIZE;
		}

		byte[] ba = result.getBytes();

		// Write string to MoSync memory.
//		MoSyncThread mosyncThread = ((MoSync) m_activity).getMoSyncThread( );
		mMoSyncThread.mMemDataSection.position( memBuffer );
		mMoSyncThread.mMemDataSection.put( ba );
		mMoSyncThread.mMemDataSection.put( (byte)0 );

		return result.length( );
	}

	/**
	 * Schedule a local notification for delivery.
	 */
	public int maNotificationLocalRegister(int handle, Context appContext)
	{
		if ( m_NotificationTable.get(handle) == null )
		{
			Log.e("@@MoSync","maNotificationRegisterLocal Invalid notification handle " + handle);
			return -1;
//			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		// Get the id of the notification.
		mService.startService(appContext, m_NotificationTable.get(handle).getId());
		return 0;
	}

	/**
	 * Cancel the delivery of the specified scheduled local notification.
	 */
	public int maNotificationLocalUnregister(int handle)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( notification == null )
		{
			Log.e("@@MoSync","maNotificationUnregisterLocal: Invalid notification handle " + handle);
			return -1;
//			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		m_NotificationTable.remove(handle);
		return mService.stopService();
	}

	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	public MoSyncThread mMoSyncThread;

	/**
	 * Application context.
	 */
	public Context mAppContext;

	/**
	 * The service that notifies the user, even though the application is in background.
	 */
	public LocalNotificationsService mService;

	/**
	 * A table that contains a mapping between a handle and a notification.
	 */
	private HandleTable<LocalNotificationObject> m_NotificationTable = new HandleTable<LocalNotificationObject>();
}