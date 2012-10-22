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
 * Handles JSON messages sent from JavaScript.
 */

#include <maapi.h>
#include <mastdlib.h> // C string conversion functions
#include <conprint.h>
#include "PhoneGapMessageHandler.h"

namespace Wormhole
{
	// NameSpaces we want to access.
	using namespace MAUtil; // Class Moblet, String
	using namespace NativeUI; // WebView widget

	/**
	 * Constructor.
	 */
	PhoneGapMessageHandler::PhoneGapMessageHandler(NativeUI::WebView* webView) :
		mWebView(webView),
		mPhoneGapSensors(this),
		mPhoneGapSensorManager(this),
		mPhoneGapFile(this),
		mPhoneGapCapture(this),
		mPhoneGapCamera(this),
		mPushNotificationManager(this),
		mBeepSound(0)
	{
		enableHardware();

		for(int i = 0; i < MAXIMUM_SENSORS; i++)
		{
			mSensorEventToManager[MAXIMUM_SENSORS] = false;
		}
	}

	/**
	 * Destructor.
	 */
	PhoneGapMessageHandler::~PhoneGapMessageHandler()
	{
	}

	/**
	 * Initializes PhoneGap by sending the native Ready Message.
	 */
	void PhoneGapMessageHandler::initializePhoneGap()
	{
		callJS("try{PhoneGap.onNativeReady.fire()}catch(e){_nativeReady = true}");
	}

	/**
	 * Set the sound resource played as the beep sound.
	 */
	void PhoneGapMessageHandler::setBeepSound(MAHandle sound)
	{
		mBeepSound = sound;
	}

