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

import java.util.Date;
import java.util.Hashtable;
import java.util.TimeZone;
import java.util.Timer;
import java.util.TimerTask;

import android.content.Context;
import android.content.pm.PackageManager;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_LOCAL_NOTIFICATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ERROR;
//import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ALREADY_SCHEDULED;
//import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_CANNOT_UNSCHEDULE;

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
		if ( null != notification )
		{
			// Remove the service notification if it is pending.
			if ( notification.isActive() )
			{
				LocalNotificationsService.removeServiceNotification(
						m_NotificationTable.get(handle).getId(),
						mMoSyncThread.getActivity());

				// Stop the service, even when the application is in background.
				LocalNotificationsService.stopService();
			}
			// Cancel the task assigned to the notification if scheduled.
			if ( null != m_TimerTasks.get(handle) )
			{
				m_TimerTasks.get(handle).cancel();
				m_TimerTasks.remove(handle);
			}

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
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if (  null != notification )
		{
			try{
//				if ( notification.getScheduled() )
//				{
//					Log.e("@@MoSync", "maNotificationLocalSetProperty cannot be called after scheduling the notification.");
//					return MA_NOTIFICATION_RES_ALREADY_SCHEDULED;
//				}
				 if ( property.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS)
						 &&
					  mMoSyncThread.getActivity().getApplicationContext().getPackageManager().
						 hasSystemFeature(PackageManager.FEATURE_CAMERA_FLASH) )
					 return MA_NOTIFICATION_RES_ERROR;
				return notification.setProperty(property, value);
			}catch (PropertyConversionException pce){
				Log.e("@@MoSync", "maNotificationLocalSetProperty: Error while converting property value " + value + ":" + pce.getMessage());
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
	public int getProperty(
			int handle,
			String property,
			int memBuffer,
			int memBufferSize)
	{
		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( null == notification )
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
	public int schedule(final int handle, final Context appContext)
	{
		Log.e("@@MoSync", "LocalNotificationsManager: schedule");

		final LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( null == notification )
		{
			Log.e("@@MoSync","maNotificationRegisterLocal Invalid notification handle " + handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		else
		{
//			if ( notification.getScheduled() )
//			{
//				Log.e("@@MoSync","maNotificationLocalSchedule was already called.");
//				return MA_NOTIFICATION_RES_ALREADY_SCHEDULED;
//			}
			notification.setScheduled(true);

		    TimerTask timerTask = new TimerTask()
		    {
		        @Override
		        public void run()
		        {
					LocalNotificationsService.startService(appContext, notification);
		        }
		    };
		    m_TimerTasks.put(handle, timerTask);

		    // Get the current date in milliseconds from UTC for this time zone.
			Date date = new Date();
			long timeNow = date.getTime();
			TimeZone tz = TimeZone.getDefault();
			int timeOffset = tz.getOffset(timeNow);
			long now = timeNow + (long) timeOffset;
	        long task = notification.getFireDate() - now;
		    m_Timer.schedule(timerTask, task);

			return MA_NOTIFICATION_RES_OK;
		}
	}

	/**
	 * Cancel the delivery of the specified scheduled local notification.
	 * @param handle The local notification handle.
	 * @return The result code.
	 */
	public int unschedule(int handle)
	{
		Log.e("@@MoSync", "LocalNotificationsManager: unschedule");

		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( null == notification )
		{
			Log.e("@@MoSync","maNotificationUnregisterLocal: Invalid notification handle " + handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		else
		{
//			if ( !notification.getScheduled() )
//			{
//				Log.e("@@MoSync","maNotificationLocalUnschedule: failed because notification was not scheduled.");
//				return MA_NOTIFICATION_RES_CANNOT_UNSCHEDULE;
//			}
			notification.setScheduled(false);

			// Remove the service notification if it is pending.
			if ( notification.isActive() )
			{
				LocalNotificationsService.removeServiceNotification(
						m_NotificationTable.get(handle).getId(),
						mMoSyncThread.getActivity());
				// The task has been executed, just remove it below.

				// Set the state of the notification to inactive.
				notification.setInactive();

				// No need to delete the notification, just stop
				// the service if it's started.
				return LocalNotificationsService.stopService();
			}

			// Cancel the  already scheduled task.
			m_TimerTasks.get(handle).cancel();
			m_TimerTasks.remove(handle);

			return MA_NOTIFICATION_RES_OK;
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

	/**
	 * The notification manager is notified when the application has
	 * come in foreground again. In this case, the local notifications
	 * will not be triggered.
	 */
	public static void focusGained()
	{
		// ToDo see if this is truly needed.
	}

	/**
	 * The notification manager is notified when the application has
	 * gone to background. Only now the local notifications can be triggered.
	 */
	public static void focusLost()
	{
		// ToDo see if this is truly needed.
	}
	/************************ Class members ************************/
	/**
	 * The MoSync thread object.
	 */
	private static MoSyncThread mMoSyncThread;

	/**
	 * A table that contains a mapping between a handle and a notification.
	 */
	private HandleTable<LocalNotificationObject> m_NotificationTable =
		new HandleTable<LocalNotificationObject>();

	/**
	 * The timer that takes care of scheduling notifications.
	 */
	Timer m_Timer = new Timer();

	/**
	 * A table that contains mapping between a notification handle and a task assigned to it.
	 * The timer task represents a task to run at a specified time in the schedule method.
	 * The key is the handle of the notification.
	 */
	private Hashtable<Integer,TimerTask> m_TimerTasks =
		new Hashtable<Integer,TimerTask>();
}