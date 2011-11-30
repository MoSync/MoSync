package com.mosync.internal.android.notifications;

import android.app.Activity;
import android.content.Context;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_LOCAL_NOTIFICATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;

/**
 * The Notifications Manager that holds all the local notifications that
 * are created by the user.
 * @author emma tresanszki
 */
public class LocalNotificationsManager
{
	/**
	 * Constructor.
	 * @param thread The MoSync thread object.
	 */
	public LocalNotificationsManager(MoSyncThread thread)
	{
		mMoSyncThread = thread;
	}

	/**
	 * Create a notification object.
	 * @param appContext Application's context.
	 * @return The unique notification id.
	 */
	public int create(Context appContext)
	{
		return createNotification(appContext);
	}

	/**
	 * Create an empty notification object.
	 * Set it's properties via maNotificationSetProperty calls.
	 * @param appContext application's context.
	 * @return notification's unique Id.
	 */
	public int createNotification(Context appContext)
	{
		LocalNotificationObject notification = new LocalNotificationObject(appContext);
		notification.setId( m_NotificationTable.add(notification) );
		return notification.getId();
	}

	/**
	 * Destroy a notification object.
	 * @param handle The notification handle.
	 * @return result code.
	 */
	public int destroy(int handle)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( notification != null )
		{
			// Remove the service notification if it is pending.
			if ( notification.isActive() )
			{
				LocalNotificationsService.removeServiceNotification(
						m_NotificationTable.get(handle).getId(),
						mMoSyncThread.getActivity());
			}
			// Stop the service, even when the application is in background.
			LocalNotificationsService.stopService();
			// Remove the internal notification object.
			m_NotificationTable.remove(handle);
			return MA_NOTIFICATION_RES_OK;
		}

		Log.e("@@MoSync", "maNotificationDestroy: Invalid notification handle " + handle);
		return MA_NOTIFICATION_RES_INVALID_HANDLE;
	}

	/**
	 * Internal function for the maNotificationLocalSetProperty system call.
	 * Sets a property on the given notification, by accessing it from
	 * the notification table and calling its setProperty method.
	 * @param handle The notification handle.
	 * @param property The property name.
	 * @param value The property value.
	 */
	public int setProperty(int handle, String property, String value)
	{
		LocalNotificationObject notif = m_NotificationTable.get(handle);
		if (  notif != null )
		{
			try{
				return notif.setProperty(property, value);
			}catch (PropertyConversionException pce){
				Log.e("@@MoSync", "maNotificationLocalSetProperty: Error while converting property value "+ value + ":" + pce.getMessage());
				return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
			}
		}
		else
		{
			Log.e("@@MoSync", "maNotificationLocalSetProperty: Invalid notification handle: "+ handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
	}

	/**
	 * Internal function for the maNotificationLocalGetProperty system call.
	 * Gets a property on the given notification, by accessing it from
	 * the notification table and calling its getProperty method.
	 * @param handle the notification handle.
	 * @param property The property name.
	 * @return the property of the wrapped widget. If no property is found,
	 *         an empty string is returned.
	 */
	/**
	 * Get a specific property of a notification.
	 * @param handle The notification handle.
	 * @param property
	 */
	public int getProperty(
			int handle,
			String property,
			int memBuffer,
			int memBufferSize)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( notification == null )
		{
			Log.e("@@MoSync", "maNotificationLocalGetProperty: Invalid notification handle: "+ handle);
			return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
		}
		String result = notification.getProperty(property);
		if( result.length( ) + 1 > memBufferSize )
		{
			Log.e( "MoSync", "maNotificationLocalGetProperty: Buffer size " + memBufferSize +
					" too short to hold buffer of size: " + result.length( ) + 1 );
			return IX_WIDGET.MAW_RES_INVALID_STRING_BUFFER_SIZE;
		}
		else if ( result.equals(notification.NOTIFICATION_INVALID_PROPERTY_NAME) )
		{
			return MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
		}

		byte[] ba = result.getBytes();

		// Write string to MoSync memory.
		mMoSyncThread.mMemDataSection.position( memBuffer );
		mMoSyncThread.mMemDataSection.put( ba );
		mMoSyncThread.mMemDataSection.put( (byte)0 );

		return result.length( );
	}

	/**
	 * Schedule a local notification for delivery.
	 * @param handle The local notification handle.
	 * @param appContext The application's context.
	 * @return the result code.
	 */
	public int schedule(int handle, Context appContext)
	{
		if ( m_NotificationTable.get(handle) == null )
		{
			Log.e("@@MoSync","maNotificationRegisterLocal Invalid notification handle " + handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		else
		{
			LocalNotificationsService.startService(appContext, m_NotificationTable.get(handle));
			return MA_NOTIFICATION_RES_OK;
		}
	}

	/**
	 * Cancel the delivery of the specified scheduled local notification.
	 * @param handle The local notification handle.
	 */
	public int unschedule(int handle)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( notification == null )
		{
			Log.e("@@MoSync","maNotificationUnregisterLocal: Invalid notification handle " + handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		else
		{
			// Remove the service notification if it is pending.
			if ( notification.isActive() )
			{
				LocalNotificationsService.removeServiceNotification(
						m_NotificationTable.get(handle).getId(),
						mMoSyncThread.getActivity());
				// Set the state of the notification to inactive.
				notification.unschedule();
			}
			// No need to delete the notification, just stop
			// the service if it's started.
			return LocalNotificationsService.stopService();
//			return LocalNotificationsService.stopServiceIntent();
		}
	}

	/**
	 * Post a message to the MoSync event queue.
	 * Send the handle of the local notification.
	 * @param index The notification handle.
	 */
	public static void postEventNotificationReceived(int handle)
	{
		int[] event = new int[2];
		event[0] = EVENT_TYPE_LOCAL_NOTIFICATION;
		event[1] = handle;

		mMoSyncThread.postEvent(event);
	}
	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMoSyncThread;

	/**
	 * A table that contains a mapping between a handle and a notification.
	 */
	private HandleTable<LocalNotificationObject> m_NotificationTable = new HandleTable<LocalNotificationObject>();
}