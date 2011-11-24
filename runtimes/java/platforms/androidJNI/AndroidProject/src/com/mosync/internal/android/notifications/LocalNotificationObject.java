package com.mosync.internal.android.notifications;

import android.app.Notification;
import android.net.Uri;
import android.util.Log;

import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.IntConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.LongConverter;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_TICKER_TEXT;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FIRE_DATE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_CONTENT_TITLE;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_FLAG;
import static com.mosync.internal.generated.MAAPI_consts.MA_NOTIFICATION_LOCAL_SOUND;
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

	/**
	 * Flashing LED lights
	 * Define color and pattern.
	 */
	class Flash_Lights{
		public int ledARGB;
		// Length of time, in milliseconds, to keep the light on.
		public int ledOnMS;
		// Length of time, in milliseconds, to keep the light off.
		public int ledOffMS;
	}

	public final String NOTIFICATION_INVALID_PROPERTY_NAME = "Invalid property name";

	/*
	 * Create an empty notification object.
	 * @param icon The notification icon, typically
	 * the app icon.
	 */
	public LocalNotificationObject(int icon)
	{
		mNotification = new Notification();
		mNotification.icon = icon;
	}

	/**
	 * Trigger the local notification.
	 */
	public void trigger()
	{
		mIsActive = true;
		// Set the time to now.
		mNotification.when = System.currentTimeMillis();
		mNotification.tickerText = mTickerText;
		mNotification.flags |= Notification.FLAG_ONGOING_EVENT;
	}

	/**
	 * Set a specific property on the notification.
	 * @param property The property name.
	 * @param value The property value.
	 * @return result code.
	 */
	public int setProperty(String name, String value)
		throws PropertyConversionException
	{
		if ( name.equals(MA_NOTIFICATION_LOCAL_TICKER_TEXT) )
		{
			mTickerText = value;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FIRE_DATE) )
		{
			mFireDate = LongConverter.convert(value);
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
				mNotification.defaults |= Notification.DEFAULT_SOUND;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_SOUND) )
		{
			if ( value.isEmpty() )
				throw new PropertyConversionException(value);
			mNotification.sound = Uri.parse(value);
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLAG) )
		{
			int flag = IntConverter.convert(value);
			mNotification.defaults |= flag;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE) )
		{
			Boolean vibrate = BooleanConverter.convert(value);
			mVibrate = vibrate;
			if ( vibrate )
			{
				mNotification.defaults |= Notification.DEFAULT_VIBRATE;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE_DURATION) )
		{
				long[] sequence  = new long[2];
				sequence[0] = 0;
				sequence[1] = IntConverter.convert(value);
				mNotification.vibrate = sequence;
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS) )
		{
			Boolean flashing = BooleanConverter.convert(value);
			mFlashingLights = flashing;
			if ( flashing )
			{
				mNotification.defaults = Notification.DEFAULT_LIGHTS;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS_PATTERN) )
		{
			// Test values.
//			mNotification.ledARGB = 0xff00ff00;
//			mNotification.ledOnMS = 300;
//			mNotification.ledOffMS = 1000;
//			mNotification.flags |= Notification.FLAG_SHOW_LIGHTS;
			if ( value.isEmpty() )
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
					ledColor = IntConverter.convert(value1);
					String next = value.substring(commaIndex+1);
					commaIndex = next.indexOf(",");
					if ( commaIndex != -1 )
					{
						value2 = next.substring(0,commaIndex);
						ledOn = IntConverter.convert(value2);
						value3 = next.substring(commaIndex+1);
						ledOff = IntConverter.convert(value3);
						// Apply the pattern.
						mNotification.ledARGB = ledColor;
						mNotification.ledOnMS = ledOn;
						mNotification.ledOffMS = ledOff;
						mNotification.flags |= Notification.FLAG_SHOW_LIGHTS;
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
		else if( name.equals(MA_NOTIFICATION_LOCAL_FIRE_DATE) )
		{
			return Long.toString(mFireDate);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_PLAY_SOUND) )
		{
			return Boolean.toString(mPlaySound);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_VIBRATE) )
		{
			return Boolean.toString(mVibrate);
		}
		else if( name.equals(MA_NOTIFICATION_LOCAL_FLASH_LIGHTS) )
		{
			return Boolean.toString(mFlashingLights);
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
	 * Get the content body of the notification.
	 * @return The body.
	 */
	public String getText()
	{
		return mText;
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

	/************************ Class members ************************/

	/**
	 * The local notification.
	 */
	private Notification mNotification;

	/**
	 * If true, the notification is active.
	 */
	public Boolean mIsActive = false;

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
	private long mFireDate = -1;

	/**
	 * Enable/disable the sound played when an alert is displayed.
	 */
	private Boolean mPlaySound = false;

	/**
	 * Enable/disable the vibration when an alert is displayed.
	 */
	private Boolean mVibrate = false;

	/**
	 * Enable/disable the flashing lights.
	 */
	private Boolean mFlashingLights = false;
}