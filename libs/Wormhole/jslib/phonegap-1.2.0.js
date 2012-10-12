/*
PhoneGap
========

PhoneGap is available under *either* the terms of the modified BSD license *or* the
MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.

 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 * Copyright (c) 2012, MoSync AB
 */

/**
 * The order of events during page load and PhoneGap startup is as follows:
 *
 * onDOMContentLoaded         Internal event that is received when the web page is loaded and parsed.
 * window.onload              Body onload event.
 * onNativeReady              Internal event that indicates the PhoneGap native side is ready.
 * onPhoneGapInit             Internal event that kicks off creation of all PhoneGap JavaScript objects (runs constructors).
 * onPhoneGapReady            Internal event fired when all PhoneGap JavaScript objects have been created
 * onPhoneGapInfoReady        Internal event fired when device properties are available
 * onDeviceReady              User event fired to indicate that PhoneGap is ready
 * onResume                   User event fired to indicate a start/resume lifecycle event
 * onPause                    User event fired to indicate a pause lifecycle event
 * onDestroy                  Internal event fired when app is being destroyed (User should use window.onunload event, not this one).
 *
 * The only PhoneGap events that user code should register for are:
 *      onDeviceReady
 *      onResume
 *
 * Listeners can be registered as:
 *      document.addEventListener("deviceready", myDeviceReadyListener, false);
 *      document.addEventListener("resume", myResumeListener, false);
 *      document.addEventListener("pause", myPauseListener, false);
 */

/**
 * We override the JSON.parse function because it does not work properly on all phones.
 */
JSON.parse = function(JSONDocument)
{
	var parsedObject = eval('(' + JSONDocument + ')');
	return parsedObject;
};

 if (typeof(DeviceInfo) !== 'object') {
    var DeviceInfo = {};
}

var PhoneGap = {
        queue: {
        ready: true,
        commands: [],
        timer: null
    },
    available:false,
    callbackId:0,
    callbacks:{},
    resources:{}
};

PhoneGap.callbackStatus = {
    NO_RESULT: 0,
    OK: 1,
    CLASS_NOT_FOUND_EXCEPTION: 2,
    ILLEGAL_ACCESS_EXCEPTION: 3,
    INSTANTIATION_EXCEPTION: 4,
    MALFORMED_URL_EXCEPTION: 5,
    IO_EXCEPTION: 6,
    INVALID_ACTION: 7,
    JSON_EXCEPTION: 8,
    ERROR: 9
};

/**
 * Determine if resource has been loaded by PhoneGap
 *
 * @param name
 * @return
 */
PhoneGap.hasResource = function(name) {
    return PhoneGap.resources[name];
};

/**
 * Add a resource to list of loaded resources by PhoneGap
 *
 * @param name
 */
PhoneGap.addResource = function(name) {
    PhoneGap.resources[name] = true;
};

PhoneGap.exec = function(success, fail, service, action, args)
{
    var callbackId = service + PhoneGap.callbackId++;
    if (typeof success == "function" || typeof fail == "function")
    {
        PhoneGap.callbacks[callbackId] = {success:success, fail:fail};
    }

    // MOSYNC: Use the bridge library to send the message to C++.
    mosync.bridge.PhoneGap.send(callbackId, service, action, args);
};

// MOSYNC: We currently only call this function for key, pause and resume events
//We call PhoneGap.CallbackSuccess and PhoneGap.CallbackError directly  from C++.
PhoneGapCommandResult = function(status,callbackId,args,cast)
{
    if(status === "backbutton") {

        PhoneGap.fireEvent(document,"backbutton");
        return "true";

    } else if(status === "resume") {

        PhoneGap.onResume.fire();
        return "true";

    } else if(status === "pause") {

        PhoneGap.onPause.fire();
        return "true";
    }

    var safeStatus = parseInt(status);
    if(safeStatus === PhoneGap.callbackStatus.NO_RESULT ||
       safeStatus === PhoneGap.callbackStatus.OK) {
        PhoneGap.CallbackSuccess(callbackId,args,cast);
    }
    else
    {
        PhoneGap.CallbackError(callbackId,args,cast);
    }
};

/**
 * Called by native code when returning successful result from an action.
 *
 * @param callbackId
 * @param args
 * @param cast
 */
PhoneGap.CallbackSuccess = function(callbackId, args, cast)
{
    var commandResult;
    try
    {
        commandResult = JSON.parse(args);

        if (typeof cast !== 'undefined')
        {
            eval('commandResult = ' + cast + '(commandResult);');
        }
    }
    catch(exception)
    {
        console.log("PhoneGap.CallbackSuccess Exception: " + exception.message);
        return exception.message;
    }

    if (PhoneGap.callbacks[callbackId] ) {

        // If result is to be sent to callback
        if (commandResult.status === PhoneGap.callbackStatus.OK) {
            try {
                if (PhoneGap.callbacks[callbackId].success) {
                    result = PhoneGap.callbacks[callbackId].success(commandResult.message);
                }
            }
            catch (e) {
                console.log("Error in success callback: "+callbackId+" = " + e.message);
            }
        }

        // Clear callback if not expecting any more results
        if (!commandResult.keepCallback) {
            delete PhoneGap.callbacks[callbackId];
        }
    }

    // Note that in WP7, this method can return a value to the native calling code
    return "";
};

/**
 * Called by native code when returning error result from an action.
 *
 * @param callbackId
 * @param args
 * @param cast - not supported
 */
PhoneGap.CallbackError = function (callbackId, args, cast) {

    var commandResult;
    try
    {
        commandResult = JSON.parse(args);
    }
    catch(exception)
    {
        console.log("PhoneGap.CallbackError Exception: " + exception.message);
        return exception.message;
    }

    if (PhoneGap.callbacks[callbackId]) {
        try {
            if (PhoneGap.callbacks[callbackId].fail) {
                PhoneGap.callbacks[callbackId].fail(commandResult.message);
            }
        }
        catch (e) {
            console.log("Error in error callback: "+callbackId+" = "+e);
        }

        // Clear callback if not expecting any more results
        if (!args.keepCallback) {
            delete PhoneGap.callbacks[callbackId];
        }
    }
};

/**
 * Create a UUID
 *
 * @return {String}
 */
PhoneGap.createUUID = function() {
    return PhoneGap.UUIDcreatePart(4) + '-' +
        PhoneGap.UUIDcreatePart(2) + '-' +
        PhoneGap.UUIDcreatePart(2) + '-' +
        PhoneGap.UUIDcreatePart(2) + '-' +
        PhoneGap.UUIDcreatePart(6);
};

PhoneGap.UUIDcreatePart = function(length) {
    var uuidpart = "";
    var i, uuidchar;
    for (i=0; i<length; i++) {
        uuidchar = parseInt((Math.random() * 256),0).toString(16);
        if (uuidchar.length === 1) {
            uuidchar = "0" + uuidchar;
        }
        uuidpart += uuidchar;
    }
    return uuidpart;
};

/**
 * Does a deep clone of the object.
 *
 * @param obj
 * @return {Object}
 */
PhoneGap.clone = function(obj) {
    var i, retVal;
    if(!obj) {
        return obj;
    }

    if(obj instanceof Array){
        retVal = [];
        for(i = 0; i < obj.length; ++i){
            retVal.push(PhoneGap.clone(obj[i]));
        }
        return retVal;
    }

    if (typeof obj === "function") {
        return obj;
    }

    if(!(obj instanceof Object)){
        return obj;
    }

    if (obj instanceof Date) {
        return obj;
    }

    retVal = {};
    for(i in obj){
        if(!(i in retVal) || retVal[i] !== obj[i]) {
            retVal[i] = PhoneGap.clone(obj[i]);
        }
    }
    return retVal;
};

/*Clones object, but catches exception*/
PhoneGap.safeClone = function(obj)
{
    try
    {
        return PhoneGap.clone(obj);
    }
    catch(e)
    {
        console.log("CloneError::" + e.message);
    }
    return null;
};


/**
 * Custom pub-sub channel that can have functions subscribed to it
 * @constructor
 */
PhoneGap.Channel = function(type)
{
    this.type = type;
    this.handlers = {};
    this.guid = 0;
    this.fired = false;
    this.enabled = true;
};

/**
 * Subscribes the given function to the channel. Any time that
 * Channel.fire is called so too will the function.
 * Optionally specify an execution context for the function
 * and a guid that can be used to stop subscribing to the channel.
 * Returns the guid.
 */
PhoneGap.Channel.prototype.subscribe = function(f, c, g) {
    // need a function to call
    if (f === null) { return; }

    var func = f;
    if (typeof c === "object" && typeof f === "function") { func = PhoneGap.close(c, f); }

    g = g || func.observer_guid || f.observer_guid || this.guid++;
    func.observer_guid = g;
    f.observer_guid = g;
    this.handlers[g] = func;
    return g;
};

/**
 * Like subscribe but the function is only called once and then it
 * auto-unsubscribes itself.
 */
PhoneGap.Channel.prototype.subscribeOnce = function(f, c) {
    var g = null;
    var _this = this;
    var m = function() {
        f.apply(c || null, arguments);
        _this.unsubscribe(g);
    };
    if (this.fired) {
        if (typeof c === "object" && typeof f === "function") { f = PhoneGap.close(c, f); }
        f.apply(this, this.fireArgs);
    } else {
        g = this.subscribe(m);
    }
    return g;
};

/**
 * Unsubscribes the function with the given guid from the channel.
 */
PhoneGap.Channel.prototype.unsubscribe = function(g) {
    if (typeof g === "function") { g = g.observer_guid; }
    this.handlers[g] = null;
    delete this.handlers[g];
};

/**
 * Calls all functions subscribed to this channel.
 */
PhoneGap.Channel.prototype.fire = function(e) {
    if (this.enabled) {
        var fail = false;
        var item, handler, rv;
        for (item in this.handlers) {
            if (this.handlers.hasOwnProperty(item)) {
                handler = this.handlers[item];
                if (typeof handler === "function") {
                    rv = (handler.apply(this, arguments) === false);
                    fail = fail || rv;
                }
            }
        }
        this.fired = true;
        this.fireArgs = arguments;
        return !fail;
    }
    return true;
};

/**
 * Calls the provided function only after all of the channels specified
 * have been fired.
 */
PhoneGap.Channel.join = function(h, c) {
    var i = c.length;
    var f = function() {
        if (!(--i)) {
            h();
        }
    };
    var len = i;
    var j;
    for (j=0; j<len; j++) {
        if (!c[j].fired) {
            c[j].subscribeOnce(f);
        }
        else {
            i--;
        }
    }
    if (!i) {
        h();
    }
};

/**
 * Boolean flag indicating if the PhoneGap API is available and initialized.
 */ // TODO: Remove this, it is unused here ... -jm
PhoneGap.available = DeviceInfo.uuid !== undefined;

/**
 * Add an initialization function to a queue that ensures it will run and initialize
 * application constructors only once PhoneGap has been initialized.
 * @param {Function} func The function callback you want run once PhoneGap is initialized
 */
PhoneGap.addConstructor = function(func)
{
    PhoneGap.onPhoneGapInit.subscribeOnce(function() {
        try {
            func();
        } catch(e) {
            console.log("Failed to run constructor: " + e);
        }
    });
};

/*
 * Plugins object
 */
if (!window.plugins) {
    window.plugins = {};
};

/**
 * Adds a plugin object to window.plugins.
 * The plugin is accessed using window.plugins.<name>
 *
 * @param name          The plugin name
 * @param obj           The plugin object
 */
PhoneGap.addPlugin = function(name, obj) {
    if (!window.plugins[name]) {
        window.plugins[name] = obj;
    }
    else {
        console.log("Error: Plugin "+name+" already exists.");
    }
};

/**
 * onDOMContentLoaded channel is fired when the DOM content
 * of the page has been parsed.
 */
PhoneGap.onDOMContentLoaded = new PhoneGap.Channel('onDOMContentLoaded');

/**
 * onNativeReady channel is fired when the PhoneGap native code
 * has been initialized.
 */
PhoneGap.onNativeReady = new PhoneGap.Channel('onNativeReady');

/**
 * onPhoneGapInit channel is fired when the web page is fully loaded and
 * PhoneGap native code has been initialized.
 */
PhoneGap.onPhoneGapInit = new PhoneGap.Channel('onPhoneGapInit');

/**
 * onPhoneGapReady channel is fired when the JS PhoneGap objects have been created.
 */
PhoneGap.onPhoneGapReady = new PhoneGap.Channel('onPhoneGapReady');

/**
 * onPhoneGapInfoReady channel is fired when the PhoneGap device properties
 * has been set.
 */
PhoneGap.onPhoneGapInfoReady = new PhoneGap.Channel('onPhoneGapInfoReady');

/**
 * onPhoneGapConnectionReady channel is fired when the PhoneGap connection properties
 * has been set.
 */
PhoneGap.onPhoneGapConnectionReady = new PhoneGap.Channel('onPhoneGapConnectionReady');

/**
 * onResume channel is fired when the PhoneGap native code
 * resumes.
 */
