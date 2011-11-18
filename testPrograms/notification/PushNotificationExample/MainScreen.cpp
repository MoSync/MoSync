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

/**
 * Constructor.
 */
MainScreen::MainScreen() :
    NativeUI::Screen(),
    mSoundListener(NULL),
    mConnection(NULL),
    mMessageLabel(NULL),
    mImage(NULL)
{
    mCurrentImageHandle = RES_IMAGE1;
    this->createMainLayout();

    Notification::NotificationManager::getInstance()->registerPushNotification(
        Notification::PUSH_NOTIFICATION_TYPE_BADGE |
        Notification::PUSH_NOTIFICATION_TYPE_ALERT |
        Notification::PUSH_NOTIFICATION_TYPE_SOUND,
        "");
    Notification::NotificationManager::getInstance()->addPushNotificationListener(this);
    MAUtil::Environment::getEnvironment().addTimer(this, 200, 0);
    mSoundListener = new SoundListener();
    mSoundListener->start();
    mConnection = new TCPConnection();
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
    mMessageLabel->setPropertyInt(MAW_LABEL_MAX_NUMBER_OF_LINES, 0);
    mMessageLabel->setPropertyFloat(MAW_WIDGET_LEFT, left);
    mMessageLabel->setPropertyFloat(MAW_WIDGET_TOP, top);
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
    mConnection->sendData(token);
}

/**
 * Called if the application did not registered for push notification.
 */
void MainScreen::didFaildToRegister(
    const int code,
    MAUtil::String& error)
{
    printf("MainScreen::didFaildToRegister %s", error.c_str());
    mConnection->sendData(error);
    maMessageBox(REGISTRATION_ERROR_TITLE, REGISTRATION_ERROR_MESSAGE);
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
