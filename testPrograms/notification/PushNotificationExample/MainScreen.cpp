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

#define REGISTRATION_ERROR_TITLE "Error"
#define REGISTRATION_ERROR_MESSAGE "Cannot register for push notification!"

// The number of images that will be displayed.
#define COUNT_IMAGES 3

// Left coordinate for label in percentage(9.37%)
#define LABEL_LEFT_CORNER 0.0937
// Top coordinate for label in percentage(9.37%)
#define LABEL_TOP_CORNER 0.0937

// Bottom coordinate for label in percentage(49%)
#define LABEL_BOTTOM_CORNER 0.49

#include "MainScreen.h"

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <maxtoa.h>
#include <mastdlib.h>

#include "MAHeaders.h"
#include "SoundListener.h"
#include "TCPConnection.h"
#include "Util.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
    NativeUI::Screen(),
    mSoundListener(NULL),
    mConnection(NULL),
    mMessageLabel(NULL),
    mImage(NULL),
	mSendRegistrationNeeded(false)
{
    mCurrentImageHandle = RES_IMAGE1;
    this->createMainLayout();

	if ( isAndroid() )
	{
		// Store the reg ID, and call registration only once per app.
		checkStore();
	}
	else
	{
		Notification::NotificationManager::getInstance()->registerPushNotification(
				Notification::PUSH_NOTIFICATION_TYPE_BADGE |
				Notification::PUSH_NOTIFICATION_TYPE_ALERT |
				Notification::PUSH_NOTIFICATION_TYPE_SOUND,
				"emma.tresanszki@mobilesorcery.com");
		mConnection = new TCPConnection(this);
	}

    Notification::NotificationManager::getInstance()->addPushNotificationListener(this);
    MAUtil::Environment::getEnvironment().addTimer(this, 200, 0);
    //mSoundListener = new SoundListener();
    //mSoundListener->start();
	Notification::NotificationManager::getInstance()->setPushNotificationsTickerText("My message");
	Notification::NotificationManager::getInstance()->setPushNotificationsTitle("MoSync Push notification");
}


/**
 * Android only.
 * Check if store exists, and get the registration value.
 * If not exist, create it and write to it the received reg.
 */
void MainScreen::checkStore()
{
	MAHandle myStore = maOpenStore("MyStore", 0);

		if(myStore == STERR_NONEXISTENT)
		{

			printf("Need to create the store -----------------------------");

			mSendRegistrationNeeded = true;

			// Request registration ID.
			Notification::NotificationManager::getInstance()->registerPushNotification(
					Notification::PUSH_NOTIFICATION_TYPE_BADGE |
					Notification::PUSH_NOTIFICATION_TYPE_ALERT |
					Notification::PUSH_NOTIFICATION_TYPE_SOUND,
					"emma.tresanszki@mobilesorcery.com");

			// Initiate connection to the server.
//			mConnection = new TCPConnection();

			// Close store without deleting it.
			maCloseStore(myStore,0);
		}
		else
		{
			printf("Store is already created ----------------------------");
			mSendRegistrationNeeded = false;
/*
		     // Store does exist, and myData is a valid handle to it.
			// This means that the registration ID was already send to the server.
			MAHandle myData = maCreatePlaceholder();
			//MAHandle myStore = maOpenStore("MyStore", 0);
			// The store exists, so we can read from it.
			int result = maReadStore(myStore, myData);
			if(result == RES_OUT_OF_MEMORY)
			{
				// This store is too large to read into memory - error
			}
			char registration[maGetDataSize(myData)];
			maReadData(myData, &registration, 0, maGetDataSize(myData));
*/
			// Close store without deleting it.
			maCloseStore(myStore,0);
		}

}