PhoneGap.onResume = new PhoneGap.Channel('onResume');

/**
 * onPause channel is fired when the PhoneGap native code
 * pauses.
 */
PhoneGap.onPause = new PhoneGap.Channel('onPause');

/**
 * onDestroy channel is fired when the PhoneGap native code
 * is destroyed.  It is used internally.
 * Window.onunload should be used by the user.
 */
PhoneGap.onDestroy = new PhoneGap.Channel('onDestroy');
PhoneGap.onDestroy.subscribeOnce(function() {
    PhoneGap.shuttingDown = true;
});
PhoneGap.shuttingDown = false;

// _nativeReady is global variable that the native side can set
// to signify that the native code is ready. It is a global since
// it may be called before any PhoneGap JS is ready.
if (typeof _nativeReady !== 'undefined') { PhoneGap.onNativeReady.fire(); }

/**
 * onDeviceReady is fired only after all PhoneGap objects are created and
 * the device properties are set.
 */
PhoneGap.onDeviceReady = new PhoneGap.Channel('onDeviceReady');


// Array of channels that must fire before "deviceready" is fired
// MOSYNC: Added PhoneGap.onNativeReady to channels array to fix
// bug that caused onDeviceReady to fire too early.
PhoneGap.deviceReadyChannelsArray = [
	PhoneGap.onPhoneGapReady,
	PhoneGap.onPhoneGapInfoReady,
	PhoneGap.onPhoneGapConnectionReady,
	PhoneGap.onNativeReady];

// Hashtable of user defined channels that must also fire before "deviceready" is fired
PhoneGap.deviceReadyChannelsMap = {};

/**
 * Indicate that a feature needs to be initialized before it is ready to be used.
 * This holds up PhoneGap's "deviceready" event until the feature has been initialized
 * and PhoneGap.initComplete(feature) is called.
 *
 * @param feature {String}     The unique feature name
 */
PhoneGap.waitForInitialization = function(feature) {
    if (feature) {
        var channel = new PhoneGap.Channel(feature);
        PhoneGap.deviceReadyChannelsMap[feature] = channel;
        PhoneGap.deviceReadyChannelsArray.push(channel);
    }
};

/**
 * Indicate that initialization code has completed and the feature is ready to be used.
 *
 * @param feature {String}     The unique feature name
 */
PhoneGap.initializationComplete = function(feature) {
    var channel = PhoneGap.deviceReadyChannelsMap[feature];
    if (channel) {
        channel.fire();
    }
};

/*
 * Create all PhoneGap objects once page has fully loaded and native side is ready.
 */
PhoneGap.Channel.join(
    function()
    {
        setTimeout(function()
        {

            PhoneGap.UsePolling = false;
            //PhoneGap.JSCallback();
        },1);

        // Run PhoneGap constructors
        PhoneGap.onPhoneGapInit.fire();

        // Fire event to notify that all objects are created
        PhoneGap.onPhoneGapReady.fire();

        // Fire onDeviceReady event once all constructors have run and PhoneGap info has been
        // received from native side, and any user defined initialization channels.
        PhoneGap.Channel.join(function() {
            PhoneGap.onDeviceReady.fire();

            // Fire the onresume event, since first one happens before JavaScript is loaded
            PhoneGap.onResume.fire();
        }, PhoneGap.deviceReadyChannelsArray);

    },
    [ PhoneGap.onDOMContentLoaded ]);



// Listen for DOMContentLoaded and notify our channel subscribers
document.addEventListener('DOMContentLoaded', function() {
    PhoneGap.onDOMContentLoaded.fire();
}, false);

PhoneGap.m_document_addEventListener = document.addEventListener;
document.addEventListener = function(evt, handler, capture)
{
    console.log("document.addEventListener event named " + evt);

    var e = evt.toLowerCase();
    if (e === 'deviceready')
    {
        PhoneGap.onDeviceReady.subscribeOnce(handler);
    }
    else if (e === 'resume')
    {
        PhoneGap.onResume.subscribe(handler);
        if (PhoneGap.onDeviceReady.fired)
        {
            PhoneGap.onResume.fire();
        }
    }
    else if (e === 'pause')
    {
        PhoneGap.onPause.subscribe(handler);
    }
    else
    {

        if (e === 'backbutton')
        {
            PhoneGap.exec(null, null, "CoreEvents", "overrideBackbutton", [true]);
        }
        PhoneGap.m_document_addEventListener.call(document, evt, handler, capture);
    }
};

PhoneGap.m_document_removeEventListener = document.removeEventListener;
document.removeEventListener = function(evt, handler, capture)
{
    console.log("document.removeEventListener event named " + evt);

    var e = evt.toLowerCase();

    if (e === 'backbutton')
    {
        PhoneGap.exec(null, null, "CoreEvents", "overrideBackbutton", [false]);
    }
    PhoneGap.m_document_removeEventListener.call(document, evt, handler, capture);

}


PhoneGap.fireEvent = function(_targ,evtName)
{
    var target = _targ || window;
    var eventObj = document.createEvent('MouseEvents');
    eventObj.initEvent( evtName, true, false );
    target.dispatchEvent( eventObj );
}

