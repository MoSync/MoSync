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
 * @author Bogdan Iusco.
 * @date 10 Nov 2011
 *
 * @brief Application's screen.
 * It's responsible for creating the UI and listens for push notifications.
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <maapi.h>
#include <MAUtil/util.h>
#include <MAUtil/Environment.h>

#include <Notification/NotificationManager.h>
#include <Notification/PushNotification.h>
#include <Notification/PushNotificationListener.h>

#include "TCPListener.h"
// Forward declarations
class TCPConnection;
class SoundListener;

/**
 * @brief Application's screen.
 * It's responsible for creating the UI and listens for push notifications.
 */
class MainScreen:
    public NativeUI::Screen,
    public MAUtil::TimerListener,
    public Notification::PushNotificationListener,
    public TCPListener
{

public:
    /**
     * Constructor.
     */
        MainScreen();

    /**
     * Destructor.
     */
    ~MainScreen();

    /**
     * Show a screen. Only one screen at a time is visible.
     * The previous screen will be hidden when showing a screen.
     * Note: This method is only applicable to screens.
     */
    virtual void show();
	virtual void ConnectionEstablished();
private:
    /**
     * Creates and adds main layout to the screen.
     */
    void createMainLayout();

    /**
     * Called when the application receives a push notification.
     * @param pushNotification The received push notification.
     */
    virtual void didReceivePushNotification(
        Notification::PushNotification& pushNotification);

    /**
     * Called when application has been registered for push notification.
     */
    virtual void didApplicationRegistered(MAUtil::String& token);

    /**
     * Called if the application did not registered for push notification.
     */
    virtual void didFaildToRegister(
        MAUtil::String& error);

    /**
     * Timer callback method for changing the background image.
     */
    virtual void runTimerEvent();

    void checkStore();

    void storeRegistrationID(MAUtil::String& token);
private:
    /**
     * Used for playing a sound repeatedly.
     */
    SoundListener* mSoundListener;

    /**
     * Used for sending data to server application.
     */
    TCPConnection* mConnection;

    /**
     * Displays the message received from the server.
     */
    NativeUI::Label* mMessageLabel;

    /**
     * Displays the background image.
     */
    NativeUI::Image* mImage;

	// Android only.
	// Send reg ID to the server only first time
	// the app is launched.
	bool mSendRegistrationNeeded;
	MAHandle mCurrentImageHandle;
	MAUtil::String mToken;
};


#endif /* MAINSCREEN_H_ */