	/**
	 * Implementation of PhoneGap and other APIs exposed in JavaScript.
	 * This function is used to detect different messages and call the
	 * respective function in MoSync.
	 *
	 * @return true if message was handled, false if not.
	 */
	bool PhoneGapMessageHandler::handlePhoneGapMessage(JSONMessage& message)
	{
		// MoSync servcies implemented on top of the PhoneGap protocol
		// for convenience. We can move this to its own message handler
		// at a later point.
		if ((message.getParam("service") == "mosync") &&
			(message.getParam("action") == "mosync.notification.messageBox"))
		{
			String titleText = message.getParam("title");
			String messageText = message.getParam("message");
			maMessageBox(titleText.c_str(), messageText.c_str());
		}
		// Send device information to PhoneGap
		else if ((message.getParam("service") == "Device") &&
				(message.getParam("action") == "Get"))
		{
			sendDeviceProperties(message.getParam("PhoneGapCallBackId"));
		}
		// Process the vibration message
		else if ((message.getParam("service") == "Notification") &&
				(message.getParam("action") == "vibrate"))
		{
			int duration = message.getArgsFieldInt("duration");
			maVibrate(duration);
		}
		//Process the beep message
		else if ((message.getParam("service") == "Notification") &&
				(message.getParam("action") == "beep"))
		{
			int repeatCount = message.getParamInt("args");
			for (int i = 0; i < repeatCount; i++)
			{
				if (mBeepSound > 0)
				{
					maSoundPlay(mBeepSound, 0, maGetDataSize(mBeepSound));
				}
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
		else if (message.getParam("service") == "SensorManager")
		{
			mPhoneGapSensorManager.handleMessage(message);
		}
		else if (message.getParam("service") == "File")
		{
			mPhoneGapFile.handleFileMessage(message);
		}
		else if (message.getParam("service") == "FileTransfer")
		{
			mPhoneGapFile.handleFileTransferMessage(message);
		}
		else if (message.getParam("service") == "PushNotification")
		{
			mPushNotificationManager.handleMessage(message);
		}
		else if (message.getParam("service") == "Capture")
		{
			mPhoneGapCapture.handleMessage(message);
		}
//		else if (message.getParam("service") == "Camera")
//		{
//			mPhoneGapCamera.handleMessage(message);
//		}
		else
		{
			// Message was not handled.
			return false;
		}

		// Message was handled.
		return true;
	}

	/**
	 * Processes the Key Events and sends the appropriate
	 * message to PhoneGap.
	 */
	void PhoneGapMessageHandler::processKeyEvent(int keyCode, int nativeKeyCode)
	{
		if (MAK_BACK == keyCode)
		{
			callJS("PhoneGapCommandResult('backbutton');");
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
		sprintf(
			buffer,
			"\\\"%s\\\"",
			networkType);

		callSuccess(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_OK,
			buffer);
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

		//Due to some limitations on some devices
		//We have to check the UUID separately
		if(uuidRes < 0)
		{
			//PhoneGap does not return an error if it cannot read UUID
			//So we just return a value for the phoneGap apps to work
			sprintf(deviceUUID, "Not Accessible");
		}
		//if Any of the above commands fail send an error to PhoneGap
		if((nameRes < 0)
			|| (osRes < 0)
			|| (osVersionRes < 0))
		{
			//send an error to PhoneGap
			callError(
				callbackID,
				PHONEGAP_CALLBACK_STATUS_ERROR,
				"MoSync: Failed to get device information");
			return;
		}

		// Send the result back to PhoneGap.
		sprintf(buffer,
			"\\'{"
				"\"platform\":\"%s\","
				"\"name\":\"%s\","
				"\"uuid\":\"%s\","
				"\"version\":\"%s\","
				"\"phonegap\":\"1.2.0\""
			"}\\'",
			deviceOS,
			deviceName,
			deviceUUID,
			deviceOSVersion
			);

		callSuccess(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_OK,
			buffer);
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
		if (event.type == EVENT_TYPE_LOCATION)
		{
			mPhoneGapSensors.sendLocationData(event);
		}
		else if (event.type == EVENT_TYPE_FOCUS_LOST)
		{
			//let the phoneGap app know that it should go to sleep
			callJS("PhoneGapCommandResult('pause');");
		}
		else if (event.type == EVENT_TYPE_FOCUS_GAINED)
		{
			//let the PhoneGap side know that it should resume
			callJS("PhoneGapCommandResult('resume');");
		}
	}

	/**
	 * Receives sensor events and forwards them to PhoneGap
	 *
	 * @param sensorData sensor number and data (See sensor API for more info)
	 */
	void PhoneGapMessageHandler::sensorEvent(MASensor sensorData)
	{
		if(mSensorEventToManager[sensorData.type] == false)
		{
			if (sensorData.type == SENSOR_TYPE_ACCELEROMETER)
			{
				mPhoneGapSensors.sendAccelerometerData(sensorData);
			}
			else if (sensorData.type == SENSOR_TYPE_COMPASS)
			{
				mPhoneGapSensors.sendCompassData(sensorData);
			}
		}
		else
		{
			mPhoneGapSensorManager.sendSensorData(sensorData);
		}
	}

	/**
	 * Call the PhoneGap success function.
	 *
	 * @param callbackID The id of the JS callback function.
	 * @param status Status code.
	 * @param args Return values as a JSON string.
	 * @param keepCallback true if this callback should be kept by PhoneGap.
	 * @param castFunction Name of an optional JS function that
	 * will convert the JSON args to a JS object.
	 */
	void PhoneGapMessageHandler::callSuccess(
		const String& callbackID,
		const MAUtil::String& status,
		const String& args,
		bool keepCallback,
		const String& castFunction
		)
	{
		callCallback(
			"PhoneGap.CallbackSuccess",
			callbackID,
			status,
			args,
			keepCallback,
			castFunction);
	}

	/**
	 * Call the PhoneGap error function.
	 *
	 * @param callbackID The id of the JS callback function.
	 * @param errorCode The error code.
	 * @param errorMessage Optional error message.
	 * @param keepCallback true if this callback should be kept by PhoneGap.
	 */
	void PhoneGapMessageHandler::callError(
		const String& callbackID,
		const String& errorCode,
		const String& errorMessage,
		bool keepCallback
		)
	{
		// Use string "error" if there is
		// no error message.
		// TODO: Does PhoneGap use this param?
		String err = errorMessage;
		if (0 == err.size())
		{
			err = "error";
		}

		String args =
			"{\"code\":" + errorCode +
			",\"message\":\"" + err + "\"}";

		callCallback(
			"PhoneGap.CallbackError",
			callbackID,
			PHONEGAP_CALLBACK_STATUS_ERROR,
			args,
			keepCallback);
	}

	/**
	 * Evaluate a callback function in JavaScript.
	 *
	 * @param callbackFunction The JS function to handle the callback.
	 * @param callbackID The id of the JS callback.
	 * @param status Status code.
	 * @param args Result values as a JSON string.
	 * @param keepCallback true if this callback should be kept by PhoneGap.
	 * @param castFunction Name of an optional JS function that
	 * will convert the JSON args to a JS object.
	 */
	void PhoneGapMessageHandler::callCallback(
		const String& callbackFunction,
		const String& callbackID,
		const MAUtil::String& status,
		const String& args,
		bool keepCallback,
		const String& castFunction
		)
	{
		// Generate JavaScipt code.

		String script = callbackFunction + "(";
		script += "'" + callbackID + "'";

		script += ",'{";
		script += "\"status\":" + status;
		script += ",\"message\":" + args;

		if (keepCallback)
		{
			script += ",\"keepCallback\":true";
		}

		script += "}'";

		if (castFunction.size() > 0)
		{
			script += ",'" + castFunction + "'";
		}

		script += ")";

		callJS(script);
	}

	/**
	 * Evaluate JavaScript code in the WebView.
	 *
	 * @param script Code that should be evaluated.
	 */
	void PhoneGapMessageHandler::callJS(const String& script)
	{
		mWebView->callJS(script);
	}

	/**
	 * Set the target class for sensor event messages.
	 * @param sensor The sensor that is configured.
	 * @param toSensorManager If true, the SensorManager object will receive the events, normal PhoneGap API if false
	 */
	void PhoneGapMessageHandler::setSensorEventTarget(int sensor, bool toSensorManager)
	{
		mSensorEventToManager[sensor] = toSensorManager;
	}
} // namespace
