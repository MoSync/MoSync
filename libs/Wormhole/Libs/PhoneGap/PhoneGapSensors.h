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
 * @file PhoneGapSensors.h
 * @author Ali Sarrafi
 *
 * Implementation of PhoneGap sensor calls made from JavaScript.
 */

#ifndef WORMHOLE_LIBS_PHONEGAP_PHONEGAPSENSORS_H_
#define WORMHOLE_LIBS_PHONEGAP_PHONEGAPSENSORS_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>

namespace Wormhole
{
	class JSONMessage;
	class PhoneGapMessageHandler;

	/**
	 * Class that implements PhoneGap sensor APIs.
	 */
	class PhoneGapSensors
	{
	public:
		/**
		 * Constructor.
		 */
		PhoneGapSensors(PhoneGapMessageHandler* messageHandler);

		/**
		 * Destructor.
		 */
		virtual ~PhoneGapSensors();

		/**
		 * Implementation of File API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		virtual void handleMessage(JSONMessage& message);

		virtual void sendAccelerometerData(MASensor sensorData);

		virtual void sendCompassData(MASensor sensorData);

		/**
		 * Called from PhoneGapMessageHandler on EVENT_TYPE_LOCATION.
		 */
		virtual void sendLocationData(const MAEvent& event);

	protected:
		/**
		 * Processes Accelerometer Requests from PhoneGap JavaScript
		 */
		virtual void processAcelerometerRequest(
				MAUtil::String callbackID,
				bool isWatched);

		/**
		 * Processes Location Requests from PhoneGap JavaScript
		 */
		virtual void processLocationRequest(
				MAUtil::String callbackID,
				bool isWatched);

		/**
		 * Processes Compass Requests from PhoneGap JavaScript
		 */
		virtual void processCompassRequest(
				MAUtil::String callbackID,
				bool isWatched);

		/**
		 * Sends the data to Accelerometer, it creates a JSON string for the
		 * callback and passes it to the succes callback of PhoneGap
		 *
		 * @param callbackID ID of the PhoneGapCallback to be used
		 * @param sensorData Accelerometer data
		 */
		virtual void sendAccelerometerData(
				MAUtil::String callbackID,
				MASensor sensorData);

		/**
		 * Sends the data to Accelerometer, it creates a JSON string for the
		 * callback and passes it to the succes callback of PhoneGap
		 *
		 * @param callbackID ID of the PhoneGapCallback to be used
		 * @param sensorData Accelerometer data
		 */
		virtual void sendCompassData(
				MAUtil::String callbackID,
				MASensor sensorData);

	protected:
		PhoneGapMessageHandler* mMessageHandler;

		/**
		 * A Pointer to the main webview
		 * Used for communicating with PhoneGap
		 */
		NativeUI::WebView* mWebView;

		/**
		 * Accelerometer Watch flag which indicates the type of call from PhoneGap
		 */
		bool mAccelerometerWatchStarted;

		/**
		 * Stores the CallbackID of the accelerometer so the result can be sent
		 * after receiving the sensor event
		 */
		MAUtil::String mAccelerometerWatchCallBack;

		/**
		 * Stores the CallbackID of the accelerometer so the result can be sent
		 * after receiving the sensor event
		 */
		MAUtil::String mLocationWatchCallBack;

		/**
		 * Accelerometer Watch flag which indicates the type of call from PhoneGap
		 */
		bool mLocationWatchStarted;

		/**
		 * Stores the CallbackID of the accelerometer so the result can be sent
		 * after receiving the sensor event
		 */
		MAUtil::String mCompassWatchCallBack;

		/**
		 * Compass Watch flag which indicates the type of call from PhoneGap
		 */
		bool mCompassWatchStarted;
	};
} // namespace

#endif
