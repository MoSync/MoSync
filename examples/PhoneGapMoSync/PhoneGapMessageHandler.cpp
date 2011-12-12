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
 * @file PhoneGapMessageHandler.cpp
 * @author Ali Sarrafi
 *
 * Implementation of PhoneGap calls made from JavaScript.
 */

#include <Wormhole/FileUtil.h>
#include <mastdlib.h> // C string conversion functions
#include <conprint.h>
#include "PhoneGapMessageHandler.h"
#include "MAHeaders.h"

// NameSpaces we want to access.
using namespace MAUtil; // Class Moblet, String
using namespace NativeUI; // WebView widget
using namespace Wormhole; // Class WebAppMoblet

/**
 * Constructor.
 */
PhoneGapMessageHandler::PhoneGapMessageHandler(NativeUI::WebView* webView) :
	mWebView(webView),
	mPhoneGapSensors(this),
	mPhoneGapFile(this)
{
	enableHardware();
}

/**
 * Destructor.
 */
PhoneGapMessageHandler::~PhoneGapMessageHandler()
{
	// Nothing needs to be explicitly destroyed.
}

/**
 * Initializes PhoneGap by sending the native Ready Message.
 */
void PhoneGapMessageHandler::initializePhoneGap()
{
	// Send native ready event to PhoneGap using the lazy version.
	// This way we allow PhoneGap to initialize itself whenever
	// it is convenient.
	mWebView->callJS("{_nativeReady = true;}");
}

/**
 * Implementation of standard API exposed to JavaScript
 * This function is used to detect different messages from phonegap
 * and call the respective function in MoSync.
 *
 * @return true if message was handled, false if not.
 */
bool PhoneGapMessageHandler::handleMessage(PhoneGapMessage& message)
{
	// Send device information to PhoneGap
	if ((message.getParam("service") == "Device") &&
			(message.getParam("action") == "Get"))
	{
		sendDeviceProperties(message.getParam("PhoneGapCallBackId"));
	}
	// Process the vibration message
	else if ((message.getParam("service") == "Notification") &&
			(message.getParam("action") == "vibrate"))
	{
		int duration = message.getJSONFieldInt("duration");
		maVibrate(duration);
	}
	//Process the beep message
	else if ((message.getParam("service") == "Notification") &&
			(message.getParam("action") == "beep"))
	{
		int repeatCount = message.getParamInt("args");
		for (int i = 0; i < repeatCount; i++)
		{
			//We play a local file asa beep alert, users can change the file
			maSoundPlay(BEEP_WAV, 0, maGetDataSize(BEEP_WAV));
		}
	}
	else if ((message.getParam("service") == "Connection") &&
			(message.getParam("action") == "getConnectionInfo"))
	{
		sendConnectionType(message.getParam("PhoneGapCallBackId"));
	}
	else if ((message.getParam("service") == "Accelerometer")
		|| (message.getParam("service") == "GeoLocation")
		|| (message.getParam("service") == "Compass"))
	{
		mPhoneGapSensors.handleMessage(message);
	}
	else if (message.getParam("service") == "File")
	{
		mPhoneGapFile.handleMessage(message);
	}
}

void PhoneGapMessageHandler::sendConnectionType(MAUtil::String callbackID)
{
	//currently we only send offline status to PhoneGap
	char buffer[1024];
	char networkType[256];
	int netRes = maGetSystemProperty(
		"mosync.network.type",
		networkType,
		256);
	//if the property is not available set the result to unknown
	if(netRes < 0 )
	{
		sprintf(networkType, "unknown");
	}
	else if (strcmp(networkType, "mobile") == 0)
	{
		sprintf(networkType, "3g");
	}
	//create the callback
	//TODO: we need to convert the type to PhoneGaps simplified version
	sprintf(
		buffer,
		"'%s', \"{ 'status' : 1, 'message': '%s'}\"",
		callbackID.c_str(),
		networkType);

	sendPhoneGapSuccess(buffer);
}

/**
 * Sends Device's information to PhoneGap's JavaScript Instance
 *
 * @param callbackID ID of the PhoneGapCallback to be used
 */
void PhoneGapMessageHandler::sendDeviceProperties(MAUtil::String callbackID)
{
	char deviceName[256];
	char deviceUUID[256];
	char deviceOS[256];
	char deviceOSVersion[256];
	char buffer[1024];

	int nameRes = maGetSystemProperty(
		"mosync.device.name",
		deviceName,
		256);

	int uuidRes = maGetSystemProperty(
		"mosync.device.UUID",
		deviceUUID,
		256);

	int osRes = maGetSystemProperty(
		"mosync.device.OS",
		deviceOS,
		256);

	int osVersionRes = maGetSystemProperty(
		"mosync.device.OS.version",
		deviceOSVersion,
		256);

	//if Any of the above commands fail send an error to PhoneGap
	if((nameRes < 0)
		|| (uuidRes < 0)
		|| (osRes < 0)
		|| (osVersionRes < 0))
	{
		//send an error to PhoneGap
		sendPhoneGapError(
			"MoSync: Failed to get device informations",
			callbackID);
		return;
	}

	// Send the result back to PhoneGap.
	sprintf(buffer,
		"'%s', \"{ 'status' : 1,"
		"'message': \\\"{"
			"'platform': '%s',"
			"'name': '%s',"
			"'uuid': '%s',"
			"'version':'%s',"
			"'phonegap': '1.2.0'}\\\"}\"",
		callbackID.c_str(),
		deviceOS,
		deviceName,
		deviceUUID,
		deviceOSVersion
		);
	sendPhoneGapSuccess(buffer);
}

/**
 * Hardware Listeners and Hardware Access is initialized in this function.
 */
void PhoneGapMessageHandler::enableHardware()
{
	//Adding this Class as a Sensor Listener
	MAUtil::Environment::getEnvironment().addSensorListener(this);
	MAUtil::Environment::getEnvironment().addCustomEventListener(this);
}

/**
 * Handles location events generated by the location API.
 */
void PhoneGapMessageHandler::customEvent(const MAEvent& event)
{
	lprintfln("Got a custom event with Type %d", event.type);
	if (event.type == EVENT_TYPE_LOCATION)
	{
		mPhoneGapSensors.sendLocationData(event);
	}
}

/**
 * Receives sensor events and forwards them to PhoneGap
 *
 * @param sensorData sensor number and data (See sensor API for more info)
 */
void PhoneGapMessageHandler::sensorEvent(MASensor sensorData)
{
	if (sensorData.type == SENSOR_TYPE_ACCELEROMETER)
	{
		mPhoneGapSensors.sendAccelerometerData(sensorData);
	}
	else if (sensorData.type == SENSOR_TYPE_ORIENTATION)
	{
		mPhoneGapSensors.sendCompassData(sensorData);
	}
}

/**
 * General wrapper for phoneGap success callback.
 * If an operation is successful this function should be called.
 *
 * @param data the data that should be passed to the callback function
 */
void PhoneGapMessageHandler::sendPhoneGapSuccess(const char* data)
{
	char script[1024];
	sprintf(script, "PhoneGap.CallbackSuccess(%s)", data);
	mWebView->callJS(script);
}

/**
 * General wrapper for phoneGap success callback.
 * If an operation is successful this function should be called.
 *
 * @param data the data that should be passed to the callback function
 */
void PhoneGapMessageHandler::sendPhoneGapError(
		const char* data,
		MAUtil::String callbackID)
{
	char script[1024];
	sprintf(
		script,
		"PhoneGap.CallbackError('%s', \"{'message' : '%s'}\")",
		callbackID.c_str(),
		data);
	mWebView->callJS(script);
}
