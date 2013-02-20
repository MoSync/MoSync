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

import com.mosync.nativeui.util.properties.BooleanConverter;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
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
import static com.mosync.internal.android.notifications.LocalNotificationsService.LOCAL_NOTIFICATION_ID;

/**
 * Utility class for storing and accessing
 * editor preferences.
 * Create a new Editor for these preferences, through which
 * you can make modifications to the data in the preferences
 * and atomically commit those changes back to the SharedPreferences object.
 * @author emma tresanszki
 *
 */
public class LocalNotificationsUtil
{
	/**
	 * The name of the extra data sent when launching MoSync activity.
	 * Used at incoming local notifications.
	 */
	public static String MOSYNC_INTENT_EXTRA_MESSAGE = "com.mosync.java.android.IntentExtra";
	public static String MOSYNC_INTENT_EXTRA_NOTIFICATION_HANDLE = "push.notification.handle";
	public static String MOSYNC_INTENT_EXTRA_NOTIFICATION = "push.notification";

	// The name of the shared preference.
    public static final String PREFERENCE = "com.mosync.internal.android.notifications.local";

    static void setLocalNotificationInfo(Context context, LocalNotificationObject object)
    {
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_MULTI_PROCESS);
        Editor editor = prefs.edit();
        editor.clear();
        editor.putString(MA_NOTIFICATION_LOCAL_CONTENT_TITLE,
				object.getTitle())
        .putString(MA_NOTIFICATION_LOCAL_CONTENT_BODY,
				object.getText())
        .putString(MA_NOTIFICATION_LOCAL_TICKER_TEXT,
				object.getTicker())
        .putInt(LocalNotificationsService.LOCAL_NOTIFICATION_ID,
				object.getId())
        .putInt(MA_NOTIFICATION_LOCAL_FLAG, object.getFlag())
        .putBoolean(MA_NOTIFICATION_LOCAL_PLAY_SOUND,
				Boolean.valueOf(object.getProperty(MA_NOTIFICATION_LOCAL_PLAY_SOUND)))
        .putString(MA_NOTIFICATION_LOCAL_SOUND_PATH,
				object.getSoundPath())
        .putBoolean(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS,
				Boolean.valueOf(object.getProperty(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS)))
        .putString(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN,
				object.getFlashPattern())
        .putBoolean(MA_NOTIFICATION_LOCAL_VIBRATE,
				Boolean.valueOf(object.getProperty(MA_NOTIFICATION_LOCAL_VIBRATE)))
        .putLong(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION,
				object.getVibrateDuration());
        editor.commit();
//        editor.apply(); Available only since API level 9. Use it when we upgrade the version.
    }

    static LocalNotificationObject getLocalNotificationInfo(Context context)
    {
		LocalNotificationObject object = new LocalNotificationObject(context);
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_MULTI_PROCESS);

        object.setId(
				prefs.getInt(LocalNotificationsService.LOCAL_NOTIFICATION_ID,
				0));
        object.setProperty(
				MA_NOTIFICATION_LOCAL_CONTENT_TITLE,
				prefs.getString(MA_NOTIFICATION_LOCAL_CONTENT_TITLE, ""));
        object.setProperty(
				MA_NOTIFICATION_LOCAL_CONTENT_BODY,
				prefs.getString(MA_NOTIFICATION_LOCAL_CONTENT_BODY, ""));
        object.setProperty(
				MA_NOTIFICATION_LOCAL_TICKER_TEXT,
				prefs.getString(MA_NOTIFICATION_LOCAL_TICKER_TEXT, ""));
        object.setFlag(prefs.getInt(MA_NOTIFICATION_LOCAL_FLAG, 0));
        object.setProperty(
				MA_NOTIFICATION_LOCAL_PLAY_SOUND,
				String.valueOf(prefs.getBoolean(MA_NOTIFICATION_LOCAL_PLAY_SOUND, false)));
        String soundPath = prefs.getString(MA_NOTIFICATION_LOCAL_SOUND_PATH, "");
        if (!soundPath.isEmpty())
        {
			object.setProperty(
					MA_NOTIFICATION_LOCAL_SOUND_PATH,
					soundPath);
        }
        object.setProperty(
				MA_NOTIFICATION_LOCAL_FLASH_LIGHTS,
				String.valueOf(prefs.getBoolean(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS, false)));
        String flashPattern = prefs.getString(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN, "");
        if (!flashPattern.isEmpty())
        {
            object.setProperty(
					MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN,
					flashPattern);
        }
        object.setProperty(
				MA_NOTIFICATION_LOCAL_VIBRATE,
				String.valueOf(prefs.getBoolean(MA_NOTIFICATION_LOCAL_VIBRATE, false)));
        object.setVibrateDuration(
				prefs.getLong(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, 0));
        //clearPreferences(context);

        return object;
    }

    static void clearPreferences(Context context)
    {
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_MULTI_PROCESS);
        Editor editor = prefs.edit();
        editor.remove(MA_NOTIFICATION_LOCAL_CONTENT_BODY);
        editor.remove(MA_NOTIFICATION_LOCAL_CONTENT_TITLE);
        editor.remove(MA_NOTIFICATION_LOCAL_FLAG);
        editor.remove(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS);
        editor.remove(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN);
        editor.remove(MA_NOTIFICATION_LOCAL_PLAY_SOUND);
        editor.remove(MA_NOTIFICATION_LOCAL_SOUND_PATH);
        editor.remove(MA_NOTIFICATION_LOCAL_VIBRATE);
        editor.remove(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION);
        editor.remove(MA_NOTIFICATION_LOCAL_FLAG);
        editor.remove(LocalNotificationsService.LOCAL_NOTIFICATION_ID);
        editor.commit();
    }

}