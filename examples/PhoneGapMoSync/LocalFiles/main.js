/**
 * The number of milliseconds to wait before executing the animation step.
 * Used in showing / hiding the push notification popover.
 */
var sAnimationInterval = 10;

/**
 * Value used in increasing / decresing the push notificaion popover opacity.
 */
var sOpacityValue = 0.3;

/**
 * Push notification popover close icon size in percentage.
 */
var sPopoverCloseIconSize = 0.1;

/**
 * Account id used for push notification.
 */
var sAccountID = "emmaTestMosync@gmail.com";

/**
 * Text that will be displayed if the application has registered for receiving
 * push notifications.
 */
var sSuccessfullyRegistrationText = "The application has successfully registered " +
	"for receiving push notifications. Use the following key for sending " +
	"push notifications:";

/**
 * Text that will be displayed if the application cannot register for receiving
 * push notifications.
 */
var sFailureRegistrationText = "The application has encountered the following " +
	"error while registering for receiving push notifications:";

/**
 * Text that will be displyed when a new push notification is received.
 */
var sNewPushNotificationText = "The application received a push notification.";

/**
 * Push notification registrotion button text.
 */
var sRegisterButtonText = "Register push notification";

/**
 * Push notification unregister button text.
 */
var sUnregisterButtonText = "Unregister push notification";

/**
 * Flag that indicates if the application is registered or not for receiving
 * push notifications.
 */
var sIsApplicationRegistered = false;

var deviceInfo = function() {
	document.getElementById("platform").innerHTML = device.platform;
	document.getElementById("version").innerHTML = device.version;
	document.getElementById("uuid").innerHTML = device.uuid;
	document.getElementById("name").innerHTML = device.name;
	document.getElementById("width").innerHTML = screen.width;
	document.getElementById("height").innerHTML = screen.height;
	document.getElementById("colorDepth").innerHTML = screen.colorDepth;
};

var getLocation = function() {
	var suc = function(p) {
		alert(p.coords.latitude + " " + p.coords.longitude);
	};
	var locFail = function() {
	};
	navigator.geolocation.watchPosition(suc, locFail);
};

var beep = function() {
	navigator.notification.beep(2);
};

var vibrate = function() {
	navigator.notification.vibrate(1000);
};

function roundNumber(num) {
	var dec = 3;
	var result = Math.round(num * Math.pow(10, dec)) / Math.pow(10, dec);
	return result;
}

var accelerationWatch = null;

function updateAcceleration(a) {
	document.getElementById('x').innerHTML = roundNumber(a.x);
	document.getElementById('y').innerHTML = roundNumber(a.y);
	document.getElementById('z').innerHTML = roundNumber(a.z);
}

var toggleAccel = function() {
	if (accelerationWatch !== null) {
		navigator.accelerometer.clearWatch(accelerationWatch);
		updateAcceleration({
			x : "",
			y : "",
			z : ""
		});
		accelerationWatch = null;
	} else {
		var options = {};
		options.frequency = 1000;
		accelerationWatch = navigator.accelerometer.watchAcceleration(
				updateAcceleration, function(ex) {
					alert("accel fail (" + ex.name + ": " + ex.message + ")");
				}, options);
	}
};

var preventBehavior = function(e) {
	e.preventDefault();
};

function dump_pic(data) {
	var viewport = document.getElementById('viewport');
	console.log(data);
	viewport.style.display = "";
	viewport.style.position = "absolute";
	viewport.style.top = "10px";
	viewport.style.left = "10px";
	document.getElementById("test_img").src = "data:image/jpeg;base64," + data;
}

function fail(msg) {
	alert(msg);
}

function show_pic() {
	navigator.camera.getPicture(dump_pic, fail, {
		quality : 50
	});
}

function close() {
	var viewport = document.getElementById('viewport');
	viewport.style.position = "relative";
	viewport.style.display = "none";
}

function contacts_success(contacts) {
	alert(contacts.length
			+ ' contacts returned.'
			+ (contacts[2] && contacts[2].name ? (' Third contact is ' + contacts[2].name.formatted)
					: ''));
}

