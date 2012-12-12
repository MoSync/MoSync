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