/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("accelerometer"))
{
PhoneGap.addResource("accelerometer");

/**
 * @constructor
 */
var Acceleration = function(x, y, z) {
  this.x = x;
  this.y = y;
  this.z = z;
  this.timestamp = new Date().getTime();
};

/**
 * This class provides access to device accelerometer data.
 * @constructor
 */
var Accelerometer = function() {

    /**
     * The last known acceleration.  type=Acceleration()
     */
    this.lastAcceleration = null;

    /**
     * List of accelerometer watch timers
     */
    this.timers = {};
};

Accelerometer.ERROR_MSG = ["Not running", "Starting", "", "Failed to start"];

/**
 * Asynchronously aquires the current acceleration.
 *
 * @param {Function} successCallback    The function to call when the acceleration data is available
 * @param {Function} errorCallback      The function to call when there is an error getting the acceleration data. (OPTIONAL)
 * @param {AccelerationOptions} options The options for getting the accelerometer data such as timeout. (OPTIONAL)
 \code
 function onSuccess(acceleration) {
    alert('Acceleration X: ' + acceleration.x + '\n' +
          'Acceleration Y: ' + acceleration.y + '\n' +
          'Acceleration Z: ' + acceleration.z + '\n' +
          'Timestamp: '      + acceleration.timestamp + '\n');
};

function onError() {
    alert('onError!');
};

navigator.accelerometer.getCurrentAcceleration(onSuccess, onError);
 \endcode
 */
Accelerometer.prototype.getCurrentAcceleration = function(successCallback, errorCallback, options) {

    // successCallback required
    if (typeof successCallback !== "function") {
        console.log("Accelerometer Error: successCallback is not a function");
        return;
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Accelerometer Error: errorCallback is not a function");
        return;
    }

    var self = this;

    var onSuccess = function(result)
    {
        var accResult = JSON.parse(result);
        console.log("Accel x = " + accResult.x);
        self.lastAcceleration = new Acceleration(accResult.x,accResult.y,accResult.z);
        successCallback(self.lastAcceleration);
    }

    var onError = function(err)
    {
        errorCallback(err);
    }

    // Get acceleration
    PhoneGap.exec(onSuccess, onError, "Accelerometer", "getAcceleration",options);
};


/**
 * Asynchronously aquires the acceleration repeatedly at a given interval.
 *
 * @param {Function} successCallback    The function to call each time the acceleration data is available
 * @param {Function} errorCallback      The function to call when there is an error getting the acceleration data. (OPTIONAL)
 * @param {AccelerationOptions} options The options for getting the accelerometer data such as timeout. (OPTIONAL)
 * @return String                       The watch id that must be passed to #clearWatch to stop watching.
 \code
 function onSuccess(acceleration) {
    alert('Acceleration X: ' + acceleration.x + '\n' +
          'Acceleration Y: ' + acceleration.y + '\n' +
          'Acceleration Z: ' + acceleration.z + '\n' +
          'Timestamp: '      + acceleration.timestamp + '\n');
};

function onError() {
    alert('onError!');
};

navigator.accelerometer.getCurrentAcceleration(onSuccess, onError);
 \endcode
 */
Accelerometer.prototype.watchAcceleration = function(successCallback, errorCallback, options)
{

    // successCallback required
    if (typeof successCallback !== "function") {
        console.log("Accelerometer Error: successCallback is not a function");
        return;
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Accelerometer Error: errorCallback is not a function");
        return;
    }

    var onSuccess = function (result) {
        var accResult = JSON.parse(result);
        console.log("Accel x = " + accResult.x);
        self.lastAcceleration = new Acceleration(accResult.x, accResult.y, accResult.z);
        successCallback(self.lastAcceleration);
    }

    var onError = function (err) {
        errorCallback(err);
    }

    var id = PhoneGap.createUUID();

    var params = new Object();
    params.id = id;
    // Default interval (10 sec)
    params.frequency = (options && options.frequency) ? options.frequency : 10000;

    PhoneGap.exec(onSuccess, onError, "Accelerometer", "startWatch", params);

    return id;
};

/**
 * Clears the specified accelerometer watch.
 *
 * @param {String} id       The id of the watch returned from #watchAcceleration.
 \code
 var watchID = navigator.accelerometer.watchAcceleration(onSuccess, onError, options);

// ... later on ...

navigator.accelerometer.clearWatch(watchID);
 \endcode
 */
Accelerometer.prototype.clearWatch = function(id) {

    PhoneGap.exec(null, null, "Accelerometer", "stopWatch", { id: id });
};

PhoneGap.addConstructor(
function()
{
    if (!navigator.accelerometer)
    {
        console.log("Installing accelerometer");
        navigator.accelerometer = new Accelerometer();
    }
});
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("camera")) {
PhoneGap.addResource("camera");

/**
 * This class provides access to the device camera.
 *
 * @constructor
 */
var Camera = function() {
    this.successCallback = null;
    this.errorCallback = null;
    this.options = null;
};

/**
 * Format of image that returned from getPicture.
 *
 * Example: navigator.camera.getPicture(success, fail,
 *              { quality: 80,
 *                destinationType: Camera.DestinationType.DATA_URL,
 *                sourceType: Camera.PictureSourceType.PHOTOLIBRARY})
 \code
 navigator.camera.getPicture(onSuccess, onFail, { quality: 50 });

function onSuccess(imageData) {
    var image = document.getElementById('myImage');
    image.src = "data:image/jpeg;base64," + imageData;
}

function onFail(message) {
    alert('Failed because: ' + message);
}
 \endcode
 */
Camera.DestinationType = {
    DATA_URL: 0,                // Return base64 encoded string
    FILE_URI: 1                 // Return file uri (content://media/external/images/media/2 for Android)
};
Camera.prototype.DestinationType = Camera.DestinationType;

/**
 * Encoding of image returned from getPicture.
 *
 * Example: navigator.camera.getPicture(success, fail,
 *              { quality: 80,
 *                destinationType: Camera.DestinationType.DATA_URL,
 *                sourceType: Camera.PictureSourceType.CAMERA,
 *                encodingType: Camera.EncodingType.PNG})
*/
Camera.EncodingType = {
    JPEG: 0,                    // Return JPEG encoded image
    PNG: 1                      // Return PNG encoded image
};
Camera.prototype.EncodingType = Camera.EncodingType;

/**
 * Source to getPicture from.
 *
 * Example: navigator.camera.getPicture(success, fail,
 *              { quality: 80,
 *                destinationType: Camera.DestinationType.DATA_URL,
 *                sourceType: Camera.PictureSourceType.PHOTOLIBRARY})
 */
Camera.PictureSourceType = {
    PHOTOLIBRARY : 0,           // Choose image from picture library (same as SAVEDPHOTOALBUM for Android)
    CAMERA : 1,                 // Take picture from camera
    SAVEDPHOTOALBUM : 2         // Choose image from picture library (same as PHOTOLIBRARY for Android)
};
Camera.prototype.PictureSourceType = Camera.PictureSourceType;

/**
 * Gets a picture from source defined by "options.sourceType", and returns the
 * image as defined by the "options.destinationType" option.

 * The defaults are sourceType=CAMERA and destinationType=DATA_URL.
 *
 * @param {Function} successCallback
 * @param {Function} errorCallback
 * @param {Object} options
 \code
 navigator.camera.getPicture(onSuccess, onFail, { quality: 50 });

function onSuccess(imageData) {
    var image = document.getElementById('myImage');
    image.src = "data:image/jpeg;base64," + imageData;
}

function onFail(message) {
    alert('Failed because: ' + message);
}
 \endcode
 */
Camera.prototype.getPicture = function(successCallback, errorCallback, options) {
    console.log("Camera.prototype.getPicture");
    // successCallback required
    if (typeof successCallback !== "function") {
        console.log("Camera Error: successCallback is not a function");
        return;
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Camera Error: errorCallback is not a function");
        return;
    }

    this.options = options;

// TODO: This is duplicate - default values initialization exists in native C# code
//    var quality = 80;
//    if (options.quality) {
//        quality = this.options.quality;
//    }
//
//    var maxResolution = 0;
//    if (options.maxResolution) {
//        maxResolution = this.options.maxResolution;
//    }
//
//    var destinationType = Camera.DestinationType.DATA_URL;
//    if (this.options.destinationType) {
//        destinationType = this.options.destinationType;
//    }
//    var sourceType = Camera.PictureSourceType.CAMERA;
//    if (typeof this.options.sourceType === "number") {
//        sourceType = this.options.sourceType;
//    }
//    var encodingType = Camera.EncodingType.JPEG;
//    if (typeof options.encodingType == "number") {
//        encodingType = this.options.encodingType;
//    }
//
//    var targetWidth = -1;
//    if (typeof options.targetWidth == "number") {
//        targetWidth = options.targetWidth;
//    } else if (typeof options.targetWidth == "string") {
//        var width = new Number(options.targetWidth);
//        if (isNaN(width) === false) {
//            targetWidth = width.valueOf();
//        }
//    }

//    var targetHeight = -1;
//    if (typeof options.targetHeight == "number") {
//        targetHeight = options.targetHeight;
//    } else if (typeof options.targetHeight == "string") {
//        var height = new Number(options.targetHeight);
//        if (isNaN(height) === false) {
//            targetHeight = height.valueOf();
//        }
//    }

    PhoneGap.exec(successCallback, errorCallback, "Camera", "getPicture", this.options);
};

PhoneGap.addConstructor(function() {
    if (typeof navigator.camera === "undefined") {
        navigator.camera = new Camera();
    }
});
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("capture")) {
PhoneGap.addResource("capture");

/**
 * Represents a single file.
 *
 * name {DOMString} name of the file, without path information
 * fullPath {DOMString} the full path of the file, including the name
 * type {DOMString} mime type
 * lastModifiedDate {Date} last modified date
 * size {Number} size of the file in bytes
 */
var MediaFile = function(name, fullPath, type, lastModifiedDate, size){
    this.name = name || null;
    this.fullPath = fullPath || null;
    this.type = type || null;
    this.lastModifiedDate = lastModifiedDate || null;
    this.size = size || 0;
};

/**
 * Get file meta information
 *
 * @param {Function} successCB
 * @param {Function} errorCB
 */
MediaFile.prototype.getFormatData = function(successCallback, errorCallback){
    PhoneGap.exec(successCallback, errorCallback, "Capture", "getFormatData", {fullPath: this.fullPath, type: this.type});
};


/**
 * Open file in device media player
 *
 * @param {Function} successCB
 * @param {Function} errorCB
 */
MediaFile.prototype.play = function(successCallback, errorCallback){
    PhoneGap.exec(successCallback, errorCallback, "Capture", "play", this);
};


/**
 * MediaFileData encapsulates format information of a media file.
 *
 * @param {DOMString} codecs
 * @param {long} bitrate
 * @param {long} height
 * @param {long} width
 * @param {float} duration
 */
var MediaFileData = function(codecs, bitrate, height, width, duration){
    this.codecs = codecs || null;
    this.bitrate = bitrate || 0;
    this.height = height || 0;
    this.width = width || 0;
    this.duration = duration || 0;
};

/**
 * The CaptureError interface encapsulates all errors in the Capture API.
 */
var CaptureError = function(){
    this.code = null;
};

// Capture error codes
CaptureError.CAPTURE_INTERNAL_ERR = 0;
CaptureError.CAPTURE_APPLICATION_BUSY = 1;
CaptureError.CAPTURE_INVALID_ARGUMENT = 2;
CaptureError.CAPTURE_NO_MEDIA_FILES = 3;
CaptureError.CAPTURE_NOT_SUPPORTED = 20;

/**
 * The Capture interface exposes an interface to the camera and microphone of the hosting device.
 */
var Capture = function(){
    this.supportedAudioModes = [];
    this.supportedImageModes = [];
    this.supportedVideoModes = [];
};

/**
 * Launch audio recorder application for recording audio clip(s).
 *
 * @param {Function} successCB
 * @param {Function} errorCB
 * @param {CaptureAudioOptions} options
 \code
 // capture callback
var captureSuccess = function(mediaFiles) {
    var i, path, len;
    for (i = 0, len = mediaFiles.length; i < len; i += 1) {
        path = mediaFiles[i].fullPath;
        // do something interesting with the file
    }
};

// capture error callback
var captureError = function(error) {
    navigator.notification.alert('Error code: ' + error.code, null, 'Capture Error');
};

// start audio capture
navigator.device.capture.captureAudio(captureSuccess, captureError, {limit:2});
 \endcode
 */
Capture.prototype.captureAudio = function(successCallback, errorCallback, options){
    PhoneGap.exec(successCallback, errorCallback, "Capture", "captureAudio", options);
};

/**
 * Launch camera application for taking image(s).
 *
 * @param {Function} successCB
 * @param {Function} errorCB
 * @param {CaptureImageOptions} options
 \code
 // capture callback
var captureSuccess = function(mediaFiles) {
    var i, path, len;
    for (i = 0, len = mediaFiles.length; i < len; i += 1) {
        path = mediaFiles[i].fullPath;
        // do something interesting with the file
    }
};

// capture error callback
var captureError = function(error) {
    navigator.notification.alert('Error code: ' + error.code, null, 'Capture Error');
};

// start image capture
navigator.device.capture.captureImage(captureSuccess, captureError, {limit:2});
\endcode
 */
Capture.prototype.captureImage = function (successCallback, errorCallback, options) {
	PhoneGap.exec(	function(mediaFiles)
			{
				successCallback(Capture.prototype._castMediaFile(mediaFiles).message);
			},
			function(error)
			{
				errorCallback({code:CaptureError[error.code]});
			}, "Capture", "captureImage", options);
};

/**
 * Launch device camera application for recording video(s).
 *
 * @param {Function} successCB
 * @param {Function} errorCB
 * @param {CaptureVideoOptions} options
 \code
 // capture callback
var captureSuccess = function(mediaFiles) {
    var i, path, len;
    for (i = 0, len = mediaFiles.length; i < len; i += 1) {
        path = mediaFiles[i].fullPath;
        // do something interesting with the file
    }
};

// capture error callback
var captureError = function(error) {
    navigator.notification.alert('Error code: ' + error.code, null, 'Capture Error');
};

// start video capture
navigator.device.capture.captureVideo(captureSuccess, captureError, {limit:2});
 \endcode
 */
Capture.prototype.captureVideo = function(successCallback, errorCallback, options){
	PhoneGap.exec(	function(mediaFiles)
			{
				successCallback(Capture.prototype._castMediaFile(mediaFiles).message);
			},
			function(error)
			{
				errorCallback({code:CaptureError[error.code]});
			}, "Capture", "captureVideo", options);
};

/**
* This function returns and array of MediaFiles.  It is required as we need to convert raw
* JSON objects into MediaFile objects.
*/
Capture.prototype._castMediaFile = function(pluginResult){
    var mediaFiles = [];
    var i;
    for (i = 0; i < pluginResult.message.length; i++) {
        var mediaFile = new MediaFile();
        mediaFile.name = pluginResult.message[i].name;
        mediaFile.fullPath = pluginResult.message[i].fullPath;
        mediaFile.type = pluginResult.message[i].type;
        mediaFile.lastModifiedDate = pluginResult.message[i].lastModifiedDate;
        mediaFile.size = pluginResult.message[i].size;
        mediaFiles.push(mediaFile);
    }
    pluginResult.message = mediaFiles;
    return pluginResult;
};

/**
 * Encapsulates a set of parameters that the capture device supports.
 */
var ConfigurationData = function(){
    // The ASCII-encoded string in lower case representing the media type.
    this.type = null;
    // The height attribute represents height of the image or video in pixels.
    // In the case of a sound clip this attribute has value 0.
    this.height = 0;
    // The width attribute represents width of the image or video in pixels.
    // In the case of a sound clip this attribute has value 0
    this.width = 0;
};

/**
 * Encapsulates all image capture operation configuration options.
 */
var CaptureImageOptions = function(){
    // Upper limit of images user can take. Value must be equal or greater than 1.
    this.limit = 1;
    // The selected image mode. Must match with one of the elements in supportedImageModes array.
    this.mode = null;
};

/**
 * Encapsulates all video capture operation configuration options.
 */
var CaptureVideoOptions = function(){
    // Upper limit of videos user can record. Value must be equal or greater than 1.
    this.limit = 1;
    // Maximum duration of a single video clip in seconds.
    this.duration = 0;
    // The selected video mode. Must match with one of the elements in supportedVideoModes array.
    this.mode = null;
};

/**
 * Encapsulates all audio capture operation configuration options.
 */
var CaptureAudioOptions = function(){
    // Upper limit of sound clips user can record. Value must be equal or greater than 1.
    this.limit = 1;
    // Maximum duration of a single sound clip in seconds.
    this.duration = 0;
    // The selected audio mode. Must match with one of the elements in supportedAudioModes array.
    this.mode = null;
};

PhoneGap.addConstructor(function () {
    if (typeof navigator.device === "undefined") {
        navigator.device = window.device = new Device();
    }
    if (typeof navigator.device.capture === "undefined") {
        console.log("Installing capture");
        navigator.device.capture = window.device.capture = new Capture();
    }
});
}

