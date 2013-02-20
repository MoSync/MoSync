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

import java.util.Hashtable;

import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.mosync.internal.android.MoSyncThread;
import com.mosync.internal.generated.IX_WIDGET;
import com.mosync.nativeui.util.HandleTable;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.LongConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;
import static com.mosync.internal.android.MoSyncHelpers.SYSLOG;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_LOCAL_NOTIFICATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_BODY;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_TITLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLAG;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_TICKER_TEXT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_PLAY_SOUND;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_SOUND_PATH;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE_DURATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_HANDLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_ALREADY_SCHEDULED;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_CANNOT_UNSCHEDULE;

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
			// Cancel the intent assigned to the notification if scheduled.
			if ( null != m_Intents.get(handle) )
			{
				AlarmManager alarmManager  =
					(AlarmManager) mMoSyncThread.getActivity().getSystemService(Context.ALARM_SERVICE);

				// Set the unique request code as the handle.
				PendingIntent pendingIntent = PendingIntent.getBroadcast(
						mMoSyncThread.getActivity(),
						notification.getRequestCode(),
						m_Intents.get(handle),
						PendingIntent.FLAG_NO_CREATE);
				if ( pendingIntent != null )
				{
					alarmManager.cancel(pendingIntent);
				}
				m_Intents.remove(handle);
			}

			// Remove the internal notification object.
			m_NotificationTable.remove(handle);

			return MA_NOTIFICATION_RES_OK;
		}

		SYSLOG("@@MoSync maNotificationDestroy: Invalid notification handle " + handle);
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
			if ( notification.getScheduled() )
			{
				SYSLOG("@@MoSync maNotificationLocalSetProperty cannot be called after scheduling the notification.");
				return MA_NOTIFICATION_RES_ALREADY_SCHEDULED;
			}

			try{
				return notification.setProperty(property, value);
			}catch (PropertyConversionException pce){
				SYSLOG("@@MoSync maNotificationLocalSetProperty: Error while converting property value " + value + ":" + pce.getMessage( ) );
				return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
			}catch (InvalidPropertyValueException ipve){
				SYSLOG("@@MoSync maNotificationLocalSetProperty: Error while setting property: " + ipve.getMessage( ) );
				return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
			}
		}
		else
		{
			SYSLOG("@@MoSync maNotificationLocalSetProperty: Invalid notification handle: "+ handle);
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
			SYSLOG("@@MoSync maNotificationLocalGetProperty: Invalid notification handle: "+ handle);
			return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
		}

		String result = notification.getProperty(property);
		if( result.length( ) + 1 > memBufferSize )
		{
			SYSLOG( "MoSync maNotificationLocalGetProperty: Buffer size " + memBufferSize +
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
		SYSLOG("@@MoSync LocalNotificationsManager: schedule notification " + String.valueOf(handle));

		final LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( null == notification )
		{
			Log.e("@@MoSync","maNotificationRegisterLocal Invalid notification handle " + handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		else
		{
			if ( notification.getScheduled() )
			{
				SYSLOG("@@MoSync maNotificationLocalSchedule was already called.");
				return MA_NOTIFICATION_RES_ALREADY_SCHEDULED;
			}

			notification.setScheduled(true);

			AlarmManager alarmManager  = (AlarmManager) appContext.getSystemService(Context.ALARM_SERVICE);

			// Prepare the intent that will be launched at scheduled time.
			Intent intent = new Intent(appContext, com.mosync.internal.android.notifications.AlarmReceiver.class);
			intent.setAction("MoSync " + notification.getId());

			intent.putExtra(
					LocalNotificationsService.LOCAL_NOTIFICATION_ID,
					handle);
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_CONTENT_TITLE,
					notification.getTitle());
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_CONTENT_BODY,
					notification.getText());
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_TICKER_TEXT,
					notification.getProperty(MA_NOTIFICATION_LOCAL_TICKER_TEXT));
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_FLAG,
					notification.getFlag());
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_PLAY_SOUND,
					Boolean.valueOf(notification.getProperty(MA_NOTIFICATION_LOCAL_PLAY_SOUND)));
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_SOUND_PATH,
					notification.getProperty(MA_NOTIFICATION_LOCAL_SOUND_PATH));
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_VIBRATE,
					Boolean.valueOf(notification.getProperty(MA_NOTIFICATION_LOCAL_VIBRATE)));
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_VIBRATE_DURATION,
					LongConverter.convert(notification.getProperty(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION)));
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS,
					Boolean.valueOf(notification.getProperty(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS)));
			intent.putExtra(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN,
					notification.getProperty(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN));

			int uniqueRequestId = (int)System.currentTimeMillis();
			notification.setRequestCode(uniqueRequestId);

			// Set the unique request code as the handle.
			PendingIntent pendingIntent = PendingIntent.getBroadcast(
					appContext, uniqueRequestId, intent, PendingIntent.FLAG_CANCEL_CURRENT);

			// Register the alarm to the system, for the current date in milliseconds from UTC for this time zone.
			alarmManager.set(AlarmManager.RTC_WAKEUP, notification.getFireDate(), pendingIntent);
			m_Intents.put(handle, intent);

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
		SYSLOG("@@MoSync LocalNotificationsManager: unschedule");

		LocalNotificationObject notification = m_NotificationTable.get(handle);
		if ( null == notification )
		{
			Log.e("@@MoSync","maNotificationUnregisterLocal: Invalid notification handle " + handle);
			return MA_NOTIFICATION_RES_INVALID_HANDLE;
		}
		else
		{
			if ( !notification.getScheduled() )
			{
				SYSLOG("@@MoSync maNotificationLocalUnschedule: failed because notification was not scheduled.");
				return MA_NOTIFICATION_RES_CANNOT_UNSCHEDULE;
			}
			notification.setScheduled(false);

			// Remove the service notification if it is pending.
			if ( notification.isActive() )
			{
				LocalNotificationsService.removeServiceNotification(
						m_NotificationTable.get(handle).getId(),
						mMoSyncThread.getActivity());

				// Set the state of the notification to inactive.
				notification.setInactive();

				// No need to delete the notification, just stop
				// the service if it's started.
				return LocalNotificationsService.stopService();
			}

			// Cancel the  already scheduled intent.
			if (null != m_Intents.get(handle))
			{
				AlarmManager alarmManager  =
					(AlarmManager) mMoSyncThread.getActivity().getSystemService(Context.ALARM_SERVICE);

				// Set the unique request code as the handle.
				PendingIntent pendingIntent = PendingIntent.getBroadcast(
						mMoSyncThread.getActivity(),
						notification.getRequestCode(),
						m_Intents.get(handle),
						PendingIntent.FLAG_NO_CREATE);
				if ( pendingIntent != null )
				{
					alarmManager.cancel(pendingIntent);
				}
				m_Intents.remove(handle);
			}

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
	 * A table that contains mapping between a notification handle and a  pending intent assigned to it.
	 *
	 * The pending intent represents a task to run at a specified time in the schedule method.
	 * The key is the handle of the notification.
	 */
	private Hashtable<Integer,Intent> m_Intents =
		new Hashtable<Integer,Intent>();

	/**
	 * The MoSync application's focus state.
	 * If true the app is in foreground.
	 * If false the app is in background.
	 */
	private static Boolean mFocusState = true;
}