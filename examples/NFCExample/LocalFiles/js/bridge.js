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
 * @file bridge.js
 * @author Mikael Kindborg
 *
 * Library for JavaScript to C++ communication on the MoSync platform.
 */

/**
 * Create a global instance of the library.
 */
var bridge = function()
{
	var self = {};

	// The messagehandler submodule.

	self.messagehandler = function()
	{
		var self = {};
		var callbackTable = {};
		var callbackIdCounter = 0;
		var messageQueue = [];

		/**
		 * Utility function that creates a mosync:// url
		 * that contains the supplied parameters. Format:
		 * mosync://messageName?param1=value1&param2=value2...
		 *
		 * @param message A dictionary with the message parameters.
		 * The parameter "messageName" specifyes the name of the
		 * message selector (the "command name") and must always be
		 * included.
		 *
		 * @return A string that includes the message parameters
		 * formatted as a mosync:// url. Returns null on error.
		 */
		self.createMessageUrl = function(message)
		{
			// The message must contain the parameter messageName.
			var messageName = message["messageName"];
			if (null == messageName)
			{
				return null;
			}

			var messageUrl = "mosync://" + messageName;
			var addParamStartSeparator = true;

			for (var key in message)
			{
				// Do not add the messageName parameter.
				if ("messageName" == key)
				{
					continue;
				}

				if (addParamStartSeparator)
				{
					// First time, we should add the question mark that
					// indicates start of the parameter list.
					addParamStartSeparator = false;
					messageUrl += "?";
				}
				else
				{
					// Otherwise, we add the paramater separator ampersand.
					messageUrl += "&";
				}

				// Add parameter.
				messageUrl += escape(key) + "=" + escape(message[key]);
			}

			return messageUrl;
		};

		/**
		 * Send a message to C++. If a callback function is
		 * supplied, a callbackId parameter will be added to
		 * the message, this id can be used when sending a reply
		 * back to JavaScript from C++.
		 *
		 * @param message A dictionary with the message parameters.
		 * The parameter "messageName" specifyes the name of the
		 * message selector (the "command name") and must always be
		 * included.
		 *
		 * @param callbackFun An optional function to receive the
		 * result of the message asynchronosly. Set to null if no
		 * callback is used.
		 */
		self.send = function(message, callbackFun)
		{
			// If there is a callback function supplied, create
			// a callbackId and add it to the callback table.
			if (null != callbackFun)
			{
				callbackIdCounter = callbackIdCounter + 1;
				callbackTable[callbackIdCounter] = callbackFun;
				message["callbackId"] = callbackIdCounter;
			}

			// Create message url and invoke the url.
			var messageUrl = self.createMessageUrl(message);

			// Add message to queue.
			messageQueue.push(messageUrl);
			if (1 == messageQueue.length)
			{
				// No messages waiting, send it.
				setTimeout(function() {
					document.location = messageUrl; },
					1);
			}
		};

		/**
		 * Call this function from C++ to tell the message
		 * handler that the last message sent has been processed.
		 */
		self.processedMessage = function()
		{
			// Remove first message.
			if (messageQueue.length > 0)
			{
				messageQueue.shift();
			}

			// If there are more messages, send the next
			// message in the queue.
			if (messageQueue.length > 0)
			{
				document.location = messageQueue[0];
			}
		};

		/**
		 * This function is meant to be used to call back from C++ to
		 * JavaScript. The function takes a variable number of parameters.
		 *
		 * For example, to return the value 'Hello World' to the callback
		 * with id 82, you can use this code in a WebAppMoblet:
		 *
		 *   callJS("bridge.messagehandler.reply(82, 'Hello World')");
		 *
		 * You can obtain the callbackId from the C++ WebViewMessage
		 * object, if you use that class to parse the message.
		 *
		 * @param callBackId The first parameter is the id of the
		 * callback function. Remaning parameters are applied to the
		 * function refered to by the callbackId.
		 */
		self.reply = function(callbackId)
		{
			var callbackFun = callbackTable[callbackId];
			if (undefined != callbackFun)
			{
				// Remove the first param, the callbackId.
				var args = Array.prototype.slice.call(arguments);
				args.shift();

				// Call the function.
				callbackFun.apply(null, args);
			}
		};

		return self;
	}();

	// Return the library object.
	return self;
}();
