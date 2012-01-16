/**
 * @file mosync.nativeui.js
 * @author Ali Sarrafi
 *
 * The library for supporting Native widgets in Javascript and Web applications.
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
 */
mosync.nativeui.callBackTable = {};

/**
 * List of registered callback functions for WidgetEvents
 */
mosync.nativeui.eventCallBackTable = {};

/**
 * used to generate IDs for widgets that do not have one
 */
mosync.nativeui.widgetCounter = 0;

/**
 * Creates a mosync.nativeui Widget and registers it callback for return of the
 * handle
 *
 * @param widgetType
 *            A string that includes type of the widget defined in MoSync Api
 *            reference
 * @param widgetID
 *            An ID set by the user for high level access to the widget
 * @param successCallback
 *            The function that would be called when the widget is created
 * @param errorCallback
 *            The function that would be called is an error happens
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
 */
mosync.nativeui.maWidgetCreate = function(widgetType, widgetID,
		successCallback, errorCallback, processedCallback, properties) {

	callbackID = "create" + widgetID;
	var message = [ "NativeUI", "maWidgetCreate", widgetType, widgetID,
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
	mosync.nativeui.callBackTable[callbackID] = {
		success : successCallback,
		error : errorCallback
	};
};

/**
 * Destroys a widget
 *
 * @param widgetID
 *            ID for the widget in Question
 * @param processedCallback
 *            optional call back for knowing that the message is processed See
 *            mosync.nativeui.getElementById for getting handles
 */
mosync.nativeui.maWidgetDestroy = function(widgetID, successCallBack,
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
 *            Id of the screen that should be shown
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
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
 *            Id of the screen that should be shown
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
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
 * the pushed screen it. Pushing it to the stack will make it automatically go
 * back to the previous screen when popped.
 *
 * @param stackScreenID
 *            Javascript ID of the stackscreen widget
 * @param screenID
 *            Javascript ID of the screen widget
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
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
 * popped screen before the If there is no previous screen in the screen stack,
 * an empty screen will be shown.
 *
 * @param stackScreenID
 *            JavaScript ID of the StackScreen
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
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
 *            optional call back for knowing that the message is processed
 *
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
 *            name of the property that should be retrived
 * @param callBackFunction
 *            the function that will be called when the property is retrived
 * @param processedCallback
 *            optional call back for knowing that the message is processed
 *
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
 *            Javascript Id of the widget
 *
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

mosync.nativeui.success = function(callbackID) {
	var callBack = mosync.nativeui.callBackTable[callbackID];

	if (callBack.success) {
		var args = Array.prototype.slice.call(arguments);
		// remove the callbakID from the argument list
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
 *            Type of the event (maybe followed by at most 3 event data
 *            variables)
 *
 */
mosync.nativeui.event = function(widgetHandle, eventType) {

	var callbackID = widgetHandle + eventType;
	var callbackFunctions = mosync.nativeui.eventCallBackTable[callbackID];
	// if we have a listener registered for this combination call it
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
 *            Type of the events the users wants to listen to.
 * @param callBackFunction
 *            function that will be called when the widget sends an event.
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
 *
 * A widget object that user can interact with instead of using the low level
 * functions. It is used in
 *
 * @class represents a widget that can be manipulated
 * @param widgetType
 *            Type of the widget that has been created
 * @param widgetID
 *            ID of the widget used for identifying the widget(can be ignored by
 *            the user)
 *
 */
mosync.nativeui.NativeWidgetElement = function(widgetType, widgetID, params,
		successCallback, errorCallback) {
	var self = this;

	self.commandQueue = [];

	self.params = params;

	self.eventQueue = [];

	var type = widgetType;
	/**
	 * Internal function used for synchronizing the widget operations. It makes
	 * sure that the widget is created before calling any other functions.
	 *
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
	this.isScreen = ((type == "Screen") || (type == "TabScreen") || (type == "StackScreen")) ? true
			: false;

	// Detect to see if the current widget is a dialog
	this.isDialog = (type == "ModalDialog") ? true : false;

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
	var onSuccess = function(widgetID, widgetHandle) {
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
	var onError = function(errorCode) {
		self.latestError = errorCode;
		if (errorCallback) {
			errorCallback.apply(null, [ errorCode ]);
		}

	};
	/*
	 * Create the widget in the Native Side
	 */
	mosync.nativeui.maWidgetCreate(widgetType, self.id, onSuccess, onError,
			self.processedMessage, self.params);

	/**
	 * sets a property to the widget in question
	 */
	this.setProperty = function(property, value, successCallback, errorCallback) {
		if (self.created) {
			mosync.nativeui.maWidgetSetProperty(self.id, property, value,
					successCallback, errorCallback, self.processedMessage);
		} else {
			self.commandQueue.push({
				func : self.setProperty,
				args : [ property, value, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * retirves a property and call the respective callback
	 *
	 * @param property
	 *            name of the property
	 * @param successCallback
	 *            a function that will be called if the operation is successfull
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 */
	this.getProperty = function(property, successCallback, errorCallback) {
		if (self.created) {
			mosync.nativeui.maWidgetGetProperty(self.id, property,
					successCallback, errorCallback, self.processedMessage);
		} else {
			self.commandQueue.push({
				func : self.getProperty,
				args : [ property, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Registers an event listener for this widget
	 *
	 * @param eventType
	 *            type of the event that the user wants to listen to
	 * @param listenerFunction
	 *            a function that will be called when that event is fired.
	 *
	 */
	this.addEventListener = function(eventType, listenerFunction) {
		if (self.created) {
			mosync.nativeui.registerEventListener(self.id, eventType,
					listenerFunction);
		} else {
			self.eventQueue.push({
				event : eventType,
				callback : listenerFunction
			});
		}

	};

	/**
	 * Adds a child widget to the cureent widget
	 *
	 * @param childID
	 *            the ID for th echild widget
	 * @param successCallback
	 *            a function that will be called if the operation is successfull
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 *
	 */
	this.addChild = function(childID, successCallback, errorCallback) {

		if ((self.created) && (childID != undefined)) {
			mosync.nativeui.maWidgetAddChild(self.id, childID, successCallback,
					errorCallback, self.processedMessage);
		} else {
			self.commandQueue.push({
				func : self.addChild,
				args : [ childID, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Inserts a new child widget in the specifed index
	 *
	 * @param childID
	 *            ID of the child widget
	 * @param index
	 *            the index for the place that the new child should be insterted
	 * @param successCallback
	 *            a function that will be called if the operation is successfull
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 */
	this.insertChild = function(childID, index, successCallback, errorCallback) {
		if ((self.created) && (childID != undefined)) {
			mosync.nativeui.maWidgetInsertChild(self.id, childID, index,
					successCallback, errorCallback, self.processedMessage);
		} else {
			self.commandQueue.push({
				func : self.insertChild,
				args : [ childID, index, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Removes a child widget fro mthe child list of the current widget
	 *
	 * @param childID
	 *            Id of the child widget that will be removed
	 * @param successCallback
	 *            a function that will be called if the operation is successfull
	 * @param errorCallback
	 *            a function that will be called if an error occurs
	 *
	 */
	this.removeChild = function(childID, successCallback, errorCallback) {
		if ((self.created) && (childID != undefined)) {
			mosync.nativeui.maWidgetRemoveChild(childID, successCallback,
					errorCallback, self.processedMessage);
		} else {
			self.commandQueue.push({
				func : self.removeChild,
				args : [ childID, successCallback, errorCallback ]
			});
		}
	};

	/**
	 * Adds the current widget as a child to another widget.
	 *
	 * @param parentId
	 *            JavaScript ID of the parent widget.
	 * @param successCallback
	 *            (optional) a function that will be called when the operation
	 *            is done successfully
	 * @param errorCallback
	 *            (optional) a function that will be called when the operation
	 *            encounters an error
	 *
	 */
	this.addTo = function(parentId, successCallback, errorCallback) {
		var parent = document.getNativeElementById(parentId);
		if ((self.created) && (parent != undefined) && (parent.created)
				&& (self.created != undefined)) {
			mosync.nativeui.maWidgetAddChild(parentId, self.id,
					successCallback, errorCallback, self.processedMessage);
		} else {
			self.commandQueue.push({
				func : self.addTo,
				args : [ parentId, successCallback, errorCallback ]
			});
		}
	};
	/*
	 * Only create screen related functions if the widget is a screen
	 */
	if (self.isScreen) {
		/**
		 * Shows a screen widget on the screen. Will call the error callback if
		 * the widget is not of type screen.
		 *
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successfull
		 * @param errorCallback
		 *            a function that will be called if an error occurs
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
		 * pushs a screen to a stackscreen
		 *
		 * @param stackScreenID
		 *            the ID for the stackscreen that should be used for pushing
		 *            the current screen
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successfull
		 * @param errorCallback
		 *            a function that will be called if an error occurs
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
		 * Pops a screen fr om the current stackscreen, Use only for StackScreen
		 * widgets
		 *
		 * @param successCallback
		 *            a function that will be called if the operation is
		 *            successfull
		 * @param errorCallback
		 *            a function that will be called if an error occurs
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
		 *            successfull
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
		 *            successfull
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
 * @param widgetID
 *            the ID attribute used for identifying the widget in DOM
 *
 */
document.getNativeElementById = function(widgetID) {
	return mosync.nativeui.NativeElementsTable[widgetID];
};

/**
 * creates a widget and returns a NativeWidgetElement object.
 *
 * usage: var myButton = mosync.nativeui.create("Button", "myButton");
 *
 *
 * @param widgetType
 *            type of the widet that should be created
 * @param widgetID
 *            ID that will be used for refrencing to the widget
 * @param successCallback
 *            (optional) a function that will be called when the operation is
 *            done successfully
 * @param errorCallback
 *            (optional) a function that will be called when the operation
 *            encounters an error
 *
 * @returns {mosync.nativeui.NativeWidgetElement}
 */
mosync.nativeui.create = function(widgetType, widgetID, params,
		successCallback, errorCallback) {
	var widget = new mosync.nativeui.NativeWidgetElement(widgetType, widgetID,
			params, successCallback, errorCallback);
	return widget;
};

/**
 * Stores the number of widgets that are waiting to be created. Used when
 * parsing the XML based input
 */
mosync.nativeui.numWidgetsRequested = 0;

/**
 * Stores the number of widgets that are created. Used when parsing the XML
 * based input
 */
mosync.nativeui.numWidgetsCreated = 0;

/**
 * The interval for checking the availability of all widgets Used when parsing
 * the XML based input
 */
mosync.nativeui.showInterval;

/**
 * List of WidetIDs and handles. Used for accessign widgets through their IDs
 */
mosync.nativeui.widgetIDList = {};

/**
 * Provides access to C++ handles through IDs.
 *
 * @param elementID
 *            ID of the widget in question
 * @returns MoSync handle value for that widget
 */
mosync.nativeui.getElementById = function(elementID) {
	return mosync.nativeui.widgetIDList[elementID];
};

/**
 * An internal function that returns the correct property name Used to overcome
 * case sensitivity problems in browsers.
 *
 * @param attributeName
 *            name of the attribute used in HTML markup
 * @returns new name for the attribute
 */
mosync.nativeui.getNativeAttrName = function(attributeName) {
	switch (String(attributeName).toLowerCase()) {
	case "fontsize":
		return "fontSize";
		break;
	case "fontcolor":
		return "fontColor";
		break;
	case "backgrouncolor":
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
		return attributeName;
	}
};

mosync.nativeui.getNativeAttrValue = function(value) {
	switch (String(value).toLowerCase()) {
	case "100%":
		return "-1";
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
 */
mosync.nativeui.createWidget = function(widget, parent) {
	var widgetNode = widget;
	var widgetID = widget.id;
	var imageResources = null;
	var widgetType = widgetNode.getAttribute("widgetType");
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
				if (attrName == "onevent") {

					var functionData = attrValue;
					eventList = {
						type : "Clicked",
						func : function(widgetHandle, eventType) {
							// TODO: Improve event function parsing mechanism
							eval(functionData);
						}
					};
				} else if ((attrName == "image") || (attrName == "icon")) {
					imageResources = {
						propertyType : attrName,
						value : attrValue
					};
				} else if ((mosync.isAndroid) && (attrName == "icon_android")) {
					console.log("detected an Icon" + attrValue);
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
					if ((attrName != "icon_ios")
							&& (attrName != "icon_android")) {
						propertyList[attrName] = attrValue;
					}
				}
			}
		}
	}
	var currentWidget = mosync.nativeui
			.create(widgetType, widgetID, propertyList,
					function(widgetID, handle) {
						var thisWidget = document
								.getNativeElementById(widgetID);
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
 * functionality. See mosync.nativeui.initUI for more information
 */
mosync.nativeui.UIReady = function() {
	// This is the low level way of showing the default screen
	// If you want to override this fucntion,
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
 */
mosync.nativeui.createChilds = function(parent, widget) {
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
					mosync.nativeui.createChilds(node, nodeChilds[i]);
				}
			}
		}

	}
};

/**
 * Checks the status of UI and calls UIReady when it is ready.
 * @internal
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
 * usage:
 *  <body onload="mosync.nativeui.initUI()">
 *
 *  After finalizing the widgets, the UI system will call the UIReady function.
 *  In order to add your operation you can override the UIReady function as below:
 *
 *  mosync.nativeui.UIReady = function()
 *  {
 *  //Do something, and show your main screen
 *  }
 *
 */
mosync.nativeui.initUI = function() {
	var MoSyncDiv = document.getElementById("NativeUI");
	MoSyncDiv.style.display = "none"; //hide the Native Container
	var MoSyncNodes = document.getElementById("NativeUI").childNodes;
	for ( var i = 1; i < MoSyncNodes.length; i++) {
		if ((MoSyncNodes[i] != null) && (MoSyncNodes[i].tagName != undefined)) {
			if (MoSyncNodes[i].id == null) {
				MoSyncNodes[i].id = "widget" + mosync.nativeui.widgetCounter;
				mosync.nativeui.widgetCounter++;
			}
			mosync.nativeui.createChilds(null, MoSyncNodes[i]);
		}
	}
	mosync.nativeui.showInterval = self.setInterval(
			"mosync.nativeui.CheckUIStatus()", 100);
};

/*
 * store the screen size information coming from MoSync
 * in the namespace
 */
if (mosyncScreenWidth) {
	mosync.nativeui.screenWidth = mosyncScreenWidth;
}
if (mosyncScreenHeight) {
	mosync.nativeui.screenHeight = mosyncScreenHeight;
}
