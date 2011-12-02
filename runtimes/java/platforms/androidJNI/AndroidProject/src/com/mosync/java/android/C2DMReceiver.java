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
public class C2DMReceiver extends C2DMBaseReceiver
{
	/**
	 * The name of the extra data sent when launching MoSync activity.
	 * Used at incoming push notifications.
	 */
	public static String MOSYNC_INTENT_EXTRA_MESSAGE = "com.mosync.java.android.IntentExtra";

	public C2DMReceiver()
	{
		// Email address currently not used by the C2DM Messaging framework
		super("dummy@google.com");
	}

	/**
	 * Launch the MoSync application when a push notification is received.
	 * @param context
	 * @param message The push message.
	 */
	private static void activateMoSyncApp(Context context, String message)
	{
		Log.e("@@MoSync","Launch MoSync activity");

		Intent launcherIntent = new Intent(context, MoSync.class);
		launcherIntent.setAction(C2DMBaseReceiver.C2DM_INTENT);
		// Add the push message to the intent.
		launcherIntent.putExtra(MOSYNC_INTENT_EXTRA_MESSAGE, message);
		launcherIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
				| Intent.FLAG_ACTIVITY_REORDER_TO_FRONT
				| Intent.FLAG_ACTIVITY_SINGLE_TOP);
		context.startActivity(launcherIntent);
	}

	/**
	 * Called when a registration token has been received.
	 */
	@Override
	public void onRegistered(Context context, String registrationId)
			throws java.io.IOException
	{
		Log.e("@@MoSync", "C2DM Registration success");
		// Notify the manager of this event.
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.registrationReady(registrationId);
	};

	/**
	 * Called when a cloud message has been received.
	 * @param context
	 * @param intent The intent that
	 */
	@Override
	protected void onMessage(Context context, Intent intent)
	{
		Log.e("@@MoSync", "C2DM Message received");
		// Create new PushNotificationObject that holds the payload.
		final String message = intent.getStringExtra("payload");
		// Process the message only if the payload string is not empty.
		if ( message != null )
		{
			// If the MoSync activity is already started,
			// just display the notification and send the push event.
			if ( PushNotificationsManager.getRef() != null )
			{
				PushNotificationsManager.getRef().messageReceived(message);
			}
			else
			{
				// Otherwise, start the MoSync activity and display the notification.
				activateMoSyncApp(context, message);
			}
		}
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