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
 * @file PushNotificationManager.h
 * @author Bogdan Iusco
 *
 * Implementation of notification calls made from JavaScript.
 */

#ifndef WORMHOLE_LIBS_NOTIFICATION_PUSHNOTIFICATIONMANAGER_H_
#define WORMHOLE_LIBS_NOTIFICATION_PUSHNOTIFICATIONMANAGER_H_

#include <Wormhole/WebViewMessage.h>
#include <NativeUI/WebView.h>
#include <Notification/PushNotificationListener.h>
#include "../JSONMessage.h"
#include "TCPConnection.h"

namespace Wormhole
{
	// Forward declaration
	class Notification::PushNotification;

	class PhoneGapMessageHandler;

	/**
	 * Class that implements Notification APIs.
	 */
	class PushNotificationManager:
		public Notification::PushNotificationListener
	{
	public:
		/**
		 * Constructor.
		 */
		PushNotificationManager(PhoneGapMessageHandler* messageHandler);

		/**
		 * Destructor.
		 */
		virtual ~PushNotificationManager();

		/**
		 * Implementation of Notification API exposed to JavaScript.
		 * @return true if message was handled, false if not.
		 */
		void handleMessage(JSONMessage& message);

	private:
		/**
		 * Called when the application receives a push notification.
		 * @param pushNotification The received push notification.
		 */
		virtual void didReceivePushNotification(
			Notification::PushNotification& pushNotification);
		/**
		 * Called when application has been registered for push notifications.
		 */
		virtual void didApplicationRegistered(MAUtil::String& token);

		/**
		 * Called when the application has been unregistered for push notifications.
		 * Platform: Android only.
		 */
		virtual void didApplicationUnregister();

		/**
		 * Called if the application did not registered for push notifications.
		 */
		virtual void didFailedToRegister(
			MAUtil::String& error);

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
		void setPushNotificationTypes(JSONMessage& message);

		/**
		 * Connects to a push notification server
		 *
		 * @param serverIP IPAddress of the server
		 * @param port TCP port number of the server
		 */
		void connectToServer(MAUtil::String & serverIP, const int port);

		/**
		 * Remove the quotation marks from the begging and ending of a given string.
		 * For example if the given string is "myString" the returned string will
		 * be myString.
		 * @param string String to be parsed.
		 * @return The parsed string.
		 */
		MAUtil::String parseJSString(const MAUtil::String& string);

	private:
		PhoneGapMessageHandler* mMessageHandler;

		/**
		 * Used for sending the device token / application id to a specified
		 * server.
		 */
		TCPConnection* mConnection;

		/**
		 * Stores the CallbackID of the caller so the result can be sent
		 * after receiving an event.
		 */
		MAUtil::String mRegistrationCallBack;

		/**
		 * Stores the CallbackID of the caller so the result can be sent
		 * after receiving the unregister event.
		 */
		MAUtil::String mUnregisterCallBack;

		/**
		 * Stores the CallbackID of the caller so the result can be sent
		 * after receiving an event.1
		 */
		MAUtil::String mListenerCallBack;

		/**
		 * The ID of the account authorized to send messages to the application,
		 * typically the email address of an account set up by the application's
		 * developer.
		 */
		MAUtil::String mAccountID;

		/**
		 * A bit mask specifying the types of notifications the
		 * application accepts.
		 */
		int mPushNotificationTypes;
	};
} // namespace

#endif
