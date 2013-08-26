/* Copyright 2013 David Axmark

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

/**
 * @file SensorManager.h
 * @author Iraklis Rossis
 *
 * Implementation of W3C sensor API for Javascript.
 */

#ifndef WORMHOLE_LIBS_W3C_SENSORMANAGER_H_
#define WORMHOLE_LIBS_W3C_SENSORMANAGER_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>

// The maximum amount of sensors in a device
#define MAXIMUM_SENSORS 8

namespace Wormhole
{
	class JSONMessage;
	class PhoneGapMessageHandler;

	class SensorManager
	{
	public:
		/**
		 * Constructor.
		 */
		SensorManager(PhoneGapMessageHandler* messageHandler);

		/**
		 * Destructor.
		 */
		virtual ~SensorManager();

		/**
		 * Implementation of the Sensor Manager API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		virtual void handleMessage(JSONMessage& message);

		/**
		 * Dispatching of sensor events.
		 * @param sensorData The sensor data of the event
		 */
		virtual void sendSensorData(MASensor sensorData);

	protected:
		/**
		 * Handles the findSensor message. It then posts the list
		 * of sensors back to Phonegap
		 * @param message The phonegap message
		 */
		virtual void findSensors(JSONMessage& message);

		PhoneGapMessageHandler* mMessageHandler;

		/**
		 * A Pointer to the main webview
		 * Used for communicating with PhoneGap
		 */
		NativeUI::WebView* mWebView;

		/**
		 * Stores the CallbackIDs of the various sensors so the result can be sent
		 * after receiving the sensor event
		 */
		MAUtil::String mSensorWatchCallBack[MAXIMUM_SENSORS];

		/**
		 * Makes sure that only a single reading will be returned
		 */
		bool mSensorSingleReadFlag[MAXIMUM_SENSORS];

	};
} // namespace

#endif
