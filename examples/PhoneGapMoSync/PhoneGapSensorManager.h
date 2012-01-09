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

private:

	void findSensors(PhoneGapMessage& message);

	PhoneGapMessageHandler* mMessageHandler;

	/**
	 * A Pointer to the main webview
	 * Used for communicating with PhoneGap
	 */
	NativeUI::WebView* mWebView;

};


#endif /* PHONEGAPSENSORMANAGER_H_ */