/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("compass")) {
PhoneGap.addResource("compass");

/**
 * This class provides access to device Compass data.
 * @constructor
 */
var Compass = function() {
    /**
     * The last known Compass position.
     */
    this.lastHeading = null;
    this.isCompassSupported = true; // default assumption
};

Compass.ERROR_MSG = ["Not running", "Starting", "", "Failed to start", "Not Supported"];

/**
 * Asynchronously aquires the current heading.
 *
 * @param {Function} successCallback The function to call when the heading data is available
 * @param {Function} errorCallback The function to call when there is an error getting the heading data. (OPTIONAL)
 * @param {PositionOptions} options The options for getting the heading data such as timeout. (OPTIONAL)
 \code
 function onSuccess(heading) {
    alert('Heading: ' + heading.magneticHeading);
};

function onError(error) {
    alert('CompassError: ' error.code);
};

navigator.compass.getCurrentHeading(onSuccess, onError);
\endcode
 */
Compass.prototype.getCurrentHeading = function(successCallback, errorCallback, options) {

    // successCallback required
    if (typeof successCallback !== "function") {
        console.log("Compass Error: successCallback is not a function");
        return;
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Compass Error: errorCallback is not a function");
        //return;

        errorCallback = function(){};
    }

    if(this.isCompassSupported)
    {
        var self = this;
        var onSuccess = function(result)
        {
            var compassResult = JSON.parse(result);
            //console.log("compassResult = " + compassResult);
            self.lastHeading = compassResult;
            successCallback(self.lastHeading);
        }

        var onError = function(err)
        {
            if(err == 4)
            {
                self.isCompassSupported = false;
            }
            errorCallback(err);
        }

        // Get heading
        PhoneGap.exec(onSuccess, onError, "Compass", "getHeading", []);
    }
    else
    {
        var funk = function()
        {
            errorCallback(4);
        };
        window.setTimeout(funk,0);
    }
};

/**
 * Asynchronously aquires the heading repeatedly at a given interval.
 *
 * @param {Function} successCallback    The function to call each time the heading data is available
 * @param {Function} errorCallback      The function to call when there is an error getting the heading data. (OPTIONAL)
 * @param {HeadingOptions} options      The options for getting the heading data such as timeout and the frequency of the watch. (OPTIONAL)
 * @return String                       The watch id that must be passed to #clearWatch to stop watching.
 \code
 function onSuccess(heading) {
    var element = document.getElementById('heading');
    element.innerHTML = 'Heading: ' + heading.magneticHeading;
};

function onError(compassError) {
        alert('Compass error: ' + compassError.code);
};

var options = { frequency: 3000 };  // Update every 3 seconds

var watchID = navigator.compass.watchHeading(onSuccess, onError, options);
 \endcode
 */
Compass.prototype.watchHeading= function(successCallback, errorCallback, options) {

    // successCallback required
    if (typeof successCallback !== "function") {
        console.log("Compass Error: successCallback is not a function");
        return -1; // in case caller later calls clearWatch with this id
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Compass Error: errorCallback is not a function");
        return -1; // in case caller later calls clearWatch with this id
    }

    var id;
    if(this.isCompassSupported)
	{
		id = PhoneGap.createUUID();
		var onSuccess = function(result)
        {
            var compassResult = JSON.parse(result);
            //console.log("compassResult = " + compassResult);
            self.lastHeading = compassResult;
            successCallback(self.lastHeading);
        }

        var onError = function(err)
        {
            if(err == 4)
            {
                self.isCompassSupported = false;
            }
            errorCallback(err);
        }
		PhoneGap.exec(onSuccess, onError, "Compass", "startWatch", {id: id});
    }
    else
    {
        var funk = function()
        {
            errorCallback(4);
        };
        window.setTimeout(funk,0);
        return -1;
    }
    return id;
};


/**
 * Clears the specified heading watch.
 *
 * @param {String} id       The ID of the watch returned from #watchHeading.
 \code
 var watchID = navigator.compass.watchHeading(onSuccess, onError, options);

// ... later on ...

navigator.compass.clearWatch(watchID);
 \endcode
 */
Compass.prototype.clearWatch = function(id) {

    PhoneGap.exec(null, null, "Compass", "stopWatch", {id: id});

};

PhoneGap.addConstructor(
function()
{
    if (!navigator.compass)
    {
        navigator.compass = new Compass();
    }
});
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("contact")) {
PhoneGap.addResource("contact");

/**
* Contains information about a single contact.
* @constructor
* @param {DOMString} id unique identifier
* @param {DOMString} displayName
* @param {ContactName} name
* @param {DOMString} nickname
* @param {Array.<ContactField>} phoneNumbers array of phone numbers
* @param {Array.<ContactField>} emails array of email addresses
* @param {Array.<ContactAddress>} addresses array of addresses
* @param {Array.<ContactField>} ims instant messaging user ids
* @param {Array.<ContactOrganization>} organizations
* @param {DOMString} birthday contact's birthday
* @param {DOMString} note user notes about contact
* @param {Array.<ContactField>} photos
* @param {Array.<ContactField>} categories
* @param {Array.<ContactField>} urls contact's web sites
*/
var Contact = function (id, displayName, name, nickname, phoneNumbers, emails, addresses,
    ims, organizations, birthday, note, photos, categories, urls) {
    this.id = id || null;
    this.rawId = null;
    this.displayName = displayName || null;
    this.name = name || null; // ContactName
    this.nickname = nickname || null;
    this.phoneNumbers = phoneNumbers || null; // ContactField[]
    this.emails = emails || null; // ContactField[]
    this.addresses = addresses || null; // ContactAddress[]
    this.ims = ims || null; // ContactField[]
    this.organizations = organizations || null; // ContactOrganization[]
    this.birthday = birthday || null;
    this.note = note || null;
    this.photos = photos || null; // ContactField[]
    this.categories = categories || null; // ContactField[]
    this.urls = urls || null; // ContactField[]
};

/**
 *  ContactError.
 *  An error code assigned by an implementation when an error has occurreds
 * @constructor
 */
var ContactError = function(errCode) {
    this.code=errCode;
};

/**
 * Error codes
 */
ContactError.UNKNOWN_ERROR = 0;
ContactError.INVALID_ARGUMENT_ERROR = 1;
ContactError.TIMEOUT_ERROR = 2;
ContactError.PENDING_OPERATION_ERROR = 3;
ContactError.IO_ERROR = 4;
ContactError.NOT_SUPPORTED_ERROR = 5;
ContactError.PERMISSION_DENIED_ERROR = 20;

/**
* Removes contact from device storage.
* @param successCB success callback
* @param errorCB error callback
*/
Contact.prototype.remove = function(successCB, errorCB)
{
    if (!this.id)
    {
        var errorObj = new ContactError(ContactError.UNKNOWN_ERROR);
        setTimeout(function(){
        errorCB(errorObj);
        },0);
        return ContactError.UNKNOWN_ERROR;
    }
    else
    {
        PhoneGap.exec(successCB, errorCB, "Contacts", "remove",this.id);
    }
};

/**
* Creates a deep copy of this Contact.
* With the contact ID set to null.
* @return copy of this Contact
*/
Contact.prototype.clone = function() {
    var clonedContact = PhoneGap.safeClone(this);
    var i;
    clonedContact.id = null;
    clonedContact.rawId = null;
    // Loop through and clear out any id's in phones, emails, etc.
    var myArrayProps = ["phoneNumbers","emails","addresses","ims","organizations","tags","photos","urls"];

    for(var n=0, pLen=myArrayProps.length;n < pLen; n++)
    {
        var arr = clonedContact[myArrayProps[n]];
        if (arr && arr.length)
        {
            for(var i=0,len=arr.length; i<len;i++)
            {
                arr[i].id = null;
            }
        }
    }
    return clonedContact;
};

/**
* Persists contact to device storage.
* @param successCB success callback
* @param errorCB error callback
*/
Contact.prototype.save = function(successCB, errorCB)
{
    var self = this;
    function onSuccess(res)
    {
        setTimeout(function()
        {
            successCB(self);
        },0);
    }
    PhoneGap.exec(onSuccess, errorCB, "Contacts", "save", this);
};

/**
* Contact name.
* @constructor
* @param formatted
* @param familyName
* @param givenName
* @param middle
* @param prefix
* @param suffix
*/
var ContactName = function(formatted, familyName, givenName, middle, prefix, suffix) {
    this.formatted = formatted || null;
    this.familyName = familyName || null;
    this.givenName = givenName || null;
    this.middleName = middle || null;
    this.honorificPrefix = prefix || null;
    this.honorificSuffix = suffix || null;
};

/**
* Generic contact field.
* @constructor
* @param {DOMString} id unique identifier, should only be set by native code
* @param type
* @param value
* @param pref
*/
var ContactField = function(type, value, pref) {
    this.id = null;
    this.type = type || null;
    this.value = value || null;
    this.pref = pref || null;
};

/**
* Contact address.
* @constructor
* @param {DOMString} id unique identifier, should only be set by native code
* @param formatted
* @param streetAddress
* @param locality
* @param region
* @param postalCode
* @param country
*/
var ContactAddress = function(pref, type, formatted, streetAddress, locality, region, postalCode, country) {
    this.id = null;
    this.pref = pref || null;
    this.type = type || null;
    this.formatted = formatted || null;
    this.streetAddress = streetAddress || null;
    this.locality = locality || null;
    this.region = region || null;
    this.postalCode = postalCode || null;
    this.country = country || null;
};

/**
* Contact organization.
* @constructor
* @param {DOMString} id unique identifier, should only be set by native code
* @param name
* @param dept
* @param title
* @param startDate
* @param endDate
* @param location
* @param desc
*/
var ContactOrganization = function(pref, type, name, dept, title) {
    this.id = null;
    this.pref = pref || null;
    this.type = type || null;
    this.name = name || null;
    this.department = dept || null;
    this.title = title || null;
};

/**
* Represents a group of Contacts.
* @constructor
*/
var Contacts = function() {
    this.inProgress = false;
    this.records = [];
};
/**
* Returns an array of Contacts matching the search criteria.
* @param fields that should be searched
* @param successCB success callback
* @param errorCB error callback
* @param {ContactFindOptions} options that can be applied to contact searching
* @return array of Contacts matching search criteria
\code
function onSuccess(contacts) {
    alert('Found ' + contacts.length + ' contacts.');
};

function onError(contactError) {
    alert('onError!');
};

// find all contacts with 'Bob' in any name field
var options = new ContactFindOptions();
options.filter="Bob";
var fields = ["displayName", "name"];
navigator.contacts.find(fields, onSuccess, onError, options);
\endcode
*/
Contacts.prototype.find = function(fields, successCB, errorCB, options) {
    if (successCB === null) {
        throw new TypeError("You must specify a success callback for the find command.");
    }
    if (fields === null || fields === "undefined" || fields.length === "undefined" || fields.length <= 0) {
        if (typeof errorCB === "function")
        {
            // escape this scope before we call the errorCB
            setTimeout(function() {
            errorCB({"code": ContactError.INVALID_ARGUMENT_ERROR});
            },0);
        }
        console.log("Contacts.find::ContactError::INVALID_ARGUMENT_ERROR");
    }
    else
    {
        var onSuccess = function(res)
        {
            setTimeout(function()
            {
                successCB(res);
            },0);
        }
        PhoneGap.exec(onSuccess, errorCB, "Contacts", "search", {"fields":fields,"options":options});
    }
};

/**
* This function creates a new contact, but it does not persist the contact
* to device storage. To persist the contact to device storage, invoke
* contact.save().
* @param properties an object who's properties will be examined to create a new Contact
* @returns new Contact object
\code
var myContact = navigator.contacts.create({"displayName": "Test User"});
\endcode
*/
Contacts.prototype.create = function(properties) {
    var i;
    var contact = new Contact();
    for (i in properties) {
        if (contact[i] !== 'undefined') {
            contact[i] = properties[i];
        }
    }
    return contact;
};

/**
* This function returns and array of contacts.  It is required as we need to convert raw
* JSON objects into concrete Contact objects.  Currently this method is called after
* navigator.contacts.find but before the find methods success call back.
*
* @param jsonArray an array of JSON Objects that need to be converted to Contact objects.
* @returns an array of Contact objects
*/
Contacts.prototype.cast = function(pluginResult) {
    var contacts = [];
    var i;
    for (i=0; i<pluginResult.message.length; i++) {
        contacts.push(navigator.contacts.create(pluginResult.message[i]));
    }
    pluginResult.message = contacts;
    return pluginResult;
};

/**
 * ContactFindOptions.
 * @constructor
 * @param filter used to match contacts against
 * @param multiple boolean used to determine if more than one contact should be returned
 */
var ContactFindOptions = function(filter, multiple) {
    this.filter = filter || '';
    this.multiple = multiple || false;
};

/*
 * Add the contact interface into the browser.
 */
PhoneGap.addConstructor(function() {
    if(typeof navigator.contacts === "undefined") {
        navigator.contacts = new Contacts();
    }
});
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("device")) {
PhoneGap.addResource("device");

/**
 * This represents the mobile device, and provides properties for inspecting the model, version, UUID of the
 * phone, etc.
 * @constructor
 */
var Device = function() {
    this.available = PhoneGap.available;
    this.platform = null;
    this.version = null;
    this.name = null;
    this.uuid = null;
    this.phonegap = null;

    var me = this;
    this.getInfo(
        function (res) {
            var info = JSON.parse(res);
            console.log("GotDeviceInfo :: " + info.version);
            me.available = true;
            me.platform = info.platform;
            me.version = info.version;
            me.name = info.name;
            me.uuid = info.uuid;
            me.phonegap = info.phonegap;

            PhoneGap.onPhoneGapInfoReady.fire();
        },
        function(e) {
            me.available = false;
            console.log("Error initializing PhoneGap: " + e);
        });
};

/**
 * Get device info
 *
 * @param {Function} successCallback The function to call when the heading data is available
 * @param {Function} errorCallback The function to call when there is an error getting the heading data. (OPTIONAL)
 */
Device.prototype.getInfo = function(successCallback, errorCallback) {

    // successCallback required
    if (typeof successCallback !== "function") {
        console.log("Device Error: successCallback is not a function");
        return;
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Device Error: errorCallback is not a function");
        return;
    }

    // Get info
    PhoneGap.exec(successCallback, errorCallback, "Device", "Get");
};

PhoneGap.addConstructor(function() {
    if (typeof navigator.device === "undefined") {
        navigator.device = window.device = new Device();
    }
});
}

// this is a WP7 Only implementation of the Storage API for use in webpages loaded from the local file system
// inside phonegap application.
// there is a native implementation which is backing this and providing the persistance of values.
// webpages loaded from a domain will not need to use this as IE9 has support for WebStorage
// Javascript Interface is as defined here : http://dev.w3.org/html5/webstorage/#storage-0
//

