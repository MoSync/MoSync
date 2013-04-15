/*
Copyright (C) 2011-2013 MoSync AB

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

	// Logging support.

	/**
	 * Write log output using maWriteLog in the MoSync C++ API.
	 * @param s Log message string.
	 */
	mosync.log = function(s)
	{
		mosync.bridge.send(["MoSync", "SysLog", s]);
	};

	// Application functions.

	mosync.app = {};

	/**
	 * Exit the application.
	 * Supported on Android. Not supported on iOS.
	 */
	mosync.app.exit = function()
	{
		mosync.bridge.send(["MoSync", "ExitApplication"]);
	};

	/**
	 * Send application to background.
	 */
	mosync.app.sendToBackground = function()
	{
		mosync.bridge.send(["MoSync", "SendToBackground"]);
	};

	/**
	 * The device is in portrait mode, with the device held upright and the home button on
	 * the bottom.
	 */
	mosync.SCREEN_ORIENTATION_PORTRAIT_UP = 0x000001;

	/**
	 * The device is in portrait mode but upside down, with the device held upright and the
	 * home button at the top.
	 * Note: not available on Windows Phone.
	 */
	mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN = 0x000002;

	/**
	 * The device is in portrait mode, based on sensor. Depending on user rotation,
	 * #mosync.MA_SCREEN_ORIENTATION_PORTRAIT_UP or #mosync.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN will be used.
	 * Value equals: #mosync.MA_SCREEN_ORIENTATION_PORTRAIT_UP | #mosync.MA_SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN.
	 */
	mosync.SCREEN_ORIENTATION_PORTRAIT = 0x000003;

	/**
	 * The device is in landscape mode, with the device held upright and the home button on
	 * the left side.
	 */
	mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT = 0x000004;

	/**
	 * The device is in landscape mode, with the device held upright and the home button on
	 * the right side.
	 */
	mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT = 0x000008;

	/**
	 * The device is in landscape mode, based on sensor. Depending on user rotation,
	 * #mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT or #mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT will be used.
	 */
	mosync.SCREEN_ORIENTATION_LANDSCAPE = 0x00000C;

	/**
	 * Screen orientation is based on device sensor.
	 * Value equals: #mosync.SCREEN_ORIENTATION_PORTRAIT| #mosync.SCREEN_ORIENTATION_LANDSCAPE.
	 */
	mosync.SCREEN_ORIENTATION_DYNAMIC = 0x00000F;

	/**
	 * Set the screen orientation of the device.
	 * @deprecated Use #mosync.app.screenSetSupportedOrientations().
	 *
	 * @param orientation The desired screen orientation.
	 * Can be one of the constants:
	 *   mosync.SCREEN_ORIENTATION_DYNAMIC
	 *   mosync.SCREEN_ORIENTATION_PORTRAIT
	 *   mosync.SCREEN_ORIENTATION_LANDSCAPE
	 *
	 * Example:
	 *   mosync.app.screenSetOrientation(mosync.SCREEN_ORIENTATION_DYNAMIC);
	 */
	mosync.app.screenSetOrientation = function(orientation)
	{
		mosync.app.screenSetSupportedOrientations(orientation);
	};

	/**
	 * Set the supported screen orientation of the device.
	 *
	 * @param orientation The desired screen orientation.
	 * The bitmask can be created using the following constants:
	 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UP
	 *   mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
	 *   mosync.SCREEN_ORIENTATION_PORTRAIT
	 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
	 *   mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
	 *   mosync.SCREEN_ORIENTATION_LANDSCAPE
	 *   mosync.SCREEN_ORIENTATION_DYNAMIC
	 *
	 * Example:
	 *   mosync.app.screenSetSupportedOrientations(mosync.SCREEN_ORIENTATION_DYNAMIC);
	 */
	mosync.app.screenSetSupportedOrientations = function(orientation)
	{
		mosync.bridge.send(["MoSyncOrientation", "ScreenSetOrientation", orientation]);
	};

	/**
	 * An array containing all callback functions that are registered for getting
	 * supported orientations.
	 * @private
	 */
	var orientationCallBackArray = [];

	/**
	 * Get the supported screen orientations of the device.
	 *
	 * @param callback
	 *            Function to call with supported orientations flag as param.
	 *            The flag is created using the following constants:
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT_UP
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE
	 *   			mosync.SCREEN_ORIENTATION_DYNAMIC
	 *
	 * Example:
	 *   mosync.app.screenGetSupportedOrientations(
	 *		function(supportedOrientations)
	 *		{
	 *			mosync.log("supported orietations bitmask is " + supportedOrientations);
	 *		});
	 */
	mosync.app.screenGetSupportedOrientations = function(callback)
	{
		mosync.bridge.send(["MoSyncOrientation", "ScreenGetOrientation"]);
		orientationCallBackArray.push(callback);
	};

	/**
	 * Get the current screen orientation of the device.
	 *
	 * @param callback
	 *            Function to call with current orientation flag as param.
	 *            The flag can be one of the following constants:
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT_UP
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
	 *
	 * Example:
	 *   mosync.app.screenGetCurrentOrientation(
	 *		function(currentOrientation)
	 *		{
	 *			mosync.log("current orientation is " + currentOrientation);
	 *		});
	 */
	mosync.app.screenGetCurrentOrientation = function(callback)
	{
		mosync.bridge.send(["MoSyncOrientation", "ScreenGetCurrentOrientation"]);
		orientationCallBackArray.push(callback);
	};

	/**
	* Callback function for getting the supported orientations.
	* Registered functions will be called.
	* @param supportedOrientations
	*            Supported screen orientations of the device.
	* @private
	*/
	mosync.app.fireSupportedOrientations = function(orientation)
	{
		var callback = orientationCallBackArray.shift();
		callback(orientation);
	};

	/**
	* Callback function for getting the current orientation.
	* Registered functions will be called.
	* @param currentOrientation
	*            Supported screen orientations of the device.
	* @private
	*/
	mosync.app.fireCurrentOrientation = function(currentOrientation)
	{
		var callback = orientationCallBackArray.shift();
		callback(currentOrientation);
	};

	/**
	 * An array containing all callback functions that are registered to be notified
	 * when the orientation did change.
	 * @private
	 */
	var orientationDidChangeArray = [];

	/**
	 * Register a function to be called when the orientation has changed.
	 *
	 * @param callback
	 *            Function to call with current orientation flag as param when the
	 *            orientation has changed.
	 *            The flag can be one of the following constants:
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT_UP
	 *   			mosync.SCREEN_ORIENTATION_PORTRAIT_UPSIDE_DOWN
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE_LEFT
	 *   			mosync.SCREEN_ORIENTATION_LANDSCAPE_RIGHT
	 *
	 * Example:
	 *   mosync.app.registerOrientationDidChange(
	 *		function(currentOrientation)
	 *		{
	 *			mosync.log("orientation changed to " + currentOrientation);
	 *		});
	 */
	mosync.app.registerOrientationDidChange = function(callback)
	{
		orientationDidChangeArray.push(callback);
		if (orientationDidChangeArray.length == 1)
		{
			mosync.bridge.send(["MoSyncOrientation", "RegisterOrientationDidChange"]);
		}
	};

	/**
	 * Unregister a function from calling when the orientation has changed.
	 */
	mosync.app.unregisterOrientationDidChange = function(callback)
	{
		var index = orientationDidChangeArray.indexOf(callback);
		orientationDidChangeArray.splice(index, 1);
		if (orientationDidChangeArray.length == 0)
		{
			mosync.bridge.send(["MoSyncOrientation", "UnregisterOrientationDidChange"]);
		}
	};

	/**
	* Callback function for notifying registered functions when orientation did change.
	* @param newOrientation
	*            Current screen orientation of the device.
	* @private
	*/
	mosync.app.orientationDidChange = function(newOrientation)
	{
		for(var i = 0; i < orientationDidChangeArray.length; i++)
		{
			var callback = orientationDidChangeArray[i];
			callback(newOrientation);
		}
	};

	/**
	 * An array containing all callback functions that are registered to be notified
	 * before the orientation is changed.
	 * @private
	 */
	var orientationWillChangeArray = [];

	/**
	 * Register a function to be called before the orientation is changed.
	 * Only on iOS platform.
	 *
	 * @param callback
	 *            Function to call before the orientation is changed.
	 *
	 * Example:
	 *   mosync.app.registerOrientationWillChange(
	 *		function()
	 *		{
	 *			mosync.log("orientation will be changed");
	 *		});
	 */
	mosync.app.registerOrientationWillChange = function(callback)
	{
		orientationWillChangeArray.push(callback);
		if (orientationWillChangeArray.length == 1)
		{
			mosync.bridge.send(["MoSyncOrientation", "RegisterOrientationWillChange"]);
		}
	};

	/**
	 * Unregister a function to be called before the orientation is changed.
	 * Only on iOS platform.
	 */
	mosync.app.unregisterOrientationWillChange = function(callback)
	{
		var index = orientationWillChangeArray.indexOf(callback);
		orientationWillChangeArray.splice(index, 1);
		if (orientationWillChangeArray.length == 0)
		{
			mosync.bridge.send(["MoSyncOrientation", "UnregisterOrientationWillChange"]);
		}
	};

	/**
	* Callback function for notifying registered functions before the orientation is changed.
	* @private
	*/
	mosync.app.orientationWillChange = function()
	{
		for(var i = 0; i < orientationWillChangeArray.length; i++)
		{
			var callback = orientationWillChangeArray[i];
			callback();
		}
	};

	/**
	 * Opens a url in an external browser. Supported url
	 * schemas are are "http:" to open a web page and
	 * "tel:" to dial a number. See examples below.
	 *
	 * @param url The url to be opened.
	 *
	 * Example of opening a web page in an external browser:
	 *   mosync.app.openExternalURL("http://maps.google.com/");
	 *
	 * Example of dialing a number:
	 *   mosync.app.openExternalURL("tel:046850510300");
	 *
	 * Note that your app needs phone permissions to be set for
	 * dialing a phone number. This is set in the MoSync SDK.
	 * Note that MoSync Reload does not have phone permissions set.
	 *
	 * This function calls the MoSync syscall maPlatformRequest.
	 */
	mosync.app.openExternalURL = function(url)
	{
		mosync.bridge.send(["MoSync", "OpenExternalURL", url]);
	};

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
	// Define console if undefined.
	if (typeof console === "undefined")
	{
		console = {}
	}

	// Define console.log if undefined.
	if (typeof console.log === "undefined")
	{
		console.log = function(s)
		{
			mosync.log(s);
		};
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
		 * Example: mosync.bridge.send(["Custom", "Vibrate", "500"]);
		 *
		 * See this page for a tutorial: http://www.mosync.com/documentation/manualpages/how-communicate-between-javascript-and-c-mosync
		 *
		 * The project template "HTML5/JS/C++ Hybrid Project" is a
		 * good starting point for leaning how to add custom C++ code
		 * to your JavaScript application. Look at the files index.html
		 * and main.cpp in the project generated from the template.
		 *
		 * This method queues messages and can be called multiple
		 * times in sequential JS code. When the sequential code executes,
		 * a timer will be activated and wil send all messages
		 * in the queue in one chunk. This enhances performance of
		 * message sending.
		 *
		 * Note: the "close" message is deprecated. To close the application, use "mosync.app.exit" instead.
		 * \code
		 *    //Deprecated method:
		 *    mosync.bridge.send(["close"]);
		 *
		 *    //Preferred method:
		 *    mosync.app.exit();
		 *
		 * \endcode
		 *
		 * @param message An array of message strings.
		 *
		 * @param callbackFun An optional function to receive the
		 * result of the message asynchronosly. The ID of the
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
		 * times in sequential JS code. When execution of sequential
		 * code is done, a timer will is activated and sends all messages
		 * in the queue in one chunk. This enhances performance of
		 * message sending.
		 *
		 * @param message A dictionary with the message parameters.
		 * The parameter "messageName" specifies the name of the
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
		 * with ID 82, you can use this code in a WebAppMoblet:
		 *
		 *   callJS("mosync.bridge.reply(82, 'Hello World')");
		 *
		 * You can obtain the callbackId from the C++ WebViewMessage
		 * object, if you use that class to parse the message.
		 *
		 * @param callBackId The first parameter is the ID of the
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
