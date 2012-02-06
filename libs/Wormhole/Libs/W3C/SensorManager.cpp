/*
Copyright (C) 2011 MoSync AB

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

/**
 * @file SensorManager.cpp
 * @author Iraklis Rossis
 *
 * Implementation of W3C sensor calls made from JavaScript.
 */

#include <conprint.h>
#include <MAUtil/String.h>
#include "../PhoneGap/PhoneGapMessageHandler.h"
#include "SensorManager.h"

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	SensorManager::SensorManager(PhoneGapMessageHandler* messageHandler)
		: mMessageHandler(messageHandler)
	{
		for(int i = 0; i< MAXIMUM_SENSORS; i++)
		{
			mSensorSingleReadFlag[i] = false;
		}
	}

	/**
	 * Destructor.
	 */
	SensorManager::~SensorManager()
	{
	}

	/**
	 * Implementation of sensor API:s exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	void SensorManager::handleMessage(JSONMessage& message)
	{
		String action = message.getParam("action");
		if (action == "findSensors")
		{
			findSensors(message);
		}
		else if ((action == "startSensor") || (action == "stopSensor"))
		{
			String type = message.getArgsField("type");

			int maType = 0;
			if(type == "Accelerometer")
			{
				maType = SENSOR_TYPE_ACCELEROMETER;
			}
			else if(type == "MagneticField")
			{
				maType = SENSOR_TYPE_MAGNETIC_FIELD;
			}
			else if(type == "Orientation")
			{
				maType = SENSOR_TYPE_ORIENTATION;
			}
			else if(type == "Gyroscope")
			{
				maType = SENSOR_TYPE_GYROSCOPE;
			}
			else if(type == "Proximity")
			{
				maType = SENSOR_TYPE_PROXIMITY;
			}

			if(maType != 0)
			{
				if(action == "startSensor")
				{
					int interval = message.getArgsFieldInt("interval");
					if(interval == -1) //A single reading
					{
						interval = 0;
						mSensorSingleReadFlag[maType] = true;
					}

					int result = maSensorStart(maType,SENSOR_RATE_NORMAL);
					if(result == SENSOR_ERROR_NONE)
					{
						//The the message handler to send sensor events to this object
						mMessageHandler->setSensorEventTarget(maType, true);
						mSensorWatchCallBack[maType] = message.getParam("PhoneGapCallBackId");
					}
					else //There was an error
					{
						mSensorSingleReadFlag[maType] = false;
						String errorArgs;
						switch(result)
						{
							case SENSOR_ERROR_NOT_AVAILABLE:
								errorArgs = "{\"code\":-1,\"message\":\"Sensor not available\"}";
								break;
							case SENSOR_ERROR_INTERVAL_NOT_SET:
								errorArgs = "{\"code\":-2,\"message\":\"Interval not set\"}";
								break;
							case SENSOR_ERROR_ALREADY_ENABLED:
								errorArgs = "{\"code\":-3,\"message\":\"Sensor already enabled\"}";
								break;
						}
						mMessageHandler->callError(
							message.getParam("PhoneGapCallBackId"),
							PHONEGAP_CALLBACK_STATUS_ERROR,
							errorArgs,
							false);
					}

				}
				else
				{
					int result = maSensorStop(maType);
					//The the message handler to stop sending this object sensor events
					mMessageHandler->setSensorEventTarget(maType, false);
					if(result != SENSOR_ERROR_NONE)
					{
						String errorArgs;
						switch(result)
						{
							case SENSOR_ERROR_NOT_ENABLED:
								errorArgs = "{\"code\":-4,\"message\":\"Sensor not enabled\"}";
								break;
							case SENSOR_ERROR_CANNOT_DISABLE:
								errorArgs = "{\"code\":-5,\"message\":\"Interval not set\"}";
								break;
						}
						mMessageHandler->callError(
							message.getParam("PhoneGapCallBackId"),
							PHONEGAP_CALLBACK_STATUS_ERROR,
							errorArgs,
							false);
					}
				}
			}
		}
	}

	/**
	 * Handles the findSensor message. It then posts the list
	 * of sensors back to Phonegap
	 * @param message The phonegap message
	 */
	void SensorManager::findSensors(JSONMessage& message)
	{
		String type = message.getArgsField("type");
		String result = "{result:["; //Start building the reply message
		int errorCode;
		int longInterval = 10000;

		//Type can be either empty (which means enumerate everything), or
		//a single sensor type
		if(type == "" || type == "Accelerometer")
		{
			errorCode = maSensorStart(SENSOR_TYPE_ACCELEROMETER, longInterval);
			if(errorCode != SENSOR_ERROR_NOT_AVAILABLE)
			{
				if(errorCode != SENSOR_ERROR_ALREADY_ENABLED)
				{
					maSensorStop(SENSOR_TYPE_ACCELEROMETER);
				}
				result += "{\"name\":\"Accelerometer\",\"type\":\"Accelerometer\"},";
			}
		}

		if(type == "" || type == "MagneticField")
		{
			errorCode = maSensorStart(SENSOR_TYPE_MAGNETIC_FIELD, longInterval);
			if(errorCode != SENSOR_ERROR_NOT_AVAILABLE)
			{
				if(errorCode != SENSOR_ERROR_ALREADY_ENABLED)
				{
					maSensorStop(SENSOR_TYPE_MAGNETIC_FIELD);
				}
				result += "{\"name\":\"MagneticField\",\"type\":\"MagneticField\"},";
			}
		}

		if(type == "" || type == "Orientation")
		{
			errorCode = maSensorStart(SENSOR_TYPE_ORIENTATION, longInterval);
			if(errorCode != SENSOR_ERROR_NOT_AVAILABLE)
			{
				if(errorCode != SENSOR_ERROR_ALREADY_ENABLED)
				{
					maSensorStop(SENSOR_TYPE_ORIENTATION);
				}
				result += "{\"name\":\"Orientation\",\"type\":\"Orientation\"},";
			}
		}

		if(type == "" || type == "Gyroscope")
		{
			errorCode = maSensorStart(SENSOR_TYPE_GYROSCOPE, longInterval);
			if(errorCode != SENSOR_ERROR_NOT_AVAILABLE)
			{
				if(errorCode != SENSOR_ERROR_ALREADY_ENABLED)
				{
					maSensorStop(SENSOR_TYPE_GYROSCOPE);
				}
				result += "{\"name\":\"Gyroscope\",\"type\":\"Gyroscope\"},";
			}
		}

		if(type == "" || type == "Proximity")
		{
			errorCode = maSensorStart(SENSOR_TYPE_PROXIMITY, longInterval);
			if(errorCode != SENSOR_ERROR_NOT_AVAILABLE)
			{
				if(errorCode != SENSOR_ERROR_ALREADY_ENABLED)
				{
					maSensorStop(SENSOR_TYPE_PROXIMITY);
				}
				result += "{\"name\":\"Proximity\",\"type\":\"Proximity\"},";
			}
		}

		result += "]}";
		mMessageHandler->callSuccess(
			message.getParam("PhoneGapCallBackId"),
			PHONEGAP_CALLBACK_STATUS_OK,
			result,
			false);
	}

	/**
	 * Dispatching of sensor events.
	 * @param sensorData The sensor data of the event
	 */
	void SensorManager::sendSensorData(MASensor sensorData)
	{
		char result[256];
		int timestamp = maGetMilliSecondCount(); //Time of sampling

		sprintf(result,"{\"x\":%f,\"y\":%f,\"z\":%f,\"timestamp\":%d,\"reason\":\"watch\"}",
			sensorData.values[0], sensorData.values[1], sensorData.values[2], timestamp);

		bool keepCallback = true;

		//Check to see if this is a single reading only
		if(mSensorSingleReadFlag[sensorData.type] == true)
		{
			mSensorSingleReadFlag[sensorData.type] = false;
			keepCallback = false;
			maSensorStop(sensorData.type);
		}

		mMessageHandler->callSuccess(
			mSensorWatchCallBack[sensorData.type],
			PHONEGAP_CALLBACK_STATUS_OK,
			result,
			keepCallback);
	}
} // namespace
