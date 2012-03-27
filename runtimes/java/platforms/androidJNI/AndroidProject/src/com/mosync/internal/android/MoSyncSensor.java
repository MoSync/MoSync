package com.mosync.internal.android;

import static com.mosync.internal.android.MoSyncHelpers.*;

import java.util.Hashtable;
import java.util.Iterator;
import java.util.Map;
import java.util.Set;
import java.util.TreeSet;

import android.app.Activity;
import android.content.Context;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

import static com.mosync.internal.generated.MAAPI_consts.EVENT_TYPE_SENSOR;

import static com.mosync.internal.generated.MAAPI_consts.SENSOR_TYPE_ACCELEROMETER;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_TYPE_MAGNETIC_FIELD;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_TYPE_ORIENTATION;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_TYPE_GYROSCOPE;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_TYPE_PROXIMITY;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_TYPE_COMPASS;

import static com.mosync.internal.generated.MAAPI_consts.SENSOR_RATE_FASTEST;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_RATE_GAME;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_RATE_NORMAL;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_RATE_UI;

import static com.mosync.internal.generated.MAAPI_consts.SENSOR_ERROR_ALREADY_ENABLED;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_ERROR_CANNOT_DISABLE;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_ERROR_INTERVAL_NOT_SET;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_ERROR_NONE;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_ERROR_NOT_AVAILABLE;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_ERROR_NOT_ENABLED;

import static com.mosync.internal.generated.MAAPI_consts.SENSOR_PROXIMITY_VALUE_FAR;
import static com.mosync.internal.generated.MAAPI_consts.SENSOR_PROXIMITY_VALUE_NEAR;

import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_FACE_DOWN;
import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_FACE_UP;
import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_LANDSCAPE_LEFT;
import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT;
import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_PORTRAIT;
import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
import static com.mosync.internal.generated.MAAPI_consts.UIDEVICE_ORIENTATION_UNKNOWN;

public class MoSyncSensor implements SensorEventListener {

	/*
	 * Constants defining the event structure
	 */
	private static final int SENSOR_EVENT_TYPE = 0;
	private static final int SENSOR_EVENT_SENSOR_TYPE = 1;
	private static final int SENSOR_EVENT_VALUES = 2;
	private static final int SENSOR_EVENT_SIZE = 5;

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
	 * A variable used to determine so that we don't send
	 * more events then we need to.
	 */
	private int mLastOrientation = -1;

	/**
	 * @brief Internal class for keeping sensor data
	 */
	class SensorData
	{
		public SensorData(Sensor sensor, int rate)
		{
			mSensor = sensor;
			mRate = rate;
		}
		public Sensor mSensor;
		public int mRate;
	};

	/*
	 * @brief Each Android sensor used is stored in the table
	 */
	Hashtable<Integer, SensorData> mSensors =
			new Hashtable<Integer, SensorData>();

	/*
	 * @brief A set of all the active sensors
	 */
	Set<Integer> mActiveSensors = new TreeSet<Integer>();

	/*
	 * @brief Conversion table between MoSync and native Android sensors.
	 */
	Hashtable<Integer, Integer> mSensorConversion = new Hashtable<Integer, Integer>();

	/**
	 * @return The Activity object.
	 */
	private Activity getActivity()
	{
		return mMoSyncThread.getActivity();
	}

	/**
	 * @brief Constructor, activates the sensor manager and
	 * Initializes the conversion table
	 *
	 * @param thread The MoSync thread.
	 */
	public MoSyncSensor(MoSyncThread thread)
	{
		mMoSyncThread = thread;
		try
		{
			mSensorManager = (SensorManager)getActivity().getSystemService(Context.SENSOR_SERVICE);
		} catch (Exception ex)
		{
			Log.e("@@MoSync", "MoSyncSensor manager initialize error!");
		}

		// Set up conversion table
		mSensorConversion.put(SENSOR_TYPE_ACCELEROMETER, Sensor.TYPE_ACCELEROMETER);
		mSensorConversion.put(SENSOR_TYPE_COMPASS, Sensor.TYPE_ORIENTATION);
		mSensorConversion.put(SENSOR_TYPE_GYROSCOPE, Sensor.TYPE_GYROSCOPE);
		mSensorConversion.put(SENSOR_TYPE_MAGNETIC_FIELD, Sensor.TYPE_MAGNETIC_FIELD);
		mSensorConversion.put(SENSOR_TYPE_ORIENTATION,Sensor.TYPE_ORIENTATION);
		mSensorConversion.put(SENSOR_TYPE_PROXIMITY, Sensor.TYPE_PROXIMITY);
	}

