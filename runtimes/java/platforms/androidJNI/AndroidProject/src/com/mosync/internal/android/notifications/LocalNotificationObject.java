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


import android.app.Notification;
import android.content.Context;
import android.net.Uri;
import android.util.Log;

import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.LongConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_TICKER_TEXT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FIRE_DATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_TITLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLAG;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_DISPLAY_FLAG;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_FLAG_AUTO_CANCEL;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_FLAG_HIGH_PRIORITY;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_FLAG_INSISTENT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_FLAG_NO_CLEAR;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_SOUND_PATH;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_PLAY_SOUND;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_VIBRATE_DURATION;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_BODY;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_OK;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;

/**
 * An instance of a notification.
 * Holds required information like: text,title,id, flags.
 * When the user registers it to the NotificationManager
 * the Notification instance is created with current timebase.
 * @author emma tresanszki
 */
public class LocalNotificationObject {

	public final String NOTIFICATION_INVALID_PROPERTY_NAME = "Invalid property name";
	// The fire date default value.
	public final static int NOTIFICATION_FIRE_DATE_UNSET = -1;

	/*
	 * Create an empty notification object.
	 * @param icon The notification icon, typically
	 * the application icon.
	 */
	public LocalNotificationObject(Context appContext)
	{
		int icon = appContext.getResources().getIdentifier(
				"icon",
				"drawable",
				appContext.getPackageName());
		mNotification = new Notification();
		mNotification.icon = icon;
		mFireDate = NOTIFICATION_FIRE_DATE_UNSET;
	}

	/**
	 * Checks if the notification is already triggered.
	 * @return
	 */
	public boolean isActive()
	{
		return mIsActive;
	}

	/**
	 * Set the internal state of the notification.
	 * So we later know that we
	 * need to start/stop the service if there're
	 * no pending notifications.
	 */
	public void setActive(boolean active)
	{
		mIsActive = active;
	}

	/**
	 * Set the internal state of the notification.
	 * To inactive, so we later know that we
	 * don't need to stop the service if there're
	 * no pending notifications.
	 */
	public void setInactive()
	{
		mIsActive = false;
	}

	/**
	 * Set the scheduled state of this notification.
	 * @param state If true is was already scheduled for
	 * delivery, if false it wasn't scheduled or it was
	 * unscheduled.
	 */
	public void setScheduled(Boolean state)
	{
		mIsScheduled = state;
	}

	/**
	 * Get the scheduled state of this notification.
	 * @return True if it was scheduled, or false if it
	 * wasn't scheduled or it was unscheduled.
	 */
	public Boolean getScheduled()
	{
		return mIsScheduled;
	}

	/**
	 * Trigger the local notification.
	 */
	public void trigger()
	{
		mIsActive = true;
		// Set the time to now.
		mNotification.when = System.currentTimeMillis();

		// Set now the default settings like Flashing and Vibration,
		// because they could override the custom patterns.
		if ( mFlashingDefault )
		{
			mNotification.defaults |= Notification.DEFAULT_LIGHTS;
		}
		if ( mVibrateDefault )
		{
			mNotification.defaults |= Notification.DEFAULT_VIBRATE;
		}
		if ( mSoundDefault )
		{
			mNotification.defaults |= Notification.DEFAULT_SOUND;
		}
	}

