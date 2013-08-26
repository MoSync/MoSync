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

package com.mosync.java.android;

import android.app.IntentService;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

import com.mosync.internal.android.notifications.PushNotificationsManager;
import com.mosync.internal.android.notifications.PushNotificationsUtil;
import com.google.android.gcm.GCMBaseIntentService;
import com.google.android.gcm.GCMConstants;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME;

/**
 * {@link IntentService} responsible for handling GCM messages.
 */
public class GCMIntentService extends GCMBaseIntentService {

    @SuppressWarnings("hiding")
    private static final String TAG = "GCMIntentService";

    public GCMIntentService() {
        super("dummy@google.com");
    }

    @Override
    protected void onRegistered(Context context, String registrationId) {
		Log.e("@@MoSync", "CGM Registration success");

		// Notify the manager of this event.
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.registrationReady(registrationId);
    }

    @Override
    protected void onUnregistered(Context context, String registrationId) {
		Log.e("@@MoSync", "GCM Unregistered");
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.unregistered();
    }

	@Override
	protected void onMessage(Context context, Intent intent) {
		Log.e("@@MoSync", "GCM Message received");

		// Create new PushNotificationObject that holds the payload.
		String message = intent.getStringExtra("payload");

		// Notify also blank messages, as the CGM service demo sends empty
		// payload.
		if (message == null)
			message = "";

		// If the MoSync activity is already started, send the push event,
		// but display the notification only if
		// MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME
		// flag was set via maNotificationPushSetDisplayFlag syscall.
		if (PushNotificationsManager.getRef() != null)
		{
			if (PushNotificationsUtil.getPushNotificationDisplayFlag(context)
					== MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME)
			{
				// Notify the NotificationsManager on the new message. Display
				// the notification.
				PushNotificationsManager.getRef()
						.messageReceived(message, true);
			}
			else
			{
				// Notify the NotificationsManager on the new message. But don't
				// display the notification.
				PushNotificationsManager.getRef().messageReceived(message,
						false);
				Log.e("@@MoSync",
						"PushNotifications: new message received. The notification is not displayed because the application is running");
			}
		}
		else
		{
			// Display the notification, and when the MoSync activity is started
			// by clicking on the notification the push event is received.
			PushNotificationsManager.messageReceivedWhenAppNotRunning(message,
					context);
		}
	}

    @Override
    protected void onDeletedMessages(Context context, int total) {
        Log.i(TAG, "Received deleted messages notification");
    }

    @Override
    public void onError(Context context, String errorId) {
		Log.e("@@MoSync", " GCM Registration error");
		PushNotificationsManager manager = PushNotificationsManager.getRef();
		manager.registrationFail(errorId);
    }

    @Override
    protected boolean onRecoverableError(Context context, String errorId) {
        // log message
        Log.i(TAG, "Received recoverable error: " + errorId);
        return super.onRecoverableError(context, errorId);
    }

	/**
	 * Launch the MoSync application when a push notification is received.
	 * Save it for later use in case we need to launch MoSync activity
	 * automatically when new push notification is received.
	 * @param context
	 * @param message The push message.
	 */
	private static void activateMoSyncApp(Context context, String message)
	{
		Log.e("@@MoSync","Launch MoSync activity");

		Intent launcherIntent = new Intent(context, MoSync.class);
		launcherIntent.setAction(GCMConstants.INTENT_FROM_GCM_MESSAGE);
		// Add the push message to the intent.
		launcherIntent.putExtra(
				PushNotificationsUtil.MOSYNC_INTENT_EXTRA_MESSAGE, message);
		launcherIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK
				| Intent.FLAG_ACTIVITY_REORDER_TO_FRONT
				| Intent.FLAG_ACTIVITY_SINGLE_TOP);
		context.startActivity(launcherIntent);
	}
}
