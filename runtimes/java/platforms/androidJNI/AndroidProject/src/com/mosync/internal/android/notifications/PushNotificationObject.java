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

package com.mosync.internal.android.notifications;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import com.mosync.java.android.MoSync;

/**
 * An instance of a push notification.
 * A push notification is created when GCMBaseIntentService is processing a new message.
 * Holds required information like: content body, content title, icon, ticker text.
 * The content body can consist of the message payload, or a user defined string.
 * @author emma tresanszki
 */
public class PushNotificationObject
{
	public PushNotificationObject(){
	}

	/**
	 * Constructor.
	 */
	PushNotificationObject(String payload, int icon, String tickerText, String title)
	{
		mMessage = payload;
		mIcon = icon;
		mTicker = tickerText;
		mMessageTitle = title;
	}

	/**
	 * Triggers a notification for the message received from GCM/C2DM.
	 * Apply the ticker text, message payload and title.
	 * @param context Application's context.
	 */
	public void triggerNotification(Context context, int id)
	{
		NotificationManager notificationManager = (NotificationManager) context
		.getSystemService(Context.NOTIFICATION_SERVICE);
		Notification notification = new Notification(
				mIcon, mTicker, System.currentTimeMillis());
		// Hide the notification after its selected.
		notification.flags |= Notification.FLAG_AUTO_CANCEL;

		Intent intent = new Intent(context, MoSync.class);
		intent.putExtra(PushNotificationsUtil.MOSYNC_INTENT_EXTRA_MESSAGE, mMessage);
		intent.putExtra(PushNotificationsUtil.MOSYNC_INTENT_EXTRA_NOTIFICATION, true);
		// Send also the notification handle so it can later be used at posting events.
		intent.putExtra(PushNotificationsUtil.MOSYNC_INTENT_EXTRA_NOTIFICATION_HANDLE, id);
		PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);
		notification.setLatestEventInfo(context, mMessageTitle, mMessage, pendingIntent);
		notificationManager.notify(0, notification);
	}

	/**
	 * Get the message payload.
	 * @return The payload.
	 */
	String getMessage()
	{
		return mMessage;
	}

	/************************ Class members ************************/
	/**
	 * Content body.
	 */
	private String mMessage = null;

	/**
	 * Content title.
	 */
	private String mMessageTitle = null;

	/**
	 * Ticker text in the status bar.
	 */
	private String mTicker = null;

	/**
	 * The icon of the notification,
	 * typically the app icon.
	 */
	private int mIcon;
}