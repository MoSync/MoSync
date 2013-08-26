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
 * @file MainScreen.h
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Application's main screen(tab screen).
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <Notification/NotificationManager.h>
#include <Notification/PushNotification.h>
#include <Notification/PushNotificationListener.h>

#include "Listeners.h"

using namespace NativeUI;
using namespace Notification;

//Forward declarations
class DisplayNotificationScreen;
class SettingsScreen;
class TCPConnection;
class HTTPConnection;

/**
 * @brief Application's main screen(tab screen).
 * Listens for push notifications.
 */
class MainScreen:
	public TabScreen,
	public PushNotificationListener,
	public TCPListener,
	public HTTPListener,
	public SettingsScreenListener
{

public:
	/**
	 * Constructor.
	 */
		MainScreen();

	/**
	 * Destructor.
	 */
	virtual ~MainScreen();
private:

	/**
	 * Used for Android only.
	 * Check if the store exists.
	 * If it does not exist, call the registration method,
	 * create the store for later writing to it after the
	 * connection to the server is established.
	 */
    void checkStore();

    /**
     * Used for Android only.
     * Stores the registration ID in a store for later use.
     * @param token The registration_ID.
     */
    void storeRegistrationID(MAUtil::String* token);

	// PushNotificationListener methods

    /**
     * Called when the application receives a push notification.
     * @param pushNotification The received push notification.
     */
    virtual void didReceivePushNotification(
        PushNotification& pushNotification);

    /**
     * Called when application has been registered for push notifications.
     */
    virtual void didApplicationRegistered(MAUtil::String& token);

    /**
     * Called when the application has been unregistered for push notifications.
     * Platform: Android only.
     */
    virtual void didApplicationUnregister();

    /**
     * Called if the application did not registered for push notifications.
     */
    virtual void didFailedToRegister(
        MAUtil::String& error);

    /**
     * Called when the application is connected to the server.
     */
	virtual void connectionEstablished();

	/**
	 * Called when the server has received the authorization key.
	 */
	virtual void messageSent();

	/**
	 * Called when connect button is pressed.
	 * @param ipAddress Server's ip address written by the user.
	 * @param port Server's port written by the user.
	 */
	virtual void connectToServer(const MAUtil::String& ipAddress,
		const MAUtil::String& port);

private:
    /**
     * Screen used for displaying push notifications.
     */
    DisplayNotificationScreen* mDisplayNotificationScreen;

    /**
     * Screen used for setup the environment.
     */
    SettingsScreen* mSettingsScreen;

	/**
	 * Used to establish connection to server.
	 */
	TCPConnection* mTCPConnection;
	HTTPConnection* mHttpConnection;

	/**
	 * Token received after registering to APNs / Google service.
	 */
	MAUtil::String* mToken;

	/**
	 * Android only.
	 * Send reg ID to the server only first time the app is launched.
	 */
	bool mSendRegistrationNeeded;
};

#endif /* MAINSCREEN_H_ */