void MainScreen::storeRegistrationID(MAUtil::String& token)
{
	mToken = token;

	// Store doesn't exist.
	MAHandle myStore = maOpenStore("MyStore", MAS_CREATE_IF_NECESSARY);

	// Create store and write Registration ID
	MAHandle myData = maCreatePlaceholder();
	if(maCreateData(myData, token.length()) == RES_OK)
	{
		 maWriteData(myData, token.c_str(), 0, token.length());
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
    delete mSoundListener;
    MAUtil::Environment::getEnvironment().removeTimer(this);
    Notification::NotificationManager::getInstance()->removePushNotificationListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
    // Create and add the main layout to the screen.
    NativeUI::RelativeLayout* mainLayout = new NativeUI::RelativeLayout();
    NativeUI::Screen::setMainWidget(mainLayout);

    // Get the screen size.
    MAExtent screenSize = maGetScrSize();
    int screenWidthInt = EXTENT_X(screenSize);
    int screenHeightInt = EXTENT_Y(screenSize);
    float screenHeight = (float) screenHeightInt;
    float screenWidth = (float) screenWidthInt;

    // Add image to layout.
    mImage = new NativeUI::Image();
    mImage->setImage(mCurrentImageHandle);
    mImage->setScaleMode(NativeUI::IMAGE_SCALE_XY);
    mImage->setWidth(screenWidthInt);
    mImage->setHeight(screenHeightInt);
    mainLayout->addChild(mImage);

    // Calculate the top left coordinate for the message label.
    float left = screenWidth * LABEL_LEFT_CORNER;
    float top = screenHeight * LABEL_TOP_CORNER;

    // Calculate the label's size.
    float widgetWidth = screenWidth - (2.0 * left);
    float widgetHeight = screenHeight * LABEL_BOTTOM_CORNER - top;

    // Add the message label to layout.
    mMessageLabel = new NativeUI::Label();
    mMessageLabel->setText("");
	mMessageLabel->setLeftPosition((int)left);
	mMessageLabel->setTopPosition((int)top);
	mMessageLabel->setFontColor(0xFF0000);
   // mMessageLabel->setPropertyInt(MAW_LABEL_MAX_NUMBER_OF_LINES, 0);
	mMessageLabel->setWidth((int)widgetWidth);
	mMessageLabel->setHeight((int)widgetHeight);
    mainLayout->addChild(mMessageLabel);
}

/**
 * Called when the application receives a push notification.
 * @param pushNotification The received push notification.
 */
void MainScreen::didReceivePushNotification(
    Notification::PushNotification& pushNotification)
{
    printf("MainScreen::didReceivePushNotification message: %s",
        pushNotification.getMessage().c_str());
    mMessageLabel->setText(pushNotification.getMessage().c_str());
    Notification::NotificationManager::getInstance()->setApplicationIconBadgeNumber(0);
}

/**
 * Called when application has been registered for push notification.
 */
void MainScreen::didApplicationRegistered(MAUtil::String& token)
{
    printf(" MainScreen::didApplicationRegistered %s", token.c_str());

    mMessageLabel->setText("Your app registered for push notifications");
	if ( mSendRegistrationNeeded && isAndroid() )
	{
		mToken = token;
		// Initiate connection to the server.
		mConnection = new TCPConnection(this);
	}
	else if ( !isAndroid() )
	{
		mConnection->sendData(token);
	}
}

/**
 * Called if the application did not registered for push notification.
 */
void MainScreen::didFaildToRegister(
    MAUtil::String& error)
{
    printf("MainScreen::didFaildToRegister %s", error.c_str());
    mConnection->sendData(error);
    maMessageBox(REGISTRATION_ERROR_TITLE, error.c_str());
}

/**
 * Timer callback method for changing the background image.
 */
void MainScreen::runTimerEvent()
{
    int random = (rand() % COUNT_IMAGES) + 1;
    if (mCurrentImageHandle == random)
    {
        random = (rand() % COUNT_IMAGES) + 1;
    }
    mImage->setImage(random);
    mCurrentImageHandle = random;
}

/**
 * Show a screen. Only one screen at a time is visible.
 * The previous screen will be hidden when showing a screen.
 * Note: This method is only applicable to screens.
 */
void MainScreen::show()
{
    printf("MainScreen::show");
    NativeUI::Screen::show();
    Notification::NotificationManager::getInstance()->setApplicationIconBadgeNumber(0);
}

void MainScreen::ConnectionEstablished()
{
	// This is the first time the app is launched on Android phone.
	// Need to send the token. Also, store it to the store.
	storeRegistrationID(mToken);

	// Finally, send it over TCP to the server.
	mConnection->sendData(mToken);
}
