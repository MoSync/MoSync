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

import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

/**
 * Utility class for storing and accessing
 * editor preferences.
 * Create a new Editor for these preferences, through which
 * you can make modifications to the data in the preferences
 * and atomically commit those changes back to the SharedPreferences object.
 * @author emma tresanszki
 *
 */
public class PushNotificationsUtil
{
	// Fields to store in Editor preferences.
    public static final String PREFERENCE = "com.mosync.internal.android.notifications";
    public static final String NOTIFICATION_TITLE = "title";
    public static final String NOTIFICATION_TICKER = "ticker";

    /**
     * Get the registration back off time.
     * @param context
     * @return The preferred title, or
     * an empty string if it was not defined.
     */
    static String getNotificationTitle(Context context)
    {
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_PRIVATE);

        return prefs.getString(NOTIFICATION_TITLE,"");
    }

    /**
     * Store push notifications title,
     * so it can be accessed for incoming notifications.
     *
     * @param context
     * @param text The notification title.
     */
    static void setPushNotificationTitle(Context context, String text)
    {
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_PRIVATE);
        Editor editor = prefs.edit();
        editor.putString(NOTIFICATION_TITLE, text);
        editor.commit();
    }

    /**
     * Get the text that was set for notification title.
     * @param context
     * @return The preferred title, or
     * an empty string if it was not defined.
     */
    static String getNotificationTicker(Context context)
    {
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_PRIVATE);

        return prefs.getString(NOTIFICATION_TITLE,"");
    }

    /**
     * Store push notifications ticker text,
     * so it can be accessed for incoming notifications.
     *
     * @param context
     * @param text The ticker text for the status bar.
     */
    static void setPushNotificationTicker(Context context, String text)
    {
        final SharedPreferences prefs = context.getSharedPreferences(
                PREFERENCE,
                Context.MODE_PRIVATE);
        Editor editor = prefs.edit();
        editor.putString(NOTIFICATION_TITLE, text);
        editor.commit();
    }
}