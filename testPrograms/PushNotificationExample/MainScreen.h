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
 */

#ifndef MAINSCREEN_H_
#define MAINSCREEN_H_

// Include all the wrappers.
#include <NativeUI/Widgets.h>

#include <maapi.h>
#include <MAUtil/util.h>
#include <MAUtil/Environment.h>

#include "notification/NotificationManager.h"
#include "notification/PushNotification.h"
#include "notification/PushNotificationListener.h"
#include "SoundListener.h"

using namespace NativeUI;
using namespace MAUtil;
using namespace Notification;

class MainScreen:
    public Screen,
    public TimerListener,
    public PushNotificationListener
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
        PushNotification& pushNotification);

    /**
     * Called when application has been registered for push notification.
     */
    virtual void didApplicationRegistered(MAUtil::String& token);

    /**
     * Called if the application did not registered for push notification.
     */
    virtual void didFaildToRegister(
        const int code,
        MAUtil::String& error);

    virtual void runTimerEvent();

private:
    SoundListener* mSoundListener;

    /**
     * Main layout.
     */
    RelativeLayout* mMainLayout;
    Label* mMessageLabel;
    Image* mImage;
    MAHandle mCurrentImageHandle;
};


#endif /* MAINSCREEN_H_ */
