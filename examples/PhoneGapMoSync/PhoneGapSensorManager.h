/*
 * PhoneGapSensorManager.h
 *
 *  Created on: Jan 9, 2012
 *      Author: iraklis
 */

#ifndef PHONEGAPSENSORMANAGER_H_
#define PHONEGAPSENSORMANAGER_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include "PhoneGapMessage.h"

// The maximum amount of sensors in a device
#define MAXIMUM_SENSORS 8

class PhoneGapSensorManager
{
public:
	/**
	 * Constructor.
	 */
	PhoneGapSensorManager(PhoneGapMessageHandler* messageHandler);


	/**
	 * Destructor.
	 */
	virtual ~PhoneGapSensorManager();

	/**
	 * Implementation of the Sensor Manager API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	void handleMessage(PhoneGapMessage& message);

	/**
	 * Dispatching of sensor events.
	 * @param sensorData The sensor data of the event
	 */
	void sendSensorData(MASensor sensorData);
private:

	void findSensors(PhoneGapMessage& message);

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


#endif /* PHONEGAPSENSORMANAGER_H_ */
