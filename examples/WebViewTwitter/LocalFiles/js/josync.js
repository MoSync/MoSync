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
 * @file josync.js
 * @author Mikael Kindborg
 *
 * Library for JavaScript to C++ communication on the MoSync platform.
 */

/**
 * Create a global instance of the josync library.
 */
var josync = function()
{
	var jo = {};

	// The messagehandler submodule.

	jo.messagehandler = function()
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
		 * Send a message to C++.
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
		 * This function is called from C++ to tell the message
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
		 * with id 47, you can use this code in a WebAppMoblet:
		 *
		 *   callJS("jo.messagehandler.reply(47, 'Hello World')");
		 *
		 * Or use the reply method in class WebViewMessage, which
		 * handles the callbackId automatically:
		 *
		 *   webViewMessage.reply("Hello World");
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

		/**
		 * Function to be called from C++ to handle
		 * MoSync events. This function takes a variable
		 * number of arguments depending on the type of event.
		 */
		self.processEvent = function(eventType)
		{
			// Get registered listeners for the eventType.

			// Call functions.
		};

		self.registerEventListener = function(eventType, callbackFun)
		{
			var callbacks = self.eventCallbackTable[eventType];
			if (undefined == callbacks)
			{
				self.eventCallbackTable[eventType] = [callbackFun];
			}
			else
			{
				callbacks.push(callbackFun);
			}
		};

		self.unregisterEventListener = function(eventType, callbackFun)
		{
			var callbacks = self.eventCallbackTable[eventType];
			if (undefined != callbacks)
			{
				var index = callbacks.indexOf(callbackFun);
				if (-1 != index)
				{
					// Remove element.
					callbacks.splice(index, 1);
				}
			}
		};

		return self;
	}();

	// The file submodule.

	jo.file = {};

	jo.file.getLocalPath = function(callbackFun)
	{
		jo.messagehandler.send(
			{ "messageName": "josync.file.getLocalPath" },
			callbackFun);
	};

	jo.file.read = function(filePath, callbackFun)
	{
		jo.messagehandler.send(
			{ "messageName": "josync.file.read",
			  "filePath": filePath },
			callbackFun);
	};

	jo.file.write = function(filePath, data, callbackFun)
	{
		jo.messagehandler.send(
			{ "messageName": "josync.file.write",
			  "filePath": filePath,
			  "data": data },
			callbackFun);
	};

	// Top level functions.

	jo.log = function(message)
	{
		jo.messagehandler.send(
			{ "messageName": "josync.log",
			  "message": message },
			null);
	};

	// Return the library object.
	return jo;
}();