	/**
	 * @brief Called when the accuracy of a sensor has changed.
	 * Unused at this point.
	 */
	@Override
    public void onAccuracyChanged(Sensor sensor, int accuracy)
	{
    }

	/**
	 * @brief Called when a sensor gets an update
	 *
	 * @param se The sensor event with all the data
	 */
	@Override
    public void onSensorChanged(SensorEvent se)
	{

		// The event information which will be sent
		int[] event = new int[SENSOR_EVENT_SIZE];
		event[SENSOR_EVENT_TYPE] = EVENT_TYPE_SENSOR;

		try
		{
			int nativeSensorType = se.sensor.getType();

			switch (nativeSensorType)
			{
				case Sensor.TYPE_ORIENTATION:
					if(mActiveSensors.contains(SENSOR_TYPE_ORIENTATION))
					{
						// Since Orientation and Compass is using the same
						// sensor we need to do things a little bit special.

						int orientation = getOrientation(se.values[1], se.values[2]);

						if(orientation != mLastOrientation)
						{
							mLastOrientation = orientation;
							event[SENSOR_EVENT_SENSOR_TYPE] = SENSOR_TYPE_ORIENTATION;
							event[SENSOR_EVENT_VALUES] = Float.floatToIntBits(orientation);
							sendEvent(event);
						}
					}
					if(mActiveSensors.contains(SENSOR_TYPE_COMPASS))
					{
						Log.i("@@MOSYNC","got compass data");
						event[SENSOR_EVENT_SENSOR_TYPE] = SENSOR_TYPE_COMPASS;
						event[SENSOR_EVENT_VALUES] = Float.floatToIntBits(se.values[0]);
						sendEvent(event);
					}
					event = null;
					break;

				case Sensor.TYPE_PROXIMITY:
					event[SENSOR_EVENT_SENSOR_TYPE] = SENSOR_TYPE_PROXIMITY;
					event[SENSOR_EVENT_VALUES] =
						Float.floatToIntBits((se.values[0] == 0)
							? SENSOR_PROXIMITY_VALUE_NEAR : SENSOR_PROXIMITY_VALUE_FAR);
					break;

				case Sensor.TYPE_ACCELEROMETER:
					event[SENSOR_EVENT_SENSOR_TYPE] = SENSOR_TYPE_ACCELEROMETER;
					for (int i=0; i<3; i++)
					{
						event[SENSOR_EVENT_VALUES + i] =
							Float.floatToIntBits(se.values[i] / ACCELEROMETER_ADJUSTMENT);
			        }
					break;

				case Sensor.TYPE_MAGNETIC_FIELD:
					event[SENSOR_EVENT_SENSOR_TYPE] = SENSOR_TYPE_MAGNETIC_FIELD;
					for (int i=0; i<3; i++)
					{
						event[SENSOR_EVENT_VALUES + i] =
							Float.floatToIntBits(se.values[i]);
			        }
					break;

				case Sensor.TYPE_GYROSCOPE:
					event[SENSOR_EVENT_SENSOR_TYPE] = SENSOR_TYPE_GYROSCOPE;
					for (int i=0; i<3; i++)
					{
						event[SENSOR_EVENT_VALUES + i] =
							Float.floatToIntBits(se.values[i]);
			        }
					break;

				default:
					// No default ones..
					event = null;
					break;
			}

		}
		catch (Exception e)
		{
			SYSLOG("Invalid event arguments!!!");
		}

		// All sensors except for the orientation and the compass
		// are sending the event here, the other two sends them self
		if(event != null)
			sendEvent(event);
    }

	/**
	 * @brief Converts the pitch and roll into an orientation
	 * @param pitch The current pitch
	 * @param roll The current roll
	 * @return One of the UIDEVICE_ORIENTATION enumerations
	 */
	int getOrientation(float pitch, float roll)
	{
		if ( (-45 < pitch) && (pitch < 45) &&
				(-45 < roll) && (roll < 45) )
		{
			return UIDEVICE_ORIENTATION_FACE_UP;
		}
		else if ((135 < pitch) || (pitch < -135) &&
				(-45 < roll) && (roll < 45) )
		{
			return UIDEVICE_ORIENTATION_FACE_DOWN;
		}
		else if ( (-45 < roll) && (roll < 45) )
		{
			if ( pitch <= 0 )
			{
				return UIDEVICE_ORIENTATION_PORTRAIT;
			}
			else
			{
				return UIDEVICE_ORIENTATION_PORTRAIT_UPSIDE_DOWN;
			}
		}
		else if ( roll > 45 )
		{
			return UIDEVICE_ORIENTATION_LANDSCAPE_LEFT;
		}
		else if ( roll < -45 )
		{
			return UIDEVICE_ORIENTATION_LANDSCAPE_RIGHT;
		}

		return UIDEVICE_ORIENTATION_UNKNOWN;
	}

