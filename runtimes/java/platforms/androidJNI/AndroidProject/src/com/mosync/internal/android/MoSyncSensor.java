package com.mosync.internal.android;

import static com.mosync.internal.generated.MAAPI_consts.*;
import static com.mosync.internal.android.MoSyncHelpers.*;

import android.app.Activity;
import android.content.Context;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;

public class MoSyncSensor implements SensorEventListener {

	/*
	 * Constants defining the event structure
	 */
	private static final int SEVENT_TYPE = 0;
	private static final int SEVENT_SENSOR_TYPE = 1;
	private static final int SEVENT_SENSOR_VALUES = 2;
	private static final int SEVENT_SIZE = 5;

//	private static final int SENSOR_TYPE_ALL = -1;
	private static final int SENSOR_TYPE_OTHER = 0;
	private static final int SENSOR_TYPE_ACCELEROMETER = 1;
	private static final int SENSOR_TYPE_MAGNETIC_FIELD = 2;
	private static final int SENSOR_TYPE_ORIENTATION = 3;
	private static final int SENSOR_TYPE_GYROSCOPE = 4;
	private static final int SENSOR_TYPE_PROXIMITY = 5;
//	private static final int SENSOR_TYPE_LIGHT = 6;
//	private static final int SENSOR_TYPE_PRESSURE = 7;
//	private static final int SENSOR_TYPE_TEMPERATURE = 8;
//	private static final int SENSOR_TYPE_GRAVITY = 9;
//	private static final int SENSOR_TYPE_LINEAR_ACCELERATION = 10;
//	private static final int SENSOR_TYPE_ROTATION_VECTOR = 11;
	private static final int SENSOR_TYPES = 6;

	private static final int SENSOR_ERROR_NONE = 0;
	/*
	 * Error codes for maSensorStart
	 */
	private static final int SENSOR_ERROR_NOT_AVAILABLE = -1;
	private static final int SENSOR_ERROR_INTERVAL_NOT_SET = -2;
	private static final int SENSOR_ERROR_ALREADY_ENABLED = -3;

	/*
	 * Error codes for maSensorStop
	 */
	private static final int SENSOR_ERROR_NOT_ENABLED = -4;
	private static final int SENSOR_ERROR_CANNOT_DISABLE = -5;

	/*
	 * Sensor predefined rates
	 */
	private static final int SENSOR_DELAY_FASTEST = 0;
	private static final int SENSOR_DELAY_GAME = -1;
	private static final int SENSOR_DELAY_NORMAL = -2;
	private static final int SENSOR_DELAY_UI = -3;

	/*
	 * Predefined values for orientation sensor
	 */
	// The orientation of the device cannot be determined.
	private static final int UIDEVICE_ORIENTATION_UNKNOWN = 0;
		// The device is in portrait mode, with the device held upright and the home button at the bottom.
	private static final int UIDEVICE_ORIENTATION_PORTRAIT = 1;
		// The device is in portrait mode but upside down, with the device held upright and the home button at the top.
	private static final int UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN = 2;
		// The device is in landscape mode, with the device held upright and the home button on the right side.
	private static final int UIDEVICE_ORIENTATION_LANDSCAPE_LEFT = 3;
		// The device is in landscape mode, with the device held upright and the home button on the left side.
	private static final int UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT = 4;
		// The device is held parallel to the ground with the screen facing upwards.
	private static final int UIDEVICE_ORIENTATION_FACE_UP = 5;
		// The device is held parallel to the ground with the screen facing downwards.
	private static final int UIDEVICE_ORIENTATION_FACE_DOWN = 6;

	/*
	 * Predefined values for proximity sensor
	 */
	private static final int SENSOR_PROXIMITY_VALUE_FAR = 0;
	private static final int SENSOR_PROXIMITY_VALUE_NEAR = 1;

	/*
	 * Used to convert the rate from milliseconds to microseconds
	 */
	private static final int SENSOR_DELAY_MULTIPLIER = 1000;

	/*
	 * Used to keep the values in [-1; 1] interval to be the same as on iOS
	 */
	private static final int ACCELEROMETER_ADJUSTMENT = -10;

	/**
	 * The MoSync thread object.
	 */
	MoSyncThread mMoSyncThread;

	/**
	 * A sensor manager to access the device's sensors.
	 */
	private SensorManager mSensorManager;

	/**
	 * A list of all available sensors on the device
	 */
    private Sensor[] mSensorList;
    private int[] mSensorRates;

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/**
	 * Constructor.
	 * @param thread The MoSync thread.
	 */
	public MoSyncSensor(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		mSensorManager = (SensorManager)getActivity().getSystemService(Context.SENSOR_SERVICE);
		mSensorList = new Sensor[SENSOR_TYPES];
		mSensorRates = new int[SENSOR_TYPES];
		for (int i = 0; i<SENSOR_TYPES; i++ )
		{
			mSensorRates[i] = -1;
		}
	}

