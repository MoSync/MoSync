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


/*
 * @file mosync-bridge.js
 * @author Mikael Kindborg, Ali Sarrafi
 *
 * Library for JavaScript to C++ communication on the MoSync platform.
 */

/**
 * Create a global instance of the library.
 */
var mosync = (function()
{
	// The main object of the library.
	var mosync = {};

	// This variable keeps track of the time out for our
	// HTML "Toasts", so that they display sequentially.
	var HTMLToastTimeOut = 0;

	// Detect platform.

	mosync.isAndroid =
		navigator.userAgent.indexOf("Android") != -1;

	mosync.isIOS =
		(navigator.userAgent.indexOf("iPod") != -1) ||
		(navigator.userAgent.indexOf("iPhone") != -1) ||
		(navigator.userAgent.indexOf("iPad") != -1);

	mosync.isWindowsPhone =
		navigator.userAgent.indexOf("Windows Phone OS") != -1;

	// Application functions.

	mosync.app = {};

	/**
	 * Exit the application.
	 * Supported on Android. Not supported on iOS.
	 */
	mosync.app.exit = function()
	{
		mosync.bridge.send(["MoSync", "ExitApplication"]);
	}

	/**
	 * Send application to background.
	 */
	mosync.app.sendToBackground = function()
	{
		mosync.bridge.send(["MoSync", "SendToBackground"]);
	}

	// Alerts and logging.

	mosync.notification = {};

	/**
	 * Displays a "toast" message box using HTML,
	 * similar to a Toast on Android. Can be used
	 * as a replacement for alert on platforms that
	 * do not support it.
	 *
	 * @param message String with message to show.
	 * @param durationInMilliseconds Optional parameter
	 * that specifies the time the message will be shown,
	 * defaults to 3000 (three seconds) if omitted.
	 *
	 * Note: This function works less well together with
	 * JavaScript libraries that manipulate the DOM at
	 * the same time.
	 */
	mosync.notification.HTMLToast = function(message, durationInMilliseconds)
	{
		var toast = document.createElement("div");
		var width = window.innerWidth - 40;
		toast.style.width = width + "px";
		toast.style.position = "absolute";
		toast.style.left = "10px";
		toast.style.top = "10px";
		toast.style.padding = "10px";
		toast.style.borderRadius = '8px';
		toast.style.MozBorderRadius = '8px';
		toast.style.WebkitBorderRadius = '8px';
		toast.style.background = "#FFFFFF";
		toast.style.border = "1px solid #000000";
		toast.style.fontFamily = "sans-serif";
		toast.style.fontSize = "18px";
		toast.style.fontWeight = "bold";
		toast.style.color = "#000000";
		toast.style.visibility = "visible";
		toast.style.zIndex = "10000";
		toast.innerHTML = message;

		// Default value of toast display time.
		var duration = 3000;
		if (durationInMilliseconds)
		{
			duration = durationInMilliseconds;
		}

		// Time duration until time to display this toast.
		var timeToDisplayToast = 0;
		var timeNow = new Date().getTime();
		var timeToNextTimeout = HTMLToastTimeOut - timeNow;
		if (timeToNextTimeout > 0)
		{
			timeToDisplayToast = timeToNextTimeout;
		}

		// Update time point for accumulated time out.
		HTMLToastTimeOut = timeNow + timeToDisplayToast + duration;

		setTimeout(
			function()
			{
				document.body.appendChild(toast);
				setTimeout(
					function()
					{
						document.body.removeChild(toast);
					},
					duration);
			},
			timeToDisplayToast);
	};

	/**
	 * Displays a native message box.
	 *
	 * @param title String with message box title.
	 * @param message String with message to show.
	 */
	mosync.notification.messageBox = function(title, message)
	{
		mosync.bridge.sendJSON({
			messageName:"PhoneGap",
			service:"mosync",
			action:"mosync.notification.messageBox",
			title:title,
			message:message});
	};

	// console.log does not work on WP7.
	if (typeof console === "undefined")
	{
		console = {}
	}
	if (typeof console.log === "undefined")
	{
		// TODO: Send console output somewhere.
		console.log = function(s) {};
	}

	// alert does not work on WP7, replace with
	// call to maMessageBox.
	if (mosync.isWindowsPhone)
	{
		window.alert = function(message) {
			mosync.notification.messageBox("Message", message);
		};
	}

	// The encoder submodule.

	mosync.encoder = (function()
	{
		var encoder = {};
		var firstChar = 33;
		var lastChar = 126;
		var base = lastChar - firstChar;

		encoder.itox = function(i)
		{
			var n = i;
			var digits = [];

			while (n > base)
			{
				digits.push((n % base) + firstChar);
				n = (n / base) >> 0;
			}
			digits.push(n + firstChar);

			return String.fromCharCode.apply(null, digits);
		};

		encoder.xtoi = function(s)
		{
			var n = 0;
			var length = s.length;

			for (var i = 0; i < length; ++i)
			{
				n += Math.pow(base, i) * (s.charCodeAt(i) - firstChar);
			}

			return n;
		};

		// Thanks to: http://jsfromhell.com/geral/utf-8
		// Author: Jonas Raoni Soares Silva
		// Note: Function not used.
		encoder.encodeUTF8 = function(s){
			for(var c, i = -1, l = (s = s.split("")).length, o = String.fromCharCode; ++i < l;
				s[i] = (c = s[i].charCodeAt(0)) >= 127 ? o(0xc0 | (c >>> 6)) + o(0x80 | (c & 0x3f)) : s[i]
			);
			return s.join("");
		};

		// Thanks to: http://jsfromhell.com/geral/utf-8
		// Author: Jonas Raoni Soares Silva
		// Note: Function not used.
		encoder.decodeUTF8 = function(s){
			for(var a, b, i = -1, l = (s = s.split("")).length, o = String.fromCharCode, c = "charCodeAt"; ++i < l;
				((a = s[i][c](0)) & 0x80) &&
				(s[i] = (a & 0xfc) == 0xc0 && ((b = s[i + 1][c](0)) & 0xc0) == 0x80 ?
				o(((a & 0x03) << 6) + (b & 0x3f)) : o(128), s[++i] = "")
			);
			return s.join("");
		};

		/**
		 * @return The length in bytes of the string encoded
		 * as UTF8.
		 */
		encoder.lengthAsUTF8 = function(s)
		{
			var length = 0;
			for (var i = 0; i < s.length; ++i)
			{
				var c = s.charCodeAt(i);

				if (c < 128)
				{
					length += 1;
				}
				else if ((c > 127) && (c < 2048))
				{
					length += 2;
				}
				else
				{
					length += 3;
				}
			}
			return length;
		};

		/**
		 * Encode a string with a length value followed by string data.
		 */
		encoder.encodeString = function(s)
		{
			// On all current platforms (Android, iOS, Windows Phone)
			// strings are converted to UTF8 strings when passed from JS
			// to the underlying layer (Java, Objective-C, C#). Therefore
			// we need to calculate the length of the UTF8 encoded string
			// data and use that as the length of the message string.
			var length = encoder.lengthAsUTF8(s);
			var encodedString = "";
			return encodedString.concat(encoder.itox(length), " ", s, " ");
		};

		return encoder;
	}
	)();

	// The bridge submodule.

	mosync.bridge = (function()
	{
		var bridge = {};
		var callbackTable = {};
		var callbackIdCounter = 0;
		var messageQueue = [];
		var messageSender = null;
		var messageQueueJSON = [];
		var messageSenderJSON = null;
		var rawMessageQueue = [];

		/**
		 * Send message strings to C++. If a callback function is
		 * supplied, a callbackId parameter will be added to
		 * the message, this id can be used when sending a reply
		 * back to JavaScript from C++.
		 *
		 * This method queues messages and can be called multiple
		 * times in sqeuential JS code. When execution of sequential
		 * code is done, a timer will get activated and send all messages
		 * in the queue in one chunk. This enhances performance of
		 * message sending.
		 *
		 * @param message An array of message strings.
		 *
		 * @param callbackFun An optional function to receive the
		 * result of the message asynchronosly. The id of the
		 * callback function is added after the strings in the
		 * messageStrings array.
		 */
		bridge.send = function(messageStrings, callbackFun)
		{
			var callbackId = null;

			// If there is a callback function supplied, create
			// a callbackId and add it to the callback table.
			if (callbackFun)
			{
				callbackIdCounter = callbackIdCounter + 1;
				callbackTable[callbackIdCounter] = callbackFun;
				callbackId = callbackIdCounter;
			}

			// Add message strings to queue.

			var length = messageStrings.length;
			for (var i = 0; i < length; ++i)
			{
				messageQueue.push(messageStrings[i]);
			}

			// If we have a callbackId, push that too, as a string value.
			if (null != callbackId)
			{
				messageQueue.push(String(callbackId));
			}


			// Start timer for sender function.
			// This will get called once sequential
			// execution of JS code is done.
			if (null == messageSender)
			{
				messageSender = setTimeout(function()
				{
					messageSender = null;
					bridge.sendAll();
				},
				1);
			}
		};

		/**
		 * Send a JSON message to C++. If a callback function is
		 * supplied, a callbackId parameter will be added to
		 * the message, this id can be used when sending a reply
		 * back to JavaScript from C++.
		 *
		 * This method queues a message and can be called multiple
		 * times in sqeuential JS code. When execution of sequential
		 * code is done, a timer will get activated and send all messages
		 * in the queue in one chunk. This enhances performance of
		 * message sending.
		 *
		 * @param message A dictionary with the message parameters.
		 * The parameter "messageName" specifyes the name of the
		 * message selector (the "command name") and must always be
		 * included.
		 *
		 * @param callbackFun An optional function to receive the
		 * result of the message asynchronosly.
		 */
		bridge.sendJSON = function(message, callbackFun)
		{
			// If there is a callback function supplied, create
			// a callbackId and add it to the callback table.
			if (callbackFun)
			{
				callbackIdCounter = callbackIdCounter + 1;
				callbackTable[callbackIdCounter] = callbackFun;
				message["callbackId"] = callbackIdCounter;
			}

			// Add message to queue.
			messageQueueJSON.push(message);

			// Start timer for sender function.
			// This will get called once sequential
			// execution of JS code is done.
			if (null == messageSenderJSON)
			{
				messageSenderJSON = setTimeout(function()
				{
					messageSenderJSON = null;
					bridge.sendAllJSON();
				},
				1);
			}
		};

		/**
		 * Send all queued message strings.
		 */
		bridge.sendAll = function()
		{
			// Check that messageQueue is not empty!
			var length = messageQueue.length;
			if (length > 0)
			{
				// Add the "ms:" token to the beginning of the data
				// to signify that this as a message stream. This is
				// used by the C++ message parser to handle different
				// types of message formats.
				var data = "ms:";
				for (var i = 0; i < length; ++i)
				{
					data = data.concat(mosync.encoder.encodeString(String(messageQueue[i])));
				}

				messageQueue = [];
				bridge.sendRaw(data);
			}
		};

		/**
		 * Send all queued JSON messages.
		 */
		bridge.sendAllJSON = function()
		{
			// Check that messageQueue is not empty!
			if (messageQueueJSON.length > 0)
			{
				// Add the "ma:" token to the beginning of the data
				// to signify that this as a message array. This is
				// used by the C++ message parser to handle different
				// types of message formats.
				var data = "ma:" + JSON.stringify(messageQueueJSON);
				messageQueueJSON = [];
				bridge.sendRaw(data);
			}
		};

		/**
		 * Send raw data to the C++ side.
		 */
		bridge.sendRaw = function(data)
		{
			if (mosync.isAndroid)
			{
				prompt(data, "");
			}
			else if (mosync.isIOS)
			{
				rawMessageQueue.push(data);
				window.location = "mosync://DataAvailable";
			}
			else if (mosync.isWindowsPhone)
			{
				window.external.notify(data);
			}
			else
			{
				alert("mosync.bridge.sendRaw: unknown platform");
			}
		};

		/**
		 * Called from iOS runtime to get message.
		 */
		bridge.getMessageData = function()
		{
			if (rawMessageQueue.length == 0)
			{
				// Return an empty string so the iOS runtime
				// knows we don't have any message.
				return "";
			}
			var message = rawMessageQueue.pop();
			return message;
		};

		/**
		 * This function is meant to be used to call back from C++ to
		 * JavaScript. The function takes a variable number of parameters.
		 *
		 * For example, to return the value 'Hello World' to the callback
		 * with id 82, you can use this code in a WebAppMoblet:
		 *
		 *   callJS("mosync.bridge.reply(82, 'Hello World')");
		 *
		 * You can obtain the callbackId from the C++ WebViewMessage
		 * object, if you use that class to parse the message.
		 *
		 * @param callBackId The first parameter is the id of the
		 * callback function. Remaning parameters are applied to the
		 * function refered to by the callbackId.
		 */
		bridge.reply = function(callbackId)
		{
			var callbackFun = callbackTable[callbackId];
			if (callbackFun)
			{
				// Remove the first param, the callbackId.
				var args = Array.prototype.slice.call(arguments);
				args.shift();

				// Call the function.
				callbackFun.apply(null, args);
			}
		};

		return bridge;
	})();

	// Return the library object.
	return mosync;
})();

// Send OpenWormhole message to C++ when document is loaded.
document.addEventListener(
	"DOMContentLoaded",
	function()
	{
		// This signals that the document is loaded and Wormhole
		// is ready be initialized.
		mosync.bridge.send(["MoSync", "OpenWormhole"]);
	},
	false);
