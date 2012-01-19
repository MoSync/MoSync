/*
Copyright (C) 2012 MoSync AB

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
 * @file mosync-pushnotifications.js
 * @author Bogdan
 *
 * Implementation of push notification API.
 */

if (!PhoneGap.hasResource("pushNotification"))
{
PhoneGap.addResource("pushNotification");

/**
 * This class provides access to device Push Notification Service.
 * @constructor
 */
var PushNotificationManager = function() {
	/**
	 * The last known acceleration.  type=Acceleration()
	 */
	this.lastPushNotificationData = null;
};

/** @constructor */
var PushNotificationData = function(message, sound, iconBadge)
{
  this.message = message;
  this.sound = sound;
  this.iconBadge = iconBadge;
};

/**
 * Constants indicating the types of push notifications the application
 * accepts.
 * Specific to iOS.
 * On Android PUSH_NOTIFICATION_TYPE_ALERT is set by default.
 */
PushNotificationManager.type = {
		/**
		 * The application accepts notifications that badge the application icon.
		 */
		badge: 1,
		/**
		 * The application accepts alert sounds as notifications.
		 */
		sound: 2,
		/**
		 * The application accepts alert messages as notifications.
		 */
		alert: 4
	};


PushNotificationManager.prototype.initialize = function(serverAddress, serverPort)
{
	PhoneGap.exec(
			onSuccess,
			onError,
			"PushNotification",
			"initialize",
			{
				"serverAddress": serverAddress,
				"serverPort": serverPort
			});
};

/**
 * Asynchronously starts the registration process.
 *
 * @param {Function} successCallback    The function to call when the
 * registration data is available.
 * @param {Function} errorCallback      The function to call when there
 * is an error getting the registration data.
 */
PushNotificationManager.prototype.register = function(
		successCallback,
		errorCallback)
{
	// successCallback required
	if (typeof successCallback !== "function") {
		console.log("PushNotificationManager Error: successCallback is not a function");
		return;
	}

	// errorCallback required
	if (errorCallback && (typeof errorCallback !== "function")) {
		console.log("PushNotificationManager Error: errorCallback is not a function");
		return;
	}

	var onSuccess = function(result)
	{
		successCallback(result);
	};

	var onError = function(err)
	{
		errorCallback(err);
	};

	PhoneGap.exec(onSuccess, onError, "PushNotification", "register", null);
};

/**
 * Unregister application for receiving push notifications.
 *
 * @param {Function} callback    The function to call when the application
 * has unregistered. This method is called only on Android platform.
 */
PushNotificationManager.prototype.unregister = function(callback)
{
	console.log("PushNotificationManager.prototype.unregister");
	if (callback && (typeof callback !== "function")) {
		console.log("PushNotificationManager Error: callback is not a function");
		return;
	}

	var onSuccess = function(result)
	{
		callback();
	};
	PhoneGap.exec(onSuccess, null, "PushNotification", "unregister");
};

/**
 * Set push notification allowed types.
 * Call this method before registering the application for receiving push
 * notification.
 *
 * @param {Function} successCallback    The function to call on success. (OPTIONAL)
 * @param {Function} errorCallback      The function to call on error. (OPTIONAL)
 * @param {Array.<PushNotificationManager.type>} types    Types of the notifications
 * accepted by the application. If this param is not specified the application
 * will be registered for receiving all types of notification. This param is
 * applied only for iOS platform. Android platform will ignore this value.
 */
PushNotificationManager.prototype.types = function(
	successCallback,
	errorCallback,
	types)
{
	console.log("PushNotificationManager.prototype.types");

	// Convert types param to a bitmask.
	var bitmask = 0;
	if (!types)
	{
		bitmask = PushNotificationManager.type.badge |
				  PushNotificationManager.type.sound |
				  PushNotificationManager.type.alert;
	}
	else if(typeof types == "array")
	{
		for(i in types) {
			bitmask = bitmask | types[i];
		}
	}
	else
	{
		console.log("PushNotificationManager Error: types is not an array");
		return;
	}

	var onSuccess = function(result)
	{
		if (successCallback && (typeof successCallback == "function"))
		{
			successCallback(err);
		}
	};

	var onError = function(err)
	{
		if (errorCallback && (typeof errorCallback == "function"))
		{
			 errorCallback(err);
		}
	};

	PhoneGap.exec(onSuccess, onError, "PushNotification", "types", bitmask);
};

/**
 * Set the account ID used for registering the application.
 * Call this method before registering the application.
 *
 * @param {String} accountID    Is the ID of the account authorized to send messages
 * to the application, typically the email address of an account set up
 * by the application's developer.
 * On iOS platform this param is ignored.
 *
 */
PushNotificationManager.prototype.accountID = function(
	accountID)
{
	console.log("PushNotificationManager.prototype.accountID");
    PhoneGap.exec(null, null, "PushNotification", "accountID", accountID);
};

/**
 * Listener for push notification
 *
 * @param newPushNotificationCallback The function to call when a new push notification is received,
 */
PushNotificationManager.prototype.listener = function(newPushNotificationCallback)
{
	console.log("PushNotificationManager.prototype.listener");
	// newPushNotificationCallback required
	if (typeof newPushNotificationCallback !== "function") {
		console.log("PushNotificationManager Error: newPushNotificationCallback is not a function");
		return;
	}

	var self = this;

	var onSuccess = function(result)
	{
		var data = JSON.parse(result);
		var message = data.message ? data.message : "";
		var sound = data.sound ? data.sound : "";
		var iconBadge = data.iconBadge ? data.iconBadge : 0;
		self.lastPushNotificationData = new PushNotificationData(
			message,
			sound,
			iconBadge);
		newPushNotificationCallback(self.lastPushNotificationData);
	};

	PhoneGap.exec(onSuccess, null, "PushNotification", "listener");
};

PhoneGap.addConstructor(function() {
	if (typeof navigator.pushNotification === "undefined") {
		navigator.pushNotification = new PushNotificationManager();
	}
});
} // End of Push Notification API