	/**
	 * Called when the accuracy of a sensor has changed.
	 */
	@Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

	/**
	 * Called when sensor values have changed.
	 */
	@Override
    public void onSensorChanged(SensorEvent arg0) {

		// If the sensor data is unreliable return
		//if (arg0.accuracy == SensorManager.SENSOR_STATUS_UNRELIABLE)
		//	return;

		int[] event = new int[SEVENT_SIZE];

		try
		{
			event[SEVENT_TYPE] = EVENT_TYPE_SENSOR;
			event[SEVENT_SENSOR_TYPE] = getSensorMoSyncType(arg0.sensor.getType());

			switch (event[SEVENT_SENSOR_TYPE])
			{
				case SENSOR_TYPE_OTHER:
					return;
				case SENSOR_TYPE_ORIENTATION:
					int orientation;
					if ( (-45 < arg0.values[1]) && (arg0.values[1] < 45) &&
							(-45 < arg0.values[2]) && (arg0.values[2] < 45) )
					{
						orientation = UIDEVICE_ORIENTATION_FACE_UP;
					}
					else if ((135 < arg0.values[1]) || (arg0.values[1] < -135) &&
							(-45 < arg0.values[2]) && (arg0.values[2] < 45) )
					{
						orientation = UIDEVICE_ORIENTATION_FACE_DOWN;
					}
					else if ( (-45 < arg0.values[2]) && (arg0.values[2] < 45) )
					{
						if ( arg0.values[1] <= 0 )
						{
							orientation = UIDEVICE_ORIENTATION_PORTRAIT;
						}
						else
						{
							orientation = UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
						}
					}
					else if ( arg0.values[2] > 45 )
					{
						orientation = UIDEVICE_ORIENTATION_LANDSCAPE_LEFT;
					}
					else if ( arg0.values[2] < -45 )
					{
						orientation = UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT;
					}
					else
					{
						orientation = UIDEVICE_ORIENTATION_UNKNOWN;
					}
					event[SEVENT_SENSOR_VALUES] = Float.floatToIntBits(orientation);
					break;
				case SENSOR_TYPE_PROXIMITY:
					event[SEVENT_SENSOR_VALUES] =
						Float.floatToIntBits((arg0.values[0] == 0)?SENSOR_PROXIMITY_VALUE_NEAR:SENSOR_PROXIMITY_VALUE_FAR);
					break;
				case SENSOR_TYPE_ACCELEROMETER:
					for (int i=0; i<arg0.values.length; i++)
					{
						event[SEVENT_SENSOR_VALUES + i] = Float.floatToIntBits(arg0.values[i] / ACCELEROMETER_ADJUSTMENT);
			        }
					break;
				default:
					for (int i=0; i<arg0.values.length; i++)
					{
						event[SEVENT_SENSOR_VALUES + i] = Float.floatToIntBits(arg0.values[i]);
			        }
			}
		}
		catch (Exception e)
		{
			SYSLOG("Invalid event arguments!!!");
		}

		mMoSyncThread.postEvent(event);
    }

	/*
	 * Registers to the specified sensor
	 */
	int maSensorStart(int sensor, int interval)
	{
		if ( (sensor < SENSOR_TYPE_ACCELEROMETER) || (sensor > SENSOR_TYPE_PROXIMITY) )
		{
			return SENSOR_ERROR_NOT_AVAILABLE;
		}
		int rate = getSensorRate(interval);
		int sensorType = getSensorAndroidType(sensor);

		if (mSensorList[sensor - 1] != null)
		{
			return SENSOR_ERROR_ALREADY_ENABLED;
		}
		mSensorList[sensor - 1] = mSensorManager.getDefaultSensor(sensorType);
		if (mSensorList[sensor - 1] == null)
		{
			return SENSOR_ERROR_NOT_AVAILABLE;
		}
		mSensorRates[sensor - 1] = interval;
		if (!mSensorManager.registerListener(this, mSensorList[sensor - 1], rate))
		{
			return SENSOR_ERROR_INTERVAL_NOT_SET;
		}

		return SENSOR_ERROR_NONE;
	}

