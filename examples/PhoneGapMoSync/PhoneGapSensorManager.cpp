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
 * @file PhoneGapSensorManager.cpp
 * @author Iraklis Rossis
 *
 * Implementation of PhoneGap sensor calls made from JavaScript.
 */

#include <conprint.h>
#include <MAUtil/String.h>
#include "PhoneGapMessageHandler.h"

using namespace MAUtil;

/**
 * Constructor.
 */
PhoneGapSensorManager::PhoneGapSensorManager(PhoneGapMessageHandler* messageHandler)
	: mMessageHandler(messageHandler)
{
	//mAccelerometerWatchStarted = false;
}

/**
 * Destructor.
 */
PhoneGapSensorManager::~PhoneGapSensorManager()
{
}

/**
 * Implementation of sensor API:s exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
void PhoneGapSensorManager::handleMessage(PhoneGapMessage& message)
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
				maSensorStart(maType,interval);
			}
			else
			{
				maSensorStop(maType);
			}
		}
	}
}

void PhoneGapSensorManager::findSensors(PhoneGapMessage& message)
{
	String type = message.getArgsField("type");
	String result = "{result:[";
	int errorCode;

	if(type == "" || type == "Accelerometer")
	{
		errorCode = maSensorStart(SENSOR_TYPE_ACCELEROMETER, 10000);
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
		errorCode = maSensorStart(SENSOR_TYPE_MAGNETIC_FIELD, 10000);
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
		errorCode = maSensorStart(SENSOR_TYPE_ORIENTATION, 10000);
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
		errorCode = maSensorStart(SENSOR_TYPE_GYROSCOPE, 10000);
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
		errorCode = maSensorStart(SENSOR_TYPE_PROXIMITY, 10000);
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
