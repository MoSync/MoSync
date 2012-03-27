/**
 * Push notification object.
 */
var sPushNotification = new PushNotificationManager();

/**
 * Account id used for push notification.
 */
var sAccountID = "emmaTestMosync@gmail.com";

/**
 * Connect to server.
 */
function connectToServerButtonClicked()
{
	var ipAddress = document.getElementById("serverAddress").value;
	var port = document.getElementById("serverPort").value;
	sPushNotification.initialize(ipAddress, port);
	setTimeout("showRegistrationContent();", 100);
}

function showRegistrationContent()
{
	document.getElementById("connectToServerContent").style.display = "none";
	document.getElementById("registerPushNotificationButton").style.display = "block";
}
/**
 * Set push notification types, account id and register for
 * receiving push notifications.
 */
function registerPushNotification()
{
	var typesArray = [PushNotificationManager.type.badge,
					  PushNotificationManager.type.sound,
					  PushNotificationManager.type.alert];
	sPushNotification.types(typesArray);
	sPushNotification.accountID(sAccountID);
	sPushNotification.register(
		push_notification_did_registered,
		push_notification_failed_to_register);
}

/**
 * The application has successfuly registered for receiving push notifications.
 * @param token Device token/Application ID.
 */
function push_notification_did_registered(token)
{
	alert("The app has successfuly registered for receiving push notifications");
	sPushNotification.listener(did_receive_push_notification);
	document.getElementById("registerPushNotificationButton").style.display = "none";
	document.getElementById("pushNotificationContent").style.display = "block";
	document.getElementById("deviceTokenContent").style.display = "block";
	document.getElementById("deviceToken").innerHTML = split_token(token);
}

/**
 * The application did not registered for receiving push notifications.
 * @param error Reason why it failed.
 */
function push_notification_failed_to_register(error)
{
	alert("Cannot register for receiving push notifications. Reason = " + error);
}

/**
 * Called if a new push notification is received.
 * Display it on screen.
 * @param newPushNotification Received push notification.
 */
function did_receive_push_notification(newPushNotification)
{
	document.getElementById("pushNotificationMessage").innerHTML = newPushNotification.message;
	document.getElementById("pushNotificationSound").innerHTML = newPushNotification.sound;
	document.getElementById("pushNotificationIconBadge").innerHTML = newPushNotification.iconBadge;
}

/**
 * Split a given token into small strings.
 *
 * @param {String} token The given token.
 * @return {String} br tag will be added to the given token after
 * sequences of chars.
 */
function split_token(token)
{
	if (!token)
	{
		return "";
	}

	// Split token in multiple strings with a fixed length.
	var stringArray = token.match(/.{1,20}/g);
	var returnedString = "";

	for (var i = 0; i < stringArray.length; i++)
	{
		returnedString = returnedString + stringArray[i] + "<br/>";
	}

	return returnedString;
}