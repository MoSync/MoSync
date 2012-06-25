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
 * @file PushNotificationManager.cpp
 * @author Bogdan Iusco
 *
 * Implementation of notification calls made from JavaScript.
 */

// Error message for invalid push notification types param.
#define INVALID_TYPES_PARAM "MoSync error: invalid push notification types."

// Default buffer size for javascript messages.
#define JS_BUF_SIZE 1024

#include <conprint.h>
#include <MAUtil/String.h>

#include <Notification/NotificationManager.h>
#include <Notification/PushNotification.h>
#include "../../Encoder.h"
#include "../PhoneGap/PhoneGapMessageHandler.h"
#include "PushNotificationManager.h"

using namespace MAUtil;
using namespace Notification;

namespace Wormhole
{
	/**
	 * Constructor.
	 */
	PushNotificationManager::PushNotificationManager(
		PhoneGapMessageHandler* messageHandler) :
		mMessageHandler(messageHandler),
		mConnection(NULL)
	{
		NotificationManager::getInstance()->addPushNotificationListener(this);
		mConnection = new TCPConnection();
	}

	/**
	 * Destructor.
	 */
	PushNotificationManager::~PushNotificationManager()
	{
		NotificationManager::getInstance()->removePushNotificationListener(this);
	}

	/**
	 * Connects to a push notification server
	 *
	 * @param serverIP IPAddress of the server
	 * @param port TCP port number of the server
	 */
	void PushNotificationManager::connectToServer(MAUtil::String & serverIP, const int port)
	{
		mConnection->connect(serverIP, port);
	}

	/**
	 * Implementation of Notification API exposed to JavaScript.
	 * @return true if message was handled, false if not.
	 */
	void PushNotificationManager::handleMessage(JSONMessage& message)
	{
		// Push notification request from JavaScript.
		if ((message.getParam("service") == "PushNotification"))
		{
			if (message.getParam("action") == "types")
			{
				//printf("PushNotificationManager::handleMessage set types");
				this->setPushNotificationTypes(message);
			}
			else if (message.getParam("action") == "accountID")
			{
				mAccountID = this->parseJSString(message.getParam("args"));
			}
			else if (message.getParam("action") == "initialize")
			{
				MAUtil::String serverAddress = message.getArgsField("serverAddress");
				int serverPort = stringToInteger(message.getArgsField("serverPort"));
				connectToServer(serverAddress, serverPort);
			}
			else if (message.getParam("action") == "register")
			{
				mRegistrationCallBack = message.getParam("PhoneGapCallBackId");
				NotificationManager::getInstance()->registerPushNotification(
					mPushNotificationTypes,
					mAccountID);
			}
			else if (message.getParam("action") == "unregister")
			{
				mUnregisterCallBack = message.getParam("PhoneGapCallBackId");
				NotificationManager::getInstance()->unregisterPushNotification();
			}
			else if (message.getParam("action") == "listener")
			{
				mListenerCallBack = message.getParam("PhoneGapCallBackId");
			}
		}
	}

	/**
	 * Called when the application receives a push notification.
	 * @param pushNotification The received push notification.
	 */
	void PushNotificationManager::didReceivePushNotification(
		PushNotification& pushNotification)
	{
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
	void PushNotificationManager::didApplicationRegistered(MAUtil::String& token)
	{
		String tokenJSONString = Encoder::JSONStringify(token.c_str());
		mMessageHandler->callSuccess(
			mRegistrationCallBack,
			PHONEGAP_CALLBACK_STATUS_OK,
			tokenJSONString.c_str());

		// Send the token to the server.
		mConnection->sendData(token);
	}

	/**
	 * Called when the application has been unregistered for push notifications.
	 * Platform: Android only.
	 */
	void PushNotificationManager::didApplicationUnregister()
	{
		mMessageHandler->callSuccess(
			mUnregisterCallBack,
			PHONEGAP_CALLBACK_STATUS_OK,
			"");
	}

	/**
	 * Called if the application did not registered for push notifications.
	 */
	void PushNotificationManager::didFailedToRegister(
		MAUtil::String& error)
	{
		String errorJSONString = Encoder::JSONStringify(error.c_str());
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
	void PushNotificationManager::setPushNotificationTypes(
		JSONMessage& message)
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

	/**
	 * Remove the quotation marks from the begging and ending of a given string.
	 * For example if the given string is "myString" the returned string will
	 * be myString.
	 * @param string String to be parsed.
	 * @return The parsed string.
	 */
	MAUtil::String PushNotificationManager::parseJSString(
		const MAUtil::String& string)
	{
		MAUtil::String returnedString(string);
		if (returnedString.length() > 0)
		{
			char firstChar = returnedString[0];
			if (firstChar == '\"')
			{
				returnedString.remove(0, 1);
			}
		}

		if (returnedString.length() > 0)
		{
			int lastCharIndex = returnedString.length() - 1;
			char lastChar = returnedString[lastCharIndex];
			if (lastChar == '\"')
			{
				returnedString.remove(lastCharIndex, 1);
			}
		}

		return returnedString;
	}
} // namespace
