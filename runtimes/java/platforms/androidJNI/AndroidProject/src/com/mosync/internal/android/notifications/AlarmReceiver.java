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

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_TICKER_TEXT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_BODY;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_TITLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_PLAY_SOUND;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_SOUND_PATH;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE_DURATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLAG;

/**
 * The AlarmReceiver is the broadcast receiver responsible for handling
 * scheduled notifications, triggered by the AlarmManager.
 * @author emma tresanszki
 *
 */
public class AlarmReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context appContext, Intent intent) {
		Log.e("@@MoSync", "AlarmReceiver: onReceive ");

		Bundle extras = intent.getExtras();
		if (extras != null)
		{
			int notificationId = extras.getInt(
					LocalNotificationsService.LOCAL_NOTIFICATION_ID,
					LocalNotificationsService.LOCAL_NOTIFICATION_ID_DEFAULT);

			String notificationTitle = "";
			notificationTitle = extras.getString(
					MA_NOTIFICATION_LOCAL_CONTENT_TITLE);

			String notificationBody = "";
			notificationBody = extras.getString(
					MA_NOTIFICATION_LOCAL_CONTENT_BODY);

			String notificationTicker = "";
			notificationTicker = extras.getString(
					MA_NOTIFICATION_LOCAL_TICKER_TEXT);

			Boolean notificationPlaySound = extras.getBoolean(
					MA_NOTIFICATION_LOCAL_PLAY_SOUND, false);

			String notificationSoundPath = "";
			notificationSoundPath = extras.getString(
					MA_NOTIFICATION_LOCAL_SOUND_PATH);

			Boolean notificationVibrate = extras.getBoolean(
					MA_NOTIFICATION_LOCAL_VIBRATE, false);

			long notificationVibrateDuration = extras.getLong(
					MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, 0);

			Boolean notificationFlash = extras.getBoolean(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS, false);

			String notificationFlashPattern = "";
			notificationFlashPattern = extras.getString(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN);

			int notificationFlag = extras.getInt(
					MA_NOTIFICATION_LOCAL_FLAG, 0);

			LocalNotificationsService.startService(appContext, notificationId,
					notificationTitle, notificationBody, notificationTicker,
					notificationPlaySound, notificationSoundPath,
					notificationVibrate, notificationVibrateDuration,
					notificationFlash, notificationFlashPattern, notificationFlag);
		}
	}

}