function get_contacts() {
	var obj = new ContactFindOptions();
	obj.filter = "";
	obj.multiple = true;
	navigator.contacts.find(
			[ "displayName", "name" ], contacts_success,
			fail, obj);
}

function check_network() {
	var networkState = navigator.network.connection.type;

	var states = {};
	states[Connection.UNKNOWN]  = 'Unknown connection';
	states[Connection.ETHERNET] = 'Ethernet connection';
	states[Connection.WIFI]	 = 'WiFi connection';
	states[Connection.CELL_2G]  = 'Cell 2G connection';
	states[Connection.CELL_3G]  = 'Cell 3G connection';
	states[Connection.CELL_4G]  = 'Cell 4G connection';
	states[Connection.NONE]	 = 'No network connection';

	confirm('Connection type:\n ' + states[networkState]);
}

/**
 * Show the push notification popover and hide the main content.
 */
function show_pop_over()
{
	window.scroll(0,0);

	// Set the close icon size.
	var closeIconSize = screen.width * sPopoverCloseIconSize;
	document.getElementById('popOverIcon').style.height = closeIconSize + 'px';
	document.getElementById('popOverIcon').style.width = closeIconSize + 'px';

	// Show the popover.
	document.getElementById('popOverContent').style.opacity = 0;
	document.getElementById('popOverIcon').style.opacity = 0;
	document.getElementById('popOverContent').style.display = "inline";
	document.getElementById('popOverIcon').style.display = "inline";
	fade_in("popOverContent", sOpacityValue);
	fade_in("popOverIcon", sOpacityValue);

	// Hide the app main content.
	fade_out("stage", sOpacityValue);
}

/**
 * Hide the popover and show the main content.
 */
function hide_pop_over()
{
	fade_out('popOverContent', sOpacityValue);
	fade_out('popOverIcon', sOpacityValue);
	document.getElementById('stage').style.opacity = 0;
	document.getElementById('stage').style.display = "inline";
	fade_in("stage", sOpacityValue);
}

/**
 * Fade in an HTML object.
 *
 * @param {String} elementID Object's id.
 * @param {Number} value Opacity value that will be set to the given element.
 */
function fade_in(elementID, value)
{
	if (value <= 1.0)
	{
		document.getElementById(elementID).style.opacity = value;
		value = value + sOpacityValue;
		setTimeout("fade_in('" + elementID +"'," + value +")", sAnimationInterval);
	}
}

/**
 * Fade out an HTML object.
 *
 * @param {String} elementID Object's id.
 * @param {Number} value Opacity value that will be set to the given element.
 */
function fade_out(elementID, value)
{
	if (value >= 0.0)
	{
		document.getElementById(elementID).style.opacity = value;
		value = value - sOpacityValue;
		setTimeout("fade_out('" + elementID +"'," + value +")", sAnimationInterval);
	}
	else
	{
		document.getElementById(elementID).style.display = "none";
	}
}

function push_notification_button_clicked()
{
	if (!sIsApplicationRegistered)
	{
		register_push_notification();
	}
	else
	{
		unregister_push_notification();
	}
}

/**
 * Register application for receiving push notification.
 */
function register_push_notification()
{
	var typesArray = [PushNotificationManager.type.badge,
					  PushNotificationManager.type.sound,
					  PushNotificationManager.type.alert];
	navigator.pushNotification.types(typesArray);
	navigator.pushNotification.accountID(sAccountID);
	navigator.pushNotification.register(
		push_notification_did_registered,
		push_notification_failed_to_register);
}

/**
 * Unregister application for receiving push notification.
 */
function unregister_push_notification()
{
	navigator.pushNotification.unregister(null);
	document.getElementById('push_notification_button').innerHTML = sRegisterButtonText;
	sIsApplicationRegistered = false;
}

/**
 * Push notification successfully registration callback.
 *
 * @param {String} token Device token / application id received from registration.
 */