	/*
	 * Returns the MoSync type starting from the Android one
	 */
	int getSensorMoSyncType(int type)
	{
		switch (type)
		{
			case Sensor.TYPE_ACCELEROMETER:
				return SENSOR_TYPE_ACCELEROMETER;
			case Sensor.TYPE_MAGNETIC_FIELD:
				return SENSOR_TYPE_MAGNETIC_FIELD;
			case Sensor.TYPE_ORIENTATION:
				return SENSOR_TYPE_ORIENTATION;
			case Sensor.TYPE_GYROSCOPE:
				return SENSOR_TYPE_GYROSCOPE;
			case Sensor.TYPE_PROXIMITY:
				return SENSOR_TYPE_PROXIMITY;
//				case Sensor.TYPE_LIGHT:
//				return SENSOR_TYPE_LIGHT;
//			case Sensor.TYPE_PRESSURE:
//				return SENSOR_TYPE_PRESSURE;
//			case Sensor.TYPE_TEMPERATURE:
//				return SENSOR_TYPE_TEMPERATURE;
//			case Sensor.TYPE_GRAVITY:
//				return SENSOR_TYPE_GRAVITY;
//			case Sensor.TYPE_LINEAR_ACCELERATION:
//				return SENSOR_TYPE_LINEAR_ACCELERATION;
//			case Sensor.TYPE_ROTATION_VECTOR:
//				return SENSOR_TYPE_ROTATION_VECTOR;
			default:
				return SENSOR_TYPE_OTHER;
		}
	}

	/*
	 * Returns the Android type starting from the MoSync one
	 */
	int getSensorAndroidType(int type)
	{
		switch (type)
		{
			case SENSOR_TYPE_ACCELEROMETER:
				return Sensor.TYPE_ACCELEROMETER;
			case SENSOR_TYPE_MAGNETIC_FIELD:
				return Sensor.TYPE_MAGNETIC_FIELD;
			case SENSOR_TYPE_ORIENTATION:
				return Sensor.TYPE_ORIENTATION;
			case SENSOR_TYPE_GYROSCOPE:
				return Sensor.TYPE_GYROSCOPE;
			case SENSOR_TYPE_PROXIMITY:
				return Sensor.TYPE_PROXIMITY;
//				case Sensor.TYPE_LIGHT:
//				return SENSOR_TYPE_LIGHT;
//			case Sensor.TYPE_PRESSURE:
//				return SENSOR_TYPE_PRESSURE;
//			case Sensor.TYPE_TEMPERATURE:
//				return SENSOR_TYPE_TEMPERATURE;
//			case Sensor.TYPE_GRAVITY:
//				return SENSOR_TYPE_GRAVITY;
//			case Sensor.TYPE_LINEAR_ACCELERATION:
//				return SENSOR_TYPE_LINEAR_ACCELERATION;
//			case Sensor.TYPE_ROTATION_VECTOR:
//				return SENSOR_TYPE_ROTATION_VECTOR;
		}
		return 0;
	}

	/*
	 * Returns the Android rate starting from the MoSync one
	 */
	int getSensorRate(int interval)
	{
		switch (interval)
		{
			case SENSOR_DELAY_FASTEST:
				return SensorManager.SENSOR_DELAY_FASTEST;
			case SENSOR_DELAY_GAME:
				return SensorManager.SENSOR_DELAY_GAME;
			case SENSOR_DELAY_NORMAL:
				return SensorManager.SENSOR_DELAY_NORMAL;
			case SENSOR_DELAY_UI:
				return SensorManager.SENSOR_DELAY_UI;
			default:
				return (interval * SENSOR_DELAY_MULTIPLIER);
		}
	}

	/*
	 * Unregisters from the specified sensor.
	 */
	int maSensorStop(int sensor)
	{
		if ( (sensor < SENSOR_TYPE_ACCELEROMETER) || (sensor > SENSOR_TYPE_PROXIMITY) )
		{
			return SENSOR_ERROR_NOT_AVAILABLE;
		}

		if (mSensorList[sensor - 1] == null)
		{
			return SENSOR_ERROR_NOT_ENABLED;
		}
		try
		{
			mSensorManager.unregisterListener(this, mSensorList[sensor - 1]);
			mSensorList[sensor - 1] = null;
			mSensorRates[sensor - 1] = -1;
		}
		catch (Exception e)
		{
			return SENSOR_ERROR_CANNOT_DISABLE;
		}

		if (mSensorList[sensor - 1] != null)
		{
			return SENSOR_ERROR_CANNOT_DISABLE;
		}

		return 0;
	}

	/*
	 * Interrupt handling: resume
	 */
	public void onResume()
	{
		for (int i=0; i<SENSOR_TYPES; i++)
		{
			if (mSensorList[i] != null)
			{
				mSensorManager.registerListener(this, mSensorList[i], mSensorRates[i]);
			}
		}
    }

	/*
	 * Interrupt handling: pause
	 */
	public void onPause()
	{
		mSensorManager.unregisterListener(this);
    }
}
