/* Copyright (C) 2013 MoSync AB

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
public class AlarmReceiver  extends BroadcastReceiver{

	@Override
	public void onReceive(Context appContext, Intent intent) {
		Log.e("@@MoSync", "AlarmReceiver: onReceive ");

		Bundle extras = intent.getExtras();
		if (extras != null)
		{
			int notificationId = extras.getInt(
					LocalNotificationsService.LOCAL_NOTIFICATION_ID,
					LocalNotificationsService.LOCAL_NOTIFICATION_ID_DEFAULT);

			String notificationTitle = extras.getString(
					MA_NOTIFICATION_LOCAL_CONTENT_TITLE, "");

			String notificationBody = extras.getString(
					MA_NOTIFICATION_LOCAL_CONTENT_BODY, "");

			String notificationTicker = extras.getString(
					MA_NOTIFICATION_LOCAL_TICKER_TEXT, "");

			Boolean notificationPlaySound = extras.getBoolean(
					MA_NOTIFICATION_LOCAL_PLAY_SOUND, false);

			String notificationSoundPath = extras.getString(
					MA_NOTIFICATION_LOCAL_SOUND_PATH, "");

			Boolean notificationVibrate = extras.getBoolean(
					MA_NOTIFICATION_LOCAL_VIBRATE, false);

			long notificationVibrateDuration = extras.getLong(
					MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, 0);

			Boolean notificationFlash = extras.getBoolean(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS, false);

			String notificationFlashPattern = extras.getString(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN, "");

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