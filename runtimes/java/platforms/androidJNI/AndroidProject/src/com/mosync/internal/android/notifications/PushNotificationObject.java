package com.mosync.internal.android.notifications;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.Context;
import android.content.Intent;

import com.mosync.java.android.MoSync;

/**
 * An instance of a push notification.
 * A push notification is created when C2DMBaseReceiver is processing a new message.
 * Holds required information like: content body, content title, icon, ticker text.
 * The content body can consist of the message payload, or a user defined string.
 * @author emma tresanszki
 */
public class PushNotificationObject
{

	public PushNotificationObject() {}

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
	 * Triggers a notification for the message received from C2DM.
	 * Apply the ticker text, message payload and title.
	 * @param context Application's context.
	 */
	public void triggerNotification(Context context)
	{
		NotificationManager notificationManager = (NotificationManager) context
		.getSystemService(Context.NOTIFICATION_SERVICE);
		Notification notification = new Notification(
				mIcon, mTicker, System.currentTimeMillis());
		// Hide the notification after its selected.
//		notification.flags |= Notification.FLAG_AUTO_CANCEL;

		Intent intent = new Intent(context, MoSync.class);
		intent.putExtra("payload", mMessage);
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