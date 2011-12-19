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
 * @author Bogdan Iusco.
 */

#include "MainScreen.h"

#include <conprint.h>
#include <wchar.h>
#include <ma.h>
#include <maassert.h>
#include <mawstring.h>
#include <maxtoa.h>
#include <mastdlib.h>
#include "MAHeaders.h"

/**
 * Constructor.
 */
MainScreen::MainScreen() :
    Screen(),
    mSoundListener(NULL),
    mMainLayout(NULL),
    mMessageLabel(NULL)
{
    mCurrentImageHandle = RES_IMAGE1;
    createMainLayout();

    NotificationManager::getInstance()->registerPushNotification(
            PUSH_NOTIFICATION_TYPE_BADGE |
            PUSH_NOTIFICATION_TYPE_ALERT |
            PUSH_NOTIFICATION_TYPE_SOUND,
            "");
    NotificationManager::getInstance()->addPushNotificationListener(this);
    Environment::getEnvironment().addTimer(this, 200, 0);
    mSoundListener = new SoundListener();
    mSoundListener->start();
}

/**
 * Destructor.
 */
MainScreen::~MainScreen()
{
    delete mSoundListener;
    Environment::getEnvironment().removeTimer(this);
    NotificationManager::getInstance()->removePushNotificationListener(this);
}

/**
 * Creates and adds main layout to the screen.
 */
void MainScreen::createMainLayout() {
    // Create and add the main layout to the screen.
    mMainLayout = new RelativeLayout();
    Screen::setMainWidget(mMainLayout);
    MAExtent screenSize = maGetScrSize();
    int screenWidthInt = EXTENT_X(screenSize);
    int screenHeightInt = EXTENT_Y(screenSize);
    float screenHeight = (float) screenHeightInt;
    float screenWidth = (float) screenWidthInt;

    mImage = new Image();
    mImage->setImage(mCurrentImageHandle);
    mImage->setScaleMode(IMAGE_SCALE_XY);
    mImage->setWidth(screenWidthInt);
    mImage->setHeight(screenHeightInt);
    printf("widget height = %d", mImage->getHeight());
    printf("height float= %f", screenHeight);
    printf("width float= %f", screenWidth);
    mMainLayout->addChild(mImage);

    float left = screenWidth * 0.0937;
    printf("left = %f", left);
    float top = screenHeight * 0.0937;//0.10625;
    printf("top = %f", top);

    float widgetWidth = screenWidth - (2.0 * left);
    float widgetHeight = screenHeight * 0.49 - top;
    printf("widgetWidth = %f, widgetHeight = %f", widgetWidth, widgetHeight);
    mMessageLabel = new Label();
    mMessageLabel->setText("Hi from Cluj!");
    mMessageLabel->setPropertyInt(MAW_LABEL_MAX_NUMBER_OF_LINES, 0);
    mMessageLabel->setPropertyFloat(MAW_WIDGET_LEFT, left);
    mMessageLabel->setPropertyFloat(MAW_WIDGET_TOP, top);
//  mMessageLabel->setBackgroundColor(0x123456);
    mMessageLabel->setWidth((int)widgetWidth);
    mMessageLabel->setHeight((int)widgetHeight);
    mMainLayout->addChild(mMessageLabel);
}

/**
 * Called when the application receives a push notification.
 * @param pushNotification The received push notification.
 */
void MainScreen::didReceivePushNotification(
    PushNotification& pushNotification)
{
    printf("MainScreen::didReceivePushNotification");
    printf("Message: %s", pushNotification.getAlertMessage().c_str());
    mMessageLabel->setText(pushNotification.getAlertMessage().c_str());
    NotificationManager::getInstance()->setApplicationIconBadgeNumber(0);
}

/**
 * Called when application has been registered for push notification.
 */
void MainScreen::didApplicationRegistered(MAUtil::String& token)
{
    printf(" MainScreen::didApplicationRegistered %s", token.c_str());
}

/**
 * Called if the application did not registered for push notification.
 */
void MainScreen::didFaildToRegister(
    const int code,
    MAUtil::String& error)
{
    printf("MainScreen::didFaildToRegister %s", error.c_str());
}

void MainScreen::runTimerEvent()
{
    printf("MainScreen::runTimerEvent");
    int random = (rand() % 3) + 1;
    printf("Set image handle %d", mCurrentImageHandle);
    mImage->setImage(random);
}