//TODO: Replace it with another platform for MoSync
if(!window.localStorage)
{(function()
{
    "use strict";

    var DOMStorage = function(type)
    {
        // default type is local
        if(type == "sessionStorage")
        {
            this._type = type;
        }
        Object.defineProperty( this, "length",
        {
            configurable: true,
            get: function(){ return this.getLength() }
        });

    };

    DOMStorage.prototype =
    {
        _type:"localStorage",
        _result:null,
        keys:null,

        onResult:function(key,valueStr)
        {
            if(!this.keys)
            {
                this.keys = [];
            }
            this._result = valueStr;
        },

        onKeysChanged:function(jsonKeys)
        {
            this.keys = JSON.parse(jsonKeys);

            var key;
            for(var n = 0,len =this.keys.length; n < len; n++)
            {
                key = this.keys[n];
                if(!this.hasOwnProperty(key))
                {
                    console.log("didn't have a prop, now we do ...");
                    Object.defineProperty( this, key,
                    {

                        configurable: true,
                        get: function(){ return this.getItem(key); },
                        set: function(val){ return this.setItem(key,val); }
                    });
                }
            }

        },

        initialize:function()
        {
            window.external.Notify("DOMStorage/" + this._type + "/load/keys");
        },

    /*
        The length attribute must return the number of key/value pairs currently present in the list associated with the object.
    */
        getLength:function()
        {
            if(!this.keys)
            {
                this.initialize();
            }
            return this.keys.length;
        },

    /*
        The key(n) method must return the name of the nth key in the list.
        The order of keys is user-agent defined, but must be consistent within an object so long as the number of keys doesn't change.
        (Thus, adding or removing a key may change the order of the keys, but merely changing the value of an existing key must not.)
        If n is greater than or equal to the number of key/value pairs in the object, then this method must return null.
    */
        key:function(n)
        {
            if(!this.keys)
            {
                this.initialize();
            }

            if(n >= this.keys.length)
            {
                return null;
            }
            else
            {
                return this.keys[n];
            }
        },

    /*
        The getItem(key) method must return the current value associated with the given key.
        If the given key does not exist in the list associated with the object then this method must return null.
    */
        getItem:function(key)
        {
            if(!this.keys)
            {
                this.initialize();
            }

            var retVal = null;
            if(this.keys.indexOf(key) > -1)
            {
                window.external.Notify("DOMStorage/" + this._type + "/get/" + key);
                retVal = this._result;
                this._result = null;
            }
            return retVal;
        },
    /*
        The setItem(key, value) method must first check if a key/value pair with the given key already exists
        in the list associated with the object.
        If it does not, then a new key/value pair must be added to the list, with the given key and with its value set to value.
        If the given key does exist in the list, then it must have its value updated to value.
        If it couldn't set the new value, the method must raise an QUOTA_EXCEEDED_ERR exception.
        (Setting could fail if, e.g., the user has disabled storage for the site, or if the quota has been exceeded.)
    */
        setItem:function(key,value)
        {
            if(!this.keys)
            {
                this.initialize();
            }
            window.external.Notify("DOMStorage/" + this._type + "/set/" + key + "/" + value);
        },

    /*
        The removeItem(key) method must cause the key/value pair with the given key to be removed from the list
        associated with the object, if it exists.
        If no item with that key exists, the method must do nothing.
    */
        removeItem:function(key)
        {
            if(!this.keys)
            {
                this.initialize();
            }
            var index = this.keys.indexOf(key);
            if(index > -1)
            {
                this.keys.splice(index,1);
                // TODO: need sanity check for keys ? like 'clear','setItem', ...
                window.external.Notify("DOMStorage/" + this._type + "/remove/" + key);
                delete this[key];
            }

        },

    /*
        The clear() method must atomically cause the list associated with the object to be emptied of all
        key/value pairs, if there are any.
        If there are none, then the method must do nothing.
    */
        clear:function()
        {
            if(!this.keys)
            {
                this.initialize();
            }

            for(var n=0,len=this.keys.length; n < len;n++)
            {
                // TODO: do we need a sanity check for keys ? like 'clear','setItem', ...
                delete this[this.keys[n]];
            }
            this.keys = [];
          //  window.external.Notify("DOMStorage/" + this._type + "/clear/");
        }
    };
})();};

/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("file")) {
PhoneGap.addResource("file");

/**
 * Represents a single file.
 *
 * @constructor
 * @param name {DOMString} name of the file, without path information
 * @param fullPath {DOMString} the full path of the file, including the name
 * @param type {DOMString} mime type
 * @param lastModifiedDate {Date} last modified date
 * @param size {Number} size of the file in bytes
 */
var File = function(name, fullPath, type, lastModifiedDate, size) {
    this.name = name || null;
    this.fullPath = fullPath || null;
    this.type = type || null;
    this.lastModifiedDate = lastModifiedDate || null;
    this.size = size || 0;
};

/** @constructor */
var FileError = function() {
   this.code = null;
};

// File error codes
// Found in DOMException
FileError.NOT_FOUND_ERR = 1;
FileError.SECURITY_ERR = 2;
FileError.ABORT_ERR = 3;

// Added by this specification
FileError.NOT_READABLE_ERR = 4;
FileError.ENCODING_ERR = 5;
FileError.NO_MODIFICATION_ALLOWED_ERR = 6;
FileError.INVALID_STATE_ERR = 7;
FileError.SYNTAX_ERR = 8;
FileError.INVALID_MODIFICATION_ERR = 9;
FileError.QUOTA_EXCEEDED_ERR = 10;
FileError.TYPE_MISMATCH_ERR = 11;
FileError.PATH_EXISTS_ERR = 12;

//-----------------------------------------------------------------------------
// File manager
//-----------------------------------------------------------------------------

/**
 * @constructor
 */
var FileMgr = function() {
};

FileMgr.prototype.getFileBasePaths = function() {
};

FileMgr.prototype.testFileExists = function(fileName, successCallback, errorCallback) {
    return PhoneGap.exec(successCallback, errorCallback, "File", "testFileExists", {fileName: fileName});
};

FileMgr.prototype.testDirectoryExists = function(dirName, successCallback, errorCallback) {
    return PhoneGap.exec(successCallback, errorCallback, "File", "testDirectoryExists", {dirName: dirName});
};

FileMgr.prototype.getFreeDiskSpace = function(successCallback, errorCallback) {
    return PhoneGap.exec(successCallback, errorCallback, "File", "getFreeDiskSpace");
};

FileMgr.prototype.write = function(fileName, data, position, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "write", {fileName: fileName, data: data, position: position});
};

FileMgr.prototype.truncate = function(fileName, size, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "truncate", {fileName: fileName, size: size});
};

FileMgr.prototype.readAsText = function(fileName, encoding, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "readAsText", {fileName: fileName, encoding: encoding});
};

FileMgr.prototype.readAsDataURL = function(fileName, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "readAsDataURL", {fileName: fileName});
};

PhoneGap.addConstructor(function() {
    if (typeof navigator.fileMgr === "undefined") {
        navigator.fileMgr = new FileMgr();
    }
});

//-----------------------------------------------------------------------------
// File Reader
//-----------------------------------------------------------------------------
// TODO: All other FileMgr function operate on the SD card as root.  However,
//       for FileReader & FileWriter the root is not SD card.  Should this be changed?

/**
 * This class reads the mobile device file system.
 *
 * For Android:
 *      The root directory is the root of the file system.
 *      To read from the SD card, the file name is "sdcard/my_file.txt"
 * @constructor
 */
var FileReader = function() {
    this.fileName = "";

    this.readyState = 0;

    // File data
    this.result = null;

    // Error
    this.error = null;

    // Event handlers
    this.onloadstart = null;    // When the read starts.
    this.onprogress = null;     // While reading (and decoding) file or fileBlob data, and reporting partial file data (progess.loaded/progress.total)
    this.onload = null;         // When the read has successfully completed.
    this.onerror = null;        // When the read has failed (see errors).
    this.onloadend = null;      // When the request has completed (either in success or failure).
    this.onabort = null;        // When the read has been aborted. For instance, by invoking the abort() method.
};

// States
FileReader.EMPTY = 0;
FileReader.LOADING = 1;
FileReader.DONE = 2;

/**
 * Abort reading file.
 */
FileReader.prototype.abort = function() {
    var evt;
    this.readyState = FileReader.DONE;
    this.result = null;

    // set error
    var error = new FileError();
    error.code = error.ABORT_ERR;
    this.error = error;

    // If error callback
    if (typeof this.onerror === "function") {
        this.onerror({"type":"error", "target":this});
    }
    // If abort callback
    if (typeof this.onabort === "function") {
        this.onabort({"type":"abort", "target":this});
    }
    // If load end callback
    if (typeof this.onloadend === "function") {
        this.onloadend({"type":"loadend", "target":this});
    }
};

/**
 * Read text file.
 *
 * @param file          {File} File object containing file properties
 * @param encoding      [Optional] (see http://www.iana.org/assignments/character-sets)
 */
FileReader.prototype.readAsText = function(file, encoding) {
    this.fileName = "";
    if (typeof file.fullPath === "undefined") {
        this.fileName = file;
    } else {
        this.fileName = file.fullPath;
    }

    // LOADING state
    this.readyState = FileReader.LOADING;

    // If loadstart callback
    if (typeof this.onloadstart === "function") {
        this.onloadstart({"type":"loadstart", "target":this});
    }

    // Default encoding is UTF-8
    var enc = encoding ? encoding : "UTF-8";

    var me = this;

    // Read file
    navigator.fileMgr.readAsText(this.fileName, enc,

        // Success callback
        function(r) {
            var evt;

            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileReader.DONE) {
                return;
            }

            // Save result
            me.result = r;

            // If onload callback
            if (typeof me.onload === "function") {
                me.onload({"type":"load", "target":me});
            }

            // DONE state
            me.readyState = FileReader.DONE;

            // If onloadend callback
            if (typeof me.onloadend === "function") {
                me.onloadend({"type":"loadend", "target":me});
            }
        },

        // Error callback
        function(e) {
            var evt;
            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileReader.DONE) {
                return;
            }

            // Save error
            me.error = e;

            // If onerror callback
            if (typeof me.onerror === "function") {
                me.onerror({"type":"error", "target":me});
            }

            // DONE state
            me.readyState = FileReader.DONE;

            // If onloadend callback
            if (typeof me.onloadend === "function") {
                me.onloadend({"type":"loadend", "target":me});
            }
        }
        );
};


/**
 * Read file and return data as a base64 encoded data url.
 * A data url is of the form:
 *      data:[<mediatype>][;base64],<data>
 *
 * @param file          {File} File object containing file properties
 */
FileReader.prototype.readAsDataURL = function(file) {
    this.fileName = "";
    if (typeof file.fullPath === "undefined") {
        this.fileName = file;
    } else {
        this.fileName = file.fullPath;
    }

    // LOADING state
    this.readyState = FileReader.LOADING;

    // If loadstart callback
    if (typeof this.onloadstart === "function") {
        this.onloadstart({"type":"loadstart", "target":this});
    }

    var me = this;

    // Read file
    navigator.fileMgr.readAsDataURL(this.fileName,

        // Success callback
        function(r) {
            var evt;

            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileReader.DONE) {
                return;
            }

            // Save result
            me.result = r;

            // If onload callback
            if (typeof me.onload === "function") {
                me.onload({"type":"load", "target":me});
            }

            // DONE state
            me.readyState = FileReader.DONE;

            // If onloadend callback
            if (typeof me.onloadend === "function") {
                me.onloadend({"type":"loadend", "target":me});
            }
        },

        // Error callback
        function(e) {
            var evt;
            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileReader.DONE) {
                return;
            }

            // Save error
            me.error = e;

            // If onerror callback
            if (typeof me.onerror === "function") {
                me.onerror({"type":"error", "target":me});
            }

            // DONE state
            me.readyState = FileReader.DONE;

            // If onloadend callback
            if (typeof me.onloadend === "function") {
                me.onloadend({"type":"loadend", "target":me});
            }
        }
        );
};

/**
 * Read file and return data as a binary data.
 *
 * @param file          {File} File object containing file properties
 */
FileReader.prototype.readAsBinaryString = function(file) {
    // TODO - Can't return binary data to browser.
    this.fileName = file;
};

/**
 * Read file and return data as a binary data.
 *
 * @param file          {File} File object containing file properties
 */
FileReader.prototype.readAsArrayBuffer = function(file) {
    // TODO - Can't return binary data to browser.
    this.fileName = file;
};

//-----------------------------------------------------------------------------
// File Writer
//-----------------------------------------------------------------------------

/**
 * This class writes to the mobile device file system.
 *
 * For Android:
 *      The root directory is the root of the file system.
 *      To write to the SD card, the file name is "sdcard/my_file.txt"
 *
 * @constructor
 * @param file {File} File object containing file properties
 * @param append if true write to the end of the file, otherwise overwrite the file
 */
var FileWriter = function (file) {
    this.fileName = "";
    this.length = 0;
    if (file) {
        this.fileName = file.fullPath || file;
        this.length = file.size || 0;
    }
    // default is to write at the beginning of the file
    this.position = 0;

    this.readyState = 0; // EMPTY

    this.result = null;

    // Error
    this.error = null;

    // Event handlers
    this.onwritestart = null; // When writing starts
    this.onprogress = null;     // While writing the file, and reporting partial file data
    this.onwrite = null;     // When the write has successfully completed.
    this.onwriteend = null;     // When the request has completed (either in success or failure).
    this.onabort = null;     // When the write has been aborted. For instance, by invoking the abort() method.
    this.onerror = null;     // When the write has failed (see errors).
};

// States
FileWriter.INIT = 0;
FileWriter.WRITING = 1;
FileWriter.DONE = 2;

/**
 * Abort writing file.
 */
FileWriter.prototype.abort = function() {
    // check for invalid state
    if (this.readyState === FileWriter.DONE || this.readyState === FileWriter.INIT) {
        throw FileError.INVALID_STATE_ERR;
    }

    // set error
    var error = new FileError(), evt;
    error.code = error.ABORT_ERR;
    this.error = error;

    // If error callback
    if (typeof this.onerror === "function") {
        this.onerror({"type":"error", "target":this});
    }
    // If abort callback
    if (typeof this.onabort === "function") {
        this.onabort({"type":"abort", "target":this});
    }

    this.readyState = FileWriter.DONE;

    // If write end callback
    if (typeof this.onwriteend == "function") {
        this.onwriteend({"type":"writeend", "target":this});
    }
};

/**
 * Writes data to the file
 *
 * @param text to be written
 */