function push_notification_did_registered(token)
{
	// Change the push notification button text.
	document.getElementById('push_notification_button').innerHTML = sUnregisterButtonText;
	sIsApplicationRegistered = true;

	var popOverTextDiv = document.getElementById('popOverText');

	// Remove all child nodes contained by the popOverText node.
	while (popOverTextDiv.hasChildNodes())
	{
		popOverTextDiv.removeChild(popOverTextDiv.firstChild);
	}

	// Add a span node with the message.
	var spanTag = document.createElement('span');
	spanTag.innerHTML = sSuccessfullyRegistrationText;
	spanTag.className = "popOverMessage";
	popOverTextDiv.appendChild(spanTag);

	// Add a line breakers
	popOverTextDiv.appendChild(document.createElement('br'));
	popOverTextDiv.appendChild(document.createElement('br'));

	// Add a span node with the token.
	spanTag = document.createElement('span');
	spanTag.innerHTML = split_token(token);
	spanTag.className = "tokenText";
	popOverTextDiv.appendChild(spanTag);

	show_pop_over();

	// Add callback method for push notification.
	navigator.pushNotification.listener(new_push_notification);
}

/**
 * Push notification failure callback method.
 *
 * @param {Object} error Reason for failure.
 */
function push_notification_failed_to_register(error)
{
	var popOverTextDiv = document.getElementById('popOverText');

	// Remove all child nodes contained by the popOverText node.
	while (popOverTextDiv.hasChildNodes())
	{
		popOverTextDiv.removeChild(popOverTextDiv.firstChild);
	}

	// Add a span node with the message.
	var spanTag = document.createElement('span');
	spanTag.innerHTML = sFailureRegistrationText;
	spanTag.className = "popOverMessage";
	popOverTextDiv.appendChild(spanTag);

	// Add a line breakers
	popOverTextDiv.appendChild(document.createElement('br'));
	popOverTextDiv.appendChild(document.createElement('br'));

	// Add a span node with the error message.
	spanTag = document.createElement('span');
	spanTag.innerHTML = error.message;
	spanTag.className = "errorText";
	popOverTextDiv.appendChild(spanTag);

	show_pop_over();
	navigator.pushNotification.unregister(null);
}

/**
 * New push notification callback method.
 * Shows the popover.
 *
 * @param {Object} pushNotification The new notification.
 */
function new_push_notification(pushNotification)
{
	var popOverTextDiv = document.getElementById('popOverText');

	// Remove all child nodes contained by the popOverText node.
	while (popOverTextDiv.hasChildNodes())
	{
		popOverTextDiv.removeChild(popOverTextDiv.firstChild);
	}

	// Add a span node with the message.
	var spanTag = document.createElement('span');
	spanTag.innerHTML = sNewPushNotificationText;
	spanTag.className = "popOverMessage";
	popOverTextDiv.appendChild(spanTag);

	// Add line breakers.
	popOverTextDiv.appendChild(document.createElement('br'));
	popOverTextDiv.appendChild(document.createElement('br'));

	// Add a span node with the push notification message.
	if (pushNotification.message)
	{
		spanTag = document.createElement('span');
		spanTag.innerHTML = "Message: " + pushNotification.message;
		spanTag.className = "popOverMessage";
		popOverTextDiv.appendChild(spanTag);
	}

	// Add line breaker.
	popOverTextDiv.appendChild(document.createElement('br'));

	// Add a span node with the push notification sound.
	if (pushNotification.sound)
	{
		spanTag = document.createElement('span');
		spanTag.innerHTML = "Sound: " + pushNotification.sound;
		spanTag.className = "popOverMessage";
		popOverTextDiv.appendChild(spanTag);
	}

	// Add line breaker.
	popOverTextDiv.appendChild(document.createElement('br'));

	// Add a span node with the push notification icon badge.
	if (pushNotification.iconBadge != 0)
	{
		spanTag = document.createElement('span');
		spanTag.innerHTML = "Icon badge: " + pushNotification.iconBadge;
		spanTag.className = "popOverMessage";
		popOverTextDiv.appendChild(spanTag);
	}

	// Add line breaker.
	popOverTextDiv.appendChild(document.createElement('br'));

	show_pop_over();
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

function init() {
	// the next line makes it impossible to see Contacts on the HTC Evo since it
	// doesn't have a scroll button
	// document.addEventListener("touchmove", preventBehavior, false);
	document.addEventListener("deviceready", deviceInfo, true);
}
