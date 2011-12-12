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
bool PhoneGapSensors::handleMessage(PhoneGapMessage& message)
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
	else if ((message.getParam("service") == "Compass") &&
			(message.getParam("action") == "getHeading"))
	{
		processCompassRequest(
			message.getParam("PhoneGapCallBackId"),
			false); //stop Sensor after the first run
	}

	return true;
}

/**
 * Called from PhoneGapMessageHandler on EVENT_TYPE_LOCATION.
 */
void PhoneGapSensors::sendLocationData(const MAEvent& event)
{
	MALocation& loc = *(MALocation*)event.data;
	char result[1024];
	char keepCallBack[128];
	//This is used to prevent phonegap from deleting the callback after
	//receiving the first result.
	if (mLocationWatchStarted) {
		sprintf(keepCallBack," , 'keepCallback': true");
	}
	else
	{
		sprintf(keepCallBack,"");
		// stop it after first data since we are not in watched mode
		maLocationStop();
	}

	//Call the Phonegap function, Can call the commandResult function too
	sprintf(result,
		"'%s', \"{ 'status' : 1,"
		"'message': { 'coords':{"
			"'latitude': %f,"
			"'longitude': %f,"
			"'altitude': %f,"
			"'accuracy': %f,"
			"'altitudeAccuracy': %f,"
			"'heading': %f,"
			"'speed': %f"
			"}}"
		"%s}\"",
		mLocationWatchCallBack.c_str(),
		loc.lat,
		loc.lon,
		loc.alt,
		loc.horzAcc,
		loc.vertAcc,
		0, //MoSync Location API does not support heading
		0, //MoSync Location API does not support speed
		keepCallBack
		);
	lprintfln(result);
	mMessageHandler->sendPhoneGapSuccess(result);
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
	sendCompassData(mCompassWatchCallBack, sensorData);
	// Stop the sensor since it was a one time call.
	maSensorStop(SENSOR_TYPE_ORIENTATION);
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
	char keepCallBack[128];
	//This is used to prevent phonegap from deleting the callback after
	//receiving the first result.
	if (mAccelerometerWatchStarted) {
		sprintf(keepCallBack," , 'keepCallback': true");
	}
	else
	{
		sprintf(keepCallBack,"");
	}

	//Call the Phonegap function, Can call the commandResult function too
	sprintf(result,
		"'%s', \"{ 'status' : 1,"
		"'message': \\\"{"
			"'x': %f,"
			"'y': %f,"
			"'z': %f}\\\""
		"%s}\"",
		callbackID.c_str(),
		sensorData.values[0],
		sensorData.values[1],
		sensorData.values[2],
		keepCallBack
		);
	mMessageHandler->sendPhoneGapSuccess(result);
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
	//Call the Phonegap function, Can call the commandResult function too
	sprintf(
			result,
			"'%s', \"{ 'status' : 1,"
			"'message': \\\"{ 'magneticHeading': %f }\\\""
			"}\"",
			callbackID.c_str(),
			sensorData.values[1] //only x is considered as compass heading in PhoneGap
			);
	mMessageHandler->sendPhoneGapSuccess(result);
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
		mMessageHandler->sendPhoneGapError(
			"MoSync: Failed to Start Accelerometer",
			callbackID);
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
	lprintfln("Started the Location Service with : %d +++", res);
	if (res < 0)
	{
		mMessageHandler->sendPhoneGapError(
			"MoSync: Failed to Start LocationService",
			callbackID);
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
	// In PhoneGap, compass is actually the orientation sensor.
	int res = maSensorStart(SENSOR_TYPE_ORIENTATION, SENSOR_RATE_NORMAL);
	if (res < 0)
	{
		mMessageHandler->sendPhoneGapError(
			"MoSync: Failed to Start Compass",
			callbackID);
		return;
	}

	mCompassWatchCallBack = callbackID;
}