FileWriter.prototype.write = function (text) {
    // Throw an exception if we are already writing a file
    if (this.readyState === FileWriter.WRITING) {
        throw FileError.INVALID_STATE_ERR;
    }

    // WRITING state
    this.readyState = FileWriter.WRITING;

    var me = this;

    // If onwritestart callback
    if (typeof me.onwritestart === "function") {
        me.onwritestart({ "type": "writestart", "target": me });
    }

    // Write file
    navigator.fileMgr.write(this.fileName, text, this.position,

    // Success callback
        function (r) {
            var evt;
            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileWriter.DONE) {
                return;
            }

            // position always increases by bytes written because file would be extended
            me.position += r;
            // The length of the file is now where we are done writing.
            me.length = me.position;

            // If onwrite callback
            if (typeof me.onwrite === "function") {
                me.onwrite({ "type": "write", "target": me });
            }

            // DONE state
            me.readyState = FileWriter.DONE;

            // If onwriteend callback
            if (typeof me.onwriteend === "function") {
                me.onwriteend({ "type": "writeend", "target": me });
            }
        },

    // Error callback
        function (e) {
            var evt;

            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileWriter.DONE) {
                return;
            }

            // Save error
            me.error = e;

            // If onerror callback
            if (typeof me.onerror === "function") {
                me.onerror({ "type": "error", "target": me });
            }

            // DONE state
            me.readyState = FileWriter.DONE;

            // If onwriteend callback
            if (typeof me.onwriteend === "function") {
                me.onwriteend({ "type": "writeend", "target": me });
            }
        }
        );

};

/**
 * Moves the file pointer to the location specified.
 *
 * If the offset is a negative number the position of the file
 * pointer is rewound.  If the offset is greater than the file
 * size the position is set to the end of the file.
 *
 * @param offset is the location to move the file pointer to.
 */
FileWriter.prototype.seek = function(offset) {
    // Throw an exception if we are already writing a file
    if (this.readyState === FileWriter.WRITING) {
        throw FileError.INVALID_STATE_ERR;
    }
    //console.log("================== seek - offset: " + offset);
    //console.log("================== seek - this.length: " + this.length);
    if (!offset) {
        return;
    }

    // See back from end of file.
    if (offset < 0) {
        this.position = Math.max(offset + this.length, 0);
    }
    // Offset is bigger then file size so set position
    // to the end of the file.
    else if (offset > this.length) {
        this.position = this.length;
    }
    // Offset is between 0 and file size so set the position
    // to start writing.
    else {
        this.position = offset;
    }
};

/**
 * Truncates the file to the size specified.
 *
 * @param size to chop the file at.
 */
FileWriter.prototype.truncate = function(size) {
    // Throw an exception if we are already writing a file
    if (this.readyState === FileWriter.WRITING) {
        throw FileError.INVALID_STATE_ERR;
    }

    // WRITING state
    this.readyState = FileWriter.WRITING;

    var me = this;

    // If onwritestart callback
    if (typeof me.onwritestart === "function") {
        me.onwritestart({"type":"writestart", "target":this});
    }

    // Write file
    navigator.fileMgr.truncate(this.fileName, size,

        // Success callback
        function(r) {
            var evt;
            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileWriter.DONE) {
                return;
            }

            // Update the length of the file
            me.length = r;
            me.position = Math.min(me.position, r);

            // If onwrite callback
            if (typeof me.onwrite === "function") {
                me.onwrite({"type":"write", "target":me});
            }

            // DONE state
            me.readyState = FileWriter.DONE;

            // If onwriteend callback
            if (typeof me.onwriteend === "function") {
                me.onwriteend({"type":"writeend", "target":me});
            }
        },

        // Error callback
        function(e) {
            var evt;
            // If DONE (cancelled), then don't do anything
            if (me.readyState === FileWriter.DONE) {
                return;
            }

            // Save error
            me.error = e;

            // If onerror callback
            if (typeof me.onerror === "function") {
                me.onerror({"type":"error", "target":me});
            }

            // DONE state
            me.readyState = FileWriter.DONE;

            // If onwriteend callback
            if (typeof me.onwriteend === "function") {
                me.onwriteend({"type":"writeend", "target":me});
            }
        }
    );
};

/**
 * Information about the state of the file or directory
 *
 * @constructor
 * {Date} modificationTime (readonly)
 */
var Metadata = function() {
    this.modificationTime=null;
};

/**
 * Supplies arguments to methods that lookup or create files and directories
 *
 * @constructor
 * @param {boolean} create file or directory if it doesn't exist
 * @param {boolean} exclusive if true the command will fail if the file or directory exists
 */
var Flags = function(create, exclusive) {
    this.create = create || false;
    this.exclusive = exclusive || false;
};

/**
 * An interface representing a file system
 *
 * @constructor
 * {DOMString} name the unique name of the file system (readonly)
 * {DirectoryEntry} root directory of the file system (readonly)
 */
var FileSystem = function() {
    this.name = null;
    this.root = null;
};

/**
 * An interface that lists the files and directories in a directory.
 * @constructor
 */
var DirectoryReader = function(fullPath){
    this.fullPath = fullPath || null;
};

/**
 * Returns a list of entries from a directory.
 *
 * @param {Function} successCallback is called with a list of entries
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(entries) {
    var i;
    for (i=0; i<entries.length; i++) {
        console.log(entries[i].name);
    }
}

function fail(error) {
    alert("Failed to list directory contents: " + error.code);
}

// Get a directory reader
var directoryReader = dirEntry.createReader();

// Get a list of all the entries in the directory
directoryReader.readEntries(success,fail);
 \endcode
 */
DirectoryReader.prototype.readEntries = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "readEntries", {fullPath: this.fullPath});
};

/**
 * An interface representing a directory on the file system.
 *
 * @constructor
 * {boolean} isFile always false (readonly)
 * {boolean} isDirectory always true (readonly)
 * {DOMString} name of the directory, excluding the path leading to it (readonly)
 * {DOMString} fullPath the absolute full path to the directory (readonly)
 * {FileSystem} filesystem on which the directory resides (readonly)
 */
var DirectoryEntry = function() {
    this.isFile = false;
    this.isDirectory = true;
    this.name = null;
    this.fullPath = null;
    this.filesystem = null;
};

/**
 * Copies a directory to a new location
 *
 * @param {DirectoryEntry} parent the directory to which to copy the entry
 * @param {DOMString} newName the new name of the entry, defaults to the current name
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 \code
 function win(entry) {
    console.log("New Path: " + entry.fullPath);
}

function fail(error) {
    alert(error.code);
}

function copyDir(entry) {
    var parent = document.getElementById('parent').value,
        newName = document.getElementById('newName').value,
        parentEntry = new DirectoryEntry({fullPath: parent});

    // copy the directory to a new directory and rename it
    entry.copyTo(parentEntry, newName, success, fail);
}
 \endcode
 */
DirectoryEntry.prototype.copyTo = function(parent, newName, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "copyTo", {fullPath: this.fullPath, parent:parent, newName: newName});
};

/**
 * Looks up the metadata of the entry
 *
 * @param {Function} successCallback is called with a Metadata object
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(metadata) {
    console.log("Last Modified: " + metadata.modificationTime);
}

function fail(error) {
    alert(error.code);
}

// Request the metadata object for this entry
entry.getMetadata(success, fail);
\endcode
 */
DirectoryEntry.prototype.getMetadata = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getMetadata", {fullPath: this.fullPath});
};

/**
 * Gets the parent of the entry
 *
 * @param {Function} successCallback is called with a parent entry
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(parent) {
    console.log("Parent Name: " + parent.name);
}

function fail(error) {
    alert('Failed to get parent directory: ' + error.code);
}

// Get the parent DirectoryEntry
entry.getParent(success, fail);
\endcode
 */
DirectoryEntry.prototype.getParent = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getParent", {fullPath: this.fullPath});
};

/**
 * Moves a directory to a new location
 *
 * @param {DirectoryEntry} parent the directory to which to move the entry
 * @param {DOMString} newName the new name of the entry, defaults to the current name
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(entry) {
    console.log("New Path: " + entry.fullPath);
}

function fail(error) {
    alert(error.code);
}

function moveDir(entry) {
    var parent = document.getElementById('parent').value,
        newName = document.getElementById('newName').value,
        parentEntry = new DirectoryEntry({fullPath: parent});

    // move the directory to a new directory and rename it
    entry.moveTo(parentEntry, newName, success, fail);
}
 \endcode
 */
DirectoryEntry.prototype.moveTo = function(parent, newName, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "moveTo", {fullPath: this.fullPath, parent: parent, newName: newName});
};

/**
 * Removes the entry
 *
 * @param {Function} successCallback is called with no parameters
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(entry) {
    console.log("Removal succeeded");
}

function fail(error) {
    alert('Error removing directory: ' + error.code);
}

// remove this directory
entry.remove(success, fail);
\endcode
 */
DirectoryEntry.prototype.remove = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "remove", {fullPath: this.fullPath});
};

/**
 * Returns a URI that can be used to identify this entry.
 *
 * @param {DOMString} mimeType for a FileEntry, the mime type to be used to interpret the file, when loaded through this URI.
 * @return uri
 \code
 // Get the URI for this directory
var uri = entry.toURI();
console.log(uri);
\endcode
 */
DirectoryEntry.prototype.toURI = function(mimeType) {

    return encodeURI("file://" + this.fullPath);
};

/**
 * Creates a new DirectoryReader to read entries from this directory
 \code
 // create a directory reader
var directoryReader = entry.createReader();
 \endcode
 */
DirectoryEntry.prototype.createReader = function(successCallback, errorCallback) {
    return new DirectoryReader(this.fullPath);
};

/**
 * Creates or looks up a directory
 *
 * @param {DOMString} path either a relative or absolute path from this directory in which to look up or create a directory
 * @param {Flags} options to create or excluively create the directory
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(parent) {
    console.log("Parent Name: " + parent.name);
}

function fail(error) {
    alert("Unable to create new directory: " + error.code);
}

// Retrieve an existing directory, or create it if it does not already exist
entry.getDirectory("newDir", {create: true, exclusive: false}, success, fail);
\endcode
 */
DirectoryEntry.prototype.getDirectory = function (path, options, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getDirectory", { fullPath: this.fullPath, path: path, options: options });
};

/**
 * Creates or looks up a file
 *
 * @param {DOMString} path either a relative or absolute path from this directory in which to look up or create a file
 * @param {Flags} options to create or excluively create the file
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 \code
function success(parent) {
    console.log("Parent Name: " + parent.name);
}

function fail(error) {
    alert("Failed to retrieve file: " + error.code);
}

// Retrieve an existing file, or create it if it does not exist
entry.getFile("newFile.txt", {create: true, exclusive: false}, success, fail);
\endcode
 */
DirectoryEntry.prototype.getFile = function (path, options, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getFile", { fullPath: this.fullPath, path: path, options: options });
};

/**
 * Deletes a directory and all of it's contents
 *
 * @param {Function} successCallback is called with no parameters
 * @param {Function} errorCallback is called with a FileError
 \code
 function success(parent) {
    console.log("Remove Recursively Succeeded");
}

function fail(error) {
    alert("Failed to remove directory or it's contents: " + error.code);
}

// remove the directory and all it's contents
entry.removeRecursively(success, fail);
 \endcode
 */
DirectoryEntry.prototype.removeRecursively = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "removeRecursively", {fullPath: this.fullPath});
};

/**
 * An interface representing a directory on the file system.
 *
 * @constructor
 * {boolean} isFile always true (readonly)
 * {boolean} isDirectory always false (readonly)
 * {DOMString} name of the file, excluding the path leading to it (readonly)
 * {DOMString} fullPath the absolute full path to the file (readonly)
 * {FileSystem} filesystem on which the directory resides (readonly)
 */
var FileEntry = function() {
    this.isFile = true;
    this.isDirectory = false;
    this.name = null;
    this.fullPath = null;
    this.filesystem = null;
};

/**
 * Copies a file to a new location
 *
 * @param {DirectoryEntry} parent the directory to which to copy the entry
 * @param {DOMString} newName the new name of the entry, defaults to the current name
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.copyTo = function(parent, newName, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "copyTo", {fullPath: this.fullPath, parent: parent, newName: newName});
};

/**
 * Looks up the metadata of the entry
 *
 * @param {Function} successCallback is called with a Metadata object
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.getMetadata = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getMetadata", {fullPath: this.fullPath});
};

/**
 * Gets the parent of the entry
 *
 * @param {Function} successCallback is called with a parent entry
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.getParent = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getParent", {fullPath: this.fullPath});
};

/**
 * Moves a directory to a new location
 *
 * @param {DirectoryEntry} parent the directory to which to move the entry
 * @param {DOMString} newName the new name of the entry, defaults to the current name
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.moveTo = function(parent, newName, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "moveTo", {fullPath: this.fullPath, parent: parent, newName: newName});
};

/**
 * Removes the entry
 *
 * @param {Function} successCallback is called with no parameters
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.remove = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "remove", {fullPath: this.fullPath});
};

/**
 * Returns a URI that can be used to identify this entry.
 *
 * @param {DOMString} mimeType for a FileEntry, the mime type to be used to interpret the file, when loaded through this URI.
 * @return uri
 */
FileEntry.prototype.toURI = function(mimeType) {
    return encodeURI("file://" + this.fullPath);
};

