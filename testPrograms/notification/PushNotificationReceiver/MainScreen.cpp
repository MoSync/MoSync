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
#include "Util.h"

#define C2DM_USER_ACCOUNT "emmaTestMosync@gmail.com"

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

	if ( isAndroid() )
	{
		// Store the reg ID, and call registration only once per app.
		checkStore();
	}
	else
	{
		int registerCode = Notification::NotificationManager::getInstance()->registerPushNotification(
				Notification::PUSH_NOTIFICATION_TYPE_BADGE |
				Notification::PUSH_NOTIFICATION_TYPE_ALERT |
				Notification::PUSH_NOTIFICATION_TYPE_SOUND,
				C2DM_USER_ACCOUNT);

		if ( MA_NOTIFICATION_RES_UNSUPPORTED == registerCode )
			maPanic(0, "This device does not support push notifications");
	}

	NotificationManager::getInstance()->addPushNotificationListener(this);
}

/**
 * Used for Android only.
 * Check if the store exists.
 * If it does not exist, call the registration method,
 * create the store for later writing to it after the
 * connection to the server is established.
 */
void MainScreen::checkStore() {
	MAHandle myStore = maOpenStore("MyPushStore", 0);

	if (myStore == STERR_NONEXISTENT)
	{
		mSendRegistrationNeeded = true;

		// Request registration ID.
		int registerCode = Notification::NotificationManager::getInstance()->registerPushNotification(
				Notification::PUSH_NOTIFICATION_TYPE_BADGE
						| Notification::PUSH_NOTIFICATION_TYPE_ALERT
						| Notification::PUSH_NOTIFICATION_TYPE_SOUND,
				C2DM_USER_ACCOUNT);
		if ( MA_NOTIFICATION_RES_UNSUPPORTED == registerCode )
			maPanic(0, "This device does not support push notifications");

		// Close store without deleting it.
		maCloseStore(myStore, 0);
	}
	else
	{
		mSendRegistrationNeeded = false;
		// Close store without deleting it.
		maCloseStore(myStore, 0);
	}
}

/**
 * Used for Android only.
 * Stores the registration ID in a store for later use.
 * @param token The registration_ID.
 */
void MainScreen::storeRegistrationID(MAUtil::String* token)
{
	mToken = token;

	// Store doesn't exist.
	MAHandle myStore = maOpenStore("MyPushStore", MAS_CREATE_IF_NECESSARY);

	// Create store and write Registration ID
	MAHandle myData = maCreatePlaceholder();
	if(maCreateData(myData, token->length()) == RES_OK)
	{
		 maWriteData(myData, token->c_str(), 0, token->length());
		 // Write the data resource to the store.
		 int result = maWriteStore(myStore, myData);

		 if ( result < 0 )
		 {
			 printf("Cannot write to store the token!!");
		 }
		 maCloseStore(myStore, 0);
	}

	// Finally, send it over TCP to the server.
	//mConnection->sendData(token);
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
	delete mDisplayNotificationScreen;
	delete mSettingsScreen;
	delete mToken;
    Notification::NotificationManager::getInstance()->removePushNotificationListener(this);
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
	printf(token.c_str());
	mToken = new MAUtil::String(token);
	mDisplayNotificationScreen->pushRegistrationDone(true);
    //mMessageLabel->setText("Your app registered for push notifications");
    // Now you can press Connect to connect and send data to the server.
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
	mDisplayNotificationScreen->pushRegistrationDone(false);
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
	// Update the UI.
	mSettingsScreen->connectionEstablished();

	if (mToken)
	{
		// This is the first time the app is launched on Android phone.
		// Need to send the token. Also, store it to the store.
		if (isAndroid())
		{
			storeRegistrationID(mToken);
		}

		// Finally send it over TCP to the server.
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
