/*
Copyright (C) 2012-2013 MoSync AB

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
 * @file mosync.nativeui.js
 * @author Ali Sarrafi
 *
 * Library for supporting Native widgets in Javascript and Web applications.
 * Provides support for designing UI both programatically and declaratively.
 * Should be used together with mosync-resource.js
 */

/**
 * @namespace The mosync.nativeui module
 */
mosync.nativeui = {};

/**
 * A hash containing all callback functions that are registered for getting
 * response of creating a widget
 * @private
 */
mosync.nativeui.callBackTable = {};

/**
 * List of registered callback functions for WidgetEvents
 * @private
 */
mosync.nativeui.eventCallBackTable = {};

/**
 * used to generate IDs for widgets that do not have one
 * @private
 */
mosync.nativeui.widgetCounter = 0;

/**
 * A unique string ID for the main webview widget.
 */
mosync.nativeui.mainWebViewId = "mosync.nativeui.mainWebViewId";

/**
 * Creates a mosync.nativeui widget and registers its callback for return of the
 * handle. Used internally: use mosync.nativeui.create in your code.
 *
 * @param widgetType
 *            A string that includes type of the widget defined in MoSync API
 *            reference
 * @param widgetID
 *            An ID set by the user for high level access to the widget
 * @param successCallback
 *            The function that would be called when the widget is created
 * @param errorCallback
 *            The function that would be called if an error happens
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetCreate = function(
		widgetType,
		widgetID,
		successCallback,
		errorCallback,
		processedCallback,
		properties)
{

	callbackID = "create" + widgetID;
	var message = ["NativeUI","maWidgetCreate", widgetType, widgetID,
			callbackID ];
	if (properties) {
		var params = [];
		var ii = 0;
		for ( var key in properties) {
			params[ii] = String(mosync.nativeui.getNativeAttrName(key));
			ii++;
			params[ii] = String(mosync.nativeui
					.getNativeAttrValue(properties[key]));
			ii++;
		}
		message.push(String(params.length));
		message = message.concat(params);
	} else {
		message.push("0");
	}

	mosync.bridge.send(message, processedCallback);
	// TODO: Micke says: Should we move this code to before send?
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Destroys a widget
 *
 * @param widgetID
 *            ID for the widget in question
 * @param processedCallback
 *            optional call back for knowing that the message is processed. See
 *            mosync.nativeui.getElementById for getting handles
 * @private
 */