	/**
	 * Set a specific property on the notification.
	 * @param property The property name.
	 * @param value The property value.
	 * @return result code.
	 */
	@SuppressWarnings("deprecation")
	public int setProperty(String name, String value)
		throws PropertyConversionException, InvalidPropertyValueException
	{
		if ( name.equals(MA_NOTIFICATION_LOCAL_TICKER_TEXT) )
		{
			mTickerText = value;
			mNotification.tickerText = mTickerText;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FIRE_DATE) )
		{
			if ( IntConverter.convert(value) < 0 )
				throw new InvalidPropertyValueException(name, value);
			if ( value.length() == 0 )
				throw new PropertyConversionException(value);
			mFireDate = (long)IntConverter.convert(value) * (long) 1000;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_CONTENT_BODY) )
		{
			mText = value;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_CONTENT_TITLE) )
		{
			mTitle = value;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_PLAY_SOUND) )
		{
			Boolean playSound = BooleanConverter.convert(value);
			mPlaySound = playSound;
			if ( playSound )
			{
				mSoundDefault = true;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_SOUND_PATH) )
		{
//			if ( value.length() == 0 )
//				throw new PropertyConversionException(value);
			mSoundPath = value;
			// Disable default sound.
			mSoundDefault = false;
			mNotification.sound = Uri.parse(value);
			mPlaySound = true;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLAG) )
		{
			int flag = IntConverter.convert(value);
			switch (flag)
			{
				case MA_NOTIFICATION_FLAG_AUTO_CANCEL:
					mNotification.flags |= Notification.FLAG_AUTO_CANCEL;
					break;
				case MA_NOTIFICATION_FLAG_HIGH_PRIORITY:
					mNotification.flags |= Notification.FLAG_HIGH_PRIORITY;
					break;
				case MA_NOTIFICATION_FLAG_INSISTENT:
					mNotification.flags |= Notification.FLAG_INSISTENT;
					break;
				case MA_NOTIFICATION_FLAG_NO_CLEAR:
					mNotification.flags |= Notification.FLAG_NO_CLEAR;
					break;
				default:
					throw new InvalidPropertyValueException(name, value);
			}
			mFlag = flag;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_DISPLAY_FLAG) )
		{
			int flag = IntConverter.convert(value);
			switch (flag)
			{
				case MA_NOTIFICATION_DISPLAY_FLAG_DEFAULT:
					mDisplayOnlyIfInBackground = true;
					break;
				case MA_NOTIFICATION_DISPLAY_FLAG_ANYTIME:
					mDisplayOnlyIfInBackground = false;
					break;
				default:
					throw new InvalidPropertyValueException(name, value);
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE) )
		{
			Boolean vibrate = BooleanConverter.convert(value);
			mVibrate = vibrate;
			if ( vibrate )
			{
				mVibrateDefault = true;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION) )
		{
			if ( LongConverter.convert(value) < 0 )
				throw new InvalidPropertyValueException(name, value);
			mVibrateDuration = LongConverter.convert(value);
			// Ignore the default value.
			mVibrateDefault = false;
			long[] sequence  = new long[2];
			sequence[0] = 0;
			sequence[1] = LongConverter.convert(value);
			mNotification.vibrate = sequence;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS) )
		{
			Boolean flashing = BooleanConverter.convert(value);
			mFlashingLights = flashing;
			if ( flashing )
			{
				mFlashingDefault = true;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN) )
		{
			// Ignore the default value.
			mFlashingDefault = false;
			// Test values.
//			mNotification.ledARGB = 0xff00ff00;
//			mNotification.ledOnMS = 5000;
//			mNotification.ledOffMS = 1000;
//			mNotification.flags |= Notification.FLAG_SHOW_LIGHTS;
			if ( value.length() == 0 )
				throw new PropertyConversionException(value);

			// Parse the 3 values separated by comma.
			int ledColor, ledOn, ledOff;
			// Make sure bad values get caught.
			int commaIndex = value.indexOf(",");
			if ( commaIndex != -1 && commaIndex < value.length() )
			{
				String value1, value2, value3;
				try{
					value1 = value.substring(0, commaIndex);
					ledColor = ColorConverter.convert(value1);
					String next = value.substring(commaIndex+1);
					commaIndex = next.indexOf(",");
					if ( commaIndex != -1 )
					{
						value2 = next.substring(0,commaIndex);
						ledOn = IntConverter.convert(value2);
						value3 = next.substring(commaIndex+1);
						ledOff = IntConverter.convert(value3);
						if ( ledOn < 0 || ledOff < 0 )
							throw new InvalidPropertyValueException(name, value);
						// Apply the pattern.
						mNotification.ledARGB = ledColor;
						mNotification.ledOnMS = ledOn;
						mNotification.ledOffMS = ledOff;
						mNotification.flags |= Notification.FLAG_SHOW_LIGHTS;
						mFlashingPattern = value;
					}
					else
					{
						throw new InvalidPropertyValueException(name, value);
					}
				}
				catch (IndexOutOfBoundsException iofbe)
				{
					throw new InvalidPropertyValueException(name, value);
				}
			}
			else
			{
				throw new InvalidPropertyValueException(name, value);
			}
		}
		else
		{
			Log.e("@@MoSync","maNotificationLocalSetProperty: Invalid property name " + name);
			return MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
		}
		return MA_NOTIFICATION_RES_OK;
	}

	/**
	 * Gets the value of a given property.
	 * @param name The name of the property.
	 * @return the property of the notification. If no property is found,
	 *         a string describing the error is returned.
	 * @throws PropertyConversionException
	 */
	public String getProperty(String name)
		throws PropertyConversionException
	{
		if( name.equals( MA_NOTIFICATION_LOCAL_CONTENT_BODY ) )
		{
			return mText;
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_CONTENT_TITLE) )
		{
			return mTitle;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_TICKER_TEXT) )
		{
			return mTickerText;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLAG) )
		{
			return Integer.toString(mFlag);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_FIRE_DATE) )
		{
			return Long.toString(mFireDate/(long)1000);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_PLAY_SOUND) )
		{
			return Boolean.toString(mPlaySound);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_SOUND_PATH) )
		{
			return mSoundPath;
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE) )
		{
			return Boolean.toString(mVibrate);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION) )
		{
			return Long.toString(mVibrateDuration);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS) )
		{
			return Boolean.toString(mFlashingLights);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN) )
		{
			return mFlashingPattern;
		}
		Log.e("@@MoSync", "maNotificationGetProperty Invalid property name " + name);
		return NOTIFICATION_INVALID_PROPERTY_NAME;
	}

	/**
	 * Gets the notification object.
	 * @return The notification.
	 */
	public Notification getNotification()
	{
		return mNotification;
	}

	/**
	 * Gets the display flag for this notification object.
	 * @return true if the notification should be displayed only
	 * if the app is in background, false if the notification should
	 * be displayed regardless the focus state.
	 */
	public Boolean showOnlyInBackground()
	{
		return mDisplayOnlyIfInBackground;
	}

	/**
	 * Get the content body of the notification.
	 * @return The body.
	 */
	public String getText()
	{
		return mText;
	}

	public String getTicker()
	{
		return mTickerText;
	}

	public String getSoundPath()
	{
		return mSoundPath;
	}

	public long getVibrateDuration()
	{
		return mVibrateDuration;
	}

	public String getFlashPattern()
	{
		return mFlashingPattern;
	}

	public boolean setFlashPattern(String value)
	{
		try{
			setProperty(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN, value);
			return true;
		}catch(RuntimeException ex)
		{
			return false;
		}
	}

	public void setSoundPath(String value)
	{
		try{
			setProperty(MA_NOTIFICATION_LOCAL_SOUND_PATH, value);
		}catch(RuntimeException re)
		{
		}
	}

	public boolean setFireDate(String value)
	{
		try{
			setProperty(MA_NOTIFICATION_LOCAL_FIRE_DATE, value);
			return true;
		}catch(Exception ex)
		{
			return false;
		}
	}

	public boolean setVibrateDuration(long duration)
	{
		try{
			setProperty(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION, String.valueOf(duration));
			return true;
		}catch(RuntimeException re)
		{
			return false;
		}
	}
	/**
	 * Get the content title of the notification.
	 * @return The title.
	 */
	public String getTitle()
	{
		return mTitle;
	}

	/**
	 * Sets the unique Id of the notification.
	 * @param id The id.
	 */
	public void setId(final int id)
	{
		mId = id;
	}

	/**
	 * Get the unique Id of the notification.
	 * @return the Id.
	 */
	public int getId()
	{
		return mId;
	}

	/**
	 * Get the fire date of the notification.
	 * @return the fire date in milliseconds.
	 */
	public long getFireDate()
	{
		return mFireDate;
	}

	public void setFlag(int flag)
	{
		mFlag = flag;
		try{
			setProperty(MA_NOTIFICATION_LOCAL_FLAG, String.valueOf(flag));
		}catch(RuntimeException re)
		{
		}
	}

	public int getFlag()
	{
		return mFlag;
	}
	public void setRequestCode(int code)
	{
		mRequestCode = code;
	}

	public int getRequestCode()
	{
		return mRequestCode;
	}
	/************************ Class members ************************/

	/**
	 * The local notification.
	 */
	private Notification mNotification;

	/**
	 * If true, the notification is active.
	 */
	private Boolean mIsActive = false;

	/**
	 * If true, the notification is already scheduled for delivery.
	 */
	private Boolean mIsScheduled = false;

	/**
	 * The id of the notification.
	 * It's value is -1 in case of any error.
	 */
	private int mId = -1;

	/**
	 * The text that flows by in the status bar when the notification first activates.
	 */
	private String mTickerText = "";

	/**
	 * Used for setLatestEventInfo.
	 * The title that goes in the expanded entry.
	 */
	private String mTitle = "";

	/**
	 * The text that goes in the expanded entry.
	 */
	private String mText = "";

	/**
	 * The fire date in milliseconds for the notification.
	 */
	private long mFireDate;

	/**
	 * The flag applied to the notification.
	 */
	private int mFlag=0;

	/**
	 * Enable/disable the sound played when an alert is displayed.
	 */
	private Boolean mPlaySound = false;
	private String mSoundPath = "";
	private Boolean mSoundDefault = false;

	/**
	 * Enable/disable the vibration when an alert is displayed.
	 */
	private Boolean mVibrate = false;
	private long mVibrateDuration = 0;
	private Boolean mVibrateDefault = false;

	/**
	 * Enable/disable the flashing lights.
	 */
	private Boolean mFlashingLights = false;
	private String mFlashingPattern = "";
	private Boolean mFlashingDefault = false;

	/**
	 * Display this notification only if the application is in background.
	 * If set to false the notification will be presented to the user
	 * regardless of the focus state of the application.
	 */
	private Boolean mDisplayOnlyIfInBackground = true;

	/**
	 * The unique request code associated to the pending intent of this notification.
	 */
	private int mRequestCode = -1;
}