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
 * @file PhoneGapSensors.cpp
 * @author Ali Sarrafi
 *
 * Implementation of PhoneGap sensor calls made from JavaScript.
 */

#include <conprint.h>
#include <MAUtil/String.h>
#include "PhoneGapFile.h"
#include "PhoneGapMessageHandler.h"

using namespace MAUtil;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	PhoneGapSensors::PhoneGapSensors(PhoneGapMessageHandler* messageHandler)
		: mMessageHandler(messageHandler)
	{
		mAccelerometerWatchStarted = false;
	}

	/**
	 * Destructor.
	 */
	PhoneGapSensors::~PhoneGapSensors()
	{
	}

	/**
	 * Implementation of sensor API:s exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	void PhoneGapSensors::handleMessage(JSONMessage& message)
	{
		// Accelerometer request from PhoneGap
		if ((message.getParam("service") == "Accelerometer"))
		{
			if (message.getParam("action") == "getAcceleration")
			{
				processAcelerometerRequest(
					message.getParam("PhoneGapCallBackId"),
					false); //stop Accelerometer after the first run
			}
			else if (message.getParam("action") == "startWatch")
			{
				processAcelerometerRequest(
					message.getParam("PhoneGapCallBackId"),
					true); // Keep the Accelerometer running
			}
			else if (message.getParam("action") == "stopWatch")
			{
				maSensorStop(SENSOR_TYPE_ACCELEROMETER);
				mAccelerometerWatchStarted = false;
			}
		}
		// GeoLocation request from PhoneGap
		else if (message.getParam("service") == "GeoLocation")
		{
			if (message.getParam("action") == "watchPosition")
			{
				processLocationRequest(
					message.getParam("PhoneGapCallBackId"),
					true);
			}
			else if (message.getParam("action") == "getCurrentPosition")
			{
				processLocationRequest(
					message.getParam("PhoneGapCallBackId"),
					false); //stop GPS after the first run
			}
			else if (message.getParam("action") == "clearWatch")
			{
				maLocationStop();
				mLocationWatchStarted = false;
			}
		}
		// Compass request from PhoneGap
		else if (message.getParam("service") == "Compass")
		{
			if(message.getParam("action") == "getHeading")
			{
				processCompassRequest(
					message.getParam("PhoneGapCallBackId"),
					false); //stop Sensor after the first run
			}
			else if (message.getParam("action") == "startWatch")
			{
				processCompassRequest(
					message.getParam("PhoneGapCallBackId"),
					true); // Keep the Compass running
			}
			else if (message.getParam("action") == "stopWatch")
			{
				maSensorStop(SENSOR_TYPE_COMPASS);
				mAccelerometerWatchStarted = false;
			}
		}
	}

	/**
	 * Called from PhoneGapMessageHandler on EVENT_TYPE_LOCATION.
	 */
	void PhoneGapSensors::sendLocationData(const MAEvent& event)
	{
		MALocation& loc = *(MALocation*)event.data;
		char result[1024];
		bool keepCallBack;
		// This is used to prevent PhoneGap from deleting the callback after
		// receiving the first result.
		if (mLocationWatchStarted) {
			keepCallBack = true;
		}
		else
		{
			keepCallBack = false;
			// Stop it after first data since we are not in watched mode.
			maLocationStop();
		}

		// Call the PhoneGap function, can call the commandResult function too.
		sprintf(result,
			"{coords:{"
				"latitude:%f,"
				"longitude:%f,"
				"altitude:%f,"
				"accuracy:%f,"
				"altitudeAccuracy:%f,"
				"heading:%f,"
				"speed:%f"
				"}}",
			loc.lat,
			loc.lon,
			loc.alt,
			loc.horzAcc,
			loc.vertAcc,
			0.0, // MoSync Location API does not support heading
			0.0  // MoSync Location API does not support speed
			);

		mMessageHandler->callSuccess(
			mLocationWatchCallBack,
			PHONEGAP_CALLBACK_STATUS_OK,
			result,
			keepCallBack);
	}

	void PhoneGapSensors::sendAccelerometerData(MASensor sensorData)
	{
		if (true == mAccelerometerWatchStarted)
		{
			// We are sending the event continuously.
			sendAccelerometerData(mAccelerometerWatchCallBack, sensorData);
		}
		else
		{
			sendAccelerometerData(mAccelerometerWatchCallBack, sensorData);
			// Stop the sensor since it was a one time call.
			maSensorStop(SENSOR_TYPE_ACCELEROMETER);
		}
	}

	void PhoneGapSensors::sendCompassData(MASensor sensorData)
	{
		if (true == mCompassWatchStarted)
		{
			// We are sending the event continuously.
			sendCompassData(mCompassWatchCallBack, sensorData);
		}
		else
		{
			sendCompassData(mCompassWatchCallBack, sensorData);
			// Stop the sensor since it was a one time call.
			maSensorStop(SENSOR_TYPE_COMPASS);
		}
	}

	/**
	 * Sends the data to Accelerometer, it creates a JSON string for the
	 * callback and passes it to the succes callback of PhoneGap
	 *
	 * @param callbackID ID of the PhoneGapCallback to be used
	 * @param sensorData Accelerometer data
	 */
	void PhoneGapSensors::sendAccelerometerData(
			MAUtil::String callbackID,
			MASensor sensorData)
	{
		char result[1024];
		bool keepCallBack;

		// This is used to prevent PhoneGap from deleting the callback after
		// receiving the first result.
		if (mAccelerometerWatchStarted)
		{
			keepCallBack = true;
		}
		else
		{
			keepCallBack = false;
		}

		// Call the PhoneGap function, can call the commandResult function too.
		sprintf(result,
			"\\'{"
				"\"x\": %f,"
				"\"y\": %f,"
				"\"z\": %f"
			"}\\'",
			sensorData.values[0],
			sensorData.values[1],
			sensorData.values[2]
			);
		mMessageHandler->callSuccess(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_OK,
			result,
			keepCallBack);
	}

	/**
	 * Sends the data from Compass, it creates a JSON string for the
	 * callback and passes it to the succes callback of PhoneGap
	 *
	 * @param callbackID ID of the PhoneGapCallback to be used
	 * @param sensorDataOrientation Data
	 */
	void PhoneGapSensors::sendCompassData(
			MAUtil::String callbackID,
			MASensor sensorData)
	{
		char result[1024];
		bool keepCallBack;

		// This is used to prevent PhoneGap from deleting the callback after
		// receiving the first result.
		if (mCompassWatchStarted)
		{
			keepCallBack = true;
		}
		else
		{
			keepCallBack = false;
		}
		//Call the Phonegap function, Can call the commandResult function too
		sprintf(
				result,
				"\\'{\"magneticHeading\":%f }\\'",
				sensorData.values[0] // only x is considered as compass heading in PhoneGap
				);
		mMessageHandler->callSuccess(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_OK,
			result,
			keepCallBack);
	}

	/**
	 * Processes Accelerometer Requests from PhoneGap JavaScript
	 */
	void PhoneGapSensors::processAcelerometerRequest(
			MAUtil::String callbackID,
			bool isWatched)
	{
		//Enable accelerometer which is supported in PhoneGap
		int res = maSensorStart(SENSOR_TYPE_ACCELEROMETER, SENSOR_RATE_NORMAL);
		if(res < 0)
		{
			mMessageHandler->callError(
				callbackID,
				PHONEGAP_CALLBACK_STATUS_ERROR,
				"MoSync: Failed to Start Accelerometer");
			return;
		}
		mAccelerometerWatchCallBack = callbackID;
		mAccelerometerWatchStarted = isWatched;
	}

	/**
	 * Processes Accelerometer Requests from PhoneGap JavaScript
	 */
	void PhoneGapSensors::processLocationRequest(
			MAUtil::String callbackID,
			bool isWatched)
	{
		//Enable accelerometer which is supported in PhoneGap
		int res = maLocationStart();
		if (res < 0)
		{
			mMessageHandler->callError(
				callbackID,
				PHONEGAP_CALLBACK_STATUS_ERROR,
				"MoSync: Failed to Start LocationService");
			return;
		}
		mLocationWatchCallBack = callbackID;
		mLocationWatchStarted = isWatched;
	}

	/**
	 * Processes Compass Requests from PhoneGap JavaScript
	 */
	void PhoneGapSensors::processCompassRequest(
			MAUtil::String callbackID,
			bool isWatched)
	{
		int res = maSensorStart(SENSOR_TYPE_COMPASS, SENSOR_RATE_NORMAL);
		printf("result:%d",res);
		if (res < 0)
		{
			mMessageHandler->callError(
				callbackID,
				PHONEGAP_CALLBACK_STATUS_ERROR,
				"MoSync: Failed to Start Compass");
			return;
		}
		mCompassWatchStarted = isWatched;
		mCompassWatchCallBack = callbackID;
	}
} // namespace
