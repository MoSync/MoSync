package com.mosync.java.android;

import java.util.Date;

import com.mosync.nativeui.util.properties.BooleanConverter;
import com.mosync.nativeui.util.properties.ColorConverter;
import com.mosync.nativeui.util.properties.InvalidPropertyValueException;
import com.mosync.nativeui.util.properties.PropertyConversionException;

import android.app.Notification;
import android.net.Uri;
import android.util.Log;

/**
 * An instance of a notification.
 * Holds required information like: text,title,id, flags.
 * When the user registers it to the NotificationManager
 * the Notification instance is created with current timebase.
 *
 */
public class LocalNotificationObject {

	/**
	 * Flashing LED lights
	 * Define color and pattern.
	 */
	class Flash_Lights{
		int ledARGB;
		// Length of time, in milliseconds, to keep the light on.
		int ledOnMS;
		// Length of time, in milliseconds, to keep the light off.
		int ledOffMS;
	}

	/*
	 * Create an empty notification object.
	 */
	public LocalNotificationObject(int icon)
	{
//		mNotification = new Notification(icon, "", when);
		mNotification = new Notification();
		mNotification.icon = icon;
	}

	/**
	 * Trigger the notification.
	 */
	public void trigger()
	{
		mIsActive = true;
		// Set the time to now.
		mNotification.when = System.currentTimeMillis();
//		mNotification = new Notification(mIcon, mTickerText, mWhen);
		mNotification.tickerText = mTickerText;
		mNotification.flags |= Notification.FLAG_ONGOING_EVENT;
		mNotification.defaults |= Notification.DEFAULT_SOUND;
		long[] vibrate = {0,100,200,300};
		mNotification.vibrate = vibrate;
		mNotification.defaults |= Notification.DEFAULT_LIGHTS;
	}

	public int setProperty(String name, String value)
		throws PropertyConversionException
	{/*
		if ( name.equals(MA_NOTIFICATION_TICKER_TEXT) )
		{
			mTickerText = value;
		}
		else if ( name.equals(MA_NOTIFICATION_FIRE_DATE) )
		{
			// COnvert value
//			mFireDate = value;
		}
		else if ( name.equals(MA_NOTIFICATION_CONTENT_BODY) )
		{
			mText = value;
		}
		else if ( name.equals(MA_NOTIFICATION_CONTENT_TITLE) )
		{
			mTitle = value;
		}
		else if ( name.equals(MA_NOTIFICATION_PLAY_SOUND) )
		{
			mSound = BooleanConverter.convert(value);
		}
		else if ( name.equals(MA_NOTIFICATION_SOUND) )
		{
			//SOUND_DEFAULT
			if ( value.equals(MA_NOTIFICATION_SOUND_DEFAULT) )
			{
				mNotification.defaults |= Notification.DEFAULT_SOUND;
			}
			else
			{
				mNotification.sound = Uri.parse("file:///sdcard/notification/ringer.mp3");
			}
		}
		else if ( name.equals(MA_NOTIFICATION_FLAG_AUTO_CANCEL) )
		{
			// Ignore the value.
			mNotification.defaults = Notification.FLAG_AUTO_CANCEL;
		}
		else if ( name.equals(MA_NOTIFICATION_VIBRATE) )
		{
			if ( value.equals(MA_NOTIFICATION_VIBRATE_DEFAULT) )
			{
				mNotification.defaults |= Notification.DEFAULT_VIBRATE;
			}
			else
			{
				// The long array defines the alternating pattern for the length of vibration
				// off and on (in milliseconds). The first value is how long to wait (off)
				// before beginning, the second value is the length of the first vibration,
				// the third is the next length off, and so on.
				// The pattern can be as long as you like, but it can't be set to repeat.
				// Values separated by comma.

				long[] sequence = {0,100,200,300};
				int index = value.indexOf(",");
				while ( index != -1 )
				{
					index = value.indexOf(",");
				}
				int commaIndex = value.indexOf(",");
				if ( commaIndex != -1 && commaIndex < value.length() )
				{
					String color1, color2;
					try{
					sequence = value.substring(0, commaIndex);
					color2 = value.substring(commaIndex+1);
					}
					catch (IndexOutOfBoundsException iofbe)
					{
						throw new InvalidPropertyValueException(property, value);
					}
					// Make sure bad values get caught.
					int[] colors = new int[2];
					colors[0] = ColorConverter.convert(color1);
					colors[1] = ColorConverter.convert(color2);
					getView().setBackgroundDrawable(new GradientDrawable(Orientation.TOP_BOTTOM,colors ));
					updateAlpha( m_alpha );
				}
				else
				{
					throw new InvalidPropertyValueException(property, value);
				}

				mNotification.vibrate = sequence;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_FLASH_LIGHTS) )
		{
			if ( value.equals(MA_NOTIFICATION_FLASH_LIGHTS_DEFAULT) )
				{
					mNotification.defaults = Notification.DEFAULT_LIGHTS;
				}
			else
			{
				// Parse value for 3 values.
				mNotification.ledARGB = 0xff00ff00;
				mNotification.ledOnMS = 300;
				mNotification.ledOffMS = 1000;
				mNotification.flags |= Notification.FLAG_SHOW_LIGHTS;
			}
		}
		else if ( name.equals(MA_NOTIFICATION_FLAG) )
		{
			if ( value.equals(MA_NOTIFICATION_FLAG_INSISTENT) )
			{
				mNotification.flags = Notification.FLAG_INSISTENT;
			}
			else if ( value.equals(MA_NOTIFICATION_FLAG_AUTO_CANCEL) )
			{
				mNotification.flags = Notification.FLAG_AUTO_CANCEL;
			}
			else
			{
//				return MA_NOTIFICATION_RES_INVALID_PROPERTY_VALUE;
			}
		}
		else
		{
//			return MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
		}
		*/
		return 0;
//		return MA_NOTIFICATION_RES_OK;
	}

	public String getProperty(String name)
		throws PropertyConversionException
	{/*
		if( name.equals( MA_NOTIFICATION_CONTENT_BODY ) )
		{
			return mText;
		}
		else if ( name.equals(MA_NOTIFICATION_CONTENT_TITLE) )
		{
			return mTitle;
		}*/
		Log.e("@@MoSync", "maNotificationGetProperty Invalid property name " + name);
//		return MA_NOTIFICATION_RES_INVALID_PROPERTY_NAME;
		return "";
	}

	public Notification getNotification()
	{
		return mNotification;
	}

	public String getText()
	{
		return mText;
	}

	public String getTitle()
	{
		return mTitle;
	}

	public void setId(final int id)
	{
		mId = id;
	}

	public int getId()
	{
		return mId;
	}

	public int getFireDate()
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
	 * The fire date for the notification.
	 */
	private int mFireDate = -1;
//	private res mIcon;
//	private Date mFireDate;
}