/**
 * Creates a new FileWriter associated with the file that this FileEntry represents.
 *
 * @param {Function} successCallback is called with the new FileWriter
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.createWriter = function (successCallback, errorCallback) {
    this.file(function (filePointer) {
        var writer = new FileWriter(filePointer);

        if (writer.fileName === null || writer.fileName === "") {
            if (typeof errorCallback == "function") {
                errorCallback({
                    "code": FileError.INVALID_STATE_ERR
                });
            }
        }

        if (typeof successCallback == "function") {
            successCallback(writer);
        }
    }, errorCallback);
};

/**
 * Returns a File that represents the current state of the file that this FileEntry represents.
 *
 * @param {Function} successCallback is called with the new File object
 * @param {Function} errorCallback is called with a FileError
 */
FileEntry.prototype.file = function(successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "getFileMetadata", {fullPath: this.fullPath});
};

/**
 * @constructor
 */
var LocalFileSystem = function() {
};

// File error codes
LocalFileSystem.TEMPORARY = 0;
LocalFileSystem.PERSISTENT = 1;
LocalFileSystem.RESOURCE = 2;
LocalFileSystem.APPLICATION = 3;

/**
 * Requests a filesystem in which to store application data.
 *
 * @param {int} type of file system being requested
 * @param {Function} successCallback is called with the new FileSystem
 * @param {Function} errorCallback is called with a FileError
 \code
 function onSuccess(fileSystem) {
    console.log(fileSystem.name);
}

// request the persistent file system
window.requestFileSystem(LocalFileSystem.PERSISTENT, 0, onSuccess, onError);
 \endcode
 */
LocalFileSystem.prototype.requestFileSystem = function(type, size, successCallback, errorCallback) {
    if (type < 0 || type > 3) {
        if (typeof errorCallback == "function") {
            errorCallback({
                "code": FileError.SYNTAX_ERR
            });
        }
    }
    else {
        PhoneGap.exec(successCallback, errorCallback, "File", "requestFileSystem", {type: type, size: size});
    }
};

/**
 *
 * @param {DOMString} uri referring to a local file in a filesystem
 * @param {Function} successCallback is called with the new entry
 * @param {Function} errorCallback is called with a FileError
 \code
 function onSuccess(fileEntry) {
    console.log(fileEntry.name);
}

window.resolveLocalFileSystemURI("file:///example.txt", onSuccess, onError);
 \endcode
 */
LocalFileSystem.prototype.resolveLocalFileSystemURI = function(uri, successCallback, errorCallback) {
    PhoneGap.exec(successCallback, errorCallback, "File", "resolveLocalFileSystemURI", {uri: uri});
};

/**
* TODO: The following comment is misplaced (copy/paste error). The function is used
* by the file system API. Update with proper comment.
*
* This function returns and array of contacts.  It is required as we need to convert raw
* JSON objects into concrete Contact objects.  Currently this method is called after
* navigator.service.contacts.find but before the find methods success call back.
*
* @param a JSON Objects that need to be converted to DirectoryEntry or FileEntry objects.
* @returns an entry
*/
LocalFileSystem.prototype._castFS = function (pluginResult) {
    var entry = null;
    entry = new DirectoryEntry();
    entry.isDirectory = pluginResult.message.root.isDirectory;
    entry.isFile = pluginResult.message.root.isFile;
    entry.name = pluginResult.message.root.name;
    entry.fullPath = pluginResult.message.root.fullPath;
    pluginResult.message.root = entry;
    return pluginResult;
};

LocalFileSystem.prototype._castEntry = function(pluginResult) {
    var entry = null;
    if (pluginResult.message.isDirectory) {
        entry = new DirectoryEntry();
    }
    else if (pluginResult.message.isFile) {
        entry = new FileEntry();
    }
    entry.isDirectory = pluginResult.message.isDirectory;
    entry.isFile = pluginResult.message.isFile;
    entry.name = pluginResult.message.name;
    entry.fullPath = pluginResult.message.fullPath;
    pluginResult.message = entry;
    return pluginResult;
};

LocalFileSystem.prototype._castEntries = function(pluginResult) {
    var entries = pluginResult.message;
    var retVal = [];
    for (var i=0; i<entries.length; i++) {
        retVal.push(window.localFileSystem._createEntry(entries[i]));
    }
    pluginResult.message = retVal;
    return pluginResult;
};

LocalFileSystem.prototype._createEntry = function(castMe) {
    var entry = null;
    if (castMe.isDirectory) {
        entry = new DirectoryEntry();
    }
    else if (castMe.isFile) {
        entry = new FileEntry();
    }
    entry.isDirectory = castMe.isDirectory;
    entry.isFile = castMe.isFile;
    entry.name = castMe.name;
    entry.fullPath = castMe.fullPath;
    return entry;
};

/**
 * This function casts a File object, or a Date. The name is
 * confusing.
 * TODO: Rename?
 */
LocalFileSystem.prototype._castDate = function (pluginResult) {
    if (pluginResult.message.modificationTime) {
        var modTime = new Date(pluginResult.message.modificationTime);
        pluginResult.message.modificationTime = modTime;
    }
    else if (pluginResult.message.lastModifiedDate) {
        var file = new File();
        file.size = pluginResult.message.size;
        file.type = pluginResult.message.type;
        file.name = pluginResult.message.name;
        file.fullPath = pluginResult.message.fullPath;
        file.lastModifiedDate = new Date(pluginResult.message.lastModifiedDate);
        pluginResult.message = file;
    }
    return pluginResult;
};

/*
 * Add the FileSystem interface into the browser.
 */
