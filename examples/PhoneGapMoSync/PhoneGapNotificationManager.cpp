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
 * @file PhoneGapNotificationManager.h
 * @author Bogdan Iusco
 *
 * Implementation of PhoneGap push notification calls made from JavaScript.
 */

// Error message for invalid push notification types param.
#define INVALID_TYPES_PARAM "MoSync error: invalid push notification types."

// Default buffer size for javascript messages.
#define JS_BUF_SIZE 1024

#include <conprint.h>
#include <MAUtil/String.h>

#include <Notification/NotificationManager.h>
#include <Notification/PushNotification.h>

#include "PhoneGapMessage.h"
#include "PhoneGapMessageHandler.h"
#include "PhoneGapNotificationManager.h"

using namespace MAUtil;
using namespace Notification;

/**
 * Constructor.
 */
PhoneGapNotificationManager::PhoneGapNotificationManager(
	PhoneGapMessageHandler* messageHandler) :
		mMessageHandler(messageHandler)
{
	NotificationManager::getInstance()->addPushNotificationListener(this);
}

/**
 * Destructor.
 */
PhoneGapNotificationManager::~PhoneGapNotificationManager()
{
	NotificationManager::getInstance()->removePushNotificationListener(this);
}

/**
 * Implementation of Notification API exposed to JavaScript.
 * @return true if message was handled, false if not.
 */
void PhoneGapNotificationManager::handleMessage(PhoneGapMessage& message)
{
	// Push notification request from PhoneGap.
	if ((message.getParam("service") == "PushNotification"))
	{
		if (message.getParam("action") == "types")
		{
			printf("PhoneGapNotificationManager::handleMessage set types");
			this->setPushNotificationTypes(message);
		}
		else if (message.getParam("action") == "accountID")
		{
			printf("PhoneGapNotificationManager::handleMessage set accountID");
			mAccountID = message.getParam("args");
		}
		else if (message.getParam("action") == "register")
		{
			printf("PhoneGapNotificationManager::handleMessage register");
			mRegistrationCallBack = message.getParam("PhoneGapCallBackId");
			NotificationManager::getInstance()->registerPushNotification(
				mPushNotificationTypes,
				mAccountID);
		}
		else if (message.getParam("action") == "unregister")
		{
			printf("PhoneGapNotificationManager::handleMessage unregister");
			mUnregisterCallBack = message.getParam("PhoneGapCallBackId");
			NotificationManager::getInstance()->unregisterPushNotification();
		}
		else if (message.getParam("action") == "listener")
		{
			printf("PhoneGapNotificationManager::handleMessage listener");
			mListenerCallBack = message.getParam("PhoneGapCallBackId");
		}
	}
}

/**
 * Called when the application receives a push notification.
 * @param pushNotification The received push notification.
 */
void PhoneGapNotificationManager::didReceivePushNotification(
	PushNotification& pushNotification)
{
	printf("PhoneGapNotificationManager::didReceivePushNotification");

	char data[JS_BUF_SIZE];
	MAUtil::String message;
	if (pushNotification.containsMessage())
	{
		message = pushNotification.getMessage()->c_str();
	}

	MAUtil::String sound;
	if (pushNotification.containsSoundFileName())
	{
		sound = pushNotification.getSoundFileName()->c_str();
	}

	int iconBadge = 0;
	if (pushNotification.containsIconBadgeNumber())
	{
		iconBadge = pushNotification.getIconBadgeNumebr();
	}

	sprintf(
		data,
		"\\'{\"message\": \"%s\","
		"\"sound\": \"%s\","
		"\"iconBadge\":\"%d\"}\\'",
		message.c_str(),
		sound.c_str(),
		iconBadge);

	mMessageHandler->callSuccess(
		mListenerCallBack,
		PHONEGAP_CALLBACK_STATUS_OK,
		data,
		true);
}

/**
 * Called when application has been registered for push notifications.
 */
void PhoneGapNotificationManager::didApplicationRegistered(MAUtil::String& token)
{
	lprintfln("PhoneGapNotificationManager::didApplicationRegistered");
	String tokenJSONString = PhoneGapMessage::JSONStringify(token.c_str());
	mMessageHandler->callSuccess(
		mRegistrationCallBack,
		PHONEGAP_CALLBACK_STATUS_OK,
		tokenJSONString.c_str());
}

/**
 * Called when the application has been unregistered for push notifications.
 * Platform: Android only.
 */
void PhoneGapNotificationManager::didApplicationUnregister()
{
	printf("PhoneGapNotificationManager::didApplicationUnregister");
	mMessageHandler->callSuccess(
		mUnregisterCallBack,
		PHONEGAP_CALLBACK_STATUS_OK,
		"");
}

/**
 * Called if the application did not registered for push notifications.
 */
void PhoneGapNotificationManager::didFaildToRegister(
	MAUtil::String& error)
{
	printf("PhoneGapNotificationManager::didFaildToRegister");
	String errorJSONString = PhoneGapMessage::JSONStringify(error.c_str());
	if (errorJSONString.length() > 1)
	{
		errorJSONString.remove(0, 1);
		errorJSONString.remove(errorJSONString.length() - 1, 1);
	}

	mMessageHandler->callError(
			mRegistrationCallBack,
			PHONEGAP_CALLBACK_STATUS_ERROR,
			errorJSONString.c_str());
}

/**
 * Verify and set the allowed push notification types.
 * Call this method before registering the application.
 * @param message PhoneGap message.
 * The 'args' field must contain an int value(a bit mask specifying the
 * types of notifications the application accepts.)
 * See PushNotificationType for valid bit-mask values.
 * All invalid values from bit mask will be ignored.
 * If bit mask does not contain at least one good value the error callback
 * method will be called.
 */
void PhoneGapNotificationManager::setPushNotificationTypes(
	PhoneGapMessage& message)
{
	MAUtil::String callbackID  = message.getParam("PhoneGapCallBackId");
	int types = message.getParamInt("args");
	int bitmask = 0;
	if (types & PUSH_NOTIFICATION_TYPE_ALERT)
	{
		bitmask = bitmask | PUSH_NOTIFICATION_TYPE_ALERT;
	}
	if (types & PUSH_NOTIFICATION_TYPE_SOUND)
	{
		bitmask = bitmask | PUSH_NOTIFICATION_TYPE_SOUND;
	}
	if (types & PUSH_NOTIFICATION_TYPE_BADGE)
	{
		bitmask = bitmask | PUSH_NOTIFICATION_TYPE_BADGE;
	}

	// Check if bitmask contains at least one good value
	if (bitmask != 0)
	{
		mPushNotificationTypes = types;
		printf("set types: %d", types);
		mMessageHandler->callSuccess(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_OK,
			"");
	}
	else
	{
		// Call the error callback method.
		mMessageHandler->callError(
			callbackID,
			PHONEGAP_CALLBACK_STATUS_ERROR,
			INVALID_TYPES_PARAM);
	}
}