	/**
	 * @brief Sends the event
	 * @param event The event array
	 */
	void sendEvent(int[] event)
	{
		mMoSyncThread.postEvent(event);
	}

	/**
	 * @brief Stars a sensor if it available and not already active
	 *
	 * @param sensor The sensor to activate
	 * @param interval The update interval
	 *
	 * @return An error code on failure or SENSOR_ERROR_NONE on success
	 */
	int maSensorStart(int sensor, int interval)
	{
		int rate = getSensorRate(interval);

		if(!mSensorConversion.containsKey(sensor))
		{
			return SENSOR_ERROR_NOT_AVAILABLE;
		}

		if(mActiveSensors.contains(sensor))
		{
			return SENSOR_ERROR_ALREADY_ENABLED;
		}

		int sensorType = mSensorConversion.get(sensor);

		Sensor defaultSensor = mSensorManager.getDefaultSensor(sensorType);

		// If there isn't a default sensor we shouldn't try to activate it
		if(defaultSensor == null)
			return SENSOR_ERROR_NOT_AVAILABLE;

		mActiveSensors.add(sensor);

		// Same as before.. Orientation and Compass is the same thing
		if(!mSensors.containsKey(sensorType))
		{
			mSensors.put(sensorType, new SensorData(defaultSensor, rate));
		}

		if (!mSensorManager.registerListener(this, defaultSensor, rate))
			return SENSOR_ERROR_INTERVAL_NOT_SET;

		return SENSOR_ERROR_NONE;
	}

	/*
	 * Unregisters from the specified sensor.
	 */
	int maSensorStop(int sensor)
	{
		if(!mActiveSensors.contains(sensor))
			return SENSOR_ERROR_NOT_ENABLED;

		int sensorType = mSensorConversion.get(sensor);

		SensorData sensorData = mSensors.get(sensorType);
		Sensor activeSensor = sensorData.mSensor;

		boolean unregister = true;

		try
		{
			// We need to do some checks here. Orientation and Compass sensor is using
			// the same underlying sensor
			if((sensor == SENSOR_TYPE_COMPASS) || (sensor == SENSOR_TYPE_ORIENTATION))
			{
				if(mActiveSensors.contains(SENSOR_TYPE_ORIENTATION) &&
						mActiveSensors.contains(SENSOR_TYPE_COMPASS))
				{
					unregister = false;
				}
			}
			mActiveSensors.remove(sensor);

			if(unregister)
			{
				mSensorManager.unregisterListener(this, activeSensor);
			}
		}
		catch (Exception e)
		{
			return SENSOR_ERROR_CANNOT_DISABLE;
		}

		return SENSOR_ERROR_NONE;
	}

	/*
	 * Interrupt handling: resume
	 */
	public void onResume()
	{
		Set<Map.Entry<Integer,MoSyncSensor.SensorData>> sensorSet = mSensors.entrySet();
		Iterator<Map.Entry<Integer,MoSyncSensor.SensorData>> i = sensorSet.iterator();
		while (i.hasNext())
		{
			Map.Entry<Integer,MoSyncSensor.SensorData> entry =
					(Map.Entry<Integer,MoSyncSensor.SensorData>)i.next();

			SensorData sensorData = entry.getValue();

			mSensorManager.registerListener(this, sensorData.mSensor, sensorData.mRate);
		}
	}

	/*
	 * Interrupt handling: pause
	 */
	public void onPause()
	{
		if(!mActiveSensors.isEmpty())
			mSensorManager.unregisterListener(this);
	}

	/*
	 * Returns the Android rate starting from the MoSync one
	 */
	int getSensorRate(int interval)
	{
		switch (interval)
		{
			case SENSOR_RATE_FASTEST:
				return SensorManager.SENSOR_DELAY_FASTEST;
			case SENSOR_RATE_GAME:
				return SensorManager.SENSOR_DELAY_GAME;
			case SENSOR_RATE_NORMAL:
				return SensorManager.SENSOR_DELAY_NORMAL;
			case SENSOR_RATE_UI:
				return SensorManager.SENSOR_DELAY_UI;
			default:
				return (interval * SENSOR_DELAY_MULTIPLIER);
		}
	}

}