PhoneGap.addConstructor(function () {
    var pgLocalFileSystem = new LocalFileSystem();
    // Needed for cast methods
    if(typeof window.localFileSystem == "undefined") window.localFileSystem  = pgLocalFileSystem;
    if(typeof window.requestFileSystem == "undefined") window.requestFileSystem  = pgLocalFileSystem.requestFileSystem;
    if(typeof window.resolveLocalFileSystemURI == "undefined") window.resolveLocalFileSystemURI = pgLocalFileSystem.resolveLocalFileSystemURI;
});
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("filetransfer")) {
PhoneGap.addResource("filetransfer");

/**
 * FileTransfer uploads a file to a remote server.
 * @constructor
 */
var FileTransfer = function() {};

/**
 * FileUploadResult
 * @constructor
 */
var FileUploadResult = function() {
    this.bytesSent = 0;
    this.responseCode = null;
    this.response = null;
};

/**
 * FileTransferError
 * @constructor
 */
var FileTransferError = function() {
    this.code = null;
};

FileTransferError.FILE_NOT_FOUND_ERR = 1;
FileTransferError.INVALID_URL_ERR = 2;
FileTransferError.CONNECTION_ERR = 3;

/**
* Given an absolute file path, uploads a file on the device to a remote server
* using a multipart HTTP request.
* @param filePath {String}           Full path of the file on the device
* @param server {String}             URL of the server to receive the file
* @param successCallback (Function}  Callback to be invoked when upload has completed
* @param errorCallback {Function}    Callback to be invoked upon error
* @param options {FileUploadOptions} Optional parameters such as file name and mimetype
\code
// !! Assumes variable fileURI contains a valid URI to a  text file on the device

var win = function(r) {
    console.log("Code = " + r.responseCode);
    console.log("Response = " + r.response);
    console.log("Sent = " + r.bytesSent);
}

var fail = function(error) {
    alert("An error has occurred: Code = " = error.code);
}

var options = new FileUploadOptions();
options.fileKey="file";
options.fileName=fileURI.substr(fileURI.lastIndexOf('/')+1);
options.mimeType="text/plain";

var params = new Object();
params.value1 = "test";
params.value2 = "param";

options.params = params;

var ft = new FileTransfer();
ft.upload(fileURI, "http://some.server.com/upload.php", win, fail, options);
\endcode
*/
FileTransfer.prototype.upload = function(filePath, server, successCallback, errorCallback, options, debug) {

    // check for options
    var params = null;
    if (options) {
        if (options.params) {
            var dict=new Array();
            var idx = 0;

            for (var key in options.params) {
                if (options.params.hasOwnProperty(key)) {
                    var value = options.params[key];
                    var item = new Object();
                    item.Key = key;
                    item.Value = value;
                    dict[idx] = item;
                    idx++;
                }
            }

            options.params = dict;
        }
    } else {
        options = new FileUploadOptions();
    }

    options.filePath = filePath;
    options.server = server;

    PhoneGap.exec(successCallback, errorCallback, 'FileTransfer', 'upload', options);
};

/**
 * Options to customize the HTTP request used to upload files.
 * @constructor
 * @param fileKey {String}   Name of file request parameter.
 * @param fileName {String}  Filename to be used by the server. Defaults to image.jpg.
 * @param mimeType {String}  Mimetype of the uploaded file. Defaults to image/jpeg.
 * @param params {Object}    Object with key: value params to send to the server.
 */
var FileUploadOptions = function(fileKey, fileName, mimeType, params) {
    this.fileKey = fileKey || null;
    this.fileName = fileName || null;
    this.mimeType = mimeType || null;
    this.params = params || null;
};
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("media")) {
PhoneGap.addResource("media");

/**
 * This class provides access to the device media, interfaces to both sound and video
 *
 * @constructor
 * @param src                   The file name or url to play
 * @param successCallback       The callback to be called when the file is done playing or recording.
 *                                  successCallback() - OPTIONAL
 * @param errorCallback         The callback to be called if there is an error.
 *                                  errorCallback(int errorCode) - OPTIONAL
 * @param statusCallback        The callback to be called when media status has changed.
 *                                  statusCallback(int statusCode) - OPTIONAL
 * @param positionCallback      The callback to be called when media position has changed.
 *                                  positionCallback(long position) - OPTIONAL
 */
var Media = function(src, successCallback, errorCallback, statusCallback, positionCallback) {

    // successCallback optional
    if (successCallback && (typeof successCallback !== "function")) {
        console.log("Media Error: successCallback is not a function");
        return;
    }

    // errorCallback optional
    if (errorCallback && (typeof errorCallback !== "function")) {
        console.log("Media Error: errorCallback is not a function");
        return;
    }

    // statusCallback optional
    if (statusCallback && (typeof statusCallback !== "function")) {
        console.log("Media Error: statusCallback is not a function");
        return;
    }

    // statusCallback optional
    if (positionCallback && (typeof positionCallback !== "function")) {
        console.log("Media Error: positionCallback is not a function");
        return;
    }

    this.id = PhoneGap.createUUID();
    PhoneGap.mediaObjects[this.id] = this;
    this.src = src;
    this.successCallback = successCallback;
    this.errorCallback = errorCallback;
    this.statusCallback = statusCallback;
    this.positionCallback = positionCallback;
    this._duration = -1;
    this._position = -1;
};

// Media messages
Media.MEDIA_STATE = 1;
Media.MEDIA_DURATION = 2;
Media.MEDIA_POSITION = 3;
Media.MEDIA_ERROR = 9;

// Media states
Media.MEDIA_NONE = 0;
Media.MEDIA_STARTING = 1;
Media.MEDIA_RUNNING = 2;
Media.MEDIA_PAUSED = 3;
Media.MEDIA_STOPPED = 4;
Media.MEDIA_MSG = ["None", "Starting", "Running", "Paused", "Stopped"];

// TODO: Will MediaError be used?
/**
 * This class contains information about any Media errors.
 * @constructor
 */
var MediaError = function() {
    this.code = null;
    this.message = "";
};

MediaError.MEDIA_ERR_PLAY_MODE_SET = 1;
MediaError.MEDIA_ERR_ALREADY_RECORDING = 2;
MediaError.MEDIA_ERR_STARTING_RECORDING = 3;
MediaError.MEDIA_ERR_RECORD_MODE_SET = 4;
MediaError.MEDIA_ERR_STARTING_PLAYBACK = 5;
MediaError.MEDIA_ERR_RESUME_STATE = 6;
MediaError.MEDIA_ERR_PAUSE_STATE = 7;
MediaError.MEDIA_ERR_STOP_STATE = 8;

/**
 * Start or resume playing audio file.
 \code
 // Play audio
//
function playAudio(url) {
    // Play the audio file at url
    var my_media = new Media(url,
        // success callback
        function() {
            console.log("playAudio():Audio Success");
        },
        // error callback
        function(err) {
            console.log("playAudio():Audio Error: "+err);
    });

    // Play audio
    my_media.play();
}
 \endcode
 */
Media.prototype.play = function() {
    PhoneGap.exec(null, null, "Media", "startPlayingAudio", {id: this.id, src: this.src});
};

/**
 * Stop playing audio file.
 \code
 // Play audio
//
function playAudio(url) {
    // Play the audio file at url
    var my_media = new Media(url,
        // success callback
        function() {
            console.log("playAudio():Audio Success");
        },
        // error callback
        function(err) {
            console.log("playAudio():Audio Error: "+err);
    });

    // Play audio
    my_media.play();

    // Pause after 10 seconds
    setTimeout(function() {
        my_media.stop();
    }, 10000);
}
\endcode
 */
Media.prototype.stop = function() {
    return PhoneGap.exec(null, null, "Media", "stopPlayingAudio", {id: this.id});
};

/**
 * Seek or jump to a new time in the track..
 \code
// Audio player
    //
    var my_media = new Media(src, onSuccess, onError);
    my_media.play();
    // SeekTo to 10 seconds after 5 seconds
    setTimeout(function() {
        my_media.seekTo(10000);
    }, 5000);
 \endcode
 */
Media.prototype.seekTo = function(milliseconds) {
    PhoneGap.exec(null, null, "Media", "seekToAudio", {id: this.id, milliseconds: milliseconds});
};

/**
 * Pause playing audio file.
 \code
 // Play audio
//
function playAudio(url) {
    // Play the audio file at url
    var my_media = new Media(url,
        // success callback
        function() {
            console.log("playAudio():Audio Success");
        },
        // error callback
        function(err) {
            console.log("playAudio():Audio Error: "+err);
    });

    // Play audio
    my_media.play();

    // Pause after 10 seconds
    setTimeout(function() {
        media.pause();
    }, 10000);
}
 \endcode
 */
Media.prototype.pause = function() {
    PhoneGap.exec(null, null, "Media", "pausePlayingAudio", {id: this.id});
};

/**
 * Get duration of an audio file.
 * The duration is only set for audio that is playing, paused or stopped.
 *
 * @return      duration or -1 if not known.
 \code
 // Audio player
    //
    var my_media = new Media(src, onSuccess, onError);

    // Get duration
    var counter = 0;
    var timerDur = setInterval(function() {
        counter = counter + 100;
        if (counter > 2000) {
            clearInterval(timerDur);
        }
        var dur = my_media.getDuration();
        if (dur > 0) {
            clearInterval(timerDur);
            document.getElementById('audio_duration').innerHTML = (dur) + " sec";
        }
   }, 100);
 \endcode
 */
Media.prototype.getDuration = function() {
    return this._duration;
};

/**
 * Get position of audio.
 \code
     // Audio player
    //
    var my_media = new Media(src, onSuccess, onError);

    // Update media position every second
    var mediaTimer = setInterval(function() {
        // get media position
        my_media.getCurrentPosition(
            // success callback
            function(position) {
                if (position > -1) {
                    console.log((position) + " sec");
                }
            },
            // error callback
            function(e) {
                console.log("Error getting pos=" + e);
            }
        );
    }, 1000);
 \endcode
 */
Media.prototype.getCurrentPosition = function(success, fail) {
    PhoneGap.exec(success, fail, "Media", "getCurrentPositionAudio", {id: this.id});
};

/**
 * Start recording audio file.
 \code
 // Record audio
//
function recordAudio() {
    var src = "myrecording.mp3";
    var mediaRec = new Media(src,
        // success callback
        function() {
            console.log("recordAudio():Audio Success");
        },

        // error callback
        function(err) {
            console.log("recordAudio():Audio Error: "+ err.code);
        });

    // Record audio
    mediaRec.startRecord();
}
 \endcode
 */
Media.prototype.startRecord = function() {
    PhoneGap.exec(null, null, "Media", "startRecordingAudio", {id: this.id, src: this.src});
};

/**
 * Stop recording audio file.
 \code
 // Record audio
//
function recordAudio() {
    var src = "myrecording.mp3";
    var mediaRec = new Media(src,
        // success callback
        function() {
            console.log("recordAudio():Audio Success");
        },

        // error callback
        function(err) {
            console.log("recordAudio():Audio Error: "+ err.code);
        });

    // Record audio
    mediaRec.startRecord();

    // Stop recording after 10 seconds
    setTimeout(function() {
        mediaRec.stopRecord();
    }, 10000);
}
\endcode
 */
Media.prototype.stopRecord = function() {
    PhoneGap.exec(null, null, "Media", "stopRecordingAudio", {id: this.id});
};

/**
 * Release the resources.
 \code
     // Audio player
    //
    var my_media = new Media(src, onSuccess, onError);

    my_media.play();
    my_media.stop();
    my_media.release();
 \endcode
 */
Media.prototype.release = function() {
    PhoneGap.exec(null, null, "Media", "release", {id: this.id});
};

/**
 * List of media objects.
 * PRIVATE
 */
PhoneGap.mediaObjects = {};

/**
 * Object that receives native callbacks.
 * PRIVATE
 * @constructor
 */
PhoneGap.Media = function() {};

/**
 * Get the media object.
 * PRIVATE
 *
 * @param id            The media object id (string)
 */
PhoneGap.Media.getMediaObject = function(id) {
    return PhoneGap.mediaObjects[id];
};

/**
 * Audio has status update.
 * PRIVATE
 *
 * @param id            The media object id (string)
 * @param status        The status code (int)
 * @param msg           The status message (string)
 */
PhoneGap.Media.onStatus = function(id, msg, value) {
    var media = PhoneGap.mediaObjects[id];
    // If state update
    if (msg === Media.MEDIA_STATE) {
        if (value === Media.MEDIA_STOPPED) {
            if (media.successCallback) {
                media.successCallback();
            }
        }
        if (media.statusCallback) {
            media.statusCallback(value);
        }
    }
    else if (msg === Media.MEDIA_DURATION) {
        media._duration = value;
    }
    else if (msg === Media.MEDIA_ERROR) {
        if (media.errorCallback) {
            media.errorCallback(value);
        }
    }
    else if (msg == Media.MEDIA_POSITION) {
        media._position = value;
    }
};

// We need special proxy to invoke PhoneGap.Media.onStatus (method is not visible in other case)
// http://stackoverflow.com/questions/7322420/calling-javascript-object-method-using-webbrowser-document-invokescript
PhoneGapMediaonStatus = function (args) {
    var res = JSON.parse(args);
    PhoneGap.Media.onStatus(res.id, res.msg, res.value);
}

}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2010, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("network")) {
PhoneGap.addResource("network");

/**
 * This class contains information about the current network Connection.
 * @constructor
 */
var Connection = function()
{
    this.type = null;
    this._firstRun = true;
    this._timer = null;
    this.timeout = 500;

    var me = this;
    this.getInfo(
        function(type) {
            console.log("getInfo result" + type);
            // Need to send events if we are on or offline
            if (type == "none") {
                // set a timer if still offline at the end of timer send the offline event
                me._timer = setTimeout(function(){
                    me.type = type;
                    console.log("PhoneGap.fireEvent::offline");
                    PhoneGap.fireEvent(document,'offline');
                    me._timer = null;
                    }, me.timeout);
            } else {
                // If there is a current offline event pending clear it
                if (me._timer != null) {
                    clearTimeout(me._timer);
                    me._timer = null;
                }
                me.type = type;
                console.log("PhoneGap.fireEvent::online " + me.type);
                PhoneGap.fireEvent(document,'online');
            }

            // should only fire this once
            if (me._firstRun)
            {
                me._firstRun = false;
                console.log("onPhoneGapConnectionReady");
                PhoneGap.onPhoneGapConnectionReady.fire();
            }
        },
        function(e) {
            console.log("Error initializing Network Connection: " + e);
        });
};

Connection.UNKNOWN = "unknown";
Connection.ETHERNET = "ethernet";
Connection.WIFI = "wifi";
Connection.CELL_2G = "2g";
Connection.CELL_3G = "3g";
Connection.CELL_4G = "4g";
Connection.NONE = "none";

/**
 * Get connection info
 *
 * @param {Function} successCallback The function to call when the Connection data is available
 * @param {Function} errorCallback The function to call when there is an error getting the Connection data. (OPTIONAL)
 */
Connection.prototype.getInfo = function(successCallback, errorCallback) {
    // Get info
    PhoneGap.exec(successCallback, errorCallback, "Connection", "getConnectionInfo", []);
};


PhoneGap.addConstructor(function() {
    if (typeof navigator.network === "undefined") {
        navigator.network = new Object();
    }
    if (typeof navigator.network.connection === "undefined") {
        navigator.network.connection = new Connection();
    }
});
}
/*
 * PhoneGap is available under *either* the terms of the modified BSD license *or* the
 * MIT License (2008). See http://opensource.org/licenses/alphabetical for full text.
 *
 * Copyright (c) 2005-2011, Nitobi Software Inc.
 * Copyright (c) 2010-2011, IBM Corporation
 * Copyright (c) 2011, Microsoft Corporation
 */

if (!PhoneGap.hasResource("notification")) {
PhoneGap.addResource("notification");

/**
 * This class provides access to notifications on the device.
 * @constructor
 */
var Notification = function() {
};

/**
 * Open a native alert dialog, with a customizable title and button text.
 *
 * @param {String} message              Message to print in the body of the alert
 * @param {Function} completeCallback   The callback that is called when user clicks on a button.
 * @param {String} title                Title of the alert dialog (default: Alert)
 * @param {String} buttonLabel          Label of the close button (default: OK)
 \code
 // Android / BlackBerry WebWorks (OS 5.0 and higher) / iPhone
//
function alertDismissed() {
    // do something
}

navigator.notification.alert(
    'You are the winner!',  // message
    alertDismissed,         // callback
    'Game Over',            // title
    'Done'                  // buttonName
);

// BlackBerry (OS 4.6) / webOS
//
navigator.notification.alert('You are the winner!');
\endcode
 */
Notification.prototype.alert = function(message, completeCallback, title, buttonLabel)
{
    var _title = (title || "Alert");
    var _buttonLabels = (buttonLabel || "OK");
    PhoneGap.exec(completeCallback, null, "Notification", "alert",{"message":message,"title":_title,"buttonLabels":_buttonLabels});
};

/**
 * Open a native confirm dialog, with a customizable title and button text.
 * The result that the user selects is returned to the result callback.
 *
 * @param {String} message              Message to print in the body of the alert
 * @param {Function} resultCallback     The callback that is called when user clicks on a button.
 * @param {String} title                Title of the alert dialog (default: Confirm)
 * @param {String} buttonLabels         Comma separated list of the labels of the buttons (default: 'OK,Cancel')
 \code
 // process the confirmation dialog result
function onConfirm(button) {
    alert('You selected button ' + button);
}

// Show a custom confirmation dialog
//
function showConfirm() {
    navigator.notification.confirm(
        'You are the winner!',  // message
        onConfirm,              // callback to invoke with index of button pressed
        'Game Over',            // title
        'Restart,Exit'          // buttonLabels
    );
}
\endcode
 */
Notification.prototype.confirm = function(message, resultCallback, title, buttonLabels)
{
    var _title = (title || "Confirm");
    var _buttonLabels = (buttonLabels || "OK,Cancel");
    PhoneGap.exec(resultCallback, null, "Notification", "confirm", {'message':message,"title":_title,"buttonLabels":_buttonLabels});
};

/**
 * Start spinning the activity indicator on the statusbar
 */
Notification.prototype.activityStart = function() {
    PhoneGap.exec(null, null, "Notification", "activityStart", ["Busy","Please wait..."]);
};

/**
 * Stop spinning the activity indicator on the statusbar, if it's currently spinning
 */
Notification.prototype.activityStop = function() {
    PhoneGap.exec(null, null, "Notification", "activityStop", []);
};

/**
 * Display a progress dialog with progress bar that goes from 0 to 100.
 *
 * @param {String} title        Title of the progress dialog.
 * @param {String} message      Message to display in the dialog.
 */
Notification.prototype.progressStart = function(title, message) {
    PhoneGap.exec(null, null, "Notification", "progressStart", [title, message]);
};

/**
 * Set the progress dialog value.
 *
 * @param {Number} value         0-100
 */
Notification.prototype.progressValue = function(value) {
    PhoneGap.exec(null, null, "Notification", "progressValue", [value]);
};

/**
 * Close the progress dialog.
 */
Notification.prototype.progressStop = function() {
    PhoneGap.exec(null, null, "Notification", "progressStop", []);
};

/**
 * Causes the device to blink a status LED.
 *
 * @param {Integer} count       The number of blinks.
 * @param {String} colour       The colour of the light.
 */
Notification.prototype.blink = function(count, colour) {
    // NOT IMPLEMENTED
};

/**
 * Causes the device to vibrate.
 *
 * @param {Integer} mills       The number of milliseconds to vibrate for.
 \code
 // Vibrate for 2.5 seconds
//
navigator.notification.vibrate(2500);
\endcode
 */
Notification.prototype.vibrate = function(mills)
{
    PhoneGap.exec(null, null, "Notification", "vibrate", {duration:mills});
};

/**
 * Causes the device to beep.
 * A packaged resource is played "repeatCount" times.
 *
 * @param {Integer} repeatCount       The number of beeps. default 1
 \code
 // Beep twice!
navigator.notification.beep(2);
\endcode
 */
Notification.prototype.beep = function(repeatCount)
{
    var count = repeatCount|| 1;
    PhoneGap.exec(null, null, "Notification", "beep", count);
};

PhoneGap.addConstructor(function() {
    if (typeof navigator.notification === "undefined") {
        navigator.notification = new Notification();
    }
});
} // End of notification API
