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
 * @file MainScreen.cpp
 * @author Emma Tresanszki and Bogdan Iusco
 *
 * @brief Application's main screen(tab screen).
 */

#include "MainScreen.h"
#include "SettingsScreen.h"
#include "DisplayNotificationScreen.h"
#include "TCPConnection.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
	TabScreen(),
	mDisplayNotificationScreen(NULL),
	mSettingsScreen(NULL),
	mTCPConnection(NULL),
	mToken(NULL)
{
	mDisplayNotificationScreen = new DisplayNotificationScreen();
	mSettingsScreen = new SettingsScreen(this);
	mTCPConnection = new TCPConnection(this);

	this->addTab(mDisplayNotificationScreen);
	this->addTab(mSettingsScreen);

	NotificationManager::getInstance()->registerPushNotification(
		PUSH_NOTIFICATION_TYPE_BADGE |
		PUSH_NOTIFICATION_TYPE_SOUND |
		PUSH_NOTIFICATION_TYPE_ALERT,
		"");
	NotificationManager::getInstance()->addPushNotificationListener(this);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	delete mDisplayNotificationScreen;
	delete mSettingsScreen;
	delete mToken;
}

/**
 * Called when the application receives a push notification.
 * @param pushNotification The received push notification.
 */
void MainScreen::didReceivePushNotification(
    PushNotification& pushNotification)
{
	printf("MainScreen::didReceivePushNotification");
	mDisplayNotificationScreen->pushNotificationReceived(pushNotification);
}


/**
 * Called when application has been registered for push notifications.
 */
void MainScreen::didApplicationRegistered(MAUtil::String& token)
{
	printf("MainScreen::didApplicationRegistered");
	mToken = new MAUtil::String(token);
}

/**
 * Called when the application has been unregistered for push notifications.
 * Platform: Android only.
 */
void MainScreen::didApplicationUnregister()
{
	printf("MainScreen::didApplicationUnregister");
}

/**
 * Called if the application did not registered for push notifications.
 */
void MainScreen::didFaildToRegister(
    MAUtil::String& error)
{
	printf("MainScreen::didFaildToRegister");
}

/**
 * This method is called when a tab screen has changed to a new tab.
 * @param tabScreen The tab screen object that generated the event.
 * @param tabScreenIndex The index of the new tab.
 */
void MainScreen::tabScreenTabChanged(
    TabScreen* tabScreen,
    const int tabScreenIndex)
{

}

/**
 * Called when the application is connected to the server.
 */
void MainScreen::connectionEstablished()
{
	printf("MainScreen::connectionEstablished()");
	mSettingsScreen->connectionEstablished();
	if (mToken)
	{
		mTCPConnection->sendData(*mToken);
	}
}

/**
 * Called when connect button is pressed.
 * @param ipAddress Server's ip address written by the user.
 * @param port Server's port written by the user.
 */
void MainScreen::connectToServer(const MAUtil::String& ipAddress,
		const MAUtil::String& port)
{
	int resultCode = mTCPConnection->connect(ipAddress, port);
	if (resultCode != resultCode)
	{
		mSettingsScreen->connectionFailed();
	}
}
