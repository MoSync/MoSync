package com.mosync.java.android;

import com.google.android.c2dm.C2DMBaseReceiver;
import com.mosync.internal.android.notifications.PushNotificationsManager;

import android.content.Context;
import android.content.Intent;
import android.util.Log;

/**
 * C2DM receiver for registration and push messages from Google.
 * @author emma tresanszki
 *
 */
public class C2DMReceiver extends C2DMBaseReceiver {
	public C2DMReceiver() {
		// Email address currently not used by the C2DM Messaging framework
		super("dummy@google.com");
	}

	/**
	 * Called when a registration token has been received.
	 */
	@Override
	public void onRegistered(Context context, String registrationId)
			throws java.io.IOException {
		Log.e("@@MoSync", "C2DM Registration success");
		// Notify the manager of this event.
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.registrationReady(registrationId);
	};

	/**
	 * Called when a cloud message has been received.
	 */
	@Override
	protected void onMessage(Context context, Intent intent) {
		Log.e("@@MoSync", "C2DM Message received");
		// Create new PushNotificationObject that holds the payload.
		final String message = intent.getStringExtra("payload");
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.messageReceived(message);
	}

	/**
	 * Called on registration error.
	 */
	@Override
	public void onError(Context context, String errorId) {
		Log.e("@@MoSync", " C2DM Registration error");
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.registrationFail(errorId);
	}

	/**
	 * Called when the device has been unregistered.
	 */
	@Override
	public void onUnregistered(Context context) {
		Log.e("@@MoSync", "C2Dm Unregistered");
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.unregistered();
	}
}