mosync.nativeui.maWidgetDestroy = function(widgetID, successCallback,
		errorCallback, processedCallback) {
	callbackID = "destroy" + widgetID;
	var mosyncWidgetHandle = mosync.nativeui.widgetIDList[widgetID];
	mosync.bridge.send([ "NativeUI", "maWidgetDestroy",
			mosyncWidgetHandle + "", callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Adds a widget to the given parent as a child. Letting the parent widget
 * layout the child.
 *
 * @param widgetID
 *            ID of the widget assigned by the user
 * @param childID
 *            ID of the widget to be added as a child
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetAddChild = function(widgetID, childID, successCallback,
		errorCallback, processedCallback) {
	callbackID = "addChild" + widgetID + childID;
	var mosyncWidgetHandle = mosync.nativeui.widgetIDList[widgetID];
	var mosyncChildHandle = mosync.nativeui.widgetIDList[childID];
	mosync.bridge.send([ "NativeUI", "maWidgetAddChild",
			mosyncWidgetHandle + "", mosyncChildHandle + "", callbackID ],
			processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Inserts a widget to the given parent as a child at an index. Letting the
 * parent widget layout the child.
 *
 * @param widgetID
 *            ID of the parent widget
 * @param childID
 *            ID of the child widget
 * @param index
 *            place for the child widget, -1 means last
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetInsertChild = function(widgetID, childID, index,
		successCallback, errorCallback, processedCallback) {
	callbackID = "insertChild" + widgetID;
	var mosyncWidgetHandle = mosync.nativeui.widgetIDList[widgetID];
	var mosyncChildHandle = mosync.nativeui.widgetIDList[childID];
	mosync.bridge.send(
			[ "NativeUI", "maWidgetInsertChild", mosyncWidgetHandle + "",
					mosyncChildHandle + "", index, callbackID ],
			processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Removes a child widget from its parent (but does not destroy it). Removing a
 * currently visible top-level widget causes the MoSync view to become visible.
 *
 * @param childID
 *            ID for the child widget
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetRemoveChild = function(childID, successCallback,
		errorCallback, processedCallback) {
	callbackID = "removeChild" + childID;
	var mosyncChildHandle = mosync.nativeui.widgetIDList[childID];
	mosync.bridge.send([ "NativeUI", "maWidgetRemoveChild",
			mosyncChildHandle + "", callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Shows a screen. If native UI hasn't been initialized, it is also initialized
 * and disables regular MoSync drawing.
 *
 * @param childID
 *            Id of the screen that should be shown
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetScreenShow = function(screenID, successCallback,
		errorCallback, processedCallback) {
	callbackID = "screenShow" + screenID;
	var mosyncScreenHandle = mosync.nativeui.widgetIDList[screenID];
	mosync.bridge.send([ "NativeUI", "maWidgetScreenShow",
			mosyncScreenHandle + "", callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Shows a modalDialog.
 *
 * @param childID
 *            ID of the screen that should be shown
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetModalDialogShow = function(dialogID, successCallback,
		errorCallback, processedCallback) {
	callbackID = "dialogShow" + dialogID;
	var mosyncDialogHandle = mosync.nativeui.widgetIDList[dialogID];
	mosync.bridge.send([ "NativeUI", "maWidgetModalDialogShow",
			mosyncDialogHandle + "", callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Hides a modalDialog.
 *
 * @param childID
 *            ID of the screen that should be shown
 * @param processedCallback
 *            optional callback for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetModalDialogHide = function(dialogID, successCallback,
		errorCallback, processedCallback) {
	callbackID = "dialogHide" + dialogID;
	var mosyncDialogHandle = mosync.nativeui.widgetIDList[dialogID];
	mosync.bridge.send([ "NativeUI", "maWidgetModalDialogHide",
			mosyncDialogHandle + "", callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Pushes a screen to the given screen stack, hides the current screen and shows
 * the pushed screen. Pushing it to the stack will make it automatically go
 * back to the previous screen when popped.
 *
 * @param stackScreenID
 *            Javascript ID of the stackscreen widget
 * @param screenID
 *            Javascript ID of the screen widget
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetStackScreenPush = function(stackScreenID, screenID,
		successCallback, errorCallback, processedCallback) {
	callbackID = "StackScreenPush" + screenID;
	var mosyncStackScreenHandle = mosync.nativeui.widgetIDList[stackScreenID];
	var mosyncScreenHandle = mosync.nativeui.widgetIDList[screenID];
	mosync.bridge.send(
			[ "NativeUI", "maWidgetStackScreenPush",
					mosyncStackScreenHandle + "", mosyncScreenHandle + "",
					callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Pops a screen from a screen stack, hides the current screen and shows the
 * popped screen. If there is no previous screen in the screen stack,
 * an empty screen will be shown.
 *
 * @param stackScreenID
 *            JavaScript ID of the StackScreen
 * @param processedCallback
 *            optional callback for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetStackScreenPop = function(stackScreenID,
		successCallback, errorCallback, processedCallback) {
	callbackID = "StackScreenPop" + stackScreenID
			+ Math.round(Math.random() * 100);
	var mosyncStackScreenHandle = mosync.nativeui.widgetIDList[stackScreenID];
	mosync.bridge.send([ "NativeUI", "maWidgetStackScreenPop",
			mosyncStackScreenHandle, callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

mosync.nativeui.widgetPropertyIndexNo = 0;

/**
 * Sets a specified property on the given widget.
 *
 * @param widgetID
 *            JavaScript ID of the widget
 * @param property
 *            name of the property
 * @param value
 *            value of the property
 * @param processedCallback
 *            optional callback for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetSetProperty = function(widgetID, property, value,
		successCallback, errorCallback, processedCallback) {

	// make sure the id is unique for this call
	callbackID = "setProperty" + widgetID + property
			+ mosync.nativeui.widgetPropertyIndexNo++;
	var widgetHandle = mosync.nativeui.widgetIDList[widgetID];
	mosync.bridge.send([ "NativeUI", "maWidgetSetProperty", widgetHandle + "",
			property, value + "", callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Retrieves a specified property from the given widget.
 *
 * @param widgetID
 *            JavaScript ID of the widget
 * @param property
 *            name of the property that should be retrieved
 * @param callBackFunction
 *            the function that will be called when the property is retrieved
 * @param processedCallback
 *            optional callback for knowing that the message is processed
 *
 * @private
 */
mosync.nativeui.maWidgetGetProperty = function(widgetID, property,
		successCallback, errorCallback, processedCallback) {
	callbackID = "getProperty" + widgetID + property;
	var widgetHandle = mosync.nativeui.widgetIDList[widgetID];
	mosync.bridge.send([ "NativeUI", "maWidgetGetProperty", widgetHandle + "",
			property, callbackID ], processedCallback);
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};


/**
 * This function is called by C++ to inform creation of a widget If a creation
 * callback is registered it will be called
 *
 * @param callbackID
 *            Javascript ID of the widget
 *
 * @private
 */
mosync.nativeui.createCallback = function(callbackID, widgetID, handle) {
	var callBack = mosync.nativeui.callBackTable[callbackID];
	mosync.nativeui.widgetIDList[widgetID] = handle;

	if (callBack.success) {
		var args = Array.prototype.slice.call(arguments);
		args.shift();
		callBack.success.apply(null, args);
	}
};

/**
 * Sets the web view widget handle and maps it inside the widgetIDList
 * @param handle The handle of the web view widget.
 */
mosync.nativeui.setWebViewHandle = function(handle)
{
	// Bind the string ID of the main webview to the native widget handle.
	mosync.nativeui.widgetIDList[mosync.nativeui.mainWebViewId] = handle;

	// Create a JS widget object for the main webview.
	new mosync.nativeui.NativeWidgetElement(
		"WebView", mosync.nativeui.mainWebViewId,
		{},
		null,
		null);
};

mosync.nativeui.success = function(callbackID) {
	var callBack = mosync.nativeui.callBackTable[callbackID];

	if (callBack.success) {
		var args = Array.prototype.slice.call(arguments);
		// remove the callback ID from the argument list
		args.shift();
		callBack.success.apply(null, args);
	}
};

/**
 * The callback function for getting the widgetProperty. If a property callback
 * is registered it will be called
 *
 * @param widgetHandle
 *            C++ ID of the widget sent from C++
 * @param property
 *            retrieved property's name
 * @param value
 *            value for the retrieved property
 * @private
 */
mosync.nativeui.error = function(callbackID) {
	var callBack = mosync.nativeui.callBackTable[callbackID];
	var args = Array.prototype.slice.call(arguments);
	args.shift();
	if (callBack.error != undefined) {
		var args = Array.prototype.slice.call(arguments);
		callBack.error.apply(null, args);
	}
};

/**
 * Is called by C++ when receiving a widget event. It in turn calls the
 * registered listener for the specific Widget. You normally do not use this
 * function is called internally.
 *
 * @param widgetHandle
 *            C++ ID (MoSync Handle) of the widget that has triggered the event
 * @param eventType
 *            Type of the event (possibly followed by at most 3 event data
 *            variables)
 *
 * @private
 */
mosync.nativeui.event = function(widgetHandle, eventType) {

	var callbackID = widgetHandle + eventType;
	var callbackFunctions = mosync.nativeui.eventCallBackTable[callbackID];
	// if we have a listener registered for this combination, call it
	if (callbackFunctions != undefined) {
		// extract the function arguments
		var args = Array.prototype.slice.call(arguments);
		for (key in callbackFunctions) {

			var callbackFun = callbackFunctions[key];
			// Call the function.
			callbackFun.apply(null, args);
		}
	}
};

mosync.nativeui.NativeElementsTable = {};

/**
 * Registers a callback function for receiving widget events.
 *
 * @param widgetID
 *            JavaScript ID of the widget.
 * @param eventType
 *            Type of the events the users want to listen to.
 * @param callBackFunction
 *            function that will be called when the widget sends an event.
 * @private
 */
mosync.nativeui.registerEventListener = function(widgetID, eventType,
		listenerFunction) {
	var widgetHandle = mosync.nativeui.widgetIDList[widgetID];
	var callbackID = widgetHandle + eventType;
	if (mosync.nativeui.eventCallBackTable[callbackID]) {
		mosync.nativeui.eventCallBackTable[callbackID].push(listenerFunction);
	} else {
		mosync.nativeui.eventCallBackTable[callbackID] = [ listenerFunction ];
	}
};

/**
 * A widget object that user can interact with instead of using the low level
 * functions. This class is not used directly: see mosync.nativeui.create for usage.
 *
 *
 * @param widgetType
 *            Type of the widget that has been created
 * @param widgetID
 *            ID of the widget used for identifying the widget (can be ignored by
 *            the user)
 * @param params A dictionary that includes a list of properties to be set on the widget
 * @param successCallback
 *            a function that will be called if the operation is successful
 * @param errorCallback
 *            a function that will be called if an error occurs
 */
mosync.nativeui.NativeWidgetElement = function(widgetType, widgetID, params,
		successCallback, errorCallback) {
	var self = this;

	self.commandQueue = [];

	self.params = params;

	self.eventQueue = [];

	self.childList = [];

	//var type = widgetType;

	self.type = widgetType;
	/**
	 * Internal function used for synchronizing the widget operations. It makes
	 * sure that the widget is created before calling any other functions.
	 *
	 * @private
	 */
	this.processedMessage = function() {
		function clone(obj) {
			if (null == obj || "object" != typeof obj)
				return obj;
			var copy = obj.constructor();
			for ( var attr in obj) {
				if (obj.hasOwnProperty(attr))
					copy[attr] = obj[attr];
			}
			return copy;
		}
		// Clone the command Queue and try to send everything at once.
		var newCommandQueue = clone(self.commandQueue);
		self.commandQueue = [];
		if (newCommandQueue.length > 0) {
			for ( var i = 0; i < newCommandQueue.length; i++) {
				newCommandQueue[i].func.apply(null, newCommandQueue[i].args);
			}
		}

	};

	// Detect to see if the current widget is a screen
	this.isScreen = ((self.type == "Screen") || (self.type == "TabScreen") || (self.type == "StackScreen")) ? true
			: false;

	// Detect to see if the current widget is a dialog
	this.isDialog = (self.type == "ModalDialog") ? true : false;

	/*
	 * if the widgetID is not defined by the user, we will generate one
	 */
	if (widgetID) {
		self.id = widgetID;
	} else {
		self.id = "natvieWidget" + widgetType + mosync.nativeui.widgetCounter;
		mosync.nativeui.widgetCounter++;
	}

	/**
	 * Internal success function used for creation of the widget
	 *
	 * @private
	 */
	this.onSuccess = function(widgetID, widgetHandle) {
		self.created = true;
		self.handle = widgetHandle;
		if (self.eventQueue) {
			for (key in self.eventQueue) {
				self.addEventListener(self.eventQueue[key].event,
						self.eventQueue[key].callback);
			}
		}
		if (successCallback) {

			successCallback.apply(null, [ widgetID ]);
		}
	};
	/**
	 * Internal error function used for creation of the widget
	 *
	 * @private
	 */
	this.onError = function(errorCode)
	{
		self.latestError = errorCode;
		if (errorCallback)
		{
			errorCallback.apply(null, [ errorCode ]);
		}
	};

	// Send a message to the native layer to create the widget.
	// Note that if we get the id of the main webview, we don't
	// create a new widget, it already exists.
	// This allows us to create a widget tree where the main
	// webveiw can be inserted.
	if (self.id !== mosync.nativeui.mainWebViewId)
	{
		mosync.nativeui.maWidgetCreate(
			widgetType,
			self.id,
			this.onSuccess,
			this.onError,
			self.processedMessage,
			self.params);
	}
	else
	{
		self.created = true;
		self.handle = mosync.nativeui.widgetIDList[mosync.nativeui.mainWebViewId];
	}

	/**
	 * Sets a property to the widget in question.
	 *
	 * @param property
	 *            name of the property
	 * @param successCallback
	 *            a function that will be called if the operation is successful
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 * Example
	 * -------
	 * \code
	 * 		var myWidget = mosync.nativeui.create("Button" ,"myButton");
	 * 		myWidget.setProperty("width", "FILL_AVAILABLE_SPACE")
	 * \endcode
	 */
	this.setProperty = function(property, value, successCallback, errorCallback)
	{
		if (self.created)
		{
			self.params[property] = value;

			mosync.nativeui.maWidgetSetProperty(self.id, property, value,
					successCallback, errorCallback, self.processedMessage);
		}
		else
		{
			self.commandQueue.push({
				func : self.setProperty,
				args : [ property, value, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Retrieves a property and call the respective callback.
	 *
	 * @param property
	 *            name of the property
	 * @param successCallback
	 *            a function that will be called if the operation is successful,
	 *            called with two parameters, property name and property value, for example:
	 *            function(prop, value) { ... }
	 * @param errorCallback
	 *            a function that will be called if an error occurs, takes no parameters
	 */
	this.getProperty = function(property, successCallback, errorCallback)
	{
		if (self.created)
		{
			mosync.nativeui.maWidgetGetProperty(self.id, property,
					successCallback, errorCallback, self.processedMessage);
		}
		else
		{
			self.commandQueue.push({
				func : self.getProperty,
				args : [ property, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Registers an event listener for this widget.
	 *
	 * @param eventType
	 *            type of the event that the user wants to listen to
	 * @param listenerFunction
	 *            a function that will be called when that event is fired
	 *
	 * Example
	 * -------
	 *	\code
	 *	//Create a new button and add an event listener to it
	 *	var myButton = mosync.nativeui.create("Button" ,"myButton",
	 *	{
	 *		//properties of the button
	 *		"width": "FILL_AVAILABLE_SPACE",
	 *		"text": "Click Me!"
	 *	});
	 *	myButton.addEventListener("Clicked", function()
	 *	{
	 *		alert("second button is cliecked");
	 *	});
	 *	\endcode
	 *
	 */
	this.addEventListener = function(eventType, listenerFunction)
	{
		if (self.created)
		{
			mosync.nativeui.registerEventListener(self.id, eventType,
					listenerFunction);
		}
		else
		{
			self.eventQueue.push({
				event : eventType,
				callback : listenerFunction
			});
		}

	};

	/**
	 * Adds a child widget to the current widget.
	 *
	 * @param childID
	 *            the ID for the child widget
	 * @param successCallback
	 *            a function that will be called if the operation is successful
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 *
	 * Example
	 * -------
	 * \code
	 * 	 //Create a Native Screen
	 *   var myScreen = mosync.nativeui.create("Screen" ,"myScreen", {
	 *    	"title": "My Screen"
	 *   });
	 *
	 *   //Create a Button
	 *	 var myButton = mosync.nativeui.create("Button" ,"myButton",
	 *	 {
	 *		//properties of the button
	 *		"width": "FILL_AVAILABLE_SPACE",
	 *		"text": "Click Me!"
	 *	 });
	 *
	 *	//Add the button to the Created Screen
	 *  myScreen.addChild("myButton")
	 * \endcode
	 *
	 */
	this.addChild = function(childID, successCallback, errorCallback)
	{
		if(childID != undefined)
		{
			self.childList.push(childID);
			if ((self.created))
			{
				mosync.nativeui.maWidgetAddChild(self.id, childID, successCallback,
						errorCallback, self.processedMessage);
			}
			else
			{
				self.commandQueue.push({
					func : self.addChild,
					args : [ childID, successCallback, errorCallback ]
				});
			}
		}
		else
		{
			errorCallback.apply(null, "invalid Child Id");
		}
	};

	/**
	 * Inserts a new child widget in the specified index.
	 *
	 * @param childID
	 *            ID of the child widget
	 * @param index
	 *            the index for the place that the new child should be inserted
	 * @param successCallback
	 *            a function that will be called if the operation is successful
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 *
	 * Example
	 * -------
	 * \code
	 * 	 //Create a Native Screen
	 *   var myScreen = mosync.nativeui.create("Screen" ,"myScreen", {
	 *    	"title": "My Screen"
	 *   });
	 *
	 *   //Create a Button
	 *	 var myButton = mosync.nativeui.create("Button" ,"myButton",
	 *	 {
	 *		//properties of the button
	 *		"width": "FILL_AVAILABLE_SPACE",
	 *		"text": "Click Me!"
	 *	 });
	 *
	 *	//Insert the button to the Created Screen child list
	 *  myScreen.insertChild(0, "myButton")
	 * \endcode
	 */
	this.insertChild = function(childID, index, successCallback, errorCallback)
	{
		if(childID != undefined)
		{
			self.childList.splice(index, 0, childID);
			if (self.created)
			{
				mosync.nativeui.maWidgetInsertChild(self.id, childID, index,
						successCallback, errorCallback, self.processedMessage);
			} else {
				self.commandQueue.push({
					func : self.insertChild,
					args : [ childID, index, successCallback, errorCallback ]
				});
			}
		}
		else
		{
			errorCallback.apply(null, "invalid Child Id");
		}
	};

	/**
	 * Removes a child widget from the child list of the current widget.
	 *
	 * @param childID
	 *            Id of the child widget that will be removed
	 * @param successCallback
	 *            a function that will be called if the operation is successful
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 *
	 * Example
	 * -------
	 * \code
	 * 	 //Create a Native Screen
	 *   var myScreen = mosync.nativeui.create("Screen" ,"myScreen", {
	 *    	"title": "My Screen"
	 *   });
	 *
	 *   //Create a Button
	 *	 var myButton = mosync.nativeui.create("Button" ,"myButton",
	 *	 {
	 *		//properties of the button
	 *		"width": "FILL_AVAILABLE_SPACE",
	 *		"text": "Click Me!"
	 *	 });
	 *
	 *	//Add myButton to the screen
	 *  myButton.addTo("myScreen");
	 *
	 *	//Remove mybutton from the childs of myScreen
	 *  myScreen.removeChild("myButton")
	 * \endcode
	 */
	this.removeChild = function(childID, successCallback, errorCallback)
	{
		if(childID != undefined)
		{
			if (self.created)
			{
				//remove the child ID from the list
				for(var index in self.childList)
				{
					if(self.childList[index] ==  childID)
					{
						self.childList.splice(index,1);
					}
				}
				mosync.nativeui.maWidgetRemoveChild(childID, successCallback,
						errorCallback, self.processedMessage);
			}
			else
			{
				self.commandQueue.push({
					func : self.removeChild,
					args : [ childID, successCallback, errorCallback ]
				});
			}
		}
		else
		{
			errorCallback.apply(null, "invalid Child Id");
		}
	};

	/**
	 * Adds the current widget as a child to another widget.
	 *

	 * @param parentId
	 *            JavaScript ID of the parent widget
	 * @param successCallback
	 *            (optional) a function that will be called when the operation
	 *            is done successfuly
	 * @param errorCallback
	 *            (optional) a function that will be called when the operation
	 *            encounters an error
	 *
	 * Example
	 * -------
	 *	\code
	 *	//Create a new button and add an event listener to it
	 *	var secondButton = mosync.nativeui.create("Button" ,"SecondButton",
	 *	{
	 *		//properties of the button
	 *		"width": "FILL_AVAILABLE_SPACE",
	 *		"text": "Second Button"
	 *	});
	 *	secondButton.addTo("mainLayout");
	 *	secondButton.addEventListener("Clicked", function()
	 *	{
	 *		alert("second button is clicked");
	 *	});
	 *	\endcode
	 */
	this.addTo = function(parentId, successCallback, errorCallback)
	{
		var parent = document.getNativeElementById(parentId);
		if (
				(self.created) &&
				(parent != undefined) &&
				(parent.created) &&
				(self.created != undefined)
			)
		{
			parent.addChild(self.id, successCallback, errorCallback);
		}
		else
		{
			self.commandQueue.push({
				func : self.addTo,
				args : [ parentId, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Clones the current Widget.
	 *
	 * @param newID The ID for the newly created widget.
	 *
	 * Example
	 * -------
	 *	\code
	 *	//Create a new button and add an event listener to it
	 *	var secondButton = mosync.nativeui.create("Button" ,"SecondButton",
	 *	{
	 *		//properties of the button
	 *		"width": "FILL_AVAILABLE_SPACE",
	 *		"text": "Second Button"
	 *	});
	 *	secondButton.addTo("mainLayout");
	 *	var thirdButton = secondButton.clone();
	 *	\endcode
	 */
	this.clone = function(newID) {
		var widgetClone = mosync.nativeui.create(self.type, newID, self.params);
		for (var index in self.childList)
		{
			var currentChild = document.getNativeElementById(self.childList[index]);
			var newChildID = newID + "child" + index;
			console.log("cloning " + currentChild.id + "into " + newChildID);
			var clonedChild = currentChild.clone(newChildID);
			clonedChild.addTo(newID);
		}
		return widgetClone;
	};


	/*
	 * Only create screen related functions if the widget is a screen
	 */
	if (self.isScreen) {
		/**
		 * Shows a screen widget on the screen. Will be set to null if the widget
		 * is not of type screen.
		 *
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successful
		 * @param errorCallback
		 *            a function that will be called if an error occurs
		 *
		 * Example
		 * -------
		 * \code
		 * 	 //Create a Native Screen
		 *   var myScreen = mosync.nativeui.create("Screen" ,"myScreen", {
		 *    	"title": "My Screen"
		 *   });
		 *
		 *   //Create a Button
		 *	 var myButton = mosync.nativeui.create("Button" ,"myButton",
		 *	 {
		 *		//properties of the button
		 *		"width": "FILL_AVAILABLE_SPACE",
		 *		"text": "Click Me!"
		 *	 });
		 *
		 *	//Add the button to the Created Screen
		 *  myScreen.addChild("myButton");
		 *
		 *  //Show the created screen on the device's screen
		 *  myScreen.show();
		 * \endcode
		 *
		 */
		this.show = function(successCallback, errorCallback) {
			if (self.created) {
				mosync.nativeui.maWidgetScreenShow(self.id, successCallback,
						errorCallback, self.processedMessage);
			} else {
				self.commandQueue.push({
					func : self.show,
					args : [ successCallback, errorCallback ]
				});
			}
		};

		/**
		 * Pushes a screen to a StackScreen.
		 *
		 * @param stackScreenID
		 *            the ID for the stackscreen that should be used for pushing
		 *            the current screen
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successful
		 * @param errorCallback
		 *            a function that will be called if an error occurs
		 *
		 *
		 * Example
		 * -------
		 * \code
		 * 	 //Create a Native StackScreen
		 *   var myStackScreen = mosync.nativeui.create("StackScreen" ,"myStackScreen");
		 *   var myScreen = mosync.nativeui.create("Screen" ,"myScreen", {
		 *    	"title": "My Screen"
		 *   });
		 *
		 *   //Create a Button
		 *	 var myButton = mosync.nativeui.create("Button" ,"myButton",
		 *	 {
		 *		//properties of the button
		 *		"width": "FILL_AVAILABLE_SPACE",
		 *		"text": "Click Me!"
		 *	 });
		 *
		 *	//Add the button to the Created Screen
		 *  myScreen.addChild("myButton");
		 *
		 *  //Show the created screen on the device's screen
		 *  myStackScreen.show();
		 *
		 *  myScreen.pushTo("myStackScreen")
		 * \endcode
		 */
		this.pushTo = function(stackScreenID, successCallback, errorCallback) {
			var stackScreen = document.getNativeElementById(stackScreenID);
			if ((self.created) && (stackScreen != undefined)
					&& (stackScreen.created) && (self.created != undefined)) {
				mosync.nativeui.maWidgetStackScreenPush(stackScreenID, self.id,
						successCallback, errorCallback, self.processedMessage);
			} else {
				self.commandQueue.push({
					func : self.pushTo,
					args : [ stackScreenID, successCallback, errorCallback ]
				});
			}
		};

		/**
		 *
		 * Pops a screen from the current stackscreen, Use only for StackScreen
		 * widgets.
		 *
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successful
		 * @param errorCallback
		 *            a function that will be called if an error occurs
		 *
		 * Example
		 * -------
		 * \code
		 * 	 //Create a Native StackScreen
		 *   var myStackScreen = mosync.nativeui.create("StackScreen" ,"myStackScreen");
		 *   var myScreen = mosync.nativeui.create("Screen" ,"myScreen", {
		 *    	"title": "My Screen"
		 *   });
		 *
		 *   //Create a Button
		 *	 var myButton = mosync.nativeui.create("Button" ,"myButton",
		 *	 {
		 *		//properties of the button
		 *		"width": "FILL_AVAILABLE_SPACE",
		 *		"text": "Click Me!"
		 *	 });
		 *
		 *	//Add the button to the Created Screen
		 *  myScreen.addChild("myButton");
		 *
		 *  //Show the created screen on the device's screen
		 *  myStackScreen.show();
		 *
		 *  myScreen.pushTo("myStackScreen");
		 *
		 *  myStackScreen.pop();
		 * \endcode
		 *
		 */
		this.pop = function(successCallback, errorCallback) {
			if (self.created) {
				mosync.nativeui.maWidgetStackScreenPop(self.id,
						successCallback, errorCallback, self.processedMessage);
			} else {
				self.commandQueue.push({
					func : self.pop,
					args : [ successCallback, errorCallback ]
				});
			}
		};
	}
	/*
	 * Create dialog functions for dialog widgets only
	 */
	if (this.isDialog) {
		/**
		 * Shows a modal dialog widget on the screen.
		 *
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successful
		 * @param errorCallback
		 *            a function that will be called if an error occurs
		 *
		 */
		this.showDialog = function(successCallback, errorCallback) {
			if (self.created) {
				mosync.nativeui.maWidgetModalDialogShow(self.id,
						successCallback, errorCallback, self.processedMessage);
			} else {
				self.commandQueue.push({
					func : self.showDialog,
					args : [ successCallback, errorCallback ]
				});
			}
		};

		/**
		 * Hides a modal dialog widget from the screen.
		 *
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successful
		 * @param errorCallback
		 *            a function that will be called if an error occurs
		 *
		 */
		this.hideDialog = function(successCallback, errorCallback) {
			if (self.created) {
				mosync.nativeui.maWidgetModalDialogHide(self.id,
						successCallback, errorCallback, self.processedMessage);
			} else {
				self.commandQueue.push({
					func : self.hideDialog,
					args : [ successCallback, errorCallback ]
				});
			}
		};

	}
	// add the current widget to the table
	mosync.nativeui.NativeElementsTable[this.id] = this;

};

/**
 * Used to access the nativeWidgetElements created from the HTML markup. It
 * returns the object that can be used to change the properties of the specified
 * widget.
 *
 *
 * @param widgetID
 *            the ID attribute used for identifying the widget in DOM
 *
 * Example
 * -------
 * \code
 * 	  //Get the screen widget
 *    var myScreen = document.getNativeElementById("MyScreen")
 *    //Show it on the device's screen
 *    myScreen.show()
 * \endcode
 */
document.getNativeElementById = function(widgetID)
{
	return mosync.nativeui.NativeElementsTable[widgetID];
};

/**
 * Get the id of the main webview. This can be used to
 * insert the main webview into a widget tree.
 * @return The string id of the main webview widget.
 */
mosync.nativeui.getMainWebViewId = function()
{
	return mosync.nativeui.mainWebViewId;
};

/**
 * Creates a widget and returns a mosync.nativeui.NativeWidgetElement object.
 * The object then can be used for modifying the respective NativeElement.
 *
 *
 * @param widgetType
 *            type of the widget that should be created
 * @param widgetID
 *            ID that will be used for refrencing to the widget
 * @param successCallback
 *            (optional) a function that will be called when the operation is
 *            done successfully
 * @param errorCallback
 *            (optional) a function that will be called when the operation
 *            encounters an error
 *
 * @returns An object of type mosync.nativeui.NativeWidgetElement
 *
 * Example:
 * --------
 * \code
 * 		var myButton = mosync.nativeui.create("Button", "myButton", {
 *					"text" : "Click Me!",
 *					"width" : "FILL_AVAILABLE_SPACE"
 * 					});
 * \endcode
 */
mosync.nativeui.create = function(widgetType, widgetID, params,
		successCallback, errorCallback) {
	var widget = new mosync.nativeui.NativeWidgetElement(widgetType, widgetID,
			params, successCallback, errorCallback);
	return widget;
};

/**
 * Destroys all of the created widgets and cleans up the memory.
 * @private
 */
mosync.nativeui.destroyAll = function()
{
	if(mosync.nativeui.widgetIDList)
	{
		for (var widget in mosync.nativeui.widgetIDList)
		{
			// Destroy all widgets and do not wait for anything.
			mosync.nativeui.maWidgetDestroy(widget, null, null, null);
		}
	}
	if(mosync.resource.imageHandleTable)
	{
		for (var image in mosync.resource.imageHandleTable)
		{
			// Destroy all widgets and do not wait for anything.
			mosync.resource.DestroyPlaceholder(mosync.resource.imageHandleTable[image]);
		}

	}
};


/**
 * Stores the number of widgets that are waiting to be created. Used when
 * parsing the XML based input
 * @private
 */
mosync.nativeui.numWidgetsRequested = 0;

/**
 * Stores the number of widgets that are created. Used when parsing the XML
 * based input
 * @private
 */
mosync.nativeui.numWidgetsCreated = 0;

/**
 * The interval for checking the availability of all widgets. Used when parsing
 * the XML based input
 * @private
 */
mosync.nativeui.showInterval;

/**
 * List of WidetIDs and handles. Used for accessing MoSync widget handles through their IDs.
 * @private
 */
mosync.nativeui.widgetIDList = {};

/**
 * Provides access to C++ handles through IDs.
 *
 * @param elementID
 *            ID of the widget in question
 * @returns MoSync handle value for that widget
 *
 * Example
 * -------
 * \code
 *   var myButton = mosync.nativeui.getElementById("MyButton");
 *   myButton.addTo("myLayout");
 * \endcode
 * @private
 */
mosync.nativeui.getElementById = function(elementID)
{
	return mosync.nativeui.widgetIDList[elementID];
};

/**
 * Get the MoSync widget handle for the JavaScript NativeUI
 * element with the given ID.
 *
 * @param elementId A string id that identifies the widget (this
 * is the ID of the DOM element that holds the widget info).
 */
mosync.nativeui.getNativeHandleById = function(elementId)
{
	return mosync.nativeui.widgetIDList[elementId];
};

/**
 * Constant to be used to reference the main WebView in an app
 * when calling mosync.nativeui.callJS().
 */
mosync.nativeui.MAIN_WEBVIEW = 0;

/**
 * Evaluate JavaScript code in another WebView. This provides a
 * way to pass messages and communicate between WebViews.
 *
 * @param webViewHandle The MoSync handle of the WebView widget.
 * Use mosync.nativeui.MAIN_WEBVIEW to refer to the main WebView
 * in the application (this is the hidden WebView in a JavaScript
 * NativeUI app).
 * @param script A string with JavaScript code.
 */
mosync.nativeui.callJS = function(webViewHandle, script)
{
	mosync.bridge.send([
		"CallJS",
		"" + webViewHandle,
		script]);
};

/**
 * An internal function that returns the correct property name. Used to overcome
 * case sensitivity problems in browsers.
 *
 * @param attributeName
 *            name of the attribute used in HTML markup
 * @returns new name for the attribute
 * @private
 */
mosync.nativeui.getNativeAttrName = function(attributeName) {
	var correctAttrName = String(attributeName).split("data-").join("");
	switch ((correctAttrName).toLowerCase()) {
	case "fontsize":
		return "fontSize";
		break;
	case "fontcolor":
		return "fontColor";
		break;
	case "backgroundcolor":
		return "backgroundColor";
		break;
	case "backgroundgradient":
		return "backgroundGradient";
		break;
	case "currenttab":
		return "currentTab";
		break;
	case "backbuttonenabled":
		return "backButtonEnabled";
		break;
	case "textverticalalignment":
		return "textVerticalAlignment";
		break;
	case "texthorizontalalignment":
		return "textHorizontalAlignment";
		break;
	case "fonthandle":
		return "fontHandle";
		break;
	case "maxnumberoflines":
		return "maxNumberOfLines";
		break;
	case "backgroundimage":
		return "backgroundImage";
		break;
	case "scalemode":
		return "scaleMode";
		break;
	case "showkeyboard":
		return "showKeyboard";
		break;
	case "editmode":
		return "editMode";
		break;
	case "inputmode":
		return "inputMode";
		break;
	case "inputflag":
		return "inputFlag";
		break;
	case "accessorytype":
		return "accessoryType";
		break;
	case "childverticalalignment":
		return "childVerticalAlignment";
		break;
	case "childhorizontalalignment":
		return "childHorizontalAlignment";
		break;
	case "paddingtop":
		return "paddingTop";
		break;
	case "paddingleft":
		return "paddingLeft";
		break;
	case "paddingright":
		return "paddingRight";
		break;
	case "paddingbottom":
		return "paddingBottom";
		break;
	case "softhook":
		return "softHook";
		break;
	case "hardhook":
		return "hardHook";
		break;
	case "horizontalscrollbarenabled":
		return "horizontalScrollBarEnabled";
		break;
	case "verticalscrollbarenabled":
		return "verticalScrollBarEnabled";
		break;
	case "enablezoom":
		return "enableZoom";
		break;
	case "incrementprogress":
		return "incrementProgress";
		break;
	case "inprogress":
		return "inProgress";
		break;
	case "increasevalue":
		return "increaseValue";
		break;
	case "decreasevalue":
		return "decreaseValue";
		break;
	case "maxdate":
		return "maxDate";
		break;
	case "mindate":
		return "minDate";
		break;
	case "dayofmonth":
		return "dayOfMonth";
		break;
	case "currenthour":
		return "currentHour";
		break;
	case "currentminute":
		return "currentMinute";
		break;
	case "minvalue":
		return "minValue";
		break;
	case "maxvalue":
		return "maxValue";
		break;

	default:
		return correctAttrName;
	}
};

mosync.nativeui.getNativeAttrValue = function(value) {
	switch (String(value)) {
	// @deprecated The alias "100%" is kept for backwards compatibility.
	case "100%":
		return "-1";
		break;
	case "FILL_AVAILABLE_SPACE":
		return "-1";
		break;
	case "WRAP_CONTENT":
		return "-2";
		break;
	default:
		return value;
	}
};

/**
 * Creates a widget, sets its property and adds it to its parent.
 *
 * @param widgetID
 *            ID of the widget in question
 * @param parentID
 *            Id of the parentWidget
 * @private
 */
mosync.nativeui.createWidget = function(widget, parent) {
	var widgetNode = widget;
	var widgetID = widget.id;
	var imageResources = null;
	var widgetType = widgetNode.getAttribute("widgetType");
	if(widgetType == null)
	{
		widgetType = widgetNode.getAttribute("data-widgetType");
	}
	mosync.nativeui.numWidgetsRequested++;
	var attributeList = widgetNode.attributes;
	var propertyList = {};
	var eventList = null;
	for ( var i = 0; i < attributeList.length; i++) {
		// TODO: Add more event types and translate the attributes.
		if (attributeList[i].specified) {
			var attrName = mosync.nativeui
					.getNativeAttrName(attributeList[i].name);
			var attrValue = mosync.nativeui
					.getNativeAttrValue(attributeList[i].value);
			if ((attrName != "id") && (attrName != "widgettype")
					&& (attrValue != null)) {
				if ((attrName.toLowerCase() == "onevent") ||
					(attrName.toLowerCase() == "onclick")) {

					var functionData = attrValue;
					eventList = {
						type : "Clicked",
						func : function(widgetHandle, eventType) {
							// TODO: Improve event function parsing mechanism
							eval(functionData);
						}
					};
				} else if ((attrName.toLowerCase() == "image") || (attrName.toLowerCase() == "icon")) {
					imageResources = {
						propertyType : attrName,
						value : attrValue
					};
				} else if ((mosync.isAndroid) && (attrName == "icon_android")) {
					console.log("mosync.nativeui.createWidget detected an icon: " + attrValue);
					imageResources = {
						propertyType : "icon",
						value : attrValue
					};
				} else if ((mosync.isIOS) && (attrName == "icon_ios")) {
					imageResources = {
						propertyType : "icon",
						value : attrValue
					};
				} else {
					if ((attrName.toLowerCase() != "icon_ios")
							&& (attrName.toLowerCase() != "icon_android")) {
						propertyList[attrName] = attrValue;
					}
				}
			}
		}
	}
	var currentWidget = mosync.nativeui.create(
		widgetType,
		widgetID,
		propertyList,
		function(widgetID, handle) {
			var thisWidget = document.getNativeElementById(widgetID);
			mosync.nativeui.numWidgetsRequested--;
			if (imageResources != null) {
				mosync.resource.loadImage(imageResources.value,
						widgetID + "image", function(imageID,
								imageHandle) {
							thisWidget.setProperty(
									imageResources.propertyType,
									imageHandle, null, null);

						});
			}
			if (eventList != null) {
				thisWidget.addEventListener(eventList.type,
						eventList.func);
			}
		}, null);
	if (parent != null) {
		currentWidget.addTo(parent.id);
	}
};

/**
 * A function that is called when the UI is ready. By default it loads the
 * element with ID "mainScreen" Override this function to add extra
 * functionality. See mosync.nativeui.initUI for more information.
 *
 */
mosync.nativeui.UIReady = function()
{
	// This is the low level way of showing the default screen
	// If you want to override this function,
	// use document.getNativeElementById instead
	mosync.nativeui.maWidgetScreenShow("mainScreen");
};

/**
 * Recursively creates the UI from the HTML5 markup.
 *
 * @param parentid
 *            ID of the parent Widget
 * @param id
 *            ID of the currewnt widget
 * @private
 */
mosync.nativeui.createChildren = function(parent, widget) {
	if (widget != undefined) {
		var node = widget;
		var nodeChilds = node.childNodes;
		mosync.nativeui.createWidget(node, parent);
		if (nodeChilds != null) {
			for ( var i = 0; i < nodeChilds.length; i++) {

				if ((nodeChilds[i] != null)
						&& (nodeChilds[i].tagName != undefined)) {
					if ((nodeChilds[i].id == null)
							|| (nodeChilds[i].id == undefined)
							|| (nodeChilds[i].id == "")) {
						nodeChilds[i].id = "widget"
								+ mosync.nativeui.widgetCounter;
						mosync.nativeui.widgetCounter++;
					}
					mosync.nativeui.createChildren(node, nodeChilds[i]);
				}
			}
		}

	}
};

/**
 * Checks the status of UI and calls UIReady when it is ready.
 *
 * @private
 */
mosync.nativeui.CheckUIStatus = function() {
	if (0 == mosync.nativeui.numWidgetsRequested) {
		window.clearInterval(mosync.nativeui.showInterval);
		mosync.nativeui.UIReady();
	}
};

/**
 * Shows a MoSync Screen, can be used to change the current screen.
 *
 * usage example:
 *  mosync.nativeui.showScreen("myNewScreen");
 *
 * @param screenID
 * @private
 */
mosync.nativeui.showScreen = function(screenID) {
	if (numWidgetsCreated == numWidgetsRequested) {
		mosync.nativeui
				.maWidgetScreenShow(mosync.nativeui.widgetIDList[screenID]);

	}
};

/**
 * Initializes the UI system and parsing of the XML input.
 * This function should be called when the document body is loaded.
 *
 * @return true on success, false on error.
 *
 * \code
 *  <!-- The function can be called in the initialization phase of HTML document.-->
 *  <body onload="mosync.nativeui.initUI()">
 * \endcode
 *  After finalizing the widgets, the UI system will call the UIReady function.
 *  To add your operation you can override the UIReady function as below:
 *
 * \code
 *  mosync.nativeui.UIReady = function()
 *  {
 *  //Do something, and show your main screen
 *  }
 * \endcode
 */
mosync.nativeui.initUI = function() {
	var MoSyncDiv = document.getElementById("NativeUI");
	if (!MoSyncDiv) {
		// TODO: Add log error message.
		return false;
	}
	MoSyncDiv.style.display = "none"; //hide the Native Container
	var MoSyncNodes = document.getElementById("NativeUI").childNodes;
	if (!MoSyncNodes) {
		// TODO: Add log error message.
		return false;
	}
	for (var i = 1; i < MoSyncNodes.length; i++) {
		if ((MoSyncNodes[i] != null) && (MoSyncNodes[i].tagName != undefined)) {
			if (MoSyncNodes[i].id == null) {
				MoSyncNodes[i].id = "widget" + mosync.nativeui.widgetCounter;
				mosync.nativeui.widgetCounter++;
			}
			mosync.nativeui.createChildren(null, MoSyncNodes[i]);
		}
	}
	mosync.nativeui.showInterval = self.setInterval(
		"mosync.nativeui.CheckUIStatus()", 100);
	return true;
};

/**
 * Store the screen size information coming from MoSync
 * in the mosync.nativeui namespace. This function is
 * called from C++.
 */
mosync.nativeui.setScreenSize = function(width, height) {
	mosync.nativeui.screenWidth = width;
	mosync.nativeui.screenHeight = height;
}
