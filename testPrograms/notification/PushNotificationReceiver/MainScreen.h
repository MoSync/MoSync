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

/**
 * @brief Application's main screen(tab screen).
 * Listens for push notifications.
 */
class MainScreen:
	public TabScreen,
	public PushNotificationListener,
	public TCPListener,
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
