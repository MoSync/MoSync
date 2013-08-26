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
