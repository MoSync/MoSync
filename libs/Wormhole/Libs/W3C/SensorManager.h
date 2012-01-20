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
		void handleMessage(JSONMessage& message);

		/**
		 * Dispatching of sensor events.
		 * @param sensorData The sensor data of the event
		 */
		void sendSensorData(MASensor sensorData);

	private:
		/**
		 * Handles the findSensor message. It then posts the list
		 * of sensors back to Phonegap
		 * @param message The phonegap message
		 */
		void findSensors(JSONMessage& message);

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
