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
 */
public class PushNotificationObject {

	/**
	 * Constructor.
	 */
	PushNotificationObject(String payload, int icon)
	{
		mMessage = payload;
		mIcon = icon;
	}

	/**
	 * Triggers a notification for the message received from C2DM.
	 */
	public void triggerNotification(Context context)
	{
		NotificationManager notificationManager = (NotificationManager) context
		.getSystemService(Context.NOTIFICATION_SERVICE);
		Notification notification = new Notification(mIcon,
		"MoSync Notification", System.currentTimeMillis());
		// Hide the notification after its selected
		notification.flags |= Notification.FLAG_AUTO_CANCEL;

		Intent intent = new Intent(context, MoSync.class);
		intent.putExtra("payload", mMessage);
		PendingIntent pendingIntent = PendingIntent.getActivity(context, 0, intent, 0);
		notification.setLatestEventInfo(context, "Message received", mMessage, pendingIntent);
		notificationManager.notify(0, notification);
	}

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

	private int mIcon;
